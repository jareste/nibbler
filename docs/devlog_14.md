# Nibbler - Devlog - 13

## Table of Contents
1. [Day Eighteen Plan](#141-day-eighteen-plan)
2. [A Russian Doll of Resolutions](#142-a-russian-doll-of-resolutions)
3. [Planning Plans](#143-planning-plans)

<br>
<br>

## 14.1 Day Eighteen Plan
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

## 14.3 Planning plans