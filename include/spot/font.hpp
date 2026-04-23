#pragma once

#include <spot/colour.hpp>
#include <spot/graphics.hpp>
#include <spot/maths.hpp>
#include <spot/texture.hpp>

#include <SDL3_ttf/SDL_ttf.h>

#include <string>

namespace spot {
	class FontInterface {
	public:
		virtual Texture render_text_to_texture(const std::string& text, Colour colour) = 0;
		//virtual Text create_text(std::string text, Colour colour) = 0;
		virtual void render_text(const std::string& text, float2 pos, Colour colour, AnchorPosition anchor_position) = 0;
		virtual void render_text(const std::string& text, float2 pos, Colour colour, float scale, AnchorPosition anchor_position) = 0;
	};
	
//#ifdef SDL_TTF_MAJOR_VERSION
	class TTFFont : public FontInterface {
	public:
		TTFFont(const std::string& path, float size);
		~TTFFont();

		Texture render_text_to_texture(const std::string& text, Colour colour);
		void render_text(const std::string& text, float2 pos, Colour colour, AnchorPosition anchor_position = AnchorPosition::CENTER_CENTER);
		void render_text(const std::string& text, float2 pos, Colour colour, float scale, AnchorPosition anchor_position = AnchorPosition::CENTER_CENTER);

	private:
		TTF_Font* font;
	};
//#endif

	/*class SpritesheetFont : public FontInterface {
	public:
		Texture render_text_to_texture(std::string text, Colour colour);
		void render_text(std::string text, float2 pos, Colour colour, AnchorPosition anchor_position = AnchorPosition::CENTER_CENTER);
		void render_text(std::string text, float2 pos, Colour colour, float scale, AnchorPosition anchor_position = AnchorPosition::CENTER_CENTER);
	};*/
}
