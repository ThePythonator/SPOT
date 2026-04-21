#pragma once

#include "maths.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <memory>
#include <string>

namespace spot::sdl {
	struct SDLDeleter {
		void operator()(SDL_Window* p) const { SDL_DestroyWindow(p); }
		void operator()(SDL_Renderer* p) const { SDL_DestroyRenderer(p); }
		void operator()(SDL_Texture* p) const { SDL_DestroyTexture(p); }
	};

	// RAII versions of SDL datatypes
	using SDLWindow = std::unique_ptr<SDL_Window, SDLDeleter>;
	using SDLRenderer = std::unique_ptr<SDL_Renderer, SDLDeleter>;
	using SDLTexture = std::unique_ptr<SDL_Texture, SDLDeleter>;

	// Replacements for SDL functions
	SDLTexture load_texture(SDL_Renderer* renderer, const std::string& path);
	float2 get_texture_size(SDL_Texture* texture);

	void render_texture(SDL_Renderer* renderer, SDL_Texture* texture, frect source, frect dest);
	//void render_texture(SDL_Renderer* renderer, SDL_Texture* texture, frect source, frect dest, SDL_FlipMode flip_mode);
	void render_texture(SDL_Renderer* renderer, SDL_Texture* texture, frect source, frect dest, float angle, float2 center, SDL_FlipMode flip_mode);

	// TODO: consider moving into maths.hpp
	SDL_FRect to_sdl_rect(frect rect);
}
