#pragma once

#include "maths.hpp"

#include <SDL3/SDL.h>

#include <optional>

namespace spot {
	// Simple mapping to SDL_FlipMode
	enum class FlipMode {
		NONE = SDL_FLIP_NONE,
		HORIZONTAL = SDL_FLIP_HORIZONTAL,
		VERTICAL = SDL_FLIP_VERTICAL,
		BOTH = SDL_FLIP_HORIZONTAL_AND_VERTICAL
	};

	enum AnchorPosition {
		LEFT = 0b001000,
		CENTER_X = 0b010000,
		RIGHT = 0b100000,

		TOP = 0b000001,
		CENTER_Y = 0b000010,
		BOTTOM = 0b000100,

		TOP_RIGHT = TOP | RIGHT,
		TOP_CENTER = TOP | CENTER_X,
		TOP_LEFT = TOP | LEFT,

		CENTER_RIGHT = CENTER_Y | RIGHT,
		CENTER_CENTER = CENTER_X | CENTER_Y,
		CENTER_LEFT = CENTER_Y | LEFT,

		BOTTOM_RIGHT = BOTTOM | RIGHT,
		BOTTOM_CENTER = BOTTOM | CENTER_X,
		BOTTOM_LEFT = BOTTOM | LEFT
	};

	// Represents rotation and flipping
	struct RenderTransform {
		float angle = 0.0f;
		std::optional<float2> rotate_around;
		FlipMode flip = FlipMode::NONE;
	};
}
