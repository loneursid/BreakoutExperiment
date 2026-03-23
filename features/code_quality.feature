# features/code_quality.feature
# IMMUTABLE - Product Owner
# Last approved: 2026-03-23

Feature: Code Quality
  As a developer
  I want the codebase to follow quality standards
  So that it is maintainable and correct

  Scenario: No mutable global variables (AC-09-a)
    Given the codebase is reviewed
    When checking for global state
    Then no mutable global variables exist outside of the Game class

  Scenario: No magic numbers (AC-09-b)
    Given the codebase is reviewed
    When checking for magic numbers
    Then all numeric constants are defined in Constants.h

  Scenario: Zero compiler warnings (AC-09-c)
    Given the project is built
    When compiled with warnings enabled
    Then zero warnings are emitted

  Scenario: CMake produces runnable executable (AC-09-d)
    Given the CMakeLists.txt is present
    When CMake configure and build are run
    Then a runnable breakout.exe is produced
