#include "Color.hpp"
#include <algorithm>

void Color::alpha_blend_foreground(Color foreground)
{
	r = r * (255.0f - foreground.a)/255.0f * (a/255.0f) + foreground.r * (foreground.a/255.0f);
	g = g * (255.0f - foreground.a)/255.0f * (a/255.0f) + foreground.g * (foreground.a/255.0f);
	b = b * (255.0f - foreground.a)/255.0f * (a/255.0f) + foreground.b * (foreground.a/255.0f);

	a = std::min(foreground.a + a, 255);
}

Color Color::random()
{
	return Color(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
}

Color Color::white(255, 255, 255);
Color Color::transparent(0, 0, 0, 0);
Color Color::silver(191, 191, 191);
Color Color::gray(127, 127, 127);
Color Color::black(0, 0, 0);
Color Color::red(255, 0, 0);
Color Color::maroon(127, 0, 0);
Color Color::yellow(255, 255, 0);
Color Color::olive (127, 127, 0);
Color Color::lime(0, 255, 0);
Color Color::green(0, 127, 0);
Color Color::aqua(0, 255, 255);
Color Color::teal(0, 127, 127);
Color Color::blue(0, 0, 255);
Color Color::navy(0, 0, 127);
Color Color::fuchsia(255, 0, 255);
Color Color::purple(127, 0, 127);