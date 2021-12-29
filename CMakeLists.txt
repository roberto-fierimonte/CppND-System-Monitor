cmake_minimum_required(VERSION 2.6)

SET(GCC_COVERAGE_LINK_FLAGS    "-lstdc++fs")

project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")