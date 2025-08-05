#pragma once

#include "Renderer.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Canvas : public Renderer
{
public:

	Canvas(int w, int h, float fov = 2.4, float fr = 60, bool alpha = true);

	void display_info();

	void render_tags();

	void render_pixel(int x, int y, uint32_t color);
	

	void pre_render();
	void post_render(); 


	void set_width(int width);
	void set_height(int height);
	void set_size(int width, int height);
	
	void set_row(int begin_x, int end_x, int y, Uint32 color);

	void listen_input_events();

	void wipe();

private:

	SDL_Window* window;
	SDL_Surface* window_surface;
	TTF_Font* font;
	TTF_Font* outline_font;
};