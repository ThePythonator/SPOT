#include <spot/keyboard.hpp>

#include <algorithm>

namespace spot {
	Keyboard::Keyboard()
		: sdl_keyboard_state{ SDL_GetKeyboardState(nullptr) } {
		// Set current and last buffers to the current keyboard state
		std::copy(sdl_keyboard_state, sdl_keyboard_state + SDL_SCANCODE_COUNT, current_state_buffer.begin());
		last_state_buffer = current_state_buffer;
	}

	void Keyboard::update() {
		// Copy current state to last state
		last_state_buffer = current_state_buffer;
		// Fetch new current state
		std::copy(sdl_keyboard_state, sdl_keyboard_state + SDL_SCANCODE_COUNT, current_state_buffer.begin());
	}

	// Can overload these if needed
	bool Keyboard::is_pressed(SDL_Scancode scancode) const {
		return current_state_buffer[scancode];
	}

	bool Keyboard::just_pressed(SDL_Scancode scancode) const {
		return current_state_buffer[scancode] && !last_state_buffer[scancode];
	}

	bool Keyboard::just_released(SDL_Scancode scancode) const {
		return !current_state_buffer[scancode] && last_state_buffer[scancode];
	}
}
