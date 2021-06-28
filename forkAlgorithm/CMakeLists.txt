project(forksLib)

file(GLOB SOURCES "src/*.cpp")
add_library(forks SHARED ${SOURCES})

target_link_libraries(forks PRIVATE graph shortestPaths)

target_include_directories(forks
        PUBLIC
        include
        PRIVATE
        src
        )
