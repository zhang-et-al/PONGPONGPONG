#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <math.h>

struct Color
{
	uint8_t r, g, b, a;
	inline uint32_t hex() const
	{
		return (r << 24) + (g << 16) + (b << 8) + a;
	}
};

Color hex2rgb(uint32_t hex)
{
	uint8_t r = (hex >> 24) & 0xff;
	uint8_t g = (hex >> 16) & 0xff;
	uint8_t b = (hex >> 8) & 0xff;
	uint8_t a = hex & 0xff;
	return Color {r, g, b, a};
}

int main(int argc, char const *argv[])
{
	Color color{64, 250, 12, 24};
	uint32_t hex = color.hex();
	std::cout << hex << std::endl;
	Color c = hex2rgb(round(hex*0.5));
	printf("%d %d %d\n", c.r, c.g, c.b);
	return 0;
}