#include <spot/spritesheet.hpp>

namespace spot {
	Spritesheet::Spritesheet(std::string path, uint32_t sprite_size, float scale)
		: texture{ path }
		, sprite_size{ sprite_size }
		, original_sprite_size{ static_cast<float>(sprite_size), static_cast<float>(sprite_size) }
		, scaled_sprite_size{ sprite_size * scale, sprite_size * scale }
		, row_width{ static_cast<uint32_t>(texture.get_size().x / sprite_size) }
		, scale{ scale } {

	}

	void Spritesheet::sprite(uint32_t index, float2 pos) {
		frect source{ get_source_pos(index), original_sprite_size};
		frect dest{ pos, scaled_sprite_size };
		texture.render(source, dest);
	}

	void Spritesheet::sprite(uint32_t index, float2 pos, const RenderTransform& transform) {
		frect source{ get_source_pos(index), original_sprite_size };
		frect dest{ pos, scaled_sprite_size };
		texture.render(source, dest, transform);
	}

	float2 Spritesheet::get_source_pos(uint32_t index) const {
		uint32_t y = index / row_width;
		uint32_t x = index - y * row_width;
		return { static_cast<float>(x * sprite_size), static_cast<float>(y * sprite_size) };
	}
}
