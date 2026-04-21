#include <spot/texture.hpp>
#include <spot/graphics_context.hpp>

#include <SDL3_image/SDL_image.h>

#include <format>

namespace spot {
	Texture::Texture(std::string path)
		: texture(sdl::load_texture(GraphicsContext::get().get_renderer(), path))
		, size(sdl::get_texture_size(texture.get())) {

		// If we want colour keying using SDL_SetSurfaceColorKey, need to
		// load the surface first
		SDL_SetTextureScaleMode(texture.get(), SDL_SCALEMODE_PIXELART);
	}

	float2 Texture::get_size() const {
		return size;
	}

	void Texture::render(float2 pos) {
		// TODO: consider whether this is inefficient
		// Storing the context as a member might be better since the class
		// isn't small anyway
		// Same for the other render functions
		GraphicsContext::get().render_texture(texture.get(), { { 0, 0 }, size }, { pos, size });
	}

	void Texture::render(frect source, frect dest) {
		GraphicsContext::get().render_texture(texture.get(), source, dest);
	}

	void Texture::render(frect source, frect dest, const RenderTransform& transform) {
		GraphicsContext::get().render_texture(texture.get(), source, dest, transform);
	}
}
