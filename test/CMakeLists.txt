# CMake file for test directory

set(CMAKE_FOLDER "${CMAKE_FOLDER}/test")

set(PROJECT_SRC
  ${CMAKE_SOURCE_DIR}/src/db/table.cpp
)

addtest("alltests"
  testtable.cpp
  ${PROJECT_SRC}
)
