project(shortestPathsLib)

file(GLOB SOURCES "src/*.cpp")
add_library(shortestPaths SHARED ${SOURCES})

target_link_libraries(shortestPaths PRIVATE datastructures)

target_include_directories(shortestPaths
        PUBLIC
        include
        PRIVATE
        src
        )
