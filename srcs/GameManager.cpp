#include "../incs/GameManager.hpp"

GameManager::GameManager(GameState *state) : _state(state) {}

void GameManager::update()  {
	processNextInput();
	_state->snake_A.move();
	if (_state->config.mode == GameMode::MULTI && _state->snake_B)
		_state->snake_B->move();
	_state->isRunning = checkGameOverCollision();
	checkHeadFoodCollision();
}

void GameManager::bufferInput(Input input) {
	// Player A (Arrows)
	if (input >= Input::Up_A && input <= Input::Right_A) {
		if (inputBuffer_A.size() < MAX_BUFFER_SIZE) {
			inputBuffer_A.push(input);
		}
	} else if (_state->config.mode == GameMode::MULTI && input >= Input::Up_B && input <= Input::Right_B) { 
		if (inputBuffer_B.size() < MAX_BUFFER_SIZE) {
			inputBuffer_B.push(input); // Player B (WASD)
		}
	} /* else if (_state->config.mode == GameMode::AI)
		inputBuffer_B.push(aiController->decideNextMove(*_state)); */
}

void GameManager::processNextInput() {
	// Process Snake A inpu
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
	
	// Process Snake B input
	if (!inputBuffer_B.empty()) {
		Input input = inputBuffer_B.front();
		inputBuffer_B.pop();
		
		switch (input) {
			case Input::Up_B:
				if (_state->config.mode != GameMode::SINGLE && _state->snake_B) {
					_state->snake_B->changeDirection(Direction::Up);
				}
				break;
			case Input::Down_B:
				if (_state->config.mode != GameMode::SINGLE && _state->snake_B) {
					_state->snake_B->changeDirection(Direction::Down);
				}
				break;
			case Input::Left_B:
				if (_state->config.mode != GameMode::SINGLE && _state->snake_B) {
					_state->snake_B->changeDirection(Direction::Left);
				}
				break;
			case Input::Right_B:
				if (_state->config.mode != GameMode::SINGLE && _state->snake_B) {
					_state->snake_B->changeDirection(Direction::Right);
				}
				break;
			default:
				break;
		}
	}
}

void GameManager::checkHeadFoodCollision() {
	Vec2	head_A = _state->snake_A.getSegments()[0];
	//
	Vec2	foodPos = _state->food.getPosition();

	if (head_A.x == foodPos.x && head_A.y == foodPos.y)
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

	if (_state->config.mode != GameMode::SINGLE && _state->snake_B) {
		Vec2	head_B = _state->snake_B->getSegments()[0];

		if (head_B.x == foodPos.x && head_B.y == foodPos.y) {
			/* if (_state->audio)
				_state->audio->playSound("sound:ñomñomñomñom"); // TODO: real sound implementation */
				
			_state->snake_B->grow();
			_state->scoreB++;  // Increment score when food is eaten
			
			if (!_state->food.replaceInFreeSpace(_state)) {
				_state->isRunning = false;
				std::cout << "YOU WIN" << std::endl;
			}
		}
	}
}

bool GameManager::checkGameOverCollision()
{
	Vec2	head_A = _state->snake_A.getSegments()[0];

	if (head_A.x < 0 || head_A.x > _state->width - 1)
		_state->snake_A.setAsDead(true);

	if (head_A.y < 0 || head_A.y > _state->height - 1)
		_state->snake_A.setAsDead(true);

	for (int i = 1; i < _state->snake_A.getLength(); i++)
	{
		if (_state->snake_A.getSegments()[i].x == head_A.x && _state->snake_A.getSegments()[i].y == head_A.y)
			_state->snake_A.setAsDead(true);
	}

	if (_state->snake_A.isDead()) {
		return false;
	}

	if (_state->config.mode != GameMode::SINGLE && _state->snake_B) {
		Vec2	head_B = _state->snake_B->getSegments()[0];

		for (int i = 0; i < _state->snake_B->getLength(); i++)
		{
			if (_state->snake_B->getSegments()[i].x == head_A.x && _state->snake_B->getSegments()[i].y == head_A.y)
				_state->snake_B->setAsDead(true);
		}

		if (head_B.x < 0 || head_B.x > _state->width - 1)
			_state->snake_B->setAsDead(true);

		if (head_B.y < 0 || head_B.y > _state->height - 1)
			_state->snake_B->setAsDead(true);

		for (int i = 1; i < _state->snake_B->getLength(); i++)
		{
			if (_state->snake_B->getSegments()[i].x == head_B.x && _state->snake_B->getSegments()[i].y == head_B.y)
				_state->snake_B->setAsDead(true);
		}

		if (_state->snake_B->isDead()) {
			return false;
		}

		for (int i = 0; i < _state->snake_A.getLength(); i++)
		{
			if (_state->snake_A.getSegments()[i].x == head_B.x && _state->snake_A.getSegments()[i].y == head_B.y)
				_state->snake_A.setAsDead(true);
		}

		if (_state->snake_A.isDead()) {
			return false;
		}

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

void GameManager::setAIController(SnakeAI *ai) {
	aiController = ai;
}