cmake_minimum_required(VERSION 3.16)
project(rate_limiter LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++2b")

find_package(Threads REQUIRED)
include_directories(include)

file(GLOB_RECURSE ALL_SRCS CONFIGURE_DEPENDS "src/*.cpp")
set(LIB_SRCS ${ALL_SRCS})
list(FILTER LIB_SRCS EXCLUDE REGEX ".*main\\.cpp$")

add_executable(rate_limiter ${ALL_SRCS})
target_link_libraries(rate_limiter PRIVATE Threads::Threads)

enable_testing()
add_executable(unit_tests tests/test.cpp ${LIB_SRCS})
target_link_libraries(unit_tests PRIVATE Threads::Threads)
add_test(NAME AllTests COMMAND unit_tests)
