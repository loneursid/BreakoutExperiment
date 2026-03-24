# Breakout Clone — Project Spec

## Overview
A classic Breakout arcade game clone built in C++ using raylib, targeting Windows.
Single level, core mechanics only. The goal is a clean, well-structured, playable game.

---

## Tech Stack
- **Language**: C++17
- **Library**: raylib (latest stable)
- **Platform**: Windows (64-bit)
- **Build**: CMake (minimum 3.15)
- **Compiler**: MSVC or MinGW-w64
- **Dependency management**: vcpkg
- **BDD framework**: cucumber-cpp (Gherkin .feature files) + Boost.Test runner
- **TDD framework**: doctest (single-header, header-only)

---

## Project Structure
```
breakout/
├── CMakeLists.txt
├── CLAUDE.md
├── vcpkg.json                       # vcpkg manifest for dependencies
├── src/
│   ├── main.cpp
│   ├── Game.h / Game.cpp            # Game loop, state management
│   ├── Ball.h / Ball.cpp            # Ball entity
│   ├── Paddle.h / Paddle.cpp        # Paddle entity
│   ├── Brick.h / Brick.cpp          # Brick entity
│   ├── CollisionDetector.h / .cpp   # AABB and circle-vs-rect logic
│   ├── ScoreManager.h / .cpp        # Score tracking and formatting
│   ├── Renderer.h / .cpp            # All raylib draw calls
│   ├── AudioManager.h / .cpp        # Procedural sound generation and playback
│   └── Constants.h                  # All magic numbers in one place
├── features/                        # *** IMMUTABLE — owned by product owner ***
│   ├── step_definitions/            # C++ step wiring (Claude Code may add, never delete)
│   │   ├── startup_steps.cpp
│   │   ├── paddle_steps.cpp
│   │   ├── ball_steps.cpp
│   │   ├── brick_steps.cpp
│   │   ├── collision_steps.cpp
│   │   ├── scoring_steps.cpp
│   │   ├── game_state_steps.cpp
│   │   └── audio_steps.cpp
│   ├── startup.feature
│   ├── paddle.feature
│   ├── ball.feature
│   ├── bricks.feature
│   ├── collision.feature
│   ├── scoring.feature
│   ├── game_over.feature
│   ├── win_condition.feature
│   ├── code_quality.feature
│   └── audio.feature
├── tests/                           # TDD unit tests — owned by Claude Code
│   ├── CMakeLists.txt
│   ├── test_main.cpp
│   ├── test_ball.cpp
│   ├── test_paddle.cpp
│   ├── test_brick.cpp
│   ├── test_collision_detector.cpp
│   ├── test_score_manager.cpp
│   ├── test_game_state.cpp
│   └── test_audio_manager.cpp
└── assets/                          # Reserved for future assets
```

---

## Game Specification

### Window
- Resolution: 800 x 600 pixels
- Title: "Breakout"
- Target FPS: 60
- Background: Black

### Paddle
- Width: 100px, Height: 16px
- Positioned 40px above the bottom of the screen
- Moves left/right with arrow keys or A/D keys
- Speed: 400px/sec
- Clamped to window bounds — cannot leave the screen

### Ball
- Radius: 8px
- Starting position: Centered horizontally, 60px above the paddle
- Starting velocity: 300px/sec at a 45-degree angle (up-right)
- Reflects off left, right, and top walls
- Reflects off the paddle on contact
- If it exits the bottom of the screen: game over

### Bricks
- Grid: 10 columns × 5 rows
- Brick size: 70px wide × 20px tall
- Horizontal padding: 10px between bricks, 20px margin from screen edges
- Vertical padding: 8px between rows, 60px top margin
- All bricks start with 1 hit point (destroyed in one hit)
- Row colors (top to bottom): Red, Orange, Yellow, Green, Blue

### Scoring
- Each brick destroyed: +10 points
- Score displayed top-left: "Score: 000"
- No high score persistence required

### Game States
1. **START** — Show title and "Press SPACE to start" prompt
2. **PLAYING** — Active gameplay
3. **GAME_OVER** — Ball lost, show "Game Over", score, and "Press R to restart"
4. **WIN** — All bricks cleared, show "You Win!", score, and "Press R to restart"

### Collision Detection
- Ball vs walls: reverse appropriate velocity component
- Ball vs paddle: reverse Y velocity; optionally adjust X velocity based on hit position on paddle
- Ball vs brick: reverse appropriate velocity component, mark brick as destroyed, add score
- Use AABB (Axis-Aligned Bounding Box) collision for bricks and paddle

---

## Acceptance Criteria

All criteria are written in Given / When / Then format.
Every criterion must map to at least one unit test (see TDD section below).

### AC-01: Window and Startup

**AC-01-a**
- **Given** the executable is launched
- **When** the window initialises
- **Then** it opens at exactly 800×600 pixels with the title "Breakout"

**AC-01-b**
- **Given** the game has just launched
- **When** the START screen is displayed
- **Then** the player sees the game title and the prompt "Press SPACE to start"

**AC-01-c**
- **Given** the START screen is active
- **When** any key other than SPACE is pressed
- **Then** the game state does not change to PLAYING

**AC-01-d**
- **Given** the START screen is active
- **When** SPACE is pressed
- **Then** the game transitions to the PLAYING state

---

### AC-02: Paddle Behaviour

**AC-02-a**
- **Given** the game enters PLAYING state
- **When** the first frame renders
- **Then** the paddle is centred horizontally and positioned 40px above the bottom edge

**AC-02-b**
- **Given** the game is in PLAYING state and the paddle is not at the left edge
- **When** the left arrow or A key is held
- **Then** the paddle moves left at 400px/sec (delta-time scaled)

**AC-02-c**
- **Given** the game is in PLAYING state and the paddle is not at the right edge
- **When** the right arrow or D key is held
- **Then** the paddle moves right at 400px/sec (delta-time scaled)

**AC-02-d**
- **Given** the paddle is at the left screen edge
- **When** the left arrow or A key is held
- **Then** the paddle position does not decrease below x = 0

**AC-02-e**
- **Given** the paddle is at the right screen edge
- **When** the right arrow or D key is held
- **Then** the paddle position does not increase beyond x = SCREEN_WIDTH − PADDLE_WIDTH

---

### AC-03: Ball Behaviour

**AC-03-a**
- **Given** the game transitions to PLAYING state
- **When** the first update tick runs
- **Then** the ball is centred horizontally at 60px above the paddle and moving at 45 degrees up-right at 300px/sec

**AC-03-b**
- **Given** the ball is moving left and its left edge reaches x = 0
- **When** the collision is detected
- **Then** the ball's X velocity is negated and it moves rightward

**AC-03-c**
- **Given** the ball is moving right and its right edge reaches x = SCREEN_WIDTH
- **When** the collision is detected
- **Then** the ball's X velocity is negated and it moves leftward

**AC-03-d**
- **Given** the ball is moving upward and its top edge reaches y = 0
- **When** the collision is detected
- **Then** the ball's Y velocity is negated and it moves downward

**AC-03-e**
- **Given** the ball is moving downward and its bounding circle overlaps the paddle rect
- **When** the collision is detected
- **Then** the ball's Y velocity is negated and it moves upward

**AC-03-f**
- **Given** the ball is moving downward and its bottom edge exceeds y = SCREEN_HEIGHT
- **When** this is detected
- **Then** the game transitions to GAME_OVER state

---

### AC-04: Brick Grid

**AC-04-a**
- **Given** the game enters PLAYING state
- **When** the brick grid is initialised
- **Then** exactly 50 bricks (10 columns × 5 rows) are active and visible

**AC-04-b**
- **Given** the brick grid is rendered
- **When** rows are drawn top to bottom
- **Then** row colors are Red, Orange, Yellow, Green, Blue respectively

**AC-04-c**
- **Given** the brick grid is rendered
- **When** brick positions are calculated
- **Then** bricks are evenly spaced with 10px gaps and 20px screen margins

**AC-04-d**
- **Given** an active brick exists and the ball's bounding circle overlaps its AABB
- **When** the collision is processed
- **Then** the brick is marked inactive, removed from the display, and 10 points are added to the score

---

### AC-05: Collision Accuracy

**AC-05-a**
- **Given** the ball is approaching a brick from any direction
- **When** the ball's circle intersects the brick's AABB
- **Then** the correct velocity component is reversed (X for side hits, Y for top/bottom hits) and the ball does not tunnel through

**AC-05-b**
- **Given** the ball is approaching the paddle from above
- **When** the ball's circle intersects the paddle's AABB
- **Then** Y velocity is negated; X velocity may be adjusted proportionally to the offset from paddle centre

**AC-05-c**
- **Given** the ball is in motion
- **When** it contacts a wall, paddle, or brick
- **Then** the ball's speed magnitude remains constant after reflection (no energy gain or loss)

---

### AC-06: Scoring

**AC-06-a**
- **Given** a new game starts
- **When** the PLAYING state is entered
- **Then** the score is initialised to 0

**AC-06-b**
- **Given** the game is in PLAYING state
- **When** a brick is destroyed
- **Then** the score increases by exactly 10 points

**AC-06-c**
- **Given** the game is in PLAYING state
- **When** any frame renders
- **Then** the current score is visible in the top-left corner formatted as "Score: N"

---

### AC-07: Game Over

**AC-07-a**
- **Given** the ball exits the bottom of the screen
- **When** this is detected during the update step
- **Then** the game transitions to GAME_OVER state immediately

**AC-07-b**
- **Given** the game is in GAME_OVER state
- **When** the screen renders
- **Then** "Game Over" and the final score are displayed

**AC-07-c**
- **Given** the game is in GAME_OVER state
- **When** the player presses R
- **Then** ball, paddle, bricks, and score are fully reset and the game returns to START state

---

### AC-08: Win Condition

**AC-08-a**
- **Given** the last remaining brick is destroyed
- **When** the active brick count reaches 0
- **Then** the game transitions to WIN state immediately

**AC-08-b**
- **Given** the game is in WIN state
- **When** the screen renders
- **Then** "You Win!" and the final score are displayed

**AC-08-c**
- **Given** the game is in WIN state
- **When** the player presses R
- **Then** ball, paddle, bricks, and score are fully reset and the game returns to START state

---

### AC-09: Code Quality

**AC-09-a**
- **Given** the codebase is reviewed
- **When** checking for global state
- **Then** no mutable global variables exist — all state is owned by the Game class

**AC-09-b**
- **Given** the codebase is reviewed
- **When** checking for magic numbers
- **Then** all numeric constants are defined in Constants.h with descriptive names

**AC-09-c**
- **Given** the project is built
- **When** compiled with -Wall on MSVC or MinGW-w64
- **Then** zero warnings are emitted

**AC-09-d**
- **Given** the CMakeLists.txt is present
- **When** CMake configure and build are run
- **Then** a runnable breakout.exe is produced that links raylib correctly

---

## Two-Layer Test Strategy

This project uses two separate, complementary test layers. They have different owners,
different scopes, and different rules. Both must be green before any feature is considered done.

```
┌─────────────────────────────────────────────────────────┐
│  LAYER 1 — BDD Acceptance Tests                         │
│  Owner: Product Owner (you)                             │
│  Framework: cucumber-cpp + Gherkin .feature files       │
│  Scope: Behaviour — does the game do the right thing?   │
│  Location: features/                                    │
│  Mutability: IMMUTABLE without explicit owner approval  │
├─────────────────────────────────────────────────────────┤
│  LAYER 2 — TDD Unit Tests                               │
│  Owner: Claude Code                                     │
│  Framework: doctest                                     │
│  Scope: Implementation — do the internals work?         │
│  Location: tests/                                       │
│  Mutability: Freely evolves as design changes           │
└─────────────────────────────────────────────────────────┘
```

---

## Layer 1: BDD Acceptance Tests

### Framework Setup
- **cucumber-cpp** via vcpkg: `cucumber-cpp` + `boost-test`
- Feature files live in `features/*.feature` — plain Gherkin, no C++ knowledge required
- Step definitions live in `features/step_definitions/*.cpp` — C++ wiring only, no logic
- CMake target: `breakout_bdd` — runs headlessly against the game logic layer

### Ownership Rules for Claude Code
- **NEVER modify, rename, or delete any `.feature` file** without explicit written approval from the product owner
- **NEVER modify existing step definitions** — only add new ones when a new feature file is created by the product owner
- **NEVER add scenarios or examples to existing feature files**
- Feature files are the contract. The code exists to satisfy them, not the other way around
- If a `.feature` file and the implementation conflict, fix the implementation

### Feature File Structure
Each `.feature` file maps directly to an AC group. Example:

```gherkin
# features/ball.feature
# IMMUTABLE - Product Owner: [your name]
# Last approved: [date]

Feature: Ball behaviour
  As a player
  I want the ball to move and bounce correctly
  So that the game is fair and predictable

  Scenario: Ball starts at correct position (AC-03-a)
    Given the game has transitioned to PLAYING state
    When the first update tick runs
    Then the ball is centred horizontally
    And the ball is 60 pixels above the paddle
    And the ball is moving at 45 degrees up-right
    And the ball speed is 300 pixels per second

  Scenario: Ball reflects off left wall (AC-03-b)
    Given the ball is moving left
    And the ball's left edge is at x equals 0
    When the collision is detected
    Then the ball's x velocity is negated
    And the ball moves rightward

  Scenario: Ball triggers game over at bottom (AC-03-f)
    Given the ball is moving downward
    When the ball's bottom edge exceeds the screen height
    Then the game transitions to GAME_OVER state
```

### Step Definition Rules
Step definitions must be thin wiring only — no game logic inside them:

```cpp
// features/step_definitions/ball_steps.cpp
GIVEN("^the ball is moving left$", [&]() {
    ball.setVelocity({-300.0f, 0.0f});
});

WHEN("^the ball's left edge is at x equals (\\d+)$", [&](int x) {
    ball.setPosition({static_cast<float>(x) + BALL_RADIUS, 300.0f});
});

THEN("^the ball's x velocity is negated$", [&]() {
    BOOST_CHECK_GT(ball.getVelocity().x, 0.0f);
});
```

---

## Layer 2: TDD Unit Tests

### Framework
- **doctest** (single-header, include `doctest/doctest.h` — committed to repo under `vendor/`)
- One test file per class in `tests/`
- CMake target: `breakout_tests` — no raylib window, runs fully headlessly

### TDD Rules for Claude Code
1. **Write the failing test first.** No implementation without a test that demands it.
2. **Write the minimum code to pass.** Do not add behaviour not required by a failing test.
3. **Refactor after green.** Clean up without breaking tests.
4. **No raylib calls in tests.** All logic must be decoupled from the render/window layer.
5. **Unit tests are expendable.** If a refactor requires rewriting unit tests, that is fine — as long as the BDD layer stays green.
6. **Unit tests are not a substitute for BDD.** A feature is only done when both layers pass.

### Example Unit Test Pattern

```cpp
// tests/test_ball.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "../src/Ball.h"
#include "../src/Constants.h"

// Covers AC-03-b
TEST_CASE("Ball reflects off left wall") {
    Ball ball;
    ball.setPosition({BALL_RADIUS, 300.0f});    // touching left wall
    ball.setVelocity({-300.0f, -300.0f});

    ball.update(0.016f);                         // one frame at ~60fps

    CHECK(ball.getVelocity().x > 0.0f);          // X reversed
    CHECK(ball.getVelocity().y == doctest::Approx(-300.0f)); // Y unchanged
}

// Covers AC-05-c
TEST_CASE("Ball speed magnitude is preserved after wall reflection") {
    Ball ball;
    ball.setPosition({BALL_RADIUS, 300.0f});
    ball.setVelocity({-300.0f, -300.0f});
    float speedBefore = ball.getSpeed();

    ball.update(0.016f);

    CHECK(ball.getSpeed() == doctest::Approx(speedBefore));
}
```

---

## SOLID Principles

Apply all five SOLID principles throughout the codebase.

### Single Responsibility Principle
Each class has one reason to change:
- `Ball` — owns position, velocity, and wall/paddle reflection logic only
- `Paddle` — owns position and input-driven movement only
- `Brick` — owns position, color, and active state only
- `Game` — owns game state transitions and orchestrates update/draw calls
- `CollisionDetector` — owns all AABB and circle-vs-rect collision logic (extracted from Game)
- `ScoreManager` — owns score tracking and formatting (extracted from Game)
- `Renderer` — owns all DrawText / DrawRectangle / DrawCircle calls (extracted from Game)
- `AudioManager` — owns all sound generation, playback, and mute state (extracted from Game)

### Open/Closed Principle
- Game state transitions should be driven by the state enum, not by chains of if/else that must be edited to add new states
- Collision response should be extensible: adding a new collidable type should not require modifying existing collision code

### Liskov Substitution Principle
- If an `ICollidable` or `IDrawable` interface is introduced, all implementing classes must honour the full contract — no no-op overrides

### Interface Segregation Principle
- Do not create a single fat `IGameObject` interface. Prefer narrow interfaces: `IUpdatable`, `IDrawable`, `ICollidable` — classes implement only what they need

### Dependency Inversion Principle
- `Game` depends on abstractions, not concretions where practical
- `CollisionDetector` should accept references/pointers to interface types, not concrete Ball/Brick/Paddle
- This makes unit testing straightforward — pass mock or stub objects in tests

---

## Implementation Notes for Claude Code

### General
- Use `raylib.h` only for rendering — no other external dependencies beyond the test frameworks
- Use delta time (`GetFrameTime()`) for all movement — never hardcode pixel offsets per frame
- Decouple all logic from raylib calls so every logic class is testable without a window
- Follow SOLID: if a class is doing more than one thing, split it before moving on
- Do not use raylib's built-in audio for this version (out of scope)

### Dependency Setup (first session only)
1. Check if vcpkg is installed: `vcpkg --version`
2. If not found, instruct the user to run: `winget install Microsoft.Vcpkg` then restart PowerShell
3. Install dependencies: `vcpkg install cucumber-cpp boost-test raylib`
4. Configure CMake with vcpkg toolchain: `cmake -B build -DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake`

### Workflow per Feature
1. Read the relevant `.feature` file — understand what behaviour is required
2. Write a failing doctest unit test for the smallest piece of that behaviour
3. Write the minimum implementation to make it pass
4. Refactor if needed
5. Implement the cucumber-cpp step definitions that wire the `.feature` file to the logic
6. Run `breakout_bdd` — all BDD scenarios for that feature must be green before moving on
7. Run `breakout_tests` — all unit tests must be green
8. Only then move to the next feature

### Build Targets
- `breakout.exe` — the game
- `breakout_tests` — doctest unit tests (headless, fast)
- `breakout_bdd` — cucumber-cpp BDD acceptance tests (headless, logic layer only)

---

## Out of Scope (v1)
- Multiple levels
- Lives system (ball lost = immediate game over)
- Power-ups
- Background music
- High score persistence
- Mouse control for paddle

---

## Audio Specification (v1.1)

### Approach
All sounds are generated **procedurally at runtime** using raylib's audio API (`LoadSoundFromWave`).
No audio files are shipped with the game — all waveforms are synthesised in code.
This keeps the repo clean and satisfies the SOLID principle by isolating all audio
generation and playback behind a dedicated `AudioManager` class.

### AudioManager Class
- Owns all sound generation, storage, and playback
- Initialises raylib audio device on construction (`InitAudioDevice`)
- Closes audio device on destruction (`CloseAudioDevice`)
- Exposes named play methods: `playPaddleHit()`, `playWallHit()`, `playBrickHit()`, `playBallLost()`, `playWin()`
- Owns a mute flag toggled by the M key — when muted, play methods are no-ops
- Displays mute state on screen: "M: Mute" or "M: Unmute" in the HUD
- No other class calls raylib audio functions directly

### Sound Design
All sounds are short synthesised tones generated from raw PCM waveforms:

| Event | Character | Suggested frequency | Duration |
|---|---|---|---|
| Ball hits paddle | Mid thump | ~180 Hz sine | ~80ms |
| Ball hits wall | High bing | ~440 Hz sine | ~60ms |
| Ball destroys brick | Crisp ding | ~600 Hz sine, fast decay | ~50ms |
| Ball exits bottom | Low dull thud | ~90 Hz sine | ~120ms |
| Win condition | Rising tone | ~523→1047 Hz sweep | ~400ms |

Claude Code has latitude to tune frequencies and durations for best game feel,
provided the character described above is preserved and all AC criteria pass.

### Updated Project Structure (audio additions)
```
src/
├── AudioManager.h / AudioManager.cpp    # NEW — all audio logic
tests/
├── test_audio_manager.cpp               # NEW — mute logic, state tests
features/
├── audio.feature                        # NEW — IMMUTABLE
├── step_definitions/
│   └── audio_steps.cpp                  # NEW
```

---

## Acceptance Criteria — Audio (AC-10)

### AC-10-a: Audio initialisation
- **Given** the game executable is launched
- **When** the AudioManager is constructed
- **Then** the raylib audio device is successfully initialised with no errors
- **And** all five sounds are generated and ready to play

### AC-10-b: Ball hits paddle
- **Given** the game is in PLAYING state and audio is not muted
- **When** the ball's bounding circle collides with the paddle
- **Then** the paddle hit sound plays exactly once per collision

### AC-10-c: Ball hits wall
- **Given** the game is in PLAYING state and audio is not muted
- **When** the ball reflects off the left, right, or top wall
- **Then** the wall hit sound plays exactly once per reflection

### AC-10-d: Ball destroys brick
- **Given** the game is in PLAYING state and audio is not muted
- **When** the ball collides with and destroys a brick
- **Then** the brick hit sound plays exactly once for that collision

### AC-10-e: Ball exits bottom
- **Given** the game is in PLAYING state and audio is not muted
- **When** the ball's bottom edge exits the screen at the bottom
- **Then** the ball lost sound plays once before transitioning to GAME_OVER state

### AC-10-f: Win condition sound
- **Given** the last brick has been destroyed and audio is not muted
- **When** the game transitions to WIN state
- **Then** the win sound plays once

### AC-10-g: Mute toggle — muting
- **Given** the game is in any state and audio is currently unmuted
- **When** the player presses the M key
- **Then** the mute flag is set to true
- **And** the HUD updates to show "M: Unmute"
- **And** no further sounds play until unmuted

### AC-10-h: Mute toggle — unmuting
- **Given** the game is in any state and audio is currently muted
- **When** the player presses the M key
- **Then** the mute flag is set to false
- **And** the HUD updates to show "M: Mute"
- **And** sounds resume playing on the next triggering event

### AC-10-i: Mute persists across game states
- **Given** the player has muted audio during PLAYING state
- **When** the game transitions to GAME_OVER or WIN state and back to START
- **Then** the mute flag remains true across all state transitions

### AC-10-j: No sound when muted
- **Given** the mute flag is true
- **When** any collision or game state event occurs that would normally trigger a sound
- **Then** no sound plays

### AC-10-k: Single sound per event
- **Given** the game is in PLAYING state and audio is not muted
- **When** a single collision event is detected
- **Then** exactly one sound plays — sounds do not stack or double-trigger on the same event

---

## Feature File — audio.feature
```gherkin
# features/audio.feature
# IMMUTABLE - Product Owner approved
# Version: 1.1

Feature: Audio feedback
  As a player
  I want audio feedback for game events
  So that the game feels responsive and satisfying

  Scenario: Audio initialises successfully (AC-10-a)
    Given the game executable is launched
    When the AudioManager is constructed
    Then the audio device is initialised without errors
    And all five sounds are ready to play

  Scenario: Paddle hit plays sound (AC-10-b)
    Given the game is in PLAYING state
    And audio is not muted
    When the ball collides with the paddle
    Then the paddle hit sound plays exactly once

  Scenario: Wall hit plays sound (AC-10-c)
    Given the game is in PLAYING state
    And audio is not muted
    When the ball reflects off a wall
    Then the wall hit sound plays exactly once

  Scenario: Brick destruction plays sound (AC-10-d)
    Given the game is in PLAYING state
    And audio is not muted
    When the ball destroys a brick
    Then the brick hit sound plays exactly once

  Scenario: Ball lost plays sound (AC-10-e)
    Given the game is in PLAYING state
    And audio is not muted
    When the ball exits the bottom of the screen
    Then the ball lost sound plays once

  Scenario: Win condition plays sound (AC-10-f)
    Given the last brick has been destroyed
    And audio is not muted
    When the game transitions to WIN state
    Then the win sound plays once

  Scenario: M key mutes audio (AC-10-g)
    Given audio is currently unmuted
    When the player presses the M key
    Then the mute flag is true
    And the HUD displays "M: Unmute"

  Scenario: M key unmutes audio (AC-10-h)
    Given audio is currently muted
    When the player presses the M key
    Then the mute flag is false
    And the HUD displays "M: Mute"

  Scenario: Mute persists across game states (AC-10-i)
    Given the player has muted audio during PLAYING state
    When the game transitions to GAME_OVER state
    And the player restarts and enters PLAYING state
    Then the mute flag is still true

  Scenario: No sound plays when muted (AC-10-j)
    Given the mute flag is true
    When any collision or game state event occurs
    Then no sound plays

  Scenario: Single sound per collision event (AC-10-k)
    Given the game is in PLAYING state
    And audio is not muted
    When a single collision event is detected
    Then exactly one sound plays for that event
```
