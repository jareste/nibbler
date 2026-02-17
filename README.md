# <h1 align="center">NIBBLER</h1>

<p align="center">
   <b>A cross-platform Snake game reimagining with hot-swappable graphics libraries and zero-leak memory management. Exploring the boundaries between ASCII art, pixel graphics, and isometric 3D — all in a single executable.</b><br>
</p>

---

<p align="center">
    <img alt="C++" src="https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
    <img alt="NCurses" src="https://img.shields.io/badge/NCurses-Terminal-green?style=for-the-badge" />
    <img alt="SDL2" src="https://img.shields.io/badge/SDL2-2D-blue?style=for-the-badge" />
    <img alt="Raylib" src="https://img.shields.io/badge/Raylib-3D-orange?style=for-the-badge" />
</p>
<p align="center">
    <img alt="CI/CD" src="https://github.com/hugomgris/nibbler/actions/workflows/ci.yml/badge.svg?branch=main" />
    <img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/hugomgris/nibbler?color=lightblue" />
    <img alt="License" src="https://img.shields.io/badge/License-MIT-blue" />
    <img alt="Status" src="https://img.shields.io/badge/Status-WIP-yellow" />
</p>

---

## Table of Contents
1. [In the Graphics Gardens of Eden](#in-the-graphics-gardens-of-eden)
2. [The Core Concept](#the-core-concept)
3. [Technical Architecture](#technical-architecture)
4. [Building & Running](#building--running)
4. [In Depth Break Down](#in-depth-break-down)
5. [Contributing](#contributing)

<br>

## In the Graphics Gardens of Eden

**Nibbler** is a Snake game multi-platform recreation. It features different realms of graphics rendering by hot-swapping libraries in real time, building a visual cross-realm trip from ASCII to isometric 3D representations of the same apple filled garden. The little, squiggly protagonist starts it's chomping mission in the launching terminal thanks to some `Ncurses` superpowers, and can cross whenever it wants (maybe whenever *some conditions are met* in the near future) to a 2D, `SDL2` kingdom of vectors and particles, after which a cube based landscape awaits as a final ascension step, deep down `Raylib`'s fields.

This is a **work-in-progress game** (soon to be finished) built in **C++20** as an exploration of:
- **Dynamic library loading**: hot-swappable graphics backends
- **Memory-safe architecture**: zero leaks across all three libraries
- **Game feel experimentation**: finding the sweet spot between responsive and deliberate
- **Visual aesthetics**: from dithered terminal art to isometric perspectives

<p align="center">
  <img src="./docs/ASCII_REALM.png" alt="NCurses ASCII realm" height="250"/>
  <img src="./docs/2D_REALM.png" alt="SDL2 2D realm" height="250"/>
  <img src="./docs/3D_REALM.png" alt="Raylib isometric 3D" height="250"/>
  <br>
  <i>Three gardens, one snake</i>
</p>

<br>

## The Core Concept

### Why Three Graphics Libraries?

I wanted to see how the same game logic could manifest across radically different visual presentations, first, and then make it a round trip by seeing how different visual presentations can affect back the shared game logic:

- **NCurses (Terminal)**: Dithered shadows, Unicode box-drawing borders, ground texture from ASCII punctuation. I've always loved terminal-based graphics and games, so starting here was an easy decision to make.

- **SDL2 (2D Pixel Art)**: Crisp sprites, smooth animations, classic pixel-perfect Snake aesthetics. I've been dabbling in clean, geometrical 2D graphics for a while, so taking the chance to experiment some more was also a logical step.

- **Raylib (Isometric 3D)**: Tilted perspective, depth-aware rendering, shadows and volume. Snake but make it cubic. 3D graphics are usually the place to go for me, as I'm a way better modeler than artist, so I'm quite versed in 3D pipelines (even to achieve 2D graphics, as seen in my [ditherer tools](https://github.com/hugomgris/sprite_ditherer), for example).

The technical challenge: **runtime library switching**. No restarts, no state loss, just seamless transitions between visual realms with the hit of a button. First as a first-hand feature, then as a gameplay and narrative tool.

### The Dynamic Library Architecture

Instead of compiling graphics code directly into the executable, Nibbler loads graphics libraries as **shared objects** (`.so` files) at runtime:

```
nibbler (executable)
  ↓ dlopen()
  ├── nibbler_ncurses.so  → NCurses renderer
  ├── nibbler_sdl.so      → SDL2 renderer
  └── nibbler_raylib.so   → Raylib renderer
```

When you press `1`, `2`, or `3`:
1. Current library is unloaded (`dlclose()`)
2. New library is loaded (`dlopen()`)
3. State is reinitialized
4. Game continues without blinking

This is the same technique used in **game engine plugins**, **browser extensions**, and **professional game development** (think Unreal Engine's hot-reload), just made by hand as a little game and graphics engine.

<br>

## Technical Architecture

### Project Structure

```
nibbler/
├── incs/                         # Header files
│   ├── IGraphic.hpp              # Graphics interface (all libs implement this)
│   ├── Snake.hpp                 # Snake game logic
│   ├── Food.hpp                  # Food spawning and rendering data
│   ├── GameManager.hpp           # Game state machine
│   ├── LibraryManager.hpp        # Dynamic library loader
│   └── ...
│
├── srcs/
│   ├── main.cpp                  # Game loop and state machine
│   ├── Snake.cpp
│   ├── Food.cpp
│   ├── GameManager.cpp
│   ├── LibraryManager.cpp
│   └── graphics/
│       ├── NCursesGraphic.cpp    # Terminal renderer (compiled to .so)
│       ├── SDLGraphic.cpp        # 2D renderer (compiled to .so)
│       └── RaylibGraphic.cpp     # 3D renderer (compiled to .so)
│
├── logos/                        # Art title screens across libraries
│   ├── ncurses_title_big.txt
│   ├── ncurses_gameover_big.txt
│   └── ...
│
├── checks/                       # Memory leak testing infrastructure
│   ├── check_leaks_unified.sh    # Primary Valgrind checker
│   ├── all_libs.supp             # Suppression file for library internals
│   └── ...
│
├── docs/                         # Development logs and documentation
│   ├── devlog_01.md              # Journey from concept to first playable
│   ├── devlog_15.md              # Latest: color management fixes
│   └── ...
│
├── Makefile                      # Build system
└── README.md
```

### The Interface Pattern

All three graphics libraries implement the same interface (`IGraphic.hpp`):

```cpp
class IGraphic {
public:
    virtual void init(int width, int height) = 0;
    virtual void render(const GameState& state, float deltaTime) = 0;
    virtual void renderMenu(const GameState& state) = 0;
    virtual void renderGameOver(const GameState& state) = 0;
    virtual Input pollInput() = 0;
    virtual ~IGraphic() = default;
};
```

This means the game logic (`main.cpp`) doesn't need to know *which* library is loaded. It just calls `render()` and the active library handles the rest.

### The State Machine

The game operates as a state machine with four states:

```
┌─────────┐  Enter   ┌─────────┐  Space   ┌─────────┐
│  Menu   │ ───────> │ Playing │ <──────> │ Paused  │
└─────────┘          └─────────┘          └─────────┘
                           │
                           │ Collision
                           ↓
                      ┌─────────┐  Enter   ┌─────────┐
                      │GameOver │ ───────> │  Menu   │
                      └─────────┘          └─────────┘
```

Each state has its own rendering logic and input handling. Library switching works in **any state** — you can travel between realms mid-game.

<br>

## Building & Running

### Prerequisites

- **C++20 compiler** (GCC 11+ or Clang 13+)
- **NCurses** (usually pre-installed on Linux/macOS)
- **SDL2** development libraries
- **Raylib** (source included in `libs/raylib/`)
- **Make**

### Linux/macOS Build

```bash
# Clone the repository
git clone https://github.com/jareste/nibbler.git
cd nibbler

# Build everything (executable + all three .so libraries)
make

# Run the game
./nibbler 30 30  # 30x30 game arena
```

> If you want a quick start, just run `make game` and a sample execution will be built and launched for you

The Makefile automatically:
1. Builds Raylib from source (if not already built)
2. Compiles NCurses library
3. Compiles SDL2 library
4. Links the main executable
5. Creates symbolic links for easy loading

### Build Flags

The project uses strict compilation flags to catch bugs early:

```makefile
CXXFLAGS = -Wall -Wextra -Werror -std=c++20 -fPIC -g3
```

- `-Wall -Wextra -Werror`: Treat all warnings as errors
- `-std=c++20`: Use C++20 features (especially for better move semantics)
- `-fPIC`: Position-independent code (required for shared libraries)
- `-g3`: Debug symbols for Valgrind

<br>

## In-Depth Break Down

### The Architecture Of Life Choices

Nibbler can be understood as a **plugin architecture disguised as a Snake game**. The real challenge was building a system where:
- The game logic has **zero knowledge** of which graphics library is loaded
- Libraries can be **hot-swapped at runtime** without losing game state
- Memory management stays **leak-free** across all three backends
- The same `Input` enum works for terminal keypresses, SDL events, and Raylib polling

Let's break down how this actually works.

<br>

### **The Core Game Loop: Decoupled by Design**

The main loop in [`srcs/main.cpp`](srcs/main.cpp) is deliberately ignorant about graphics. It only knows three things:

1. **Game state exists** ([`GameState`](incs/DataStructs.hpp) struct with snake position, food, score, etc.)
2. **Something can render it** (via the [`IGraphic`](incs/IGraphic.hpp) interface)
3. **That something can be swapped** (via [`LibraryManager`](incs/LibraryManager.hpp))

The main executable **never** includes `<ncurses.h>`, `<SDL2/SDL.h>`, or `<raylib.h>`. It only sees the abstract interface. This is **Inversion of Control** in action, i.e. the graphics libraries serve the game, not the other way around:

```cpp
while (state.isRunning) {
    // 1. Get input from whatever library is loaded
    Input input = libManager.getGraphicLib()->pollInput();
    
    // 2. Update game logic (framerate-independent)
    if (accumulator >= FRAME_TIME) {
        gameManager.update();
        accumulator -= FRAME_TIME;
    }
    
    // 3. Render using whatever library is active
    libManager.getGraphicLib()->render(state, deltaTime);
    
    // 4. Handle library switching (keys 1/2/3)
    if (input == Input::SwitchLib1) {
        libManager.unload();
        libManager.load("./nibbler_ncurses.so");
    }
    // ... etc for SDL2 and Raylib
}
```
<br>

### **Dynamic Library Loading: The dlopen Dance**

When you run `./nibbler 30 30`, here's what happens behind the scenes:

#### **1. Library Loading ([`LibraryManager::load()`](srcs/LibraryManager.cpp))**

```cpp
bool LibraryManager::load(const char* libPath) {
    // Open the .so file
    handle = dlopen(libPath, RTLD_NOW);
    if (!handle) {
        std::cerr << "Failed to load: " << dlerror() << std::endl;
        return false;
    }
    
    // Find the "createGraphic" function inside the .so
    CreateFn create = (CreateFn)dlsym(handle, "createGraphic");
    if (!create) {
        std::cerr << "Symbol not found: createGraphic" << std::endl;
        dlclose(handle);
        return false;
    }
    
    // Call it to instantiate the library's renderer
    graphic = create();
    return true;
}
```

**What's happening:**
- `dlopen()` loads the `.so` file into memory (like a plugin)
- `dlsym()` finds the `createGraphic` function **by name** (as a string! Thanks to the `C` like symbol exporting)
- We call that function to create an instance of `NCursesGraphic`, `SDLGraphic`, or `RaylibGraphic`
- The returned pointer is stored as an `IGraphic*` (the interface)

#### **2. The ABI Boundary, or Why We Need C**

Each graphics library exports two C-style functions:

```cpp
// Inside NCursesGraphic.cpp, SDLGraphic.cpp, RaylibGraphic.cpp
extern "C" {
    IGraphic* createGraphic() {
        return new NCursesGraphic();  // or SDLGraphic, or RaylibGraphic
    }
    
    void destroyGraphic(IGraphic* g) {
        delete g;
    }
}
```

**Why `extern "C"`?**
- C++ **name mangling** turns `createGraphic()` into something like `_Z13createGraphicv`
- `dlsym()` expects **predictable C-style names**, not mangled ones
- `extern "C"` tells the compiler: "Don't mangle this function name"
- This is the **only** way to make C++ classes loadable via `dlopen()`

> Making things safe: Objects are **created inside the `.so`** and **destroyed inside the `.so`**. Never `new` in main and `delete` in the library (or vice versa), because we're responsible with our memory and we absolutely want to avoid heap corruptions. We're that kind of people.

<br>

### **The Three Graphic Realms**

Each graphics library implements the same interface, but the internals are wildly different.

#### **NCurses: Terminal Graphics ([`NCursesGraphic.cpp`](srcs/graphics/NCursesGraphic.cpp))**

NCurses renders everything as **Unicode characters** in the terminal. No pixels, no textures, just good old text:

```cpp
void NCursesGraphic::render(const GameState& state, float deltaTime) {
    clear();  // Wipe the terminal
    
    // Draw border using box-drawing characters
    box(stdscr, 0, 0);
    
    // Draw snake (each segment is a '█' character)
    for (int i = 0; i < state.snake_A->getLength(); i++) {
        Vec2 seg = state.snake_A->getSegments()[i];
        mvaddch(seg.y + 1, seg.x + 1, ACS_BLOCK);
    }
    
    // Draw food (blinking '@' symbol)
    Vec2 food = state.food.getPosition();
    attron(A_BLINK);
    mvaddch(food.y + 1, food.x + 1, '@');
    attroff(A_BLINK);
    
    refresh();  // Update the terminal
}
```

To try to have as much graphic control as I can after seizing the terminal control, and to get it to a similar level as the other `2D` and `3D` implementations, some techniques are used:
- **Dithering**: Gradient borders ad ground "texture" from ASCII punctuation (`.`, `,`, `'`, `"`)
- **Color pairs**: hue management via `init_pair()` and `attron(COLOR_PAIR(n))`
- **No vsync**: Terminal rendering is synchronous because `refresh()` blocks until done

<br>

#### **SDL2: 2D Pixel Graphics ([`SDLGraphic.cpp`](srcs/graphics/SDLGraphic.cpp))**

SDL2 is a **low-level 2D graphics API**. You draw shapes, load images, and handle events manually. THe visual design here is cell-based, with crisp and plain colors, around which some particle and animations are sprinkled to enhance graphics:

```cpp
void SDLGraphic::render(const GameState& state, float deltaTime) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
    SDL_RenderClear(renderer);
    
    // Draw snake (each segment is a filled rectangle)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green
    for (int i = 0; i < state.snake_A->getLength(); i++) {
        Vec2 seg = state.snake_A->getSegments()[i];
        SDL_Rect rect = {
            seg.x * cellSize,
            seg.y * cellSize,
            cellSize,
            cellSize
        };
        SDL_RenderFillRect(renderer, &rect);
    }
    
    // Draw food (red square)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect foodRect = {
        state.food.getPosition().x * cellSize,
        state.food.getPosition().y * cellSize,
        cellSize,
        cellSize
    };
    SDL_RenderFillRect(renderer, &foodRect);
    
    SDL_RenderPresent(renderer);  // Swap buffers
}
```

The main approaches here are:
- **Cell-based rendering**: Game grid (30×30) maps to pixel grid (30×30 × cellSize)
- **Double buffering**: `SDL_RenderPresent()` swaps front/back buffers for smooth rendering
- **Event-driven input**: SDL uses an event queue (`SDL_PollEvent()`) instead of direct key polling

<br>

#### **Raylib: Isometric 3D ([`RaylibGraphic.cpp`](srcs/graphics/RaylibGraphic.cpp))**

Raylib is a **game-focused library** with built-in 3D primitives, cameras, and models. This was the choice for third library to add an isometric, 3D rendering of the same base game, so that a sort of evolution theme could be read from the whole implementation (ASCII->2D->3D):

```cpp
void RaylibGraphic::render(const GameState& state, float deltaTime) {
    BeginDrawing();
    ClearBackground(BLACK);
    
    BeginMode3D(camera);  // Enter 3D space
    
        // Draw ground plane
        DrawPlane((Vector3){0, 0, 0}, (Vector2){50, 50}, DARKGRAY);
        
        // Draw snake (each segment is a cube)
        for (int i = 0; i < state.snake_A->getLength(); i++) {
            Vec2 seg = state.snake_A->getSegments()[i];
            Vector3 pos = {
                (float)seg.x - state.config.width / 2.0f,
                0.5f,  // Slightly above ground
                (float)seg.y - state.config.height / 2.0f
            };
            DrawCube(pos, 1.0f, 1.0f, 1.0f, GREEN);
        }
        
        // Draw food (glowing red cube)
        Vec2 food = state.food.getPosition();
        Vector3 foodPos = {
            (float)food.x - state.config.width / 2.0f,
            0.5f,
            (float)food.y - state.config.height / 2.0f
        };
        DrawCube(foodPos, 1.0f, 1.0f, 1.0f, RED);
    
    EndMode3D();  // Exit 3D space
    
    // 2D UI overlay (score, etc.)
    DrawText("Score: 100", 10, 10, 20, WHITE);
    
    EndDrawing();
}
```

Here, some basic 3D techniques needed to be implemented:
- **Isometric configuration**: The camera needed specific initialization values to be placed, angled and set to an isometric view (some trigonometry spice)
- **Depth sorting**: Raylib handles Z-buffering automatically (cubes behind are hidden)
- **Hybrid 2D/3D**: UI elements are drawn **after** `EndMode3D()` in screen space
- **Post Processing**: Although this was developed in quite more depth in [the second phase of the project](https://github.com/hugomgris/rosario-engine), some Post Processing noise was layed over the 3D rendering to add some flair

---

###**Memory Management: Go Leakless or Go Home**

Every dynamic library creates resources (windows, textures, font atlases, etc.). If not cleaned up properly, **switching libraries leaks memory**, which is something that I found the bad way.

#### **The Lifecycle of a Graphics Library**

```
┌─────────────────────────────────────────────────────┐
│ 1. Load .so file         (dlopen)                   │
│ 2. Create renderer       (createGraphic)            │
│ 3. Initialize resources  (init)                     │
│                                                     │
│    [ Game runs for N frames ]                       │
│                                                     │
│ 4. Cleanup resources     (destructor)               │
│ 5. Destroy renderer      (destroyGraphic)           │
│ 6. Unload .so file       (dlclose)                  │
└─────────────────────────────────────────────────────┘
```

To keep things under control (or to seize it, really), some Decisions were made:
1. **RAII everywhere**: Use destructors to free resources (no manual `free()` calls)
2. **Match allocations**: If `NCursesGraphic::init()` creates a window, `~NCursesGraphic()` must destroy it
3. **Test with Valgrind**: Run `make check_leaks` to verify zero leaks across all three libraries (some `fsanitize` was also used, but `Valgrind` feels more trustworthy)

#### **Example: SDL2 Cleanup**

```cpp
SDLGraphic::~SDLGraphic() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();  // Shutdown SDL2 subsystems
}
```

This was extremely necessary because **library switching happens mid-game**. In the exampe above, if SDL's renderer isn't destroyed, switching to Raylib **leaks the renderer**, which is disastrous. This was specifically tricky to manage with the `NCurses` pipeline because at the early stages of development I was using a system installed build that worked with its own trackers and globals, which were impossible to clean in the process of hot-swapping libraries (I fought; I lost). The only viable option after exhausting every memory juggling trick under my sleeve was to switch to a precompiled, `.so` based version of `NCurses`. I added a build pipeline to handle this library just as the other two, decoupling it from the system and it's trackers, and leaklessness was finally achieved. 

>The main takeaway from not only the `NCurses` leak quest but from the general, novelty-ish approach of the nibbler project (i.e., the graphic library hotswapping) is that there is a **strong friction** between
how the libraries themselves are built and this dream of runtime switching. In other words, the libraries are not thought for this way of swapping them around and all sorts of little, yet quite annoying issues were constantly arising during development. A very interesting and nourishing learning experience, but absolutely not a sane (and practical) way of building a game (or any other app, that is). And one of the main reasons why the next phase of the project, linked above, is rebuilt around a single library (Raylib).

<br>

### **Input Handling: One Manager to Rule Them All**

Each graphics library has a **completely different** input system:

| Library | Input Method | Example |
|---------|--------------|---------|
| **NCurses** | Blocking key read | `int ch = getch();` |
| **SDL2** | Event queue polling | `SDL_PollEvent(&event);` |
| **Raylib** | Direct key state | `IsKeyPressed(KEY_UP);` |

Therefore, the Mental Health Department called for a unified [`Input`](incs/Input.hpp) enum that all libraries could translate to, in orther to bypass the cross-differences and have all library input talk the same language (MINE):

```cpp
enum class Input {
    None,
    Up_A, Down_A, Left_A, Right_A,     // Player 1
    Up_B, Down_B, Left_B, Right_B,     // Player 2
    Pause,
    SwitchLib1, SwitchLib2, SwitchLib3,
    Quit
};
```

Each library's `pollInput()` translates its native events:

```cpp
// NCurses version
Input NCursesGraphic::pollInput() {
    int ch = getch();
    switch (ch) {
        case KEY_UP:    return Input::Up_A;
        case KEY_DOWN:  return Input::Down_A;
        case '1':       return Input::SwitchLib1;
        case 'q':       return Input::Quit;
        default:        return Input::None;
    }
}

// SDL2 version
Input SDLGraphic::pollInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:   return Input::Up_A;
                case SDLK_DOWN: return Input::Down_A;
                case SDLK_1:    return Input::SwitchLib1;
                case SDLK_q:    return Input::Quit;
            }
        }
    }
    return Input::None;
}

// Raylib version
Input RaylibGraphic::pollInput() {
    if (IsKeyPressed(KEY_UP))    return Input::Up_A;
    if (IsKeyPressed(KEY_DOWN))  return Input::Down_A;
    if (IsKeyPressed(KEY_ONE))   return Input::SwitchLib1;
    if (IsKeyPressed(KEY_Q))     return Input::Quit;
    return Input::None;
}
```

---

### **The Build System: Makefile Wonderfull Hell (MWH)**

Building three separate `.so` files with different dependencies was... non-trivial. Here's how I ended up writing the [`Makefile`](Makefile) so it could handle it both when building locally and when trying to make github actions work without spamming my email account with F A I L U R E messages:

#### **1. External Library Setup**

```makefile
# Check if libraries exist, clone if missing
check_libs:
    @if [ ! -d "$(RAYLIB_DIR)" ]; then \
        git clone --depth 1 $(RAYLIB_REPO) $(RAYLIB_DIR); \
        cd $(RAYLIB_DIR)/src && make; \
    fi
```

>`--depth 1` clones only the latest commit (not the entire git history). Saves time and disk space.

#### **2. Separate Compilation for Each Library**

```makefile
# NCurses library
nibbler_ncurses.so: $(NCURSES_OBJS)
    $(CC) -shared -o $@ $^ -lncurses

# SDL2 library
nibbler_sdl.so: $(SDL_OBJS)
    $(CC) -shared -o $@ $^ $(SDL_LDFLAGS)

# Raylib library
nibbler_raylib.so: $(RAYLIB_OBJS)
    $(CC) -shared -o $@ $^ $(RAYLIB_LDFLAGS)
```

> key flags:
>- `-shared`: Creates a `.so` file instead of an executable
>- `-fPIC`: Position-Independent Code (required for shared libraries)
>- `-lncurses`, `$(SDL_LDFLAGS)`: Links against external libraries

#### **3. Main Executable (No Graphics Dependencies!)**

```makefile
$(NAME): $(OBJS) $(NCURSES_SO) $(SDL_SO) $(RAYLIB_SO)
    $(CC) $(CFLAGS) $(OBJS) -o $(NAME) -ldl
```

**Critical:** The main executable **only** links `-ldl` (for `dlopen()`). It does **not** link `-lncurses`, `-lSDL2`, or `-lraylib`. Those are loaded at runtime.

**Verification:**
```bash
ldd ./nibbler
# Should show:
#   libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2
#   (NO libncurses, NO libSDL2, NO libraylib)
```

---

### **Testing and CI/CD**

Memory leaks in graphics code are **insidious**. A single forgotten `SDL_DestroyTexture()` leaks 4KB every frame — 240KB/second at 60 FPS, so this was a perfect opportunity to further develop my test writing and automation abilities.

#### **Local Testing**

```bash
# Run full leak check across all libraries
make check_leaks

# What it does:
# 1. Compiles with -g3 (debug symbols)
# 2. Runs Valgrind on each library for 100 frames
# 3. Checks for "definitely lost" memory
# 4. Fails if any leaks detected
```

**Sample output:**
```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 0 bytes in 0 blocks
==12345==   total heap usage: 1,234 allocs, 1,234 frees
==12345== All heap blocks were freed -- no leaks are possible
```

#### **CI/CD Pipeline ([`.github/workflows/test.yml`](.github/workflows/test.yml))**

Every push to `main` or `dev` triggers:
1. **Build test**: Compile all `.so` files
2. **Memory test**: Run Valgrind on all libraries
3. **Unit tests**: Google Test suite (48 tests)

Everything is automated and can be followed through the github actions tab in, well, github.

<br>

> The short pitch, as if you were interviewing me: Nibbler is a Snake game that loads graphics libraries as plugins, switches between them mid-game, and never leaks memory. It's equal parts game, graphics experiment, and software architecture deep-dive. The snake is just along for the ride in this version (main protagonist in `V2`, though!).

<br>

## Devologgin'
I took the change to log every development session along the way. I've always wanted to keep a work diary when building stuff, so right at the beginning of the project I started taking notes and snapshots and screen captures to leave a trail of how things were working (and mostly breaking) at each step of the journey. They are extensive, in depth and full of backs and forths, portraying the battle that nibbler presented. If you want a trully, under the hood presentation of what this project is, was and could be (*is being* in `V2`), [please refer to the logs](https://github.com/hugomgris/nibbler/blob/main/docs/devlog_01.md), which go from `_01` to `_15`. And give me any feedback or comment that comes to your mind!

<br>

## Contributing

This is a personal learning project, but if you're inspired by the architecture or want to suggest improvements:

- **Open an issue** for bugs or feature ideas
- **Fork and experiment** — the codebase is designed to be hackable
- **Share your own dynamic library projects** — I'd love to see them!

<br>

---

> *Built with C++, three graphics libraries, Valgrind, and tears, and blood, and hopes, and dreams and the sheer will to persuade you, if you are reading and have the capability to do so, to give me a game developing job <3*

