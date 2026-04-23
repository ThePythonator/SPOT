#include <spot/colour.hpp>

namespace spot {
	Colour::Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: r(r)
		, g(g)
		, b(b)
		, a(a) {

	}

	SDL_Color Colour::to_sdl() {
		return { r, g, b, a };
	}
}
