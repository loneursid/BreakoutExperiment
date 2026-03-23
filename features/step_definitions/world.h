#pragma once
#include "../../src/Game.h"
#include "../../src/Ball.h"
#include "../../src/Paddle.h"
#include "../../src/Brick.h"
#include "../../src/CollisionDetector.h"
#include "../../src/ScoreManager.h"
#include "../../src/Constants.h"

// Testable Game subclass that lets BDD steps drive input
class HeadlessGame : public Game {
public:
    bool pressLeft = false, pressRight = false,
         pressSpace = false, pressRestart = false;

    bool isLeftPressed()    const override { return pressLeft;    }
    bool isRightPressed()   const override { return pressRight;   }
    bool isSpacePressed()   const override { return pressSpace;   }
    bool isRestartPressed() const override { return pressRestart; }
};

// Shared world for all BDD step definitions
struct BreakoutWorld {
    HeadlessGame game;
    Ball         ball;
    Paddle       paddle;
    Brick        brick{100.0f, 100.0f, {255, 0, 0, 255}};
    ScoreManager score;
    int          scoreBefore = 0;

    void transitionToPlaying() {
        game.pressSpace = true;
        game.handleInput();
        game.pressSpace = false;
    }
};
