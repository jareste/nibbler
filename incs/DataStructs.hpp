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

enum class GameMode {
	SINGLE,
	MULTI,
	//AI
};

struct GameConfig {
	GameMode	mode;
};

struct GameState {
	int				width;
	int				height;
	Snake&			snake_A;
	Snake*			snake_B; // this needs to be a pointer because there is no snake_B in SINGLE player
	Food&			food;
	bool			gameOver;
	bool			isRunning;
	bool			isPaused;
	GameStateType	currentState;
	int				score;		// for single player or player A in multiplayer
	int				scoreB;		// for player B in multiplayer
	IAudio*			audio;
	GameConfig&		config;
};
