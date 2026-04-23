#include <spot/spot.hpp>
#include <SDL3/SDL_main.h>

#include <algorithm>
#include <cmath>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

static constexpr float MAX_DT = 1.0f / 20;
static constexpr float TARGET_DT = 1.0f / 60;
static const std::string TITLE = "Table tennis";
static constexpr spot::uint2 WINDOW_SIZE{ 800, 600 };

static constexpr float BALL_INITIAL_VEL = 200.0f;
static constexpr float PADDLE_ACCEL = 5000.0f;
static constexpr float PADDLE_DAMPING = 8.0f;
static constexpr float BALL_SPEED_INCREMENT = 20.0f;
static constexpr float BORDER = 16.0f; // Stop bats going too close to the edge
static constexpr int TARGET_SCORE = 3;

static const std::string BLUE_PADDLE_PATH = "assets/blue_paddle.png";
static const std::string RED_PADDLE_PATH = "assets/red_paddle.png";
static const std::string GREEN_BALL_PATH = "assets/green_ball.png";
static const std::string FONT_PATH = "assets/RobotoMono-Regular.ttf";

static constexpr float FONT_SIZE = 32.0f;

static const spot::Colour COLOUR_WHITE{ 0xff,0xff,0xff,0xff };

class Game {
public:
	Game()
		: blue_paddle(BLUE_PADDLE_PATH)
		, red_paddle(RED_PADDLE_PATH)
		, ball(GREEN_BALL_PATH) {

	}

	bool run() {
		if (!init()) {
			return false;
		}

		uint64_t last_time = SDL_GetTicks();
		bool running = true;
		while (running) {
			uint64_t start_time = SDL_GetTicks();
			float dt = std::min((start_time - last_time) / 1000.0f, MAX_DT);
			last_time = start_time;

			keyboard.update();

			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_EVENT_QUIT) {
					running = false;
				}
			}

			// Early-out if an event resulted in running being set to false
			if (!running) break;

			running = update(dt);
			render();

			// Update the display
			SDL_RenderPresent(graphics.get_renderer());

			// If we were too quick, sleep!
			uint64_t end_time = SDL_GetTicks();
			uint32_t frame_duration = static_cast<uint32_t>(end_time - start_time);
			uint32_t target_duration = static_cast<uint32_t>(TARGET_DT * 1000.0f);
			if (frame_duration < target_duration) {
				SDL_Delay(target_duration - frame_duration);
			}
		}

		quit();

		return true;
	}

	bool init() {
		if (!SDL_SetRenderVSync(graphics.get_renderer(), 1)) {
			SDL_Log("Unable to enable VSync! SDL error: %s\n", SDL_GetError());
			return false;
		}
		start_menu();
		return true;
	}

	void quit() {

	}

	bool update(float dt) {
		if (game_state == GameState::TITLE) {
			if (keyboard.just_pressed(SDL_SCANCODE_SPACE) || keyboard.just_pressed(SDL_SCANCODE_RETURN)) {
				if (current_menu_selection == MenuOptions::PLAY) {
					start_game();
				}
				else {
					// Quit game
					return false;
				}
			}
			if (keyboard.just_pressed(SDL_SCANCODE_UP) || keyboard.just_pressed(SDL_SCANCODE_W)) {
				current_menu_selection = MenuOptions::PLAY;
			}
			if (keyboard.just_pressed(SDL_SCANCODE_DOWN) || keyboard.just_pressed(SDL_SCANCODE_S)) {
				current_menu_selection = MenuOptions::QUIT;
			}
		}
		else if (game_state == GameState::PLAYING) {
			// Respond to user input
			// Blue player
			if (keyboard.is_pressed(SDL_SCANCODE_W)) {
				blue_vel.y -= PADDLE_ACCEL * dt;
			}
			if (keyboard.is_pressed(SDL_SCANCODE_S)) {
				blue_vel.y += PADDLE_ACCEL * dt;
			}
			// Red player
			if (keyboard.is_pressed(SDL_SCANCODE_UP)) {
				red_vel.y -= PADDLE_ACCEL * dt;
			}
			if (keyboard.is_pressed(SDL_SCANCODE_DOWN)) {
				red_vel.y += PADDLE_ACCEL * dt;
			}

			// Apply damping
			blue_vel.y -= blue_vel.y * std::min(PADDLE_DAMPING * dt, 1.0f);
			red_vel.y -= red_vel.y * std::min(PADDLE_DAMPING * dt, 1.0f);

			// Move objects
			blue_pos += blue_vel * dt;
			red_pos += red_vel * dt;
			ball_pos += ball_dir * ball_speed * dt;

			// Stop the blue player from moving their paddle off-screen
			if (blue_pos.y < BORDER) {
				blue_pos.y = BORDER;
				blue_vel.y = 0.0f;
			}
			else if (blue_pos.y + blue_paddle.get_size().y > WINDOW_SIZE.y - BORDER) {
				blue_pos.y = WINDOW_SIZE.y - blue_paddle.get_size().y - BORDER;
				blue_vel.y = 0.0f;
			}
			// Stop the red player from moving their paddle off-screen
			if (red_pos.y < BORDER) {
				red_pos.y = BORDER;
				red_vel.y = 0.0f;
			}
			else if (red_pos.y + red_paddle.get_size().y > WINDOW_SIZE.y - BORDER) {
				red_pos.y = WINDOW_SIZE.y - red_paddle.get_size().y - BORDER;
				red_vel.y = 0.0f;
			}

			// Make the ball bounce off the top or bottom of the screen
			if (ball_pos.y < 0.0f) {
				ball_pos.y = 0.0f;
				ball_dir.y *= -1.0f;
			}
			else if (ball_pos.y + ball.get_size().y > WINDOW_SIZE.y) {
				ball_pos.y = WINDOW_SIZE.y - ball.get_size().y;
				ball_dir.y *= -1.0f;
			}

			// Check if the ball has gone off the left or right sides of the screen
			if (ball_pos.x + ball.get_size().x < 0.0f) {
				// Head towards red
				reset_ball(1);

				// Red scores
				if (++red_score == TARGET_SCORE) {
					// End game
					start_results();
				}
			}
			else if (ball_pos.x > WINDOW_SIZE.x) {
				// Head towards blue
				reset_ball(-1);

				// Blue scores
				if (++blue_score == TARGET_SCORE) {
					// End game
					start_results();
				}
			}

			// Check if the ball is colliding with the blue paddle
			float ball_center = ball_pos.y + ball.get_size().y / 2;
			if (spot::colliding({ blue_pos, blue_paddle.get_size() }, { ball_pos, ball.get_size() })) {
				if (ball_dir.x <= 0.0f) {
					float bat_center = blue_pos.y + blue_paddle.get_size().y / 2;
					float offset = (ball_center - bat_center) / blue_paddle.get_size().y;
					// Cause ball to rebound
					ball_speed += BALL_SPEED_INCREMENT;
					// Add a vertical component if we are far away from the centre of the bat
					ball_dir.y += offset;
					// Clamp between -1 and 1
					ball_dir.y = std::max(-1.0f, std::min(1.0f, ball_dir.y));
					// Ensure velocity is normalised
					ball_dir.x = std::sqrt(1.0f - std::powf(ball_dir.y, 2));
				}
			}
			// Check if the ball is colliding with the red paddle
			if (spot::colliding({ red_pos, red_paddle.get_size() }, { ball_pos, ball.get_size() })) {
				if (ball_dir.x >= 0.0f) {
					float bat_center = red_pos.y + red_paddle.get_size().y / 2;
					float offset = (ball_center - bat_center) / red_paddle.get_size().y;
					// Cause ball to rebound
					ball_speed += BALL_SPEED_INCREMENT;
					// Add a vertical component if we are far away from the centre of the bat
					ball_dir.y += offset;
					// Clamp between -1 and 1
					ball_dir.y = std::max(-1.0f, std::min(1.0f, ball_dir.y));
					// Ensure velocity is normalised
					ball_dir.x = -std::sqrt(1.0f - std::powf(ball_dir.y, 2));
				}
			}
		}
		else if (game_state == GameState::RESULTS) {
			if (keyboard.just_pressed(SDL_SCANCODE_SPACE) || keyboard.just_pressed(SDL_SCANCODE_RETURN)) {
				// Go back to menu
				start_menu();
			}
		}
		return true;
	}

	void render() {
		// Fill screen with a dark grey
		graphics.clear({ 0x10, 0x10, 0x10 });

		if (game_state == GameState::TITLE) {
			// Render the menu text
			float play_y = WINDOW_SIZE.y * 0.42f;
			float quit_y = WINDOW_SIZE.y * 0.58f;
			font.render_text("Play", { WINDOW_SIZE.x / 2.0f, play_y }, COLOUR_WHITE);
			font.render_text("Quit", { WINDOW_SIZE.x / 2.0f, quit_y }, COLOUR_WHITE);

			// Render the currently selected option
			float selected_y = current_menu_selection == MenuOptions::PLAY ? play_y : quit_y;
			ball.render({ WINDOW_SIZE.x * 0.42f, selected_y }, spot::CENTER_CENTER);
		}
		else if (game_state == GameState::PLAYING) {
			// Render the score text
			std::string score_str = std::format("{} - {}", blue_score, red_score);
			font.render_text(score_str, { WINDOW_SIZE.x / 2.0f, BORDER }, COLOUR_WHITE, spot::TOP_CENTER);

			// Draw the two paddles
			blue_paddle.render(blue_pos);
			red_paddle.render(red_pos);
			ball.render(ball_pos);
		}
		else if (game_state == GameState::RESULTS) {
			// Render the final score text
			std::string score_str = std::format("{} - {}", blue_score, red_score);
			font.render_text(score_str, { WINDOW_SIZE.x / 2.0f, BORDER }, COLOUR_WHITE, spot::TOP_CENTER);

			// Render the winner
			std::string winner = blue_score > red_score ? "Blue" : "Red";
			std::string winner_str = std::format("{} wins!", winner);
			font.render_text(winner_str, { WINDOW_SIZE.x / 2.0f, WINDOW_SIZE.y * 0.42f }, COLOUR_WHITE);

			float menu_y = WINDOW_SIZE.y * 0.58f;
			font.render_text("Return to menu", {WINDOW_SIZE.x / 2.0f, menu_y }, COLOUR_WHITE);
			ball.render({ WINDOW_SIZE.x * 0.3f, menu_y }, spot::CENTER_CENTER);
		}
	}

private:
	void start_menu() {
		game_state = GameState::TITLE;
		current_menu_selection = MenuOptions::PLAY;
	}

	void start_game() {
		game_state = GameState::PLAYING;

		blue_pos = { blue_paddle.get_size().x, WINDOW_SIZE.y * 0.5f};
		red_pos = { WINDOW_SIZE.x - blue_paddle.get_size().x * 2, WINDOW_SIZE.y * 0.5f };

		// Reset velocities
		blue_vel = {};
		red_vel = {};

		// Always head towards red at the start
		reset_ball(1);

		// Reset scores
		blue_score = red_score = 0;
	}

	void start_results() {
		game_state = GameState::RESULTS;
	}

	void reset_ball(int ball_direction) {
		ball_pos = { WINDOW_SIZE.x * 0.5f, WINDOW_SIZE.y * 0.5f };

		// Head in the direction of the sign of ball_direction
		float sign = (ball_direction > 0) ? 1.0f : (ball_direction < 0) ? -1.0f : 0.0f;
		ball_dir = { sign, 0.0f };
		ball_speed = BALL_INITIAL_VEL;
	}

	enum class GameState {
		TITLE,
		PLAYING,
		RESULTS,
	};

	enum MenuOptions {
		PLAY,
		QUIT,
		TOTAL_MENU_OPTIONS
	};

	spot::GraphicsContext graphics{ TITLE, WINDOW_SIZE };
	spot::Keyboard keyboard;

	spot::Texture blue_paddle, red_paddle, ball;

	spot::TTFFont font{ FONT_PATH, FONT_SIZE };

	spot::float2 blue_pos, red_pos, ball_pos;
	spot::float2 blue_vel, red_vel;
	spot::float2 ball_dir;
	float ball_speed = 0.0f;

	uint32_t blue_score = 0;
	uint32_t red_score = 0;

	GameState game_state = GameState::TITLE;
	MenuOptions current_menu_selection = MenuOptions::PLAY;
};

int main(int argc, char* argv[]) {
	try {
		Game game;
		if (game.run()) return 0;
	}
	catch (const std::runtime_error& error) {
		std::cerr << "Error: " << error.what() << std::endl;
	}

	return 1;
}
