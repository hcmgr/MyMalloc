### General-purpose testing library I use for a lot of my projects

#### Structure

- test.c / test.h:
    - where all tests are specified
    - NOTE: In future, can implement test suites
        - have general 'tests' directory
        - each test suite gets its own test/X file
- testRunner.c:
    - runs all tests implemented in test.c / test.h
- testHelpers.c / testHelpers.h:
    - collection of miscellaneous helper functions