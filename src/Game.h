#pragma once

#include <vector>
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include "ScoreManager.h"
#include "AudioManager.h"

enum class GameState { START, PLAYING, GAME_OVER, WIN };

class Game {
public:
    Game();
    virtual ~Game() = default;

    // Call once per frame from the game loop
    void update(float dt);
    void handleInput();

    // State accessors for Renderer / tests
    GameState                 getState()          const;
    const Ball&               getBall()           const;
    const Paddle&             getPaddle()         const;
    const std::vector<Brick>& getBricks()         const;
    const ScoreManager&       getScoreManager()   const;
    const AudioManager&       getAudioManager()   const;

    // Returns how many bricks are still active
    int activeBrickCount() const;

    // Reset everything and return to START
    void reset();

private:
    void initBricks();
    void processCollisions();

    // Input query (abstracted so tests can subclass/override if needed)
    virtual bool isLeftPressed()    const;
    virtual bool isRightPressed()   const;
    virtual bool isSpacePressed()   const;
    virtual bool isRestartPressed() const;
    virtual bool isMutePressed()    const;

    GameState           m_state;
    Ball                m_ball;
    Paddle              m_paddle;
    std::vector<Brick>  m_bricks;
    ScoreManager        m_score;
    AudioManager        m_audio;
};
