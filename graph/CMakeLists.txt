project(graphLib) 

add_compile_options(-Wall -Wextra -pedantic)
add_library(graph SHARED src/graph.cpp)
target_include_directories(graph PUBLIC includes PRIVATE src)
