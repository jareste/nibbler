#include "../../incs/SnakeAI.hpp"
#include "../../incs/Food.hpp"

SnakeAI::SnakeAI(AIConfig config) : config(config) {}

/* Input SnakeAI::decideNextMove(const GameState& state) {
    // 1. Find path to food
    Vec2 head = state.snake_B->getSegments()[0];
    Vec2 foodPos = state.food.getPosition();
    
    std::vector<Vec2> path = pathFinder.findPath(head, foodPos, state);
    
    // 2. If path found, take first step
    if (!path.empty()) {
        Vec2 nextPos = path[0];
        return positionToInput(head, nextPos);
    }
    
    // 3. No path? Survival mode
    return survivalMove(state);
} */