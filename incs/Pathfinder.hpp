#pragma once

#include "DataStructs.hpp"
#include <vector>
#include <cmath>
#include <set>
#include <unordered_map>
#include <climits>
#include <algorithm>

class PathFinder {
private:
	struct Node {
		Vec2 pos;
		int gCost;                              // Distance from start
		int hCost;                              // Heuristic to goal
		int fCost() const { return gCost + hCost; }
		Node* parent;
	};

	struct CompareNode {
		// Added some tie breaking exits
		bool operator()(Node *a, Node *b) const {
			if (a->fCost() != b->fCost())
				return a->fCost() < b->fCost();

			if (a->pos.x != b->pos.x)
				return a->pos.x < b->pos.x;

			return a->pos.y < b->pos.y;
		}
	};

	int manhattanDistance(Vec2 a, Vec2 b);
	bool isWalkable(const GameState& state, Vec2 pos);
	std::vector<Vec2> getNeighbors(Vec2 pos);
	std::vector<Vec2> reconstructPath(Node *goalNode);
	void cleanAllNodes(std::vector<Node*> allNodes);

public:
	PathFinder() = default;
	PathFinder(const PathFinder &other) = delete;
	PathFinder &operator=(const PathFinder &other) = delete;
	~PathFinder() = default;
	
	std::vector<Vec2> findPath(
		const GameState& state,
		Vec2 start,
		Vec2 goal,
		int maxDepth = 200                      // Difficulty tuning
	);
};