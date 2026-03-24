cmake_minimum_required(VERSION 3.15)
project(breakout CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Compiler warnings ────────────────────────────────────────────────────────
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

# ── Dependencies via vcpkg ────────────────────────────────────────────────────
find_package(raylib CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

# ── Source files shared by game + headless targets ───────────────────────────
set(GAME_SOURCES
    src/Ball.cpp
    src/Paddle.cpp
    src/Brick.cpp
    src/CollisionDetector.cpp
    src/ScoreManager.cpp
    src/AudioManager.cpp
    src/Game.cpp
)

# ── breakout.exe ─────────────────────────────────────────────────────────────
add_executable(breakout
    ${GAME_SOURCES}
    src/Renderer.cpp
    src/main.cpp
)
target_link_libraries(breakout PRIVATE raylib)
target_include_directories(breakout PRIVATE src)

# ── Headless logic library (shared by both test targets) ─────────────────────
add_library(breakout_logic STATIC ${GAME_SOURCES})
target_include_directories(breakout_logic PUBLIC src)
target_compile_definitions(breakout_logic PUBLIC BREAKOUT_HEADLESS)

# ── doctest unit tests ────────────────────────────────────────────────────────
include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG        v2.4.11
)
FetchContent_MakeAvailable(doctest)

add_executable(breakout_tests
    tests/test_main.cpp
    tests/test_ball.cpp
    tests/test_paddle.cpp
    tests/test_brick.cpp
    tests/test_collision_detector.cpp
    tests/test_score_manager.cpp
    tests/test_game_state.cpp
    tests/test_audio_manager.cpp
)
target_link_libraries(breakout_tests PRIVATE breakout_logic doctest::doctest)
target_include_directories(breakout_tests PRIVATE src)

# ── Catch2 BDD feature tests ──────────────────────────────────────────────────
add_executable(breakout_bdd
    bdd/bdd_startup.cpp
    bdd/bdd_paddle.cpp
    bdd/bdd_ball.cpp
    bdd/bdd_bricks.cpp
    bdd/bdd_collision.cpp
    bdd/bdd_scoring.cpp
    bdd/bdd_game_states.cpp
    bdd/bdd_audio.cpp
)
target_link_libraries(breakout_bdd PRIVATE
    breakout_logic
    Catch2::Catch2WithMain
)
target_include_directories(breakout_bdd PRIVATE src bdd)

# ── Test discovery ────────────────────────────────────────────────────────────
enable_testing()
include(CTest)

add_test(NAME unit_tests COMMAND breakout_tests)
add_test(NAME bdd_tests  COMMAND breakout_bdd)
