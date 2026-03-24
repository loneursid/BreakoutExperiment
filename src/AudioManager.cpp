#ifndef BREAKOUT_HEADLESS
#  include "raylib.h"
#endif

#include "AudioManager.h"
#include <cmath>
#include <vector>

#ifndef M_PI
constexpr float M_PI_F = 3.14159265358979323846f;
#else
constexpr float M_PI_F = static_cast<float>(M_PI);
#endif

// ── Construction / destruction ────────────────────────────────────────────────

AudioManager::AudioManager()
{
#ifndef BREAKOUT_HEADLESS
    InitAudioDevice();
    m_deviceOpen = IsAudioDeviceReady();
    if (m_deviceOpen) {
        m_paddleHit = buildSine(180.0f,  0.080f);
        m_wallHit   = buildSine(440.0f,  0.060f);
        m_brickHit  = buildSine(600.0f,  0.050f);
        m_ballLost  = buildSine(150.0f,  0.200f, 0.9f);
        m_win       = buildSweep(523.0f, 1047.0f, 0.400f);
        m_ready = true;
    }
#else
    m_ready = true;  // headless: always "ready" for logic tests
#endif
}

AudioManager::~AudioManager()
{
#ifndef BREAKOUT_HEADLESS
    if (m_deviceOpen) {
        UnloadSound(m_paddleHit);
        UnloadSound(m_wallHit);
        UnloadSound(m_brickHit);
        UnloadSound(m_ballLost);
        UnloadSound(m_win);
        CloseAudioDevice();
    }
#endif
}

// ── Play methods ──────────────────────────────────────────────────────────────

void AudioManager::playPaddleHit()
{
#ifndef BREAKOUT_HEADLESS
    if (!m_muted && m_deviceOpen) PlaySound(m_paddleHit);
#endif
}

void AudioManager::playWallHit()
{
#ifndef BREAKOUT_HEADLESS
    if (!m_muted && m_deviceOpen) PlaySound(m_wallHit);
#endif
}

void AudioManager::playBrickHit()
{
#ifndef BREAKOUT_HEADLESS
    if (!m_muted && m_deviceOpen) PlaySound(m_brickHit);
#endif
}

void AudioManager::playBallLost()
{
#ifndef BREAKOUT_HEADLESS
    if (!m_muted && m_deviceOpen) PlaySound(m_ballLost);
#endif
}

void AudioManager::playWin()
{
#ifndef BREAKOUT_HEADLESS
    if (!m_muted && m_deviceOpen) PlaySound(m_win);
#endif
}

// ── Mute ─────────────────────────────────────────────────────────────────────

void AudioManager::toggleMute()       { m_muted = !m_muted; }
bool AudioManager::isMuted()    const { return m_muted; }
bool AudioManager::isReady()    const { return m_ready; }

std::string AudioManager::getMuteLabel() const
{
    // Shows what pressing M will DO next
    return m_muted ? "M: Unmute" : "M: Mute";
}

// ── Wave generation helpers ───────────────────────────────────────────────────

#ifndef BREAKOUT_HEADLESS

Sound AudioManager::buildSine(float frequency, float durationSec, float volume)
{
    constexpr int   SAMPLE_RATE = 44100;
    const int       frames      = static_cast<int>(SAMPLE_RATE * durationSec);

    std::vector<float> samples(frames);
    for (int i = 0; i < frames; ++i) {
        float t       = static_cast<float>(i) / SAMPLE_RATE;
        float env     = 1.0f - (static_cast<float>(i) / frames);  // linear decay
        samples[i]    = volume * env * std::sin(2.0f * M_PI_F * frequency * t);
    }

    Wave wave{};
    wave.frameCount = static_cast<unsigned int>(frames);
    wave.sampleRate = SAMPLE_RATE;
    wave.sampleSize = 32;
    wave.channels   = 1;
    wave.data       = samples.data();

    return LoadSoundFromWave(wave);
    // wave.data is on the stack vector — raylib copies it internally
}

Sound AudioManager::buildSweep(float freqStart, float freqEnd,
                                float durationSec, float volume)
{
    constexpr int   SAMPLE_RATE = 44100;
    const int       frames      = static_cast<int>(SAMPLE_RATE * durationSec);

    std::vector<float> samples(frames);
    float phase = 0.0f;
    for (int i = 0; i < frames; ++i) {
        float t       = static_cast<float>(i) / frames;
        float freq    = freqStart + (freqEnd - freqStart) * t;
        float env     = 1.0f - t * 0.5f;  // gentle fade
        samples[i]    = volume * env * std::sin(phase);
        phase        += 2.0f * M_PI_F * freq / SAMPLE_RATE;
    }

    Wave wave{};
    wave.frameCount = static_cast<unsigned int>(frames);
    wave.sampleRate = SAMPLE_RATE;
    wave.sampleSize = 32;
    wave.channels   = 1;
    wave.data       = samples.data();

    return LoadSoundFromWave(wave);
}

#endif
