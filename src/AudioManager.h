#pragma once
#include <string>

#ifndef BREAKOUT_HEADLESS
#  include "raylib.h"
#endif

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    // Disable copy — owns audio device resources
    AudioManager(const AudioManager&)            = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    void playPaddleHit();
    void playWallHit();
    void playBrickHit();
    void playBallLost();
    void playWin();

    void toggleMute();
    bool isMuted() const;

    // Returns "M: Mute" when unmuted (pressing M will mute)
    // Returns "M: Unmute" when muted  (pressing M will unmute)
    std::string getMuteLabel() const;

    bool isReady() const;  // true when audio device is initialised

private:
#ifndef BREAKOUT_HEADLESS
    static Sound buildSine(float frequency, float durationSec, float volume = 0.6f);
    static Sound buildSweep(float freqStart, float freqEnd,
                            float durationSec, float volume = 0.6f);

    Sound m_paddleHit{};
    Sound m_wallHit{};
    Sound m_brickHit{};
    Sound m_ballLost{};
    Sound m_win{};
    bool  m_deviceOpen = false;
#endif

    bool m_muted = false;
    bool m_ready = false;
};
