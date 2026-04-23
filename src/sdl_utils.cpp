#include <spot/sdl_utils.hpp>

#include <filesystem>
#include <format>
#include <stdexcept>

namespace spot::sdl {
	SDLTexture load_texture(SDL_Renderer* renderer, const std::string& path) {
		if (!std::filesystem::exists(path)) {
			throw std::runtime_error(std::format("Failed to open {}: file does not exist!", path));
		}
		SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
		if (texture == nullptr) {
			throw std::runtime_error(std::format("Failed to load {}! SDL error: {}", path, SDL_GetError()));
		}
		return SDLTexture(texture);
	}

	float2 get_texture_size(SDL_Texture* texture) {
		float w, h;
		SDL_GetTextureSize(texture, &w, &h);
		return float2(w, h);
	}

	float2 get_render_position(float2 position, float2 size, AnchorPosition anchor_position) {
		// Horizontal
		if (anchor_position & AnchorPosition::RIGHT) {
			position.x -= size.x;
		}
		else if (anchor_position & AnchorPosition::CENTER_X) {
			position.x -= size.x / 2.0f;
		}

		// Vertical
		if (anchor_position & AnchorPosition::BOTTOM) {
			position.y -= size.y;
		}
		else if (anchor_position & AnchorPosition::CENTER_Y) {
			position.y -= size.y / 2.0f;
		}

		return position;
	}

	float2 get_render_position(frect rect, AnchorPosition anchor_position) {
		return get_render_position(rect.pos, rect.size, anchor_position);
	}

	frect get_render_rect(float2 position, float2 size, AnchorPosition anchor_position) {
		return { get_render_position(position, size, anchor_position), size };
	}

	frect get_render_rect(frect rect, AnchorPosition anchor_position) {
		return get_render_rect(rect.pos, rect.size, anchor_position);
	}

	void render_texture(SDL_Renderer* renderer, SDL_Texture* texture, frect source, frect dest) {
		SDL_FRect src_rect = source.to_sdl();
		SDL_FRect dest_rect = dest.to_sdl();
		SDL_RenderTexture(renderer, texture, &src_rect, &dest_rect);
	}

	void render_texture(SDL_Renderer* renderer, SDL_Texture* texture, frect source, frect dest, float angle, float2 center, SDL_FlipMode flip_mode) {
		SDL_FRect src_rect = source.to_sdl();
		SDL_FRect dest_rect = dest.to_sdl();
		SDL_FPoint rotation_center{ center.x, center.y };
		SDL_RenderTextureRotated(renderer, texture, &src_rect, &dest_rect, angle, &rotation_center, flip_mode);
	}
}
