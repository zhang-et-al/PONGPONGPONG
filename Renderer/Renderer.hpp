#pragma once

#include "../Shapes/Visor.hpp"

#include <cstdint>
#include <chrono>
#include <barrier>
#include <thread>

class Visor;

struct Projected_point
{
	Color color;
	float z;
};

class Renderer
{
public:
	Renderer(int w, int h, float fov, float fr, bool alpha);
	~Renderer();

	const int worker_count;

	// Frame dimensions
	int get_width() const;
	int get_height() const;
	virtual void set_width(int width) = 0;
	virtual void set_height(int height) = 0;
	virtual void set_size(int width, int height) = 0;

	// FOV
	float get_fov() const;
	void set_fov(float fov);

	// Time taken to render last frame and the delay added after
	std::chrono::microseconds get_time_to_render() const;
	std::chrono::microseconds get_delay_setpoint() const;
	void set_delay_setpoint(std::chrono::microseconds delay);
	void set_framerate_setpoint(float fr);
	std::chrono::microseconds get_actual_delay() const;
	std::chrono::microseconds get_time_to_wait() const;
	std::chrono::microseconds get_time_to_segment() const;

	void translate_point(Vector& p);
	void project_point(Vector& p);
	//void adjust_point(double& x, double& y);
	//void adjust_point(int& x, int& y);
	void set_point(int x, int y, float z, Color col);
	virtual void render_pixel(int x, int y, uint32_t color) = 0;

	void draw_spatial_line(Vector a, Vector b, Color color);
	void draw_spatial_triangle(const Triangle& tri, bool outline = false, bool skiptrans = false);

	//virtual void update();
	void start();
	void end();

	void set_visor(Visor* v);
	Visor* get_visor() const;
	void remove_visor();
	
protected:
	int width, height;
	float FOV, FOV_factor;

	// The time it takes to simulate 1 tick, by definition the inverse of tickrate_setpoint
	std::chrono::microseconds time_to_render;

	// The amount of ticks every second, by definition the inverse of time_to_render
	float framerate_setpoint;

	// The ideal delay between each tick
	std::chrono::microseconds delay_setpoint;

	// The actual delay between each tick. Ideally should always equal the delay setpoint, but will inevitably be larger
	// if time_to_simulate exceeds delay_setpoint
	std::chrono::microseconds actual_delay;

	// The time the program sleeps for after rendering each frame. When time_to_render >= delay_setpoint, it's zero
	std::chrono::microseconds time_to_wait;

	std::chrono::microseconds time_to_segment;

	bool alpha_blending;

	Projected_point** current_image = nullptr;
	void initialize_image();
	void wipe_image();
	void free_image();

	void fetch_triangles();

	void work(int triangle_chunk_index);
	//void completion_function();

	virtual void pre_render() { }
	virtual void post_render() { }

	void loop(int triangle_chunk_index);

	bool run = true;

	std::barrier<std::function<void(void)>> sync_point;

	std::vector<std::vector<Triangle>> triangle_chunks;

	std::vector<std::jthread> workers;

	Visor* visor = nullptr;

	Vector visor_pos = 0;
	
	int total_triangle_count = 0;
	int rendered_triangle_count = 0;
	int last_chunk_size = 0;

	// if we have no more than 500 triangles don't dedicate a new thread
	const int triangle_thread_threshold = 200;

private:
	void draw_spatial_triangle_experimental(const Triangle& tri, bool outline = false);
};