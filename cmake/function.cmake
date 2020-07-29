# Functions for dressing "life" :)

##
function(addtest TESTNAME FIRSTSOURCE)
  add_executable(${TESTNAME} main.cpp ${FIRSTSOURCE} ${ARGN})
  target_include_directories(${TESTNAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
  target_link_libraries(${TESTNAME} gtest)
  add_test(
    NAME ${TESTNAME}
    COMMAND ${CMAKE_COMMAND} --build . --target ${TESTNAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
  set_target_properties(${TESTNAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_bin
  )
endfunction(addtest)

