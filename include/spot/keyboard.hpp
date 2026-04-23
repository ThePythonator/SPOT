#pragma once

#include <SDL3/SDL.h>

#include <array>

namespace spot {
	class Keyboard {
	public:
		Keyboard();

		// Must get called every frame - necessary for checking
		// whether keys have just been pressed or released
		void update();

		// Can overload these if needed
		bool is_pressed(SDL_Scancode scancode) const;
		bool just_pressed(SDL_Scancode scancode) const;
		bool just_released(SDL_Scancode scancode) const;

	private:
		const bool* sdl_keyboard_state = nullptr;
		std::array<bool, SDL_SCANCODE_COUNT> last_state_buffer, current_state_buffer;
	};
}
