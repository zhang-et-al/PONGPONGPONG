#pragma once

#include <cstdint>

#include <SDL2/SDL.h>

struct Color : SDL_Color
{
	//uint8_t r, g, b, a;

	Color(uint8_t R = 0, uint8_t G = 0, uint8_t B = 0, uint8_t A = 255) : SDL_Color(R, G, B, A) {}

	// Hex to RGB
	Color(uint32_t hex)
	: Color((hex >> 16) & 0xff, (hex >> 8) & 0xff, hex & 0xff) {}

	inline Color operator*(float t) const
	{
		return Color(r * t, g * t, b * t, a);
	}
	inline Color operator+(Color c) const
	{
		return Color(r + c.r, g + c.g, b + c.b, a);
	}

	inline Color invert() const
	{
		return Color(255-r, 255-g, 255-b, a);
	}

	inline bool operator==(const Color& c) const
	{
		return r == c.r && g == c.g && b == c.b && a == c.a;
	}

	inline uint32_t hex() const
	{
		//return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff); // unncessary because r g b and are already 8 bit
		return (r << 16) + (g << 8) + b;
	}

	void alpha_blend_foreground(Color foreground);

	static Color random();

	static Color white, transparent, silver, gray, black, red, maroon, yellow, olive, lime, green, aqua, teal, blue, navy, fuchsia, purple;
};