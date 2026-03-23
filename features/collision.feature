# features/collision.feature
# IMMUTABLE - Product Owner
# Last approved: 2026-03-23

Feature: Collision Accuracy
  As a player
  I want collisions to be accurate
  So that the game behaves predictably

  Scenario: Correct axis reversed for brick hit (AC-05-a)
    Given the ball is approaching a brick from the top
    When the ball's circle intersects the brick's AABB
    Then the y velocity component is reversed

  Scenario: Paddle hit reflects y velocity (AC-05-b)
    Given the ball is approaching the paddle from above
    When the ball's circle intersects the paddle's AABB
    Then the y velocity is negated

  Scenario: Speed magnitude preserved after reflection (AC-05-c)
    Given the ball is in motion
    When it contacts a wall
    Then the ball's speed magnitude remains constant
