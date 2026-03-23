# features/bricks.feature
# IMMUTABLE - Product Owner
# Last approved: 2026-03-23

Feature: Brick Grid
  As a player
  I want the bricks to be laid out correctly
  So that the game looks and plays as expected

  Scenario: Exactly 50 bricks on start (AC-04-a)
    Given the game enters PLAYING state
    When the brick grid is initialised
    Then exactly 50 bricks are active

  Scenario: Row colours top to bottom (AC-04-b)
    Given the brick grid is rendered
    When rows are drawn top to bottom
    Then row 0 colour is red
    And row 1 colour is orange
    And row 2 colour is yellow
    And row 3 colour is green
    And row 4 colour is blue

  Scenario: Brick destroyed on ball contact (AC-04-d)
    Given an active brick exists
    And the ball's bounding circle overlaps the brick's AABB
    When the collision is processed
    Then the brick is marked inactive
    And 10 points are added to the score
