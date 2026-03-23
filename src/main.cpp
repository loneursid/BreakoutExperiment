#include "raylib.h"
#include "Game.h"
#include "Renderer.h"
#include "Constants.h"

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout");
    SetTargetFPS(TARGET_FPS);

    Game     game;
    Renderer renderer;

    while (!WindowShouldClose()) {
        game.handleInput();
        game.update(GetFrameTime());
        renderer.draw(game);
    }

    CloseWindow();
    return 0;
}
