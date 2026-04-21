#pragma once

#include "maths.hpp"

#include <SDL3/SDL.h>

#include <optional>

namespace spot {
	enum class FlipMode {
		NONE = SDL_FLIP_NONE,
		HORIZONTAL = SDL_FLIP_HORIZONTAL,
		VERTICAL = SDL_FLIP_VERTICAL,
		BOTH = SDL_FLIP_HORIZONTAL_AND_VERTICAL
	};

	struct RenderTransform {
		float angle = 0.0f;
		std::optional<float2> rotate_around;
		FlipMode flip = FlipMode::NONE;
	};
}
