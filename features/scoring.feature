# features/scoring.feature
# IMMUTABLE - Product Owner
# Last approved: 2026-03-23

Feature: Scoring
  As a player
  I want to see my score update correctly
  So that I know how well I am doing

  Scenario: Score initialises to 0 (AC-06-a)
    Given a new game starts
    When the PLAYING state is entered
    Then the score is 0

  Scenario: Brick destroyed adds 10 points (AC-06-b)
    Given the game is in PLAYING state
    When a brick is destroyed
    Then the score increases by 10 points

  Scenario: Score displayed top-left (AC-06-c)
    Given the game is in PLAYING state
    When any frame renders
    Then the score label starts with "Score: "
