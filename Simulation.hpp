#pragma once


#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include <functional>

class Shape;
class Wall;

class Simulation
{
public:
	~Simulation();
	
	Simulation(float w, float h, float d, float tickrate = 1000, bool sync = false);

	std::chrono::microseconds get_time_to_simulate() const;
	std::chrono::microseconds get_actual_delay() const;
	void set_delay_setpoint(std::chrono::microseconds delay);
	void set_tickrate_setpoint(float tickrate);
	std::chrono::microseconds get_delay_setpoint() const;
	std::chrono::microseconds get_time_to_wait() const;

	std::chrono::microseconds get_elapsed_time() const;

	void set_tick_callback(std::function<void(void)> func);

	float width, height, depth;

	void add_shape(Shape* shape);

	std::vector<Shape*> get_shapes() const;

	void remove_shape(Shape* shape);

	void tick();

	void start();

	void end();

	bool paused = false;

	bool synchronous = false;

	Wall *w1, *w2, *w3, *w4, *w5, *w6;
private:
	// The time it takes to simulate 1 tick, by definition the inverse of tickrate_setpoint
	std::chrono::microseconds time_to_simulate;

	// The amount of ticks every second, by definition the inverse of time_to_simulate
	float tickrate_setpoint;

	// The ideal delay between each tick
	std::chrono::microseconds delay_setpoint;

	// The time point when the stimulation started
	std::chrono::time_point<std::chrono::system_clock> started_at;

	// The actual delay between each tick. Ideally should always equal the delay setpoint, but will inevitably be larger
	// if time_to_simulate exceeds delay_setpoint
	std::chrono::microseconds actual_delay;

	// The time the program sleeps for after simulating each tick. When time_to_simulate >= delay_setpoint, it's zero
	std::chrono::microseconds time_to_wait;

	// The time since the simulation started. Stops when paused
	std::chrono::microseconds elapsed_time;
	
	std::thread thread;

	void loop();

	std::atomic<bool> run = true;

	std::function<void(void)> on_tick;

	// Private because there has to be a wrapper function for push_back() that tells each shape
	// which simulation it belongs to
	std::vector<Shape*> shapes;

};