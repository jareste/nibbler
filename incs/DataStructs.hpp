#pragma once
#include <cstddef>
#include <memory>

struct Vec2 {
	int	x;
	int	y;
};

class Snake;
class Food;
class IAudio;

enum class GameStateType {
    Menu,
    Playing,
    Paused,
    GameOver
};

struct GameState {
	int				width;
	int				height;
	Snake&			snake_A;
	Snake&			snake_B;
	Food&			food;
	bool			gameOver;
	bool			isRunning;
	bool			isPaused;
	GameStateType	currentState;
	int				score;
	IAudio*			audio;
};
