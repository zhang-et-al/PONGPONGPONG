#include "Renderer/Canvas.hpp"
#include "Shapes/Ball.hpp"
#include "Shapes/Icosahedron.hpp"
#include "Shapes/Parallelepiped.hpp"
#include "Shapes/Arrow.hpp"
#include "Shapes/STL_Shape.hpp"
#include "Shapes/Wall.hpp"
#include "Simulation.hpp"

#include <stdio.h>
#include <iostream>


int main(void)
{
	srand(time(nullptr));

	int width = 1000, height = 1000, depth = 1000;
	Vector center(0, 0, 0);

	Simulation simulation(width, height, depth, 5000, false);
	Canvas canvas(1800, 1000, 2.4, 60, false);
	Visor visor(Vector(300, 100, -490));
	visor.set_azimuth(0.4);
	visor.set_altitude(-0.21);
	canvas.set_visor(&visor);

	//visor.pos = Vector(225, -35, 225);
	//visor.set_azimuth(0);
	//visor.set_altitude(M_PI/2);
	simulation.paused = true;

	//Ball sun(Vector(width/2, height/2, depth/2), 0, 0, 1000, Color::yellow, 1000);
	//simulation.add_shape(&sun);

	simulation.add_shape(&visor);

	//simulation.add_shape(new STL_Shape("/home/user/Desktop/c++/PONG/split/stl-assets/New_Horizons.stl", 0, 0, 200, depth/2));
	//simulation.add_shape(new STL_Shape("/home/user/Desktop/c++/PONG/split/stl-assets/Gawr-Gura.stl", 0, 0, 200, depth/2));

	/*
	for(int i = 1; i < 16; ++i)
	{
		simulation.add_shape(new Ball(Vector(i * width/16, height/2, depth/1.5), 0, 0, width/32, Color::yellow));
	}*/


	//simulation.add_shape(new Ball(Vector(width/2, height/2, 10), 0, 0, 5, Color::yellow));


	/*
	simulation.set_tick_callback([&] {
		static double x = 0;
		if(x <= 8*M_PI)
		{
			Ball* ball = new Ball(Vector(0, height/2.2 * sin(x), 0), Vector(cos(x)*300, 0, sin(x)*300), 500, 5, Color::yellow);
			//ball->collidable = false;
			simulation.add_shape(ball);
			x += 0.01;
		}
	});
	//*/


/*
	simulation.add_shape(new Ball(Vector(width/2 + 1, height/2, depth - 320), 0, 0, 140, Color::yellow, 100));
	Ball earth(Vector(width/2 + 1, height/2 + 300, depth - 320), Vector(650, 0, 0), 0, 30, Color::blue);
	earth.keep_trail = true;

	simulation.add_shape(&earth);

	Arrow arrow(earth.pos, earth.vel, 10, Color::teal);
	simulation.add_shape(&arrow);

	simulation.set_tick_callback([&] {
		arrow.set_direction(earth.vel);
		arrow.pos = earth.pos;
	});
	*/
	/*
	Ball yellow(Vector(width/2 + 1, 800, depth/2), 0, 500, 50, Color(255, 255, 0, 127));
	Ball green(Vector(width/2, 600, depth/2 + 1), 0, 500, 100, Color(0, 255, 0, 80));
	Ball hwite(Vector(width/2, 250, depth/2), 0, 500, 200, Color(255, 255, 255, 100));
	yellow.keep_trail = true;
	green.keep_trail = true;
	hwite.keep_trail = true;
	
	simulation.add_shape(&yellow);
	simulation.add_shape(&green);
	simulation.add_shape(&hwite);
	//*/

	/*
	simulation.add_shape(new Ball(Vector(width/2 + 0.001, 50, depth/2 + 0.001), 0, 500, 50, Color::lime));
	for(int i = 0; i < 6; ++i)
	{
		Ball* ball = new Ball(Vector(width/2, height - (i * 110 + 150), depth/2), 0, 500, 50, Color::random());
		simulation.add_shape(ball);
		ball->keep_trail = true;
	}
	//*/

	/*
	 _            _                _       _ 
	| |_ ___  ___| |_    __ ___  _(_) __ _| |
	| __/ _ \/ __| __|  / _` \ \/ / |/ _` | |
	| ||  __/\__ \ |_  | (_| |>  <| | (_| | |
	 \__\___||___/\__|  \__,_/_/\_\_|\__,_|_|
	                                         
	           _        _   _             
	 _ __ ___ | |_ __ _| |_(_) ___  _ __  
	| '__/ _ \| __/ _` | __| |/ _ \| '_ \ 
	| | | (_) | || (_| | |_| | (_) | | | |
	|_|  \___/ \__\__,_|\__|_|\___/|_| |_|

	*/

	/*
	Vector axis(3, 5, 1);
	Arrow arrow(0, Vector(0, 0, depth/2), 10);
	arrow.angular_vel = axis;
	Arrow axis_arrow(0, axis*15, 10, Color::teal);
	//Arrow angle_arrow(Vector(width/2, height/2, depth/2), axis*5, 9, Color::red);
	simulation.add_shape(&arrow);
	simulation.add_shape(&axis_arrow);
	//simulation.add_shape(&angle_arrow);
	*/

	//Vector angle = Vector(0, 0, depth/2).angle_to(Vector(0,0,-10));
	//arrow.rotate(angle);
	
	//printf("(%.6g, %.6g, %.6g)\n", angle.x, angle.y, angle.z);


	/*
	Orthohedron ortho(0, 0, 100, 80, 60);
	ortho.angular_vel = axis;

	simulation.add_shape(&ortho);

	Vector corner = (ortho.a + ortho.b + ortho.c)/2;

	Arrow A(0, ortho.a, 10, Color::red);
	Arrow B(0, ortho.b, 10, Color::green);
	Arrow C(0, ortho.c, 10, Color::blue);

	ortho.mesh.add(A.mesh - corner);
	ortho.mesh.add(B.mesh - corner);
	ortho.mesh.add(C.mesh - corner);
	

	simulation.set_tick_callback([&] {
		//std::cout << "A(" << ortho.a.x << ", " << ortho.a.y << ", " << ortho.a.z << ")" << std::endl;
		//arrow.set_direction(arrow.angle);
		//arrow.set_direction(Vector(0, 0, depth/2));
		//arrow.set_direction(Matrix::Rotation(arrow.angle) * Vector(0, 0, depth/2));
	});
	*/
	//*/
	
	//*/
	//simulation.add_shape(new Cube(Vector(width/2, height/2, depth/2), 0, 0, 50));

	/*
	 _            _     _____ _____     __
	| |_ ___  ___| |_  |  ___/ _ \ \   / /
	| __/ _ \/ __| __| | |_ | | | \ \ / / 
	| ||  __/\__ \ |_  |  _|| |_| |\ V /  
	 \__\___||___/\__| |_|   \___/  \_/   
                                      
	*/
	//float z = Canvas::WIDTH * tan((M_PI-Canvas::FOV)/2) / 2;
	//new Ball(Vector(0, 0, 0), Vector(Walls::WIDTH/2 + Canvas::WIDTH/2 -5, Walls::HEIGHT/2, z), 0, 10, RED);
	//new Ball(Vector(0, 0, 0), Vector(Walls::WIDTH/2 - Canvas::WIDTH/2 +5, Walls::HEIGHT/2, z), 0, 10, RED);
	

	
//*

	/*
	 _            _               _ _ _     _             
	| |_ ___  ___| |_    ___ ___ | | (_)___(_) ___  _ __  
	| __/ _ \/ __| __|  / __/ _ \| | | / __| |/ _ \| '_ \ 
	| ||  __/\__ \ |_  | (_| (_) | | | \__ \ | (_) | | | |
	 \__\___||___/\__|  \___\___/|_|_|_|___/_|\___/|_| |_|
	                                                      
	     _      _            _   _             
	  __| | ___| |_ ___  ___| |_(_) ___  _ __  
	 / _` |/ _ \ __/ _ \/ __| __| |/ _ \| '_ \ 
	| (_| |  __/ ||  __/ (__| |_| | (_) | | | |
	 \__,_|\___|\__\___|\___|\__|_|\___/|_| |_|
	                                           

	*/
	/*
	for(int i = 10; i < height; i += 22)
	{
		for(int j = 10; j < width; j += 22)
		{
			simulation.add_shape(new Ball(Vector(j, i,  10), Vector(0, 0, 500), 0, 9, Color::red));
		}
	}
	
	
	for(int i = 10; i < width; i += 22)
	{
		for(int j = 10; j < height; j += 22)
		{
			simulation.add_shape(new Ball(Vector(i, j, depth - 10), Vector(0, 0, -500), 0, 9, Color::red));
		}
	}
	
	//simulation.add_shape(new Ball(Vector(215, 610, depth - 10), Vector(0, 0, -500), 0, 9, Color::red));
	//simulation.add_shape(new Ball(Vector(215, height/2, depth - 10), Vector(0, 0, -500), 0, 9, Color::red));

	
	Vector a(100, 10, 20), b(0, 0, 180), c(10, 250, 5);
	Vector center(width/2, height/2, depth/2);
	Vector corner = center - (a + b + c)/2;
	Arrow A(corner, a, 10, Color::red);
	Arrow B(corner, b, 10, Color::green);
	Arrow C(corner, c, 10, Color::blue);
	Parallelepiped p(center, 0, a, b, c);
	simulation.add_shape(&p);
	simulation.add_shape(&A);
	simulation.add_shape(&B);
	simulation.add_shape(&C);
	//*/

	/*
	            _   _           _              _                       _           _ _ 
	  ___  _ __| |_| |__   ___ | |__   ___  __| |_ __ ___  _ __       | |__   __ _| | |
	 / _ \| '__| __| '_ \ / _ \| '_ \ / _ \/ _` | '__/ _ \| '_ \ _____| '_ \ / _` | | |
	| (_) | |  | |_| | | | (_) | | | |  __/ (_| | | | (_) | | | |_____| |_) | (_| | | |
	 \___/|_|   \__|_| |_|\___/|_| |_|\___|\__,_|_|  \___/|_| |_|     |_.__/ \__,_|_|_|
	                                                                                   
	           _ _ _     _               _            _   
	  ___ ___ | | (_)___(_) ___  _ __   | |_ ___  ___| |_ 
	 / __/ _ \| | | / __| |/ _ \| '_ \  | __/ _ \/ __| __|
	| (_| (_) | | | \__ \ | (_) | | | | | ||  __/\__ \ |_ 
	 \___\___/|_|_|_|___/_|\___/|_| |_|  \__\___||___/\__|
	                                                      
	*/

	/*
	
	float a = 100, b = 180, c = 450;


	//Parallelepiped ortho = Parallelepiped::Orthohedron(center, 0, a, b, c, 800, Color(255, 255, 255, 127));
	Parallelepiped ortho(center, 0, Vector(a, 20, 10), Vector(5, b, 50), Vector(100, 0, c), 800, Color(255, 255, 255, 127)); // not so ortho

	simulation.add_shape(&ortho);

	Arrow A(0, ortho.a, 10, Color::red);
	Arrow B(0, ortho.b, 10, Color::green);
	Arrow C(0, ortho.c, 10, Color::blue);

	Vector corner = ortho.diag()/2;

	ortho.mesh.add(A.mesh - corner);
	ortho.mesh.add(B.mesh - corner);
	ortho.mesh.add(C.mesh - corner);


	Ball projectile(center - ortho.c/2 + Vector(100, -25, -2), 20 * Vector(-500, 0, 100), 500, 9, Color::gray, 800);
	//Ball projectile(center + ortho.b*2, Vector(0, -1000, 0), 800, 9, Color::gray, 800);

	//Ball projectile(center + Vector(0, 300, 10), 0, 500, 9, Color::red, 1);
	projectile.labels.push_back(Label(0, [&] {
		char text[256];
		sprintf(text, "x=%.6g, y=%.6g, z=%.6g", projectile.pos.x, projectile.pos.y, projectile.pos.z);
		return std::string(text);
	}));


	//ortho.rotate(Vector(0.5, 0.2, 0.1));

	ortho.keep_trail = true;
	projectile.keep_trail = true;

	simulation.add_shape(&ortho);
	simulation.add_shape(&projectile);


	//ortho.angular_vel = Vector(0.05, -0.5, 0);
	ortho.vel = Vector(100, 0, 0);



	//Arrow ortho_vel(center, 0, 10, Color::lime);
	Arrow ortho_angular_vel(center, 0, 10, Color::teal);
	Arrow ortho_angular_momentum(center, 0, 10, Color::red);

	//simulation.add_shape(&ortho_vel);
	//simulation.add_shape(&ortho_angular_vel);

	
	//simulation.add_shape(&ortho_angular_vel);
	//simulation.add_shape(&ortho_angular_momentum);

	ortho_angular_vel.labels.push_back(Label(0, [&] {
		
		Vector av = ortho.get_angular_vel();

		if(av.modulus() > 0.005)
			ortho_angular_vel.set_direction(av.normalise() * 250);

		char text[256];


		sprintf(text, "omega = (%.6g, %.6g, %.6g), |omega| = %.6g\n", av.x, av.y, av.z, av.modulus());
		return std::string(text);
	}));

	
	simulation.set_tick_callback([&] {
		static double x = 0;
		if(x <= 2*M_PI)
		{
			Ball* ball = new Ball(Vector(0, height/2.2, 0), 500*Vector(cos(x), 0, sin(x)), 500, 10, Color::yellow);
			//ball->collidable = false;
			simulation.add_shape(ball);
			x += 0.01;
		}
	});
	

	ortho_angular_momentum.labels.push_back(Label(0, [&] {
		Vector angular_momentum = ortho.get_angular_momentum();
		
		
		if(angular_momentum.modulus() > 0.005)
			ortho_angular_momentum.set_direction(angular_momentum.normalise() * 300);

		char text[256];
		sprintf(text, "L = (%.6g, %.6g, %.6g), |L| = %.6g\n", angular_momentum.x, angular_momentum.y, angular_momentum.z, angular_momentum.modulus());
		return std::string(text);
	}));
	

	std::cout << ortho.moment_of_inertia.dump() << std::endl;
	std::cout << ortho.moment_of_inertia.rotate(Vector(0, M_PI/2, 0)).dump() << std::endl;
	std::cout << ortho.moment_of_inertia.inverse().dump() << std::endl;
	std::cout << (ortho.moment_of_inertia.inverse() * ortho.moment_of_inertia).dump() << std::endl;

	//*/

	/*
	__        __    _ _   _            _   
	\ \      / /_ _| | | | |_ ___  ___| |_ 
	 \ \ /\ / / _` | | | | __/ _ \/ __| __|
	  \ V  V / (_| | | | | ||  __/\__ \ |_ 
	   \_/\_/ \__,_|_|_|  \__\___||___/\__|
	                                       
	*/
	/*
	Ball apple(center + Vector(0, 10, 10), 0, 500, 9, Color::red, 1);
	apple.keep_trail = true;
	Wall wall(center, Vector(100, 20, 11), Vector(10, 20, 110));

	simulation.add_shape(&apple);

	simulation.add_shape(&wall);
	//*/

	/*
	simulation.set_tick_callback([&] {
		static int x = 0;
		if(x <= 36000*M_PI)
		{
			if(rand()%300 == 50)
			{
				Ball* ball = new Ball(Vector(cos(x)*0.7*width/2, 0, sin(x)*0.7*depth/2), Vector(cos(x)*30, 0, sin(x)*30), 500, 50, Color::yellow);
			//ball->collidable = false;
			simulation.add_shape(ball);
			}
			
			x++;
		}

		if(simulation.get_elapsed_time().count()/1e6 >= 30)
		{
			simulation.w2->pos = Vector(1000000, 0, 0);
		}
	});
	//*/


	/*
		 _   _             _               _   _            _                         
	| |_(_)_ __  _ __ (_)_ __   __ _  | |_| |__   ___  | |_ _____      _____ _ __ 
	| __| | '_ \| '_ \| | '_ \ / _` | | __| '_ \ / _ \ | __/ _ \ \ /\ / / _ \ '__|
	| |_| | |_) | |_) | | | | | (_| | | |_| | | |  __/ | || (_) \ V  V /  __/ |   
	 \__|_| .__/| .__/|_|_| |_|\__, |  \__|_| |_|\___|  \__\___/ \_/\_/ \___|_|   
	      |_|   |_|            |___/                                              

	*/

	simulation.add_shape(new Parallelepiped(Vector(0, -250, 0), 0, Vector(100, 0, 0), Vector(150, 500, 0), Vector(0, 0, 100), 800, Color(255, 255, 255, 127)));

	simulation.start();
	canvas.start();

	pause();

	return 0;
}
