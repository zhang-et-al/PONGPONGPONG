#include "Shape.hpp"
#include "Ball.hpp"

#include <algorithm>
#include <array>
#include <iostream>

Simulation* Shape::get_simulation()
{
	return simulation;
}

Shape::~Shape()
{
	if(simulation != nullptr) simulation->remove_shape(this);
}


void Shape::set_tick_callback(std::function<void(void)> func)
{
	on_tick = func;
}

double Shape::get_trail_length() const
{
	double len = 0;

	for(auto it = trail.begin(); it != trail.end()-1;)
	{
		Vector a = *(it);
		Vector b = *(++it);
		len += (a-b).modulus();
	}

	return len;
}

void Shape::rotate(Vector axis, bool conserve_angular_momentum)
{
	mesh.rotate(axis);

	for(Label& label : labels)
	{
		label.pos.rotate(axis);
	}

	moment_of_inertia = moment_of_inertia.rotate(axis);

	// If the moment of inertia is rotated, for angular momentum to be conserved
	// the angular velocity must be changed.
	// The neat little thing about this functionality is that it serves as a simple
	// integrator for Euler's equations when called iteratively (axis = ω * dt)
	if(conserve_angular_momentum)
	{
		angular_vel = moment_of_inertia.inverse() * angular_momentum;
	}
}

void Shape::tick_motion(double dt)
{
	vel += accel * dt - frict_consts * vel * dt;
	prev_pos = pos;
	pos += vel * dt;

	if(!angular_accel.zero())
		set_angular_vel(angular_vel + angular_accel * dt);

	angle += angular_vel * dt;

	if(!angular_vel.zero())
		rotate(angular_vel * dt);

	if(keep_trail)
	{
		if(trail.empty() || (pos - trail.back()).modulus() > 10)
			trail.push_back(pos);

		while(get_trail_length() > max_trail_length) trail.pop_front();
	}

	if(on_tick) on_tick();
}

void Shape::set_angular_vel(Vector av)
{
	angular_vel = av;
	angular_momentum = moment_of_inertia * av;
}

void Shape::set_angular_momentum(Vector am)
{
	angular_momentum = am;
	angular_vel = moment_of_inertia.inverse() * am;
}

/*
									 ..;cocclcl,,.                             .. ....                                                                
							   ',::::c...,:oococcdocc,    ..... .     .......,.'.,..'',;'..     ...............................;,,'.                  
							 .;,.........;oxccooodllooc.  ',..'.............';......'ddlc'............. ......       .   ...lk0O0OkOkd'               
							::.,,.,:;';,:oxdcco:cololddl;  ....'cdoolcccc:;:od;'.'..:ddlxxxdoodoololllc:::;;;',''....'....,OOkkxxoccloxl.             
						   :d:;:..''':ooddol',,::;llodoxd,       'c;.    ..,,'..,',,;c;,'...........'';:ccclloool:;;;;,,;;l0kkxxxcc:ccll:             
						 .,,;c:,;c;,:ooo;......'',;:loood,         '   .........,;:,.                         .'coool:;;::l0OOkxdx:;:,:lc             
						.c:'.,..';:l,cl:.......''',,;cokxl          ...... ...',lc.                                ..;clclldkxllcl.:c;::;             
						 ,cc;,..,:....'...........',,;lkol.        ...........,o:                                       .,lxkOk:.  ,:;;:.             
						 .c;;;.':cl...............,:ooldo;      ..........''';oc                                            ...    .,;,'              
						 ,oclc,oll'.............,dkxlollo.    .'....',,;';,:;cl                                                    .,;'               
						 'dlolclc'........;l:...,0ok0Kccd'  .........';l:,;odo.                                                    ',,                
						  .'ccooc:.....':lcoc....;:::,,';l;'.........,::codol;                                                    .,,.                
							  :cod:'...;:ld0l.....;'....,od'......''',lodoc;;,              ____  ____ __     __                 .,;'                 
							  .odol:;.,:;:c,....:ccl.'.';dx;,..':,,,,;;l:;;:c'             |  _ \|  _ \\ \   / /                .,,;.                 
							   ,lldll:.... .....,;::c..,:kxl;;''loc;;. ,;:;,;.             | |_) | |_) |\ \ / /                 .,;,                  
								 .;,:ll,'.     .;c:c..'lxxdllc;;lo;.   ';;;,'              |  __/|  _ <  \ V /                  ',;.                  
									  ..;;..........,ldkddoolcc:o'    ';:,;;.              |_|   |_| \_\  \_/                   ','                   
									  .'..',,;;;;;;cdoloooldlllco.  .;l;c,;.                                                    ''.                   
				   .,ccc;;:;,;'.     ':...     ...',ccllooolloll,....,clo;'                                                     ',.                   
				 .:looc:c,:clodo.   ;d;.',;.    ....',::cl:ccll,......,:x,.                                                     .,.                   
			   'lolddooodlcoooooo::dx'...............',;:cccccc. . ..';cl:                    __      _____                     .'                    
			 ,loolcccl:::l::lcloclod;.    .....:c:.....';:clll:.....,,cld;                   /_ |  _ |___  |                    ..                    
		   .:cccc,';.,';,o,,:llcoodo;... . ...,;:cl:c:;'',::..   ..,';:ll,                    | | (_)   / /                     .;.                   
		  .','...      .  ..,;;'';oo...... ..'',;,;:c:l:;'.    ...',,,. ,.                    | |  _   / /                      .;.                   
								  .l,...   .......'',,,.. .   ...',;'   ..                    |_| (_) /_/                       .,.                   
									.,;,..  ..  . ........,.....',;'    .                                                       .,,                   
									  ':c;...............,...''':c:    ..                                                       .;:                   
									  .l,..'.... . .. .....'',;:lxc     .                                                        ,:.                  
									  ';.. .',,.......  ...',;lxxxc     ..                                                        ;.                  
									  'l.... .':ccc:;,,,',;clodxxdl     ....                                                      ',                  
									  ...;.......,clolllodxdoododdd.    .,'..                                                     .:.                 
									 ..............;;:c:cclclolodxd.     ,;,'                                                     .:,                 
								   ',... . .. .. . ..',,';:cllloddl      .;;,'.                                             ....  .::.                
								 .,,..               ....;l::ccloo'       ':;'...                                         .lkxddl;,;:,                
								,:;'....    . .      ...;xo::cllo;        .:::,,'.......................................ll,:xol:;;l::cc.              
							  .;;',,,,,'... .. .......'.;c::cloo;        .,:;,'''.,''.,'...,''.,'...          ..........lxxooccc;odcclc.              
							 ';'.'',,;;c:cl;'.........,;c:cloodo        .... . . ..   ......                            xddddxollkdlcdl,              
							.,..  ......';;:lcc,....',::ccloxkOd;.      '.. ..........................................  .'oOk0Kkxko::lc;              
							.....  ..  ....',,cddlcclododxxodoloddc.    ''............................................''',:OkkOxlc:::cc.              
							.....  .     ...'';llkOkOxdlccc;,',,,;:c;   ,;'............'...'..''',;;:lccccccccclc:;;;;;;;;;lkOkxdlcllc'               
							.,,.. .. .......'.;cooOkxoc:;'.'......'';,   ,::;''''',;'',:,::;;;;,,,'...... .. ................,:cc:;'.                 
							 ;;'...... . ...'..,cldOdl:,,... . ....',;'    ..............                                                             
							 .c'.. .....   ....':cl0xl;....      ...',:                                                                               
							  ''.....'..    ....,ccodl',,...    .....,:.                                                                              
							  .;,,,,,,.........,::;;.ll:'..'......'.,:.                                                                               
							   .:lol;,'...'.,.,c::lxd:;..........',,:.                                                                                
								 :'.........,;::;lklc:;,.......',,,.                                                                                  
								   ......';:;;;:ldlllcc;,',,,,;;,.                                                                                    
								   ....';clccoxxoollllcccllc:;'.                                                                                      
								...'.';:cloddOloodoldlloll:,.                                                                                         
								....'::::cclxoclodocc;'..                                                                                             
							   . ...':c:::loc;l,..                                                                                                    
							  ......'cc:cl:;:,.                                                                                                       
							  ,....';::l::.;.                                                                                                         
							 .c'.'';;:,. .                                                                                                            
							 .....'';.                                                                                                                
							 . .....',                                                                                                                
							  .''....,.                                                                                                               
							  .,;,,':''                                                                                                               
								 ...,'.
*/
