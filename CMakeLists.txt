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

# ── Source files shared by game + headless targets ───────────────────────────
set(GAME_SOURCES
    src/Ball.cpp
    src/Paddle.cpp
    src/Brick.cpp
    src/CollisionDetector.cpp
    src/ScoreManager.cpp
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

# ── Headless logic library (for tests) ───────────────────────────────────────
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
)
target_link_libraries(breakout_tests PRIVATE breakout_logic doctest::doctest)
target_include_directories(breakout_tests PRIVATE src)

enable_testing()
add_test(NAME unit_tests COMMAND breakout_tests)

# ── BDD cucumber-cpp tests ────────────────────────────────────────────────────
# cucumber-cpp is not in vcpkg. To enable the BDD target, build cucumber-cpp
# manually and set CucumberCpp_DIR to its cmake install prefix, then re-run cmake.
# The .feature files in features/ remain the authoritative acceptance spec.
find_package(CucumberCpp CONFIG QUIET)
find_package(Boost QUIET COMPONENTS unit_test_framework)

if(CucumberCpp_FOUND AND Boost_FOUND)
    add_executable(breakout_bdd
        features/step_definitions/startup_steps.cpp
        features/step_definitions/paddle_steps.cpp
        features/step_definitions/ball_steps.cpp
        features/step_definitions/brick_steps.cpp
        features/step_definitions/collision_steps.cpp
        features/step_definitions/scoring_steps.cpp
        features/step_definitions/game_state_steps.cpp
    )
    target_link_libraries(breakout_bdd PRIVATE
        breakout_logic
        CucumberCpp::cucumber-cpp
        Boost::unit_test_framework
    )
    target_include_directories(breakout_bdd PRIVATE src features/step_definitions)
    add_test(NAME bdd_tests COMMAND breakout_bdd)
else()
    message(STATUS "cucumber-cpp not found — BDD target skipped (unit tests cover all ACs)")
endif()
