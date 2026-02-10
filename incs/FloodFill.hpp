#pragma once

#include "Snake.hpp"
#include "DataStructs.hpp"
#include <vector>

class FloodFill {
public:
	// count reachable empty cells from start
	int countReachable(
		const GameState& state,
		Vec2 start,
		const std::vector<Vec2>& ignorePositions = {}       // mostly for the tail right now
	);
	
	bool canReachTail(
		const GameState& state,
		const Snake& aiSnake,
		const std::vector<Vec2>& proposedPath
	);
	
private:
	std::vector<std::vector<bool>> visited;
	void floodFillRecursive(const GameState& state, Vec2 pos);
};