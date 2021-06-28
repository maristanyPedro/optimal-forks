cmake_minimum_required (VERSION 3.10)
project(intervalForks)

set(CMAKE_CXX_STANDARD 17)
#add_compile_options(-Wall -Wextra -pedantic -Werror)
add_compile_options(-Wall -Wextra -pedantic)

add_subdirectory(datastructures)
add_subdirectory(graph)
add_subdirectory(shortestPaths)
add_subdirectory(forkAlgorithm)

add_executable(forkOptimizer.o ./src/main.cpp)

target_link_libraries(forkOptimizer.o PUBLIC graph datastructures shortestPaths forks)

