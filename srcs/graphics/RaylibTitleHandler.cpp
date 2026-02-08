#include "../../incs/RaylibTitleHandler.hpp"
#include "../../incs/RaylibGraphic.hpp"

RaylibTitleHandler::RaylibTitleHandler(RaylibGraphic& parent) : graphic(parent) {}

void RaylibTitleHandler::drawTitle() {
	float yPos = 0;
	float size = graphic.cubeSize;
	
	float initialX = -2.5 * graphic.cubeSize;
	float initialZ = 19.5 * graphic.cubeSize;
	
	Vector3 position = {
		initialX,
		yPos,
		initialZ
	};

	Vector3 initialPosition = position;
	Vector3 accumulatedPosition = position;

	// n
	for (int i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z -= graphic.cubeSize;
	}

	position = initialPosition;
	position.z -= graphic.cubeSize * 3;
	position.x += graphic.cubeSize;

	for (int i = 0; i < 2; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z -= graphic.cubeSize;
	}

	position = initialPosition;
	position.x += graphic.cubeSize;
	for (int i = 0; i < 4; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = initialPosition;
	position.z -= graphic.cubeSize * 4;
	position.x += graphic.cubeSize * 2;
	for (int i = 0; i < 3; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	// i
	accumulatedPosition.z -= (graphic.cubeSize * 5) + graphic.separator;
	position = accumulatedPosition;
	position.x -= graphic.cubeSize * 7;

	// dot with pulsating effect
	float pulse = 1.0f + sinf(graphic.accumulatedTime * 3.0f) * 0.1f;
	graphic.drawCubeCustomFaces(position, size * pulse, size * pulse, size * pulse,
			                    graphic.foodFront, graphic.foodHidden, graphic.foodTop, graphic.foodHidden, graphic.foodSide, graphic.foodHidden);

	// head
	position.x += graphic.cubeSize * 2;
	graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.snakeALightFront, graphic.snakeAHidden, graphic.snakeALightTop, graphic.snakeAHidden, graphic.snakeALightSide, graphic.snakeAHidden);

	position.x += graphic.cubeSize;
	position.y += 0.1f;
	for (int i = 0; i < 11; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize * 0.8f, graphic.cubeSize * 0.8f, graphic.cubeSize * 0.8f,
									graphic.snakeALightFront, graphic.snakeAHidden, graphic.snakeALightTop, graphic.snakeAHidden, graphic.snakeALightSide, graphic.snakeAHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size * 0.8f, size * 0.8f, size * 0.8f,
			                    graphic.snakeADarkFront, graphic.snakeAHidden, graphic.snakeADarkTop, graphic.snakeAHidden, graphic.snakeADarkSide, graphic.snakeAHidden);
		}
		position.x += graphic.cubeSize;
	}

	for (int i = 1; i < 29; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize * 0.8f, graphic.cubeSize * 0.8f, graphic.cubeSize * 0.8f,
									graphic.snakeALightFront, graphic.snakeAHidden, graphic.snakeALightTop, graphic.snakeAHidden, graphic.snakeALightSide, graphic.snakeAHidden);
		} else {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize * 0.8f, graphic.cubeSize * 0.8f, graphic.cubeSize * 0.8f,
			                    graphic.snakeADarkFront, graphic.snakeAHidden, graphic.snakeADarkTop, graphic.snakeAHidden, graphic.snakeADarkSide, graphic.snakeAHidden);
		}
		position.z -= graphic.cubeSize;
	}

	// b 1
	accumulatedPosition.z -= graphic.cubeSize + graphic.separator;
	position = accumulatedPosition;
	position.x -= graphic.cubeSize * 3;

	for (int i = 0; i < 8; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.z -= graphic.cubeSize;

	for (int i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z -= graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 4;
	position.z -= graphic.cubeSize;

	for (int i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z -= graphic.cubeSize;
	}
	
	position = accumulatedPosition;
	position.x += graphic.cubeSize;
	position.z -= graphic.cubeSize * 4;

	for (int i = 0; i < 3; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	// b 2
	accumulatedPosition.z -= (graphic.cubeSize * 5) + graphic.separator;
	position = accumulatedPosition;
	position.x -= graphic.cubeSize * 3;

	for (int i = 0; i < 8; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.z -= graphic.cubeSize;

	for (int i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z -= graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 4;
	position.z -= graphic.cubeSize;

	for (int i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z -= graphic.cubeSize;
	}
	
	position = accumulatedPosition;
	position.x += graphic.cubeSize;
	position.z -= graphic.cubeSize * 4;

	for (int i = 0; i < 3; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	// l
	accumulatedPosition.z -= (graphic.cubeSize * 5) + graphic.separator;
	position = accumulatedPosition;
	position.x -= graphic.cubeSize * 3;

	for (int i = 0; i < 8; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	// e
	accumulatedPosition.z -= graphic.cubeSize + graphic.separator;
	position = accumulatedPosition;

	for (int i = 0; i < 5; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z -= graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize;

	for (int i = 0; i < 4; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize;
	position.z -= graphic.cubeSize * 4;

	for (int i = 0; i < 1; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z -= graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 2;
	position.z -= graphic.cubeSize;

	for (int i = 0; i < 4; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z -= graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.z -= graphic.cubeSize;
	position.x += graphic.cubeSize * 4;

	for (int i = 0; i < 4; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z -= graphic.cubeSize;
	}

	// r
	accumulatedPosition.z -= (graphic.cubeSize * 5) + graphic.separator;
	position = accumulatedPosition;

	for (int i = 0; i < 5; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z -= graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize;

	for (int i = 0; i < 4; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.z -= graphic.cubeSize * 4;
	position.x += graphic.cubeSize;

	for (int i = 0; i < 1; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	// DEBUG
	//DrawGrid(graphic.gridWidth, graphic.cubeSize);
}

void RaylibTitleHandler::drawGameover() {;
	float yPos = 0;
	float size = graphic.cubeSize;
	
	float initialX = -15.5 * graphic.cubeSize;
	float initialZ = -5.5 * graphic.cubeSize;
	
	Vector3 position = {
		initialX,
		yPos,
		initialZ
	};

	Vector3 initialPosition = position;
	Vector3 accumulatedPosition = position;

	// g
	for (int i = 0; i < 5; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = initialPosition;
	position.x += graphic.cubeSize * 4;
	position.z += graphic.cubeSize;

	for (int i = 0; i < 11; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = initialPosition;
	position.z += graphic.cubeSize;
	for (int i = 0; i < 4; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = initialPosition;
	position.x += graphic.cubeSize;
	position.z += graphic.cubeSize * 4;
	for (int i = 0; i < 3; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = initialPosition;
	position.z += graphic.cubeSize * 11;
	position.x += graphic.cubeSize;
	for (int i = 0; i < 3; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = initialPosition;
	position.z += graphic.cubeSize * 10;
	for (int i = 0; i < 2; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = initialPosition;
	position.z += graphic.cubeSize * 8;
	for (int i = 0; i < 2; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	// a+m
	accumulatedPosition.x += (graphic.cubeSize * 5) + graphic.separator;
	position = accumulatedPosition;

	for (int i = 0; i < 5; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize;

	for (int i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.z += graphic.cubeSize * 4;
	position.x += graphic.cubeSize;

	for (int i = 0; i < 7; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}
	
	position = accumulatedPosition;
	position.z += graphic.cubeSize;
	position.x += graphic.cubeSize * 4;

	for (int i = 0; i < 3; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}
	
	position = accumulatedPosition;
	position.x += graphic.cubeSize * 7;

	for (int i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 8;

	for (int i = 0; i < 1; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 9;

	for (int i = 0; i < 5; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 10;
	position.z += graphic.cubeSize * 4;

	for (int i = 0; i < 1; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 11;

	for (int i = 0; i < 5; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 12;

	for (int i = 0; i < 1; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 13;

	for (int i = 0; i < 5; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 14;
	position.z += graphic.cubeSize * 4;

	for (int i = 0; i < 1; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 15;

	for (int i = 0; i < 5; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 16;

	for (int i = 0; i < 1; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 17;

	for (int i = 0; i < 5; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	// e
	accumulatedPosition.x += (graphic.cubeSize * 18) + graphic.separator;
	position = accumulatedPosition;

	for (int i = 0; i < 5; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize;

	for (int i = 0; i < 4; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 4;
	position.z += graphic.cubeSize;

	for (int i = 0; i < 2; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize;
	position.z += graphic.cubeSize * 2;

	for (int i = 0; i < 3; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize;
	position.z += graphic.cubeSize * 4;

	for (int i = 0; i < 4; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	// o
	accumulatedPosition = initialPosition;
	accumulatedPosition.x += (graphic.cubeSize * 5) + graphic.separator;
	accumulatedPosition.z += (graphic.cubeSize * 5) + graphic.separator;
	position = accumulatedPosition;

	for (int i = 0; i < 5; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize;

	for (int i = 0; i < 4; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 4;
	position.z += graphic.cubeSize;

	for (int i = 0; i < 3; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}
	
	position = accumulatedPosition;
	position.x += graphic.cubeSize;
	position.z += graphic.cubeSize * 4;

	for (int i = 0; i < 4; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	// v
	accumulatedPosition.x += (graphic.cubeSize * 4.75) + graphic.separator;
	position = accumulatedPosition;

	for (int i = 0; i < 5; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.z += graphic.cubeSize * 4;

	for (int i = 0; i < 4; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}
	
	position = accumulatedPosition;
	position.z += graphic.cubeSize * 3;
	position.x += graphic.cubeSize * 3;

	for (int i = 0; i < 2; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 4;

	for (int i = 0; i < 3; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	// e
	accumulatedPosition.x += (graphic.cubeSize * 4.75) + graphic.separator;
	position = accumulatedPosition;

	for (int i = 0; i < 5; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize;

	for (int i = 0; i < 4; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize * 4;
	position.z += graphic.cubeSize;

	for (int i = 0; i < 2; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize;
	position.z += graphic.cubeSize * 2;

	for (int i = 0; i < 3; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.x += graphic.cubeSize;
	position.z += graphic.cubeSize * 4;

	for (int i = 0; i < 4; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	// r
	accumulatedPosition.x += (graphic.cubeSize * 4.75) + graphic.separator;
	position = accumulatedPosition;

	for (int i = 0; i < 5; i++) {
		if (i % 2 != 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.x += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.z += graphic.cubeSize;

	for (int i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	position = accumulatedPosition;
	position.z += graphic.cubeSize;
	position.x += graphic.cubeSize * 4;

	for (int i = 0; i < 1; i++) {
		if (i % 2 == 0) {
			graphic.drawCubeCustomFaces(position, graphic.cubeSize, graphic.cubeSize, graphic.cubeSize,
									graphic.groundLightFront, graphic.groundHidden, graphic.groundLightTop, graphic.groundHidden, graphic.groundLightSide, graphic.groundHidden);
		} else {
			graphic.drawCubeCustomFaces(position, size, size, size,
			                    graphic.groundDarkFront, graphic.groundHidden, graphic.groundDarkTop, graphic.groundHidden, graphic.groundDarkSide, graphic.groundHidden);
		}
		position.z += graphic.cubeSize;
	}

	// DEBUG
	//DrawGrid(graphic.gridWidth, graphic.cubeSize);
}