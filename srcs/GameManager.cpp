#include "../incs/GameManager.hpp"

GameManager::GameManager(GameState *state) : _state(state) {}

void GameManager::update()  {
	processNextInput();
	_state->snake_A.move();
	_state->snake_B.move();
	_state->isRunning = checkGameOverCollision();
	checkHeadFoodCollision();
}

void GameManager::bufferInput(Input input) {
	// Player A (Arrows)
	if (input >= Input::Up_A && input <= Input::Right_A) {
		if (inputBuffer_A.size() < MAX_BUFFER_SIZE) {
			inputBuffer_A.push(input);
		}
	}
	// Player B (WASD)
	else if (input >= Input::Up_B && input <= Input::Right_B) {
		if (inputBuffer_B.size() < MAX_BUFFER_SIZE) {
			inputBuffer_B.push(input);
		}
	}
}

void GameManager::processNextInput() {
	// Process Snake A input (Arrows)
	if (!inputBuffer_A.empty()) {
		Input input = inputBuffer_A.front();
		inputBuffer_A.pop();
		
		switch (input) {
			case Input::Up_A:
				_state->snake_A.changeDirection(Direction::Up);
				break;
			case Input::Down_A:
				_state->snake_A.changeDirection(Direction::Down);
				break;
			case Input::Left_A:
				_state->snake_A.changeDirection(Direction::Left);
				break;
			case Input::Right_A:
				_state->snake_A.changeDirection(Direction::Right);
				break;
			default:
				break;
		}
	}
	
	// Process Snake B input (WASD)
	if (!inputBuffer_B.empty()) {
		Input input = inputBuffer_B.front();
		inputBuffer_B.pop();
		
		switch (input) {
			case Input::Up_B:
				_state->snake_B.changeDirection(Direction::Up);
				break;
			case Input::Down_B:
				_state->snake_B.changeDirection(Direction::Down);
				break;
			case Input::Left_B:
				_state->snake_B.changeDirection(Direction::Left);
				break;
			case Input::Right_B:
				_state->snake_B.changeDirection(Direction::Right);
				break;
			default:
				break;
		}
	}
}

void GameManager::checkHeadFoodCollision() {
	Vec2	head = _state->snake_A.getSegments()[0];
	Vec2	foodPos = _state->food.getPosition();

	if (head.x == foodPos.x && head.y == foodPos.y)
	{
		/* if (_state->audio)
			_state->audio->playSound("sound:ñomñomñomñom"); // TODO: real sound implementation */
			
		_state->snake_A.grow();
		_state->score++;  // Increment score when food is eaten
		
		if (!_state->food.replaceInFreeSpace(_state)) {
			_state->isRunning = false;
			std::cout << "YOU WIN" << std::endl;
		}
	}
		
}

bool GameManager::checkGameOverCollision()
{
	Vec2	head = _state->snake_A.getSegments()[0];
	if (head.x < 0 || head.x > _state->width - 1)
		return false;

	if (head.y < 0 || head.y > _state->height - 1)
		return false;

	for (int i = 1; i < _state->snake_A.getLength(); i++)
	{
		if (_state->snake_A.getSegments()[i].x == head.x && _state->snake_A.getSegments()[i].y == head.y)
			return false;
	}

	return true;
}

void GameManager::clearInputBuffer() {
	while (!inputBuffer_A.empty()) {
		inputBuffer_A.pop();
	}
	while (!inputBuffer_B.empty()) {
		inputBuffer_B.pop();
	}
}