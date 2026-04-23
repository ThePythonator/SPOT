#pragma once

#include <SDL3/SDL_pixels.h>

#include <cstdint>

namespace spot {
	struct Colour {
		uint8_t r, g, b, a;

		Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff);

		SDL_Color to_sdl();
	};
}
