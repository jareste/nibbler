#include "../../incs/Pathfinder.hpp"
#include "../../incs/Snake.hpp"

int PathFinder::manhattanDistance(Vec2 a, Vec2 b) {
	return (abs(a.x - b.x) + abs(a.y - b.y));
}

bool PathFinder::isWalkable(const GameState &state, Vec2 pos) {
	int x = pos.x;
	int y = pos.y;

	// grid bounds check
	if (x < 0 || y < 0 || x >= state.width || y >= state.height)
		return false;

	// check snake_A collision
	const Vec2 *snakeA_body = state.snake_A.getSegments();
	int snakeA_len = state.snake_A.getLength();
	
	for (int i = 0; i < snakeA_len; i++) {
		if (snakeA_body[i].x == x && snakeA_body[i].y == y)
			return false;
	}

	// check snake_B collision (if exists)
	if (state.snake_B) {
		const Vec2 *snakeB_body = state.snake_B->getSegments();
		int snakeB_len = state.snake_B->getLength();
		
		for (int i = 0; i < snakeB_len; i++) {
			if (snakeB_body[i].x == x && snakeB_body[i].y == y)
				return false;
		}
	}

	return true;
}

std::vector<Vec2> PathFinder::getNeighbors(Vec2 pos) {
	std::vector<Vec2> neighbors;

	int x = pos.x;
	int y = pos.y;

	neighbors.push_back({x - 1, y});	//left
	neighbors.push_back({x + 1, y});	//right
	neighbors.push_back({x, y - 1});	//up
	neighbors.push_back({x, y + 1});	//down

	return neighbors;
}

void PathFinder::cleanAllNodes(std::vector<Node*> allNodes) {
	for (Node* node : allNodes) {
        delete node;
    }
}

std::vector<Vec2> PathFinder::reconstructPath(Node *goalNode) {
	std::vector<Vec2> path;
	Node *current = goalNode;

	// main idea -> follow parent nodes backwards
	while (current != nullptr && current->parent != nullptr) {
		path.push_back(current->pos);
		current = current->parent;
	}

	std::reverse(path.begin(), path.end());

	return path; // starting position is not included because the snake is there
}

std::vector<Vec2> PathFinder::findPath(const GameState &state, Vec2 start, Vec2 goal, int maxDepth) {
	std::multiset<Node*, CompareNode> openList;
    std::vector<std::vector<bool>> visited(state.width, 
											std::vector<bool>(state.height, false));
	std::vector<Node*> allNodes; //needed for clenaup

	Node *startNode = new Node {
		start,
		0,
		manhattanDistance(start, goal),
		nullptr
	};

	openList.insert(startNode);
	allNodes.push_back(startNode);
	int nodesExplored = 0;

	while (!openList.empty() && nodesExplored < maxDepth) {
		nodesExplored++;

		Node *current = *openList.begin();
		openList.erase(openList.begin()); // take the begin node because CompareNode sorts ascending, i.e. the one with lowest f-cost

		if (current->pos.x == goal.x && current->pos.y == goal.y) {
			// found the food
			std::vector<Vec2> path = reconstructPath(current);

			cleanAllNodes(allNodes);

			return path;
		}

		visited[current->pos.x][current->pos.y] = true;

		std::vector<Vec2> neighbors = getNeighbors(current->pos);

		for (Vec2 neighborPos : neighbors) {
			// skip non walkable and visited positions
			if (!isWalkable(state, neighborPos)) continue;
			if (visited[neighborPos.x][neighborPos.y]) continue;

			int tentativeGCost = current->gCost + 1; // minimum cost would be one step
			int hCost = manhattanDistance(neighborPos, goal);

			Node * neighborNode = new Node {
				neighborPos,
				tentativeGCost,
				hCost,
				current		//parent is current node
			};

			openList.insert(neighborNode);
			allNodes.push_back(neighborNode);
		}
	}

	cleanAllNodes(allNodes);

	return {};
}