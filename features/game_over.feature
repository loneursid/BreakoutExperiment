# features/game_over.feature
# IMMUTABLE - Product Owner
# Last approved: 2026-03-23

Feature: Game Over
  As a player
  I want to see a game over screen when I lose
  So that I know the game has ended and can restart

  Scenario: Ball exit triggers game over (AC-07-a)
    Given the ball is moving downward
    When the ball's bottom edge exceeds the screen height
    Then the game transitions to GAME_OVER state

  Scenario: Game over screen shows message and score (AC-07-b)
    Given the game is in GAME_OVER state
    When the screen renders
    Then "Game Over" is displayed
    And the final score is displayed

  Scenario: R key resets from game over (AC-07-c)
    Given the game is in GAME_OVER state
    When the player presses R
    Then the game state returns to START
    And the score is reset to 0
    And all bricks are active
