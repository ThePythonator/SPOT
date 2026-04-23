#pragma once

#include "graphics.hpp"
#include "sdl_utils.hpp"

#include <SDL3/SDL.h>

#include <string>

namespace spot {
	class Texture {
	public:
		Texture(std::string path);
		Texture(SDL_Surface* surface);

		float2 get_size() const;

		void render(float2 pos, AnchorPosition anchor_position = AnchorPosition::TOP_LEFT);
		void render(frect dest, AnchorPosition anchor_position = AnchorPosition::TOP_LEFT);
		void render(frect source, frect dest, AnchorPosition anchor_position = AnchorPosition::TOP_LEFT);
		void render(frect source, frect dest, const RenderTransform& transform, AnchorPosition anchor_position = AnchorPosition::TOP_LEFT);

	private:
		sdl::SDLTexture texture;
		float2 size;
	};
}
