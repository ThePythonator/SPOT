#pragma once

#include "graphics_context.hpp"
#include "maths.hpp"
#include "texture.hpp"

#include <string>

namespace spot {
	class Spritesheet {
	public:
		Spritesheet(std::string path, uint32_t sprite_size, float scale = 1.0f);

		void sprite(uint32_t index, float2 pos);
		void sprite(uint32_t index, float2 pos, const RenderTransform& transform);

	private:
		float2 get_source_pos(uint32_t index) const;

		Texture texture;
		uint32_t sprite_size;
		// Width of the spritesheet in sprites
		uint32_t row_width;
		float2 original_sprite_size;
		float2 scaled_sprite_size;
		float scale;
	};
}
