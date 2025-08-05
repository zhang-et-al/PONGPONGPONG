#include "Simulation.hpp"
#include "Shapes/Visor.hpp"
#include "Shapes/Wall.hpp"
#include "Collision/Collider.hpp"

#include <algorithm>
#include <iostream>

Simulation::Simulation(float w, float h, float d, float tickrate, bool sync)
: width(w), height(h), depth(d), synchronous(sync), tickrate_setpoint(tickrate),
delay_setpoint((long) (1e6/tickrate)), actual_delay(delay_setpoint), time_to_wait(delay_setpoint), elapsed_time(0) {

	//*
	Color col = Color::aqua;

	
	w1 = new Wall(Vector(-w/2, 0, 0), Vector(0, 0, d), Vector(0, h, 0), col);
	w2 = new Wall(Vector(w/2, 0, 0), Vector(0, 0, d), Vector(0, h, 0), col);

	w3 = new Wall(Vector(0, 0, -d/2), Vector(w, 0, 0), Vector(0, h, 0), col);
	w4 = new Wall(Vector(0, 0, d/2), Vector(w, 0, 0), Vector(0, h, 0), col);

	w5 = new Wall(Vector(0, -h/2, 0), Vector(w, 0, 0), Vector(0, 0, d), col);
	w6 = new Wall(Vector(0, h/2, 0), Vector(w, 0, 0), Vector(0, 0, d), col);

	add_shape(w1);
	add_shape(w2);
	add_shape(w3);
	add_shape(w4);
	add_shape(w5);
	add_shape(w6);
	//*/
}

Simulation::~Simulation()
{
	for(auto it = shapes.begin(); it != shapes.end(); ++it)
	{
		(*it)->simulation = nullptr;
	}
}

std::chrono::microseconds Simulation::get_time_to_simulate() const
{
	return time_to_simulate;
}

std::chrono::microseconds Simulation::get_actual_delay() const
{
	return actual_delay;
}
void Simulation::set_delay_setpoint(std::chrono::microseconds delay)
{
	delay_setpoint = delay;
	tickrate_setpoint = 1e6/delay.count();
}
void Simulation::set_tickrate_setpoint(float tickrate)
{
	tickrate_setpoint = tickrate;
	delay_setpoint = std::chrono::microseconds((long) (1e6/tickrate));
}


std::chrono::microseconds Simulation::get_delay_setpoint() const
{
	return delay_setpoint;
}

std::chrono::microseconds Simulation::get_time_to_wait() const
{
	return time_to_wait;
}

std::chrono::microseconds Simulation::get_elapsed_time() const
{
	return elapsed_time;
}

void Simulation::set_tick_callback(std::function<void(void)> func)
{
	on_tick = func;
}

void Simulation::add_shape(Shape* shape)
{
	if(shape->get_simulation())
	{
		shape->get_simulation()->remove_shape(shape);
	}
	shapes.push_back(shape);
	shape->simulation = this;
}

std::vector<Shape*> Simulation::get_shapes() const
{
	return shapes;
}

void Simulation::remove_shape(Shape* shape)
{
	shapes.erase(std::find(shapes.begin(), shapes.end(), shape));
	shape->simulation = nullptr;
}

void Simulation::tick()
{
	if(!paused && on_tick) on_tick();

	for(auto it = shapes.begin(); it != shapes.end(); ++it)
	{
		Shape* shape = *it;
		if(!paused || shape->type() == Shape::Type::visor)
		{
			for(auto i = it; i != shapes.end(); ++i)
			{
				Collider::try_colliding(shape, *i);
			}
			shape->tick_motion(actual_delay.count()/1e6);
		}
	}
}

void Simulation::loop()
{
	while(run)
	{
		auto start = std::chrono::high_resolution_clock::now();
		
		tick();

		auto end = std::chrono::high_resolution_clock::now();

		time_to_simulate = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

		if(synchronous)
		{
			time_to_wait = delay_setpoint;
		}
		else
		{
			time_to_wait = time_to_simulate > delay_setpoint ? std::chrono::microseconds(0) : delay_setpoint - time_to_simulate;
		}

		actual_delay = time_to_simulate + time_to_wait;

		if(!paused)
		{
			if(synchronous)
			{
				elapsed_time += time_to_wait;
			}
			else
			{
				elapsed_time += actual_delay;
			}
		}

		std::this_thread::sleep_for(time_to_wait);
	}
}

void Simulation::start()
{
	thread = std::thread(&Simulation::loop, this);

	started_at = std::chrono::high_resolution_clock::now();

	thread.detach();
}

void Simulation::end()
{
	run = false;
}