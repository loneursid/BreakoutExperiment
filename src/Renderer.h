#pragma once

#include "Game.h"

// Owns ALL raylib draw calls. Nothing else may call DrawText/DrawRectangle etc.
class Renderer {
public:
    void draw(const Game& game);

private:
    void drawStart(const Game& game);
    void drawPlaying(const Game& game);
    void drawGameOver(const Game& game);
    void drawWin(const Game& game);
};
