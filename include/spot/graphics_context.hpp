#pragma once

#include "colour.hpp"
#include "graphics.hpp"
#include "maths.hpp"
#include "sdl_utils.hpp"

#include <SDL3/SDL.h>

#include <memory>
#include <string>

namespace spot {
	// Handles initialisation and management of the rendering systems
	class GraphicsContext {
	public:
		GraphicsContext(const std::string& title, uint2 screen_size);
		~GraphicsContext();

		// Prevent copies
		GraphicsContext(const GraphicsContext&) = delete;
		GraphicsContext& operator=(const GraphicsContext&) = delete;

		// Static accessor
		static GraphicsContext& get();
		static bool is_initialised();

		// TODO: ideally we don't want to expose these
		SDL_Renderer* get_renderer() { return renderer; }
		SDL_Window* get_window() { return window; }

		void clear(Colour fill_colour);

		void render_texture(SDL_Texture* texture, frect source, frect dest);
		void render_texture(SDL_Texture* texture, frect source, frect dest, const RenderTransform& transform);
		
	private:
		inline static GraphicsContext* instance = nullptr;

		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;

		uint2 screen_size;
	};
}
