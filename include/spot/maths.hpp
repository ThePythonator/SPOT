#pragma once

#include <SDL3/SDL_rect.h>

#include <linalg.h>

namespace spot {
	using namespace linalg::aliases;

	struct frect {
		float2 pos;
		float2 size;

		float2 center() {
			return { pos.x + size.x / 2, pos.y + size.y / 2 };
		}

		SDL_FRect to_sdl() {
			return { pos.x, pos.y, size.x, size.y };
		}
	};

	// Returns true if the two supplied rects are overlapping
	bool colliding(spot::frect rect_a, spot::frect rect_b);
}
