#include <spot/graphics_context.hpp>

#include <format>
#include <stdexcept>

namespace spot {
	GraphicsContext::GraphicsContext(const std::string& title, uint2 screen_size)
		: screen_size{ screen_size } {

		if (instance) {
			throw std::runtime_error("GraphicsContext already exists!");
		}

		if (!SDL_Init(SDL_INIT_VIDEO)) {
			// TODO: move this to a utility function taking a std::string?
			std::string error_message = std::format("SDL could not initialise! SDL error: {}", SDL_GetError());
			SDL_Log("%s\n", error_message.c_str());
			throw std::runtime_error(error_message);
		}

		SDL_WindowFlags flags = 0;
		if (!SDL_CreateWindowAndRenderer(title.c_str(), screen_size.x, screen_size.y, flags, &window, &renderer)) {
			SDL_Quit();
			std::string error_message = std::format("Window could not be created! SDL error: {}", SDL_GetError());
			SDL_Log("%s\n", error_message.c_str());
			throw std::runtime_error(error_message);
		}

		instance = this;
	}

	GraphicsContext::~GraphicsContext() {
		if (renderer) SDL_DestroyRenderer(renderer);
		if (window) SDL_DestroyWindow(window);

		SDL_Quit();
		instance = nullptr;
	}

	GraphicsContext& GraphicsContext::get() {
		if (!instance) {
			throw std::runtime_error("GraphicsContext not initialised!");
		}
		return *instance;
	}

	bool GraphicsContext::is_initialised() {
		return instance;
	}

	void GraphicsContext::clear(const Colour& fill_colour) {
		SDL_SetRenderDrawColor(renderer, fill_colour.r, fill_colour.g, fill_colour.b, fill_colour.a);
		SDL_RenderClear(renderer);
	}

	void GraphicsContext::render_texture(SDL_Texture* texture, frect source, frect dest) {
		sdl::render_texture(renderer, texture, source, dest);
	}

	void GraphicsContext::render_texture(SDL_Texture* texture, frect source, frect dest, const RenderTransform& transform) {
		SDL_FlipMode flip_mode = static_cast<SDL_FlipMode>(transform.flip);
		float2 center = transform.rotate_around.value_or(dest.size / 2.0f);
		sdl::render_texture(renderer, texture, source, dest, transform.angle, center, flip_mode);
	}
}
