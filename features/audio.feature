# features/audio.feature
# IMMUTABLE - Product Owner approved
# Version: 1.1

Feature: Audio feedback
  As a player
  I want audio feedback for game events
  So that the game feels responsive and satisfying

  Scenario: Audio initialises successfully (AC-10-a)
    Given the game executable is launched
    When the AudioManager is constructed
    Then the audio device is initialised without errors
    And all five sounds are ready to play

  Scenario: Paddle hit plays sound (AC-10-b)
    Given the game is in PLAYING state
    And audio is not muted
    When the ball collides with the paddle
    Then the paddle hit sound plays exactly once

  Scenario: Wall hit plays sound (AC-10-c)
    Given the game is in PLAYING state
    And audio is not muted
    When the ball reflects off a wall
    Then the wall hit sound plays exactly once

  Scenario: Brick destruction plays sound (AC-10-d)
    Given the game is in PLAYING state
    And audio is not muted
    When the ball destroys a brick
    Then the brick hit sound plays exactly once

  Scenario: Ball lost plays sound (AC-10-e)
    Given the game is in PLAYING state
    And audio is not muted
    When the ball exits the bottom of the screen
    Then the ball lost sound plays once

  Scenario: Win condition plays sound (AC-10-f)
    Given the last brick has been destroyed
    And audio is not muted
    When the game transitions to WIN state
    Then the win sound plays once

  Scenario: M key mutes audio (AC-10-g)
    Given audio is currently unmuted
    When the player presses the M key
    Then the mute flag is true
    And the HUD displays "M: Unmute"

  Scenario: M key unmutes audio (AC-10-h)
    Given audio is currently muted
    When the player presses the M key
    Then the mute flag is false
    And the HUD displays "M: Mute"

  Scenario: Mute persists across game states (AC-10-i)
    Given the player has muted audio during PLAYING state
    When the game transitions to GAME_OVER state
    And the player restarts and enters PLAYING state
    Then the mute flag is still true

  Scenario: No sound plays when muted (AC-10-j)
    Given the mute flag is true
    When any collision or game state event occurs
    Then no sound plays

  Scenario: Single sound per collision event (AC-10-k)
    Given the game is in PLAYING state
    And audio is not muted
    When a single collision event is detected
    Then exactly one sound plays for that event
