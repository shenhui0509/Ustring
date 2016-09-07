project(UnitTest CXX C)

include_directories(${USTR_SOURCE_DIR})
include_directories(${USTR_SOURCE_DIR}/src)
link_directories(${USTR_BINARY_DIR}/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Wall -g")

function(cxx_executable name)
  add_executable("${name}" "${name}.cpp")
  target_link_libraries("${name}" gtest pthread ustr)
endfunction()

cxx_executable(test_mempool)
cxx_executable(test_charclass)
