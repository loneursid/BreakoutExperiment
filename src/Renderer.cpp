// Include raylib FIRST so its Vector2/Rectangle definitions win over Types.h stubs
#include "raylib.h"
#include "Renderer.h"
#include "Constants.h"

static Color toRaylibColor(BrickColor c) { return {c.r, c.g, c.b, c.a}; }

static void DrawRectF(float x, float y, float w, float h, Color col)
{
    DrawRectangleRec({x, y, w, h}, col);
}

void Renderer::draw(const Game& game)
{
    BeginDrawing();
    ClearBackground(BLACK);

    switch (game.getState()) {
    case GameState::START:     drawStart(game);    break;
    case GameState::PLAYING:   drawPlaying(game);  break;
    case GameState::GAME_OVER: drawGameOver(game); break;
    case GameState::WIN:       drawWin(game);      break;
    }

    EndDrawing();
}

void Renderer::drawStart(const Game& /*game*/)
{
    const char* title  = "BREAKOUT";
    const char* prompt = "Press SPACE to start";

    int titleW  = MeasureText(title,  60);
    int promptW = MeasureText(prompt, 24);

    DrawText(title,  (SCREEN_WIDTH - titleW)  / 2, SCREEN_HEIGHT / 2 - 60, 60, WHITE);
    DrawText(prompt, (SCREEN_WIDTH - promptW) / 2, SCREEN_HEIGHT / 2 + 20, 24, LIGHTGRAY);
}

void Renderer::drawPlaying(const Game& game)
{
    // Score
    DrawText(game.getScoreManager().getFormattedScore().c_str(), 10, 10, 20, WHITE);

    // Mute label (top-right)
    const std::string muteLabel = game.getAudioManager().getMuteLabel();
    int labelW = MeasureText(muteLabel.c_str(), 18);
    DrawText(muteLabel.c_str(), SCREEN_WIDTH - labelW - 10, 10, 18, GRAY);

    // Paddle
    const Paddle& p = game.getPaddle();
    DrawRectF(p.getX(), p.getY(), p.getWidth(), p.getHeight(), LIGHTGRAY);

    // Ball
    const Ball& b = game.getBall();
    DrawCircleV({b.getPosition().x, b.getPosition().y}, b.getRadius(), WHITE);

    // Bricks
    for (const auto& brick : game.getBricks()) {
        if (!brick.isActive()) continue;
        DrawRectF(brick.getX(), brick.getY(),
                  brick.getWidth(), brick.getHeight(),
                  toRaylibColor(brick.getColor()));
    }
}

void Renderer::drawGameOver(const Game& game)
{
    drawPlaying(game);  // frozen board behind overlay

    const char* msg   = "Game Over";
    const char* score = game.getScoreManager().getFormattedScore().c_str();
    const char* hint  = "Press R to restart";

    int msgW   = MeasureText(msg,   50);
    int scoreW = MeasureText(score, 28);
    int hintW  = MeasureText(hint,  22);

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0, 0, 150});
    DrawText(msg,   (SCREEN_WIDTH - msgW)   / 2, SCREEN_HEIGHT / 2 - 60, 50, RED);
    DrawText(score, (SCREEN_WIDTH - scoreW) / 2, SCREEN_HEIGHT / 2,      28, WHITE);
    DrawText(hint,  (SCREEN_WIDTH - hintW)  / 2, SCREEN_HEIGHT / 2 + 50, 22, LIGHTGRAY);
}

void Renderer::drawWin(const Game& game)
{
    drawPlaying(game);  // frozen board behind overlay

    const char* msg   = "You Win!";
    const char* score = game.getScoreManager().getFormattedScore().c_str();
    const char* hint  = "Press R to restart";

    int msgW   = MeasureText(msg,   50);
    int scoreW = MeasureText(score, 28);
    int hintW  = MeasureText(hint,  22);

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0, 0, 150});
    DrawText(msg,   (SCREEN_WIDTH - msgW)   / 2, SCREEN_HEIGHT / 2 - 60, 50, YELLOW);
    DrawText(score, (SCREEN_WIDTH - scoreW) / 2, SCREEN_HEIGHT / 2,      28, WHITE);
    DrawText(hint,  (SCREEN_WIDTH - hintW)  / 2, SCREEN_HEIGHT / 2 + 50, 22, LIGHTGRAY);
}
