#include <gtest/gtest.h>
#include "../../incs/Snake.hpp"
#include "../../incs/DataStructs.hpp"

// this is the initial test I wrote to make sure the testing pipeline worked. Other than that, quite irrelevant
class SnakeTest : public ::testing::Test {
	protected:
		void SetUp() override {

		}

		void TearDown() override {

		}
};

TEST_F(SnakeTest, SnakeInitialization) {
	Snake snake(20, 20);

	EXPECT_EQ(snake.getLength(), 4);
}

TEST_F(SnakeTest, SnakeMovement) {
	Snake snake(20, 20);
	Direction dir = snake.getDirection();
	Vec2 initialPos = snake.getSegments()[0];

	snake.move();
	Vec2 newPos = snake.getSegments()[0];

	if (dir == Direction::Left || dir == Direction::Right)
		EXPECT_NE(initialPos.x, newPos.x);
	else
		EXPECT_NE(initialPos.y, newPos.y);
}