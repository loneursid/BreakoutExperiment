# features/startup.feature
# IMMUTABLE - Product Owner
# Last approved: 2026-03-23

Feature: Window and Startup
  As a player
  I want the game to open correctly
  So that I can start playing immediately

  Scenario: Window opens at correct resolution (AC-01-a)
    Given the executable is launched
    When the window initialises
    Then the window width is 800 pixels
    And the window height is 600 pixels
    And the window title is "Breakout"

  Scenario: START screen shows title and prompt (AC-01-b)
    Given the game has just launched
    When the START screen is displayed
    Then the player sees the game title
    And the player sees the prompt "Press SPACE to start"

  Scenario: Non-SPACE key does not start game (AC-01-c)
    Given the START screen is active
    When any key other than SPACE is pressed
    Then the game state is START

  Scenario: SPACE starts the game (AC-01-d)
    Given the START screen is active
    When SPACE is pressed
    Then the game state is PLAYING
