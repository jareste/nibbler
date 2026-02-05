#pragma once

#include "../../incs/DataStructs.hpp"
#include "../../incs/Snake.hpp"
#include "../../incs/Food.hpp"

namespace TestHelpers {
	inline GameState createBasicGameState(int width = 20, int height = 20) {
		static Snake snake(width, height);
		static Food food({10, 10}, width, height);

		return GameState {
			width,
			height,
			snake,
			food,
			false,					// gameover
			true,					// isRunning
			false,					// isPaused
			GameStateType::Playing,
			0,						// score
			nullptr					// audio (not needed for tests right now)
		};
	}
}