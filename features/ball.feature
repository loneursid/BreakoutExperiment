# features/ball.feature
# IMMUTABLE - Product Owner
# Last approved: 2026-03-23

Feature: Ball Behaviour
  As a player
  I want the ball to move and bounce correctly
  So that the game is fair and predictable

  Scenario: Ball starts at correct position (AC-03-a)
    Given the game has transitioned to PLAYING state
    When the first update tick runs
    Then the ball is centred horizontally
    And the ball is 60 pixels above the paddle
    And the ball is moving at 45 degrees up-right
    And the ball speed is 300 pixels per second

  Scenario: Ball reflects off left wall (AC-03-b)
    Given the ball is moving left
    And the ball's left edge is at x equals 0
    When the collision is detected
    Then the ball's x velocity is negated
    And the ball moves rightward

  Scenario: Ball reflects off right wall (AC-03-c)
    Given the ball is moving right
    And the ball's right edge is at x equals 800
    When the collision is detected
    Then the ball's x velocity is negated
    And the ball moves leftward

  Scenario: Ball reflects off top wall (AC-03-d)
    Given the ball is moving upward
    And the ball's top edge is at y equals 0
    When the collision is detected
    Then the ball's y velocity is negated
    And the ball moves downward

  Scenario: Ball reflects off paddle (AC-03-e)
    Given the ball is moving downward
    And the ball's bounding circle overlaps the paddle rect
    When the collision is detected
    Then the ball's y velocity is negated
    And the ball moves upward

  Scenario: Ball triggers game over at bottom (AC-03-f)
    Given the ball is moving downward
    When the ball's bottom edge exceeds the screen height
    Then the game transitions to GAME_OVER state
