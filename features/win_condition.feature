# features/win_condition.feature
# IMMUTABLE - Product Owner
# Last approved: 2026-03-23

Feature: Win Condition
  As a player
  I want to see a win screen when I clear all bricks
  So that I know I have completed the game

  Scenario: All bricks cleared triggers win (AC-08-a)
    Given the last remaining brick is destroyed
    When the active brick count reaches 0
    Then the game transitions to WIN state

  Scenario: Win screen shows message and score (AC-08-b)
    Given the game is in WIN state
    When the screen renders
    Then "You Win!" is displayed
    And the final score is displayed

  Scenario: R key resets from win (AC-08-c)
    Given the game is in WIN state
    When the player presses R
    Then the game state returns to START
    And the score is reset to 0
    And all bricks are active
