#pragma once

#include "graphics.hpp"
#include "sdl_utils.hpp"

#include <SDL3/SDL.h>

#include <string>

namespace spot {
	class Texture {
	public:
		Texture(std::string path);

		float2 get_size() const;

		void render(float2 pos);
		void render(frect source, frect dest);
		void render(frect source, frect dest, const RenderTransform& transform);

	private:
		sdl::SDLTexture texture;
		float2 size;
	};
}
