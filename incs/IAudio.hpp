#pragma once
#include "DataStructs.hpp"

class IAudio {
	public:
		virtual ~IAudio() = default;

		virtual void init(int width, int height) = 0;
		virtual void render(const GameState &state, float deltaTime) = 0;
		virtual void renderMenu(const GameState& state, float deltaTime) = 0;
		virtual void renderGameOver(const GameState& state, float deltaTime) = 0;
};