# Nibbler - Devlog - 13

## Table of Contents
1. [Day Seventeen Plan](#131-day-seventeen-plan)
2. [Testing, Testng, One, Two, Three](#132-testing-testng-one-two-three)

<br>
<br>

## 13.1 Day Seventeen Plan
Today is all about automated tests. Well, I hope it is going to be *more* than about those, but we'll start from there, for which we'll need to set up:
- github actions
- makefile build pipeline and test-testing files to set up a `gtest` (Google) environment
- write a bunch of tests

If after all of this there's more time to today's journey, I'll get into... **checks notes*... I don't know, building the foundations for a multisnake mode or something, don't ask me, what do i know.

<br>
<br>

## 13.2 Testing, Testng, One, Two, Three
I have some experience setting up github actions with automated test pipelines based on different tools. For example, I've worked with `xUnit` in `C#`/`dotnet` environments and `criterion` for `C` programs, and after some research I decided to take the opportunity to learn about `gtest` tied to `C++`. All of these tools, as I understand them at least, give me the perfect environment to both make **assertion based tests** that can be automated a different points during development and integration (aka, we're getting into `CI/CL` from this point on), and the objective today is to **build a pipeline that auto-tests any attempt to push or merge along the project's tree**. So let's get into it!

As this is a somewhat tangly implementation, which needs substantial additions to `Makefile`, test boilerplaiting and the `yml` configuration files for the `github actions`, this is also a good opportunity to lay down the whole process, step by step. We'll do it in this order:
1. Makefile setup for `googletest`
2. github actions setup (`yml` files)
3. How to write tests, with some examples (snake tests)

### Makefiling Our Way into Googletest's Heart
I must say something, before going on: I hate makefile. I love it *conceptually*, but I'd rather eat glass than writing one of this files. Fortunately, at this point in my life, I have so many makefiles in my backpack that there's always one that works as a template, but inyecting automated tests in them is something that I've only started doing since the last months of last year. So let's break down how to do it in plain human.

Adding a `googletest` implementation to my current mafile requires the following:
- A test directory target
- The test suit URL from which we'll get the source code to implement `googletest`
- The target library (`.a` file) that the test suite builds
- In this case, the `main` related sub-library that the suite also builds
- The lib's build process includes

On top of this, to manage the test files and their setup, we need to set up directory for the test files (my written tests), their object and dependencies hidden folders and the independent binary that will be used to run the tests.

> for this last part, I use `wildcard` because I don't want to bother myself with having to write every test source file manually every time I write one. I don't really know if wildcarding is a *bad practice*, but I was taught not to use it. At this point, and after following that lesson for the game's source files, which are all listed one by one, I think that no one will be angry that I wildcard a little for testing. And if that's not the case, I can only ask for forgiveness.

Translating all of this to the cursed tongue of Makefile, and sprinkling some flags on top of everything, we get:
```Makefile
# -=-=-=-=-    GOOGLE TEST -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

GTEST_DIR		:= $(LIB_DIR)/googletest
GTEST_REPO		:= https://github.com/google/googletest.git
GTEST_LIB		:= $(GTEST_DIR)/build/lib/libgtest.a
GTEST_MAIN_LIB	:= $(GTEST_DIR)/build/lib/libgtest_main.a
GTEST_INCLUDES	:= -I$(GTEST_DIR)/googletest/include

# -=-=-=-=-    TEST CONFIGURATION -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

TEST_DIR		:= tests
TEST_OBJDIR		:= .test_obj
TEST_DEPDIR		:= .test_dep
TEST_BINARY		:= run_tests

# Test source files (we'll add these)
TEST_SRCS		:= $(wildcard $(TEST_DIR)/unit/*.cpp) \
					$(wildcard $(TEST_DIR)/integration/*.cpp)
TEST_OBJS		:= $(patsubst $(TEST_DIR)/%.cpp,$(TEST_OBJDIR)/%.o,$(TEST_SRCS))
TEST_DEPS		:= $(patsubst $(TEST_DIR)/%.cpp,$(TEST_DEPDIR)/%.d,$(TEST_SRCS))

# Need to compile main program objects for testing (exclude main.cpp)
TESTABLE_SRCS	:= $(filter-out $(SRCDIR)/main.cpp, $(SRCS))
TESTABLE_OBJS	:= $(addprefix $(OBJDIR)/, $(notdir $(TESTABLE_SRCS:.cpp=.o)))

TEST_CFLAGS		:= $(CFLAGS) $(GTEST_INCLUDES)
TEST_LDFLAGS	:= $(LDFLAGS) -lpthread
```

Once this done, the `check` and `target` sections related to test need to be added. Nothing extremely arcane here, although it freaking looks like it, let's be real. Check looks inside the test directory to see if a gtest build is in place, goes into building process if there's not. Target runs the written test through the testing pipeline. All together, this is achieved by adding the following:
```Makefile
check_gtest:
	@if [ ! -f "$(GTEST_LIB)" ]; then \
		echo "$(YELLOW)Google Test not found. Cloning and building...$(DEF_COLOR)"; \
		mkdir -p $(LIB_DIR); \
		git clone --depth 1 --branch v1.14.0 $(GTEST_REPO) $(GTEST_DIR); \
		cd $(GTEST_DIR) && mkdir -p build && cd build && \
		cmake -DCMAKE_CXX_STANDARD=20 .. && \
		make -j4; \
		echo "$(GREEN)Google Test built successfully$(DEF_COLOR)"; \
	else \
		echo "$(GREEN)Google Test already built$(DEF_COLOR)"; \
	fi
```
```Makefile
# -=-=-=-=-    TEST TARGETS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

test: check_gtest $(TEST_BINARY)
	@echo "$(CYAN)Running tests...$(DEF_COLOR)"
	./$(TEST_BINARY)

$(TEST_BINARY): $(TESTABLE_OBJS) $(TEST_OBJS) $(GTEST_LIB) $(GTEST_MAIN_LIB)
	@echo "$(YELLOW)Linking test binary...$(DEF_COLOR)"
	$(CC) -o $@ $(TESTABLE_OBJS) $(TEST_OBJS) $(GTEST_LIB) $(GTEST_MAIN_LIB) $(TEST_LDFLAGS)
	@echo "$(GREEN)Test binary created: $(TEST_BINARY)$(DEF_COLOR)"

# Compile test object files
$(TEST_OBJDIR)/%.o: $(TEST_DIR)/%.cpp | $(TEST_OBJDIR) $(TEST_DEPDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(TEST_DEPDIR)/$*.d)
	$(CC) $(TEST_CFLAGS) -MMD -MF $(TEST_DEPDIR)/$*.d -c $< -o $@
	@echo "$(BLUE)Compiled test: $<$(DEF_COLOR)"

# Create test directories
$(TEST_OBJDIR):
	@mkdir -p $(TEST_OBJDIR)/unit $(TEST_OBJDIR)/integration

$(TEST_DEPDIR):
	@mkdir -p $(TEST_DEPDIR)/unit $(TEST_DEPDIR)/integration
```

Now, all we have to do is write test (something pretty enjoyable for me during the, let's say, first hour of coding them, not so much after that >_>).

### The Google Way of Testing Stuff
All assertion based tests, when trying to automate them via some suit and strictly following my limited experience and knowledge about them, work the same:
- You set up a running environment (i.e., what the specifically needs from the whole program build to work)
- You research how the assertions are specifically done in the suite of choice
- You write small, controlled test with a very controlled focus.

For example, a snake related test in this *nibbler* written for `gtest` can look like this:
```cpp
#include <gtest/gtest.h>
#include "../../incs/Snake.hpp"
#include "../../incs/DataStructs.hpp"

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
```

There's:
- Necessary includes, both for `gtest` and just the header files that are needed for the tests included in thhe file
- A test class with minimal implementation (this varies depending on the suite)
- The test functions themselves
	- How to write them and the assertion function names are, obviously, dependent on the suite too, but conceptually they are always pretty alike

And we can now spend the rest of our day (hope not) writing tests!!!!

I'll back with some more examples of what I code today in relation to tests...

