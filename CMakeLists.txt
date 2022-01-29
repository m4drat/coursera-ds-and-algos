cmake_minimum_required(VERSION 3.18)
project(algorithms_and_ds LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions("LOCAL_ENV=1")

include_directories("utils/")

add_subdirectory("1-algorithmic-toolbox")
# add_subdirectory("2-data-structures")
# add_subdirectory("3-algorithms-on-strings")
# add_subdirectory("4-algorithms-on-graphs")
# add_subdirectory("5-advanced-algorithms-and-complexity")
