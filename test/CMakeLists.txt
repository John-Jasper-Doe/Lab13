# CMake file for test directory

set(CMAKE_FOLDER "${CMAKE_FOLDER}/test")

addtest("alltests"
  testsplit.cpp
)
