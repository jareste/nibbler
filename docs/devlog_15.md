# Nibbler - Devlog - 15

## Table of Contents
1. [Day Twenty Plan](#151-day-twenty-plan)
2. [Not That Kind of AI!](#152-not-that-kind-of-ai)
3. [Too Many Whats in the Land of Hows](#153-too-many-whats-in-the-land-of-hows)

<br>
<br>

## 15.1 Day Twenty Plan
Well, here we are. Twenty days, fifteen logs after the first steps into dynamic libraries, *nibbler* has to end. The task list for the day, though, is quite ambitious, so it is going to take every ounce of MIGHT and COURAGE to really make this the last development day on my side. (Technically, I'll have to wait for sounds on my partner side and surely spend one day integrating and testing the final build, and who knows how much time will the evaluation process chug, but that's beyond the scope of my very small, extremely average human powers). So, as I was saying, very ambitious, yet short to-do list for this twentieth battle agains the other side of this screen:
- Snake AI and `VsAI` mode enabling
- General check
- If time (haha), expand the test suite

It's already kinda late in the morning, so let's get going.

<br>
<br>

## 15.2 Not That Kind of AI!
In this day and age, which in 2026 is an expression that can only, if at all, cover a week's worth of time (who, among us, can predict the state of the world 7 days from today?), `AI` is a cursed word. With good reasons, specially when concepts like *generative* are attached to it, but luckily for us what we aim to build is a good old game AI that expands the multi-snake pattern into a `VsAI` game mode. To do so, and after some research, the bulk of the work is going to be placed on implementing well known algorithms, specifically a combination of `A*` and `Dijstra`, with some added switches and knobs to allow for difficulty settings and opponent configurations. Not the most difficult thing in the world, but something that I've not done too many times before, so it's going to be a battle. But this is where we are, *one battle after another**.

> *Interesting movie, recommended. I loved it at first, have had growing concerns over time. Better taken as a good, entertaining piece of art with some not-totally-focused will at political commentary.

<br>

### Eating Apple Is Important, But Not As Much As Not Dying
Implementing an algorithm that makes the AI snake chase apples would be quite easy. Doing so in a way that it also avoids it's own demise and feels different levels of smart is another can of worms. On top of this, once you start thinking about what a *smart* snake would entrail and the things that each of those details entrails, things start to get complicated. The best we can do before throwing ouselves into this battle for the computational ages is to lay out the needs and approachs for the AI implementation, from a conceptual level to boilerplate code translations that will then be developed. So let's break the shell with some key insights:
- **The real challenge** isn't eating, **is not dying with a belly full of apples** (after all, in the current, base design of *nibbler*, the dead player loses, no matter the distribution of apples. Being larger just gives you more ability to entrap the opponent).
- **Tail reachability is critical**, because that spot is always going to be a safe zone until the very end of the game (unlikely reached, a state of space fully covered by snake bodies, minus the 2-gap threadshold that signals the end of the game by exhaustion).
- **Difficulty will be a matter of parameter tunning**, not different and/or separate algos
- **Reacting to moving opponents** is what would give the *smart* finishing touch to the AI

Translating this into more fleshed out considerations, we can state the following:
1. **Space Management**
    - When grid gets crowded, the AI should **prefer moves that keep more open space aorund it**. Or at least this is how I play, but I'm no *snake* expert.
    - For territory control, my research tells me that `Voronoi` style **territory control** would enhance how the AI attempts to react to the opponent's presence in the game arena.
2. **Decision Priority** (matters mostly **when food path is unsafe**). In the mind of the AI snake:
    - Can I safely reach food? → Go for it!
    - If not, can I reach my tail? → Chase own tail, let's call it *survival mode*
    - Still a no? → Move towards the most open space zone
    - If at this point all we gather is a bag of Noes, just try to find the least bad movement, but this snake is cooked no matter what
3. **Opponent prediction**
    - For hard mode, predict the next 1 or 2 movements of the opponent. (I might need some black magic here)
    - If prediction succeeds, those targetted spots become D A N G E R  Z O N E S. Avoid!!
4. **Performance**
    - Even though the subject of matter (*snake*) and the controlled grid sizes allow for naive approaches without tanking implementation, refinement and optimization are paramount. The point here is to learn how to code good AI, not to simply insert an AI opponent in this basic snake game. (If that was the case, a manhattan magnitude, apple-obsessed snake would suffice, as I stated above)
    - Find the sweet spot for recalculation. Doing so at every tick might be possible in this game, but refer to previous point for further information.

And taking this into consideration, and as previously advanced, the route to AI excellence will be based on a combination of two algorthms, `A*` and `Dijkstra`. Why, you might ask, well I'm here to tell you. Both are, taken as a base approach, good for the task:
- `Dijkstra` is breadth-first with costs, which can be translated into human as "from my current position, flood the grid evenly until goal is reached", or in other words, it's a **flood-fill**. It is a good option when all moves cost the same (which right now is, I think, the case), distance maps are the objective and the subject grids are small, and has these basic properties:
    - Explores the space uniformly
    - Guarantees the shortest path
    - does *not* really know where the goal is (just finds it in real time)
- `A*` is kind of an enhancement over `Djikstra`, something along the lines of "do the same as Dijkstra but prioritize nodes closer to the goal". I.E, a refinement. It weights costs and makes decisions based on an heuristic, which in an ortogonally moving snake is usually the `Manhattan distance` (|x1 - x2| + |y1 - y2|). It's much faster, more goal oriented and relatively optmal if the heuristic is admissible, and is based on these building blocks:
    - **`g(n)`** = cost so far
    - **`h(n)`** = heuristic (estimated distance to goal)
    - **`f(n) = g(n) + h(n)`**

So, combinig both algos, we can get an approach that can be summed up as **"`A*` for intent, `Dijkstra` like flood-fill for safety**. With even *more other* words, this combination will make the snake **find the food, the simulate moving towards it, then check if tail is still rechable after the hipothetical chomp**. Alongisde, difficulty levels can be, at least ideally, design this way:
- EASY
    - greedy movement (APPLE > ALL)
    - short A* search depth
    - long-term trapping ignored
    - delayed reactions
- MEDIUM
    - full A* to food
    - basic collision avoidance
    - no tail-reachability check
- HARD
    - A* to food
    - safety simulation (path finded check)
    - tail reachability via flood fill
    - avoid risky enclosures by prioritizing open spaces
    - reactive to opponent (dynamic obstacle, prediction in the mix)
Easier written than coded, even more if we want to constrain the amount of variables and knobs, which shouldn't expand beyond:
- N ticks between recomputations
- A* node expansions cap
- Radom move chance (a cheap, artificial addition to dampen the snake's intelligece, but could add the finishing touches for a more *naturalistic* game feel)

<br>

## 15.3 Too Many Whats in the Land of Hows
Enough about words, let's lay out some code plans. A new `AI/` subdirectory will be added to `srcs/`, with a handful of, as per my predictions, needed new classes and data related stuff:
- Pathfinder → `A*` implementation
- Floodfill → `Djikstra` implementation (role of safety checker)
- SnakeAI → Main AI class, the decision maker if you will
- AIConfig → Settings (mainly for difficulty, decoupled from main game's settings because there will not always be an AI opponent)

Also, to prepare the testing grounds some already existing stuff will need to be changed/expanded, like the addition of an extra mode to `GameMode`, with the corresponding initialization and update steps in `GameManager`. All together, in code (and subject to possible future changes):
```cpp
enum class GameMode {
	SINGLE,
	MULTI,
	AI
};
```
```cpp
class PathFinder {
	public:
		struct Node {
			Vec2 pos;
			int gCost;                              // Distance from start
			int hCost;                              // Heuristic to goal
			int fCost() { return gCost + hCost; }
			Node* parent;
		};
		
		std::vector<Vec2> findPath(
			const GameState& state,
			Vec2 start,
			Vec2 goal,
			int maxDepth = 200                      // Difficulty tuning
		);
		
	private:
		int manhattanDistance(Vec2 a, Vec2 b);
		bool isReachable(const GameState& state, Vec2 pos);
	};
```
```cpp
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
```
```cpp
class SnakeAI {
public:
	SnakeAI(AIConfig config);
	
	Input decideNextMove(const GameState& state);
	
	private:
		AIConfig config;
		PathFinder pathFinder;
		FloodFill floodFill;
		
		// decision modes
		Input goToFood(const GameState& state);
		Input survivalMode(const GameState& state);
		Input maximizeSpace(const GameState& state);
		
		// safety checks
		bool isSafeMove(const GameState& state, Vec2 nextPos);
		int evaluateMoveSpace(const GameState& state, Vec2 nextPos);
};
```
```cpp
struct AIConfig {
	// this is were difficulty settings are, well, set
	enum Difficulty { EASY, MEDIUM, HARD };
	
	Difficulty level;
	
	// Timing
	int thinkDelay;			// Ticks between decisions (0 = every tick)
	
	// Pathfinding
	int maxSearchDepth;		// Node expansion limit
	bool useSafetyCheck;	// Tail reachability
	bool predictOpponent;	// Consider opponent movement
	
	// Behavior
	float randomMoveChance;	// 0.0 - 1.0 (for easy mode)
	float aggressiveness;	// 0.0 = cautious, 1.0 = greedy
	
	static AIConfig easy();
	static AIConfig medium();
	static AIConfig hard();
};
```

Now, I'll go and make the necessary changes in the current build to be able to start implementing/testing the AI. Be right back...