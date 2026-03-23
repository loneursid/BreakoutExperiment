# features/paddle.feature
# IMMUTABLE - Product Owner
# Last approved: 2026-03-23

Feature: Paddle Behaviour
  As a player
  I want the paddle to move correctly
  So that I can control the game

  Scenario: Paddle starts centred (AC-02-a)
    Given the game enters PLAYING state
    When the first frame renders
    Then the paddle is centred horizontally
    And the paddle is 40 pixels above the bottom edge

  Scenario: Paddle moves left (AC-02-b)
    Given the game is in PLAYING state
    And the paddle is not at the left edge
    When the left key is held
    Then the paddle moves left at 400 pixels per second

  Scenario: Paddle moves right (AC-02-c)
    Given the game is in PLAYING state
    And the paddle is not at the right edge
    When the right key is held
    Then the paddle moves right at 400 pixels per second

  Scenario: Paddle clamped at left edge (AC-02-d)
    Given the paddle is at the left screen edge
    When the left key is held
    Then the paddle x position is 0

  Scenario: Paddle clamped at right edge (AC-02-e)
    Given the paddle is at the right screen edge
    When the right key is held
    Then the paddle x position does not exceed the right boundary
