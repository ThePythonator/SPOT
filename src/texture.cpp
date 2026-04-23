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

	Texture::Texture(SDL_Surface* surface)
		: texture(SDL_CreateTextureFromSurface(GraphicsContext::get().get_renderer(), surface))
		, size(sdl::get_texture_size(texture.get())) {

		// TODO: perhaps make this setting optional in both ctors
		SDL_SetTextureScaleMode(texture.get(), SDL_SCALEMODE_PIXELART);
	}

	float2 Texture::get_size() const {
		return size;
	}

	void Texture::render(float2 pos, AnchorPosition anchor_position) {
		// TODO: consider whether this is inefficient
		// Storing the context as a member might be better since the class
		// isn't small anyway
		// Same for the other render functions
		GraphicsContext::get().render_texture(texture.get(), { { 0, 0 }, size }, sdl::get_render_rect(pos, size, anchor_position));
	}

	void Texture::render(frect dest, AnchorPosition anchor_position) {
		GraphicsContext::get().render_texture(texture.get(), { { 0, 0 }, size }, sdl::get_render_rect(dest, anchor_position));
	}

	void Texture::render(frect source, frect dest, AnchorPosition anchor_position) {
		GraphicsContext::get().render_texture(texture.get(), source, sdl::get_render_rect(dest, anchor_position));
	}

	void Texture::render(frect source, frect dest, const RenderTransform& transform, AnchorPosition anchor_position) {
		// TODO: not sure if this correctly handles anchor pos due to the dest rect being
		// calculated from anchor_position before rotation
		// TODO: test if this behaves as expected
		GraphicsContext::get().render_texture(texture.get(), source, sdl::get_render_rect(dest, anchor_position), transform);
	}
}
