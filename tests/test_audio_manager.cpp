#include <doctest/doctest.h>
#include "../src/AudioManager.h"

// AudioManager in headless mode: no audio device, but mute logic is fully testable

TEST_CASE("AudioManager is ready after construction") {
    AudioManager am;
    CHECK(am.isReady());
}

// AC-10-g: mute toggle — muting
TEST_CASE("AudioManager starts unmuted") {
    AudioManager am;
    CHECK_FALSE(am.isMuted());
}

TEST_CASE("toggleMute sets muted to true when unmuted") {
    AudioManager am;
    am.toggleMute();
    CHECK(am.isMuted());
}

// AC-10-h: mute toggle — unmuting
TEST_CASE("toggleMute sets muted to false when muted") {
    AudioManager am;
    am.toggleMute();  // mute
    am.toggleMute();  // unmute
    CHECK_FALSE(am.isMuted());
}

TEST_CASE("toggleMute is idempotent over even number of calls") {
    AudioManager am;
    for (int i = 0; i < 6; ++i) am.toggleMute();
    CHECK_FALSE(am.isMuted());
}

// AC-10-g HUD label
TEST_CASE("getMuteLabel returns 'M: Mute' when unmuted") {
    AudioManager am;
    CHECK(am.getMuteLabel() == "M: Mute");
}

// AC-10-h HUD label
TEST_CASE("getMuteLabel returns 'M: Unmute' when muted") {
    AudioManager am;
    am.toggleMute();
    CHECK(am.getMuteLabel() == "M: Unmute");
}

TEST_CASE("getMuteLabel toggles back to 'M: Mute' after unmuting") {
    AudioManager am;
    am.toggleMute();
    am.toggleMute();
    CHECK(am.getMuteLabel() == "M: Mute");
}
