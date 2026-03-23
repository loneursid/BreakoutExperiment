#pragma once
#include "Game.h"
#include "Constants.h"

// Testable Game subclass — injects input without raylib
class BddGame : public Game {
public:
    bool pressLeft = false, pressRight = false,
         pressSpace = false, pressRestart = false;

    bool isLeftPressed()    const override { return pressLeft;    }
    bool isRightPressed()   const override { return pressRight;   }
    bool isSpacePressed()   const override { return pressSpace;   }
    bool isRestartPressed() const override { return pressRestart; }

    void tapSpace()   { pressSpace   = true; handleInput(); pressSpace   = false; }
    void tapRestart() { pressRestart = true; handleInput(); pressRestart = false; }
};
