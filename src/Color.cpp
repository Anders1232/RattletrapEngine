#include "Color.h"

Color::Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a ) : r(r), g(g), b(b), a(a) {
}

Color::operator SDL_Color() {
	SDL_Color c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return c;
}
