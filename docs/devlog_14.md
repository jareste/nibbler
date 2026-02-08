# Nibbler - Devlog - 13

## Table of Contents
1. [Day Eighteen and Nineteen Plan](#141-day-eighteen-and-nineteen-plan)
2. [A Russian Doll of Resolutions](#142-a-russian-doll-of-resolutions)
3. [Planning Plans](#143-planning-plans)
4. [Multiplayer implementation (or I Can't Come Up With a Smart Title For This Section Right Now Because I'm Mentally Toasted)](#144-multiplayer-implementation-or-i-cant-come-up-with-a-smart-title-for-this-section-right-now-because-im-mentally-toasted)

<br>
<br>

## 14.1 Day Eighteen and Nineteen Plan
There are two main things in today's table. One is the **multiplayer implementation**, which in itself is not a big deal, but becomes a sort of one due two main reasons: the need to implement it in all three libraries and the requirement for it to be optional, i.e., some sort of mechanism needs to be put in place so that users can play single or multiplayer at choice. The first necessity is just pure hustle: work the implementation in one library (`SDL` is going to be the choice), then *port* it to the others. Work, work, work, but simple enough.

The other thing is that this a day of important decisions. In fact, I think that regarding *nibbler*, this might be the second to last devlog of the project, as the next one will be a general polish and preparation for turning in the academic foundation of what's to come (all pending in the sound stuff, done by my partner). After that, the first phase of the project ends and the second, more complex, personal and free one starts. The moment in which we go from *nibbler* to what I trully want to build as a stand alone game and portfolio piece, which is code named in my mind as *rosario*, or rosary, i.e., a chain of beads (guess what one it's main mechanics is going to be in this extended, new take on *snake* ;D).

A lot, and i mean A LOT of stuff needs to be reassessed. A comprehensive and ambitious plan needs to be drafted for a more or less two week development of phase 2. A refocus will have to be performed from the multi-library based project to a full, game-engine one. Things will get real, so to speak. And we will have to give it our all.

So let's switch orders in this log and start with the planning and decision related stuff. After that, I'll register everything related to the multiplayer implementation.

<br>
<br>

## 14.2 A Russian Doll of Resolutions
Besides some stuff happening around me at this very moment, which I rather don't talk about here, there's one big reason behind the decision to cap *nibbler's* development and go into a somewaht phase 2. It mainly derives from the multiplayer implementation experience I'm having, but it has also a lot to do with some anxieties I've been ignoring for quite sime time. Deep within this general development, there are a couple of ambitions that I strongly want to carry out:
- I want this game to be a standalone, cross-platform program that can be packaged and distributed
- I'd like to try to make a web deployment of the game, at least of one of its builds
- I have a lot of ideas for experimentation across all spheres of what developing a small game like this entrails (mechanics, visuals, sound design, ...)

All of this combined invokes a huge elephant in the room: how the three library base approach makes things way too complicated, at times even impossible or strongly insane. Up until this point, it has been both a requirement and a *novelty*, a way of getting in touch with how dynamic libraries work and an overall interesting challenge. But moving forward, there's quite some consequences to the multi-library approach that, if carried along into further levels of complexity would make very little sense, if at all. For example, the inscribed loop of destroying and rebuilding when hot swapping libraries would translate poorly to a `WASM` based web deployment, and the same would go for a stand-alone execution of the game in whatever system I would like for it to be, well, executable. Furthermore, any future development of the game, be it in the mechanics or the visual side of things, would need at least a triple implementation. I'm having that *problem* right now with the multiplayer, which while writing this section is quite advanced in `SDL` but is still pending in `NCurses` and `Raylib`. And that's if I only take into consideration the logic itself; if I add the fact that the startinc screen/menu needs to inform and manage the possibility of single player and multiplayer executions, and how that's going to affect the design, and how because there's a carefully layed out composition across libraries any change to the menu screens would need to have it's echo in all of them... Well, things escalate extremely quickly. And they do it because of an imposed rule that needs not to be kept moving forward, so it would be quite... silly on my part to keep on keeping on.

Let's lay it out with some specifics, to further develop the argument. (To whom, you might ask. Well, for myself, I can only answer. I'm just trying to record my thought process and give myself some reassurance in this trying times). Say that after the multiplayer the next step is an AI option, a way of setting up a 1v1 game session but against a computer controlled opponent. Say, too, that some options want to be added as possibilities when building a game session, like toggeable wall wrapping, friendly collisions, game speed, AI aggressivenes, ... Stuff piles up, and a simple way of managing it would be to work with a `GameConfig` data struct, fed by the user selected options, read by the `GameManager` and its systems. It could look like this:
```cpp
struct GameConfig {
	enum class GameMode { SinglePlayer, VersusAI, Versus1v1 };
	enum class Difficulty { Easy, Normal, Hard };
	
	GameMode mode = GameMode::SinglePlayer;
	Difficulty difficulty = Difficulty::Normal;
	bool wallWrapping = false;      // true = teleport, false = collision
	bool friendlyFire = false;      // can snakes collide? And if so, how would it work?
	int aiAggressiveness = 5;       // 0-10 scale, or whatever, I don't know right now
	float gameSpeed = 1.0f;
};
```

This would be independent of the `GameState` to fully decouple managing spheres, but that's not important right now. The important thing is that every little addition would need, in the current state of the project, a triple implementation, what in simpler words would be translated into a garanteed mess. It's not really about the work and time needed for things to exist in an `ASCII/2D/3D` - `NCurses/SDL/Raylib` scheme, which in the end needs to happen if I want to keep the idea of "visual dimensions" (which I want). It's more about the compatibility and specific ways that each library requires to wrap around each kernel of implementation. And all of that without taking into consideration that 3 libraries with 3 different contexts requires the control of 3 different rendering environments across devices. The `NCurses` version won't look the same in my laptop's console and in your PC's one. `SDL's` square based drawing needs to take into account the device and it's resolution to set up crucial values so that the game is correctly displayed in any given screen (not cropped, not too tiny, etc). And so on and so forth. 

Menu design, layout and code. System development with triple compatibility. Consistency and render control across environments (standalone, web, ...). The recipe spells disaster. All without adding other concerns to the mix, like the bloat of dependencies that comes with multiple libraries, the compatibility of environments with those libraries, the maintenance burden, the infinite list of etceteras that comes to mind. So an **strategic pivot** is obviously necessary.

This pivot, as I've been mentioning from the beginning of this log, will divide the development of the game in, for now, two phases. Until here, *nibbler* has been the perfect context for laying out a `C++` basic game engine, perfect for the discovery of new concepts tools and techniques, and a sort of flagship project in its academic sections. But from this point, I want to give my all to the development of the game itself, so that it can be a *personal* showcase of game development, `C++` skills and overall design abilities. To this end, **the first step in phase two will be to unify everything under `Raylib`**, taking the different looks of the game into different representation methods inside this single library. Conceptually, something like the following, all controlled by some sort of `VisualStyleManager`:
```cpp
enum class VisualStyle {
    Isometric3D,    // Current Raylib build
    Flat2D,         // 2D port of SDL into Raylib
    ASCII,          // Texture-based ASCII representation
    WhoknowsWhat	// The sky would be the only limit
};
```
Some good news is that taking everything into `Raylib` will give me much, much more control of the visuals across ways of representation. Just by tansitioning from plain, text based ASCII characters in `NCurses` to a texture-based version would open a huge spectrum of possibilities, and to the best of my knowledge `Raylib` gives me more 2D tools than `SDL`, although I might be wrong about this. Be what it may, there's a huge community around `Raylib`, so the decision kind of makes itself (not to mention that this is quite a commonly used library in the industry).

So, summing a substantial part of this up, there will be a transitioning **from library-based logic to a dimension-based one**, all baked in an extended, quite more complex `Raylib` build than the current one. This even gives me the possibility of working the dimension transitions, which right now are based on destroying-rebuilding game windows in visually non-continuous procedure that renders any kind of illusion quite impossible, or at least highly improbable.

<br>
<br>

## 14.3 Planning Plans
The immediate course of action calls for a division in development phases. Phase 1 closes the academic chapter of the project, getting *nibbler* ready for submission. Phase 2 marks the beginning of the personal, portfolio-oriented work, where the game morphs from a multi-library proof-of-concept into a unified, polished experience. Both phases have different goals, different constraints, and different measures of success.

### Phase 1: Academic Closure (February 9-11)
The first phase is about **functional completion** and **submission readiness**. This means that the multiplayer mode needs to work, even if in a basic implementation, across all three libraries. The focus here is not polish, but coverage and correctness.

#### Core Implementation (February 9)
The bulk of the work in this phase lands on making the multiplayer functional in all three rendering contexts. `SDL2` is the natural starting point due to it's already set up particle systems and text renderers, which makes prototyping faster. Once the base implementation is solid there, porting the logic to `NCurses` and `Raylib` becomes a matter of adapting interface calls and input handling.

The main tasks for this day are:
- **Finish multiplayer implementation in `SDL2`**
	- Two snake instances (`snake_A`, `snake_B`)
	- Independent input processing (arrow keys vs WASD)
	- Visual differentiation (color-coded snakes and trails)
	- Collision detection between snakes
	- Proper handling of food collision (which snake eats what)

- **Port multiplayer to `NCurses`**
	- Adapt `pollInput()` to handle both player control schemes
	- Update `drawSnake()` to render both snakes with different color pairs
	- Adjust text rendering for score/status of both players

- **Port multiplayer to `Raylib`**
	- Add second snake rendering in isometric view
	- Handle input routing for both players
	- Update camera framing if needed for larger play areas

#### Polish and Submission Prep (February 10-11)
Once the multiplayer works across libraries, the focus shifts to making sure everything is stable and submission-ready:
- **Input buffer refinement**: Ensure both players' inputs are correctly queued and processed without conflicts
- **Collision edge cases**: Test and fix any remaining collision bugs (self-collision, wall collision, snake-to-snake collision)
- **Menu state handling**: Add a basic toggle or startup parameter to choose single player vs multiplayer (doesn't need to be fancy, just functional)
- **Documentation cleanup**: Update README with build instructions, controls for both players, and any submission-specific notes
- **Final testing**: Run through the game in all three libraries, both single and multiplayer modes, to catch any last-minute issues

At the end of this phase, *nibbler* as an academic project is complete and ready to turn in.

<br>

### Phase 2: The Raylib Transition (February 12-24)
This is where the project transforms. Phase 2 is about **consolidation**, **refactoring**, and **ambition**. The multi-library approach served its purpose, but from this point forward the game needs to breathe as a unified experience. `Raylib` becomes the foundation, and the different visual representations become rendering modes within a single, cohesive architecture.

The timeline for this phase spans roughly two weeks, which at 60 hours a week gives us around 120 hours of development time. This is substantial, but not infinite, so prioritization is key. And I need to survive the process, let's not forget about it. This will be the nth sprint in a series of sprints that I've been chaining for a couple of years now, so let's try not to die. Alive and with a game: the ultimate objective.

#### Week 1: Foundation and Refactoring (February 12-18)
The first week is structural. It's about tearing down the multi-library scaffolding and building the new foundation without losing what works.

**Day 1-2: Branch and Consolidate**
- Create a `v2` branch to preserve the original multi-library implementation
- Strip out `SDL2` and `NCurses` libraries and their build processes from `Makefile`
- Centralize everything under `Raylib`
- Refactor `IGraphic` interface to fit a single-library context (might not need the interface anymore, but keep it for now in case we want to experiment with other engines later)

**Day 3-4: Visual Style System**
This is the conceptual leap. Instead of three libraries, we now have three (or more) visual styles, all rendered through `Raylib`. The implementation of a `VisualStyleManager` becomes the centerpiece:
```cpp
enum class VisualStyle {
    Isometric3D,    // Current Raylib 3D cubes
    Flat2D,         // Recreate SDL's square-based look
    ASCII,          // Texture-based monospace ASCII
};
```
Each style has its own rendering pipeline, but they all share the same game state, input handling, and logic. This means:
- **2D Mode**: Use `Camera2D`, draw snakes as colored squares, flat food sprites
- **ASCII Mode**: Render to a texture using a monospace font, maybe play around with some postprocessing (YES I WANT TO GO FULL CRT AGAIN BUT I'LL TRY TO RESTRAIN MYSELF)
- **3D Mode**: Keep the current isometric cube rendering

The key here is smooth transitions. No window destruction, no flickering. Just a camera shift, a render mode change, and maybe a brief animation. (Well, first there will be an auto-switch, an instant change. Then we'll see how to polish the transition, but planning is ever-able, ambition-embracing, dream-catching).

**Day 5-7: Gameplay Configuration System**
With the visual consolidation in place, the next step is building the `GameConfig` system to handle gameplay variants. This is where the game becomes more than a tech demo:
```cpp
struct GameConfig {
    enum class GameMode { SinglePlayer, VersusAI, Versus1v1 };
    enum class Difficulty { Easy, Normal, Hard };
    
    GameMode mode = GameMode::SinglePlayer;
    Difficulty difficulty = Difficulty::Normal;
    bool wallWrapping = false;
    bool friendlyFire = false;
    float gameSpeed = 1.0f;
    // More options as needed
};
```
This struct feeds into the `GameManager`, which uses it to drive gameplay rules. For example:
- If `wallWrapping` is true, hitting a wall teleports the snake to the opposite side instead of triggering game over
- If `friendlyFire` is true, snakes can collide with each other; otherwise they phase through
- `gameSpeed` modifies the update frequency, making the game faster or slower

This also means refactoring collision detection and movement logic to be config-aware. The goal is flexibility without bloat. Points for maintaining sanity.

#### Week 2: Features, Polish, and Deployment (February 19-24)
The second week is about bringing the game to life. Features, juice, and presentation.

**Day 8-10: AI Implementation**
One of the big additions for the portfolio version is an AI opponent. This doesn't need to be perfect, but it needs to be interesting. A basic pathfinding AI that can chase food and avoid obstacles is a good start. For extra depth, implement difficulty levels:
- **Easy AI**: Simple greedy pathfinding, sometimes makes mistakes
- **Normal AI**: Better pathfinding, avoids traps
- **Hard AI**: Anticipates player movements, plays aggressively

The AI should plug into the existing `Snake` class via a controller pattern, so that switching between human and AI control is seamless. *At least, that is the idea*.

**Day 11-12: Interactive Menu System**
The start screen evolves from a static title into an interactive menu. This is where `GameConfig` options become user-facing:
- **Play Solo**: SinglePlayer mode
- **Play vs AI**: VersusAI mode, with difficulty selection
- **Play 1v1**: Versus1v1 mode, local multiplayer
- **Options**: Toggle wall wrapping, friendly fire, game speed, visual style
- **Quit**: Exit the game

Menu navigation should be intuitive (arrow keys, Enter to select) and visually consistent with the current aesthetic (snake-based typography, particle effects, etc.).

**Day 13-14: Dimension-Switching as Mechanic**
Here's where the concept gets interesting. The dimension-switching isn't just a visual gimmick anymore—it becomes a gameplay mechanic. Different dimensions have different rules:
- **ASCII Dimension**: Wall wrapping enabled, faster game speed, retro CRT effects
- **2D Dimension**: Standard rules, balanced gameplay
- **3D Dimension**: Isometric view, maybe obstacles or gravity-based mechanics

Power-ups could force dimension shifts, or the player could choose to switch mid-game for strategic advantage. This is experimental, but it's also what makes the game unique.

**Day 15-16: Polish and Juice**
The game needs to *feel* good. This means:
- **Particle effects**: Enhanced trails, explosions on food pickup, dimension shift effects
- **Screen shake**: Subtle camera shake on collisions
- **Sound design**: Basic audio feedback for movement, eating, collisions, dimension shifts (if time allows, and all derived from what my *nibbler* partner adds before submission)
- **Animations**: Smooth transitions between game states, menus, and visual styles

**Day 17-18: Web Deployment**
`Raylib` has excellent WASM support, which means the game can run in a browser. This is huge for portfolio purposes. The steps:
1. Compile the game to WASM using `emscripten`
2. Host it on `itch.io` or a personal site
3. Test across browsers (Chrome, Firefox, Safari)
4. Optimize for load times and performance

Having a playable web link in two weeks time would be incredible, but also extremely ambitious. Let's aim for it but not lose ourselves in the way. Therefore, and because two weeks is tight for everything outlined above, if time runs short, priorities are:
1. **Visual style system** (2D and ASCII modes functional)
2. **GameConfig system** (at least wall wrapping and game mode selection)
3. **Interactive menu** (functional, even if not fully polished)
4. **AI opponent** (basic implementation, difficulty levels if time allows)
5. **Web deployment** (critical for portfolio impact)

In the land of hopes and dreams, an example of playful experimentation with the basic *snake* components would also be showcased. Something along the lines of **different types of food**, **warping walls and obstacles**, an initial prototyping of the **bead-based avatars**. This is surely not going to happen, but I like to be ambitious. (And I usually eat those ambitions, but WILLING is the first step towards WAYING, which is a silly way of referencing the good old *where there's a will, thre's a way*. I've been writing this document for the most part of a Sunday, and the rest of the day's time has gone into cleaning my house. I'm close to losing it.)

<br>
<br>

## 14.4 Multiplayer implementation (or I Can't Come Up With a Smart Title For This Section Right Now Because I'm Mentally Toasted)