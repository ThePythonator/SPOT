#include <spot/sdl_utils.hpp>

namespace spot::sdl {
	SDLTexture load_texture(SDL_Renderer* renderer, const std::string& path) {
		return SDLTexture(IMG_LoadTexture(renderer, path.c_str()));
	}

	float2 get_texture_size(SDL_Texture* texture) {
		float w, h;
		SDL_GetTextureSize(texture, &w, &h);
		return float2(w, h);
	}

	void render_texture(SDL_Renderer* renderer, SDL_Texture* texture, frect source, frect dest) {
		SDL_FRect src_rect = to_sdl_rect(source);
		SDL_FRect dest_rect = to_sdl_rect(dest);
		SDL_RenderTexture(renderer, texture, &src_rect, &dest_rect);
	}

	void render_texture(SDL_Renderer* renderer, SDL_Texture* texture, frect source, frect dest, float angle, float2 center, SDL_FlipMode flip_mode) {
		SDL_FRect src_rect = to_sdl_rect(source);
		SDL_FRect dest_rect = to_sdl_rect(dest);
		SDL_FPoint rotation_center{ center.x, center.y };
		SDL_RenderTextureRotated(renderer, texture, &src_rect, &dest_rect, angle, &rotation_center, flip_mode);
	}

	inline SDL_FRect to_sdl_rect(frect rect) {
		return { rect.pos.x, rect.pos.y, rect.size.x, rect.size.y };
	}
}
