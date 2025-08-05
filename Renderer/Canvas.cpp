#include "Canvas.hpp"
#include "../Shapes/Arrow.hpp"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <unistd.h>


Canvas::Canvas(int w, int h, float fov, float fr, bool alpha)
: Renderer(w, h, fov, fr, alpha)
{
	// Initializes SDL
	SDL_Init(SDL_INIT_VIDEO);
	
	//SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &main_renderer);
	window = SDL_CreateWindow("PONGPONGPONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	//main_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetWindowResizable(window, (SDL_bool)true);
	
	window_surface = SDL_GetWindowSurface(window);

	if(TTF_Init() < 0)
	{
		std::cout << "Error intializing SDL_ttf: " << TTF_GetError() << std::endl;
		exit(1);
	}


	std::array<std::string, 3> font_candidates {
		"/usr/share/fonts/Helvetica-Font/Helvetica.ttf", // linux (good taste)
		"/usr/share/fonts/noto/NotoSans-Regular.ttf", // linux
		"/system/fonts/DroidSansMono.ttf" // android
		// no fonts for macwinglows users
	};
	
	for(const std::string& font_cand : font_candidates)
	{
		if(std::filesystem::exists(font_cand))
		{ 
			font = TTF_OpenFont(font_cand.c_str(), 18);
			outline_font = TTF_OpenFont(font_cand.c_str(), 18);
			TTF_SetFontOutline(outline_font, 1);
			break;
		}
	}
}

void Canvas::set_width(int w)
{
	width = w;

	SDL_SetWindowSize(window, w, height);

	window_surface = SDL_GetWindowSurface(window);
}
void Canvas::set_height(int h)
{
	height = h;

	SDL_SetWindowSize(window, width, h);

	window_surface = SDL_GetWindowSurface(window);
}
void Canvas::set_size(int w, int h)
{
	width = w;
	height = h;

	SDL_SetWindowSize(window, w, h);

	window_surface = SDL_GetWindowSurface(window);
}

void Canvas::display_info()
{
	// The total energy of the simulation. Ideally, it should be constant
	float total_energy = 0;
	for(const Shape* s : visor->get_simulation()->get_shapes())
	{
		total_energy += s->mass * (s->vel.modulus2() / 2 + (s->pos.y - s->contact_radius) * s->accel.y);
	}
	
	//static SDL_Texture* text_texture;
	//static SDL_Rect text_rect;
	char info[1024];

	Simulation* sim = visor->get_simulation();

	sprintf(info,
		"X = %f\n"
		"Y = %f\n"
		"Z = %f\n"
		"Azimuth = %f\n"
		"Altitude = %f\n"
		"p = %f\n"
		"time to simulate = %.3fms\n"
		"simulation gap = %.3fms\n"
		"time to render = %.3fms\n"
		"time to segment = %.3fms\n"
		"render gap = %.3fms\n"
		"render delay setp. = %fms\n"
		"elapsed = %fs\n"
		"shapes = %ld\n"
		"triangles = %d\n",
		visor->pos.x, visor->pos.y, visor->pos.z, visor->get_azimuth(), visor->get_altitude(), total_energy, sim->get_time_to_simulate().count()/1e3,
		sim->get_time_to_wait().count()/1e3, time_to_render.count()/1e3, time_to_segment.count()/1e3, time_to_wait.count()/1e3, delay_setpoint.count()/1e3, sim->get_elapsed_time().count()/1e6,
		sim->get_shapes().size(), rendered_triangle_count);

	SDL_Surface* surface = TTF_RenderText_Shaded_Wrapped(font, info, Color::aqua, Color::black, 0);
	
	if(!surface)
	{
		std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
		return;
	}

	SDL_Rect rect = {.x = 10, .y = 10, .w = surface->w, .h = surface->h};
	SDL_BlitSurface(surface, NULL, window_surface, &rect);

	for(int x = 0; x < surface->w; ++x)
	{
		render_pixel(x + 10, 9, Color::gray.hex());
		render_pixel(x + 10, surface->h + 10, Color::gray.hex());
	}

	for(int y = 0; y < surface->h; ++y)
	{
		render_pixel(9, y + 10, Color::gray.hex());
		render_pixel(10 + surface->w, y + 10, Color::gray.hex());
	}

	SDL_FreeSurface(surface);
}


void Canvas::wipe()
{
	SDL_memset(window_surface->pixels, 0, window_surface->h * window_surface->pitch);
}

void Canvas::render_pixel(int x, int y, Uint32 color)
{
	*((Uint32*) ((Uint8*) window_surface->pixels + y * window_surface->pitch + x * window_surface->format->BytesPerPixel)) = color;
}
void Canvas::set_row(int begin_x, int end_x, int y, Uint32 color)
{
	static Uint32* target_pixel;

	target_pixel = (Uint32*) ((Uint8*) window_surface->pixels + y * window_surface->pitch + begin_x * window_surface->format->BytesPerPixel);
	//*target_pixel = color;
	memset(target_pixel, color, (end_x - begin_x) * window_surface->format->BytesPerPixel);
}


void Canvas::listen_input_events()
{
	static SDL_Event event;	
	
	if(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) {
			visor->get_simulation()->end();
			end();
			SDL_DestroyWindow(Canvas::window);
			SDL_Quit();
			exit(0);
		}
		else if(event.type == SDL_WINDOWEVENT) {
			// Window
			if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				// Must be called before changing variables width and height, as otherwise would try to free
				// pointers on an array of the wrong size, leading to segfault
				free_image();

				width = event.window.data1;
				height = event.window.data2;

				// Yes, it must be renewed after resizing
				window_surface = SDL_GetWindowSurface(window);
				
				initialize_image();
				set_fov(FOV);
			}
		}
		// We listen for this particular key press this way because we're only interested in receiving the instant the key is pressed down, not
		// on the key being currently pressed
		else if(event.type == SDL_KEYDOWN) {
			if(event.key.keysym.sym == SDLK_SPACE)
				visor->get_simulation()->paused = !visor->get_simulation()->paused;

			if(event.key.keysym.sym == SDLK_r)
				visor->rugged = !visor->rugged;

			if(visor->get_simulation()->paused && event.key.keysym.sym == SDLK_f) {
				visor->get_simulation()->paused = false;

				visor->get_simulation()->tick();

				visor->get_simulation()->paused = true;
			}
		}
	}
	
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	
	// Quit on Ctrl + Q
	if((keystate[SDL_SCANCODE_LCTRL] || keystate[SDL_SCANCODE_RCTRL]) && keystate[SDL_SCANCODE_Q])
	{
		SDL_DestroyWindow(Canvas::window);
		SDL_Quit();
		exit(0);
	}

	float step = 6;

	if(keystate[SDL_SCANCODE_LCTRL] || keystate[SDL_SCANCODE_RCTRL])
	{
		step = 2;
	}

	float azimuth = visor->get_azimuth(), altitude = visor->get_altitude();

	if(visor->rugged) {
		visor->vel = Vector(0, 0, 0);
		visor->pos.x = roundf(visor->pos.x);
		visor->pos.y = roundf(visor->pos.y);
		visor->pos.z = roundf(visor->pos.z);
		if(keystate[SDL_SCANCODE_UP]) {
			if(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]) {
				visor->pos.x -= step * sin(azimuth);
				visor->pos.z += step * cos(azimuth);
			} else
				visor->pos.y += step;
		}
		if(keystate[SDL_SCANCODE_DOWN]) {
			if(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]) {
				visor->pos.x += step * sin(azimuth);
				visor->pos.z -= step * cos(azimuth);
			} else
				visor->pos.y -= step;
		}
		
		if(keystate[SDL_SCANCODE_LEFT]) {
			visor->pos.x -= step * cos(azimuth);
			visor->pos.z += step * sin(-azimuth);
		}
		if(keystate[SDL_SCANCODE_RIGHT]) {
			visor->pos.x += step * cos(azimuth);
			visor->pos.z -= step * sin(-azimuth);
		}
	} else {
		if(keystate[SDL_SCANCODE_UP]) {
			if(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]) {
				visor->vel.x -= 3 * step * sin(azimuth);
				visor->vel.z += 3 * step * cos(azimuth);
			} else
				visor->vel.y += 3 * step;
		}
		
		if(keystate[SDL_SCANCODE_DOWN]) {
			if(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]) {
				visor->vel.x += 3 * step * sin(azimuth);
				visor->vel.z -= 3 * step * cos(azimuth);
			} else
				visor->vel.y -= 3 * step;
		}
		
		if(keystate[SDL_SCANCODE_LEFT]) {
			visor->vel.x -= 3 * step * cos(azimuth);
			visor->vel.z += 3 * step * sin(-azimuth);
		} if(keystate[SDL_SCANCODE_RIGHT]) {
			visor->vel.x += 3 * step * cos(azimuth);
			visor->vel.z -= 3 * step * sin(-azimuth);
		}
	}

	if(keystate[SDL_SCANCODE_W]) altitude += step / 200;
	
	if(keystate[SDL_SCANCODE_S]) altitude -= step / 200;
	
	if(keystate[SDL_SCANCODE_A]) azimuth += step / 200;

	if(keystate[SDL_SCANCODE_D]) azimuth -= step / 200;

	visor->set_azimuth(azimuth);
	visor->set_altitude(altitude);
	
	SDL_PumpEvents();
}

void Canvas::render_tags()
{
	for(Shape* shape : visor->get_simulation()->get_shapes())
	{
		for(const Label& label : shape->labels)
		{
			std::string text = label.text();

			if(text.empty()) continue;

			Vector pos = shape->pos + label.pos;

			if(shape->type() == Shape::Type::arrow) pos += ((Arrow*) shape)->get_direction() * 0.75;

			translate_point(pos);
			project_point(pos);

			if(pos.x < 0 || pos.x > width || pos.y < 0 || pos.y > height || pos.z < 0 || isnan(pos.x) || isnan(pos.y) || isnan(pos.z))
				continue;


			SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), label.color);
			SDL_Surface* outline_surface = TTF_RenderText_Blended(outline_font, text.c_str(), label.color.invert());
			
			
			if(!surface || !outline_surface)
			{
				std::cout << "Canvas::render_tags() Failed to render text: " << TTF_GetError() << std::endl;
				exit(1);
			}

			// X marks the spot
			Uint32 gray = Color::gray.hex();
			render_pixel(pos.x, pos.y, gray);
			render_pixel(pos.x+1, pos.y+1, gray);
			render_pixel(pos.x-1, pos.y-1, gray);
			render_pixel(pos.x+1, pos.y-1, gray);
			render_pixel(pos.x-1, pos.y+1, gray);

			// Renders the outline font
			SDL_Rect rect = {.x = (int)pos.x + 4, .y = (int)pos.y + 4, .w = surface->w, .h = surface->h};
			SDL_BlitSurface(outline_surface, NULL, window_surface, &rect);

			// And on top of it the normal font
			rect = {.x = (int)pos.x + 5, .y = (int)pos.y + 5, .w = surface->w, .h = surface->h};
			SDL_BlitSurface(surface, NULL, window_surface, &rect);

			SDL_FreeSurface(surface);
			SDL_FreeSurface(outline_surface);
		}
	}
}

void Canvas::pre_render()
{
	listen_input_events();

	wipe();

}

void Canvas::post_render()
{
	display_info();

	render_tags();

	SDL_UpdateWindowSurface(window);
}