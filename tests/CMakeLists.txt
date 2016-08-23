project(UnitTest CXX C)

include_directories(${USTR_SOURCE_DIR}/utils)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Wall -g")

function(cxx_executable name)
  add_executable("${name}" "${name}.cpp")
  target_link_libraries("${name}" gtest pthread)
endfunction()

cxx_executable(test_mempool)
  
