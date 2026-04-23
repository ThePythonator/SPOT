#include <spot/font.hpp>

#include <format>
#include <stdexcept>

namespace spot {
//#ifdef SDL_TTF_MAJOR_VERSION
	TTFFont::TTFFont(const std::string& path, float size)
		: font(TTF_OpenFont(path.c_str(), size)){

		if (font == nullptr) {
			throw std::runtime_error(std::format("Failed to load font {}! SDL error: {}", path, SDL_GetError()));
		}
	}

	TTFFont::~TTFFont() {
		TTF_CloseFont(font);
	}

	Texture TTFFont::render_text_to_texture(const std::string& text, Colour colour) {
		SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), text.size(), colour.to_sdl());

		Texture texture{ surface };
		SDL_DestroySurface(surface);

		return texture;
	}

	void TTFFont::render_text(const std::string& text, float2 pos, Colour colour, AnchorPosition anchor_position) {
		Texture texture = render_text_to_texture(text, colour);
		texture.render(pos, anchor_position);
	}

	void TTFFont::render_text(const std::string& text, float2 pos, Colour colour, float scale, AnchorPosition anchor_position) {
		Texture texture = render_text_to_texture(text, colour);
		float2 size = texture.get_size();
		texture.render({ pos, size * scale }, anchor_position);
	}
//#endif
}
