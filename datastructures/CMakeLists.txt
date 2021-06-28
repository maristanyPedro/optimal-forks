project(datastructuresLib)

file(GLOB SOURCES "src/*.cpp")
add_library(datastructures SHARED ${SOURCES})

target_include_directories(datastructures
        PUBLIC
        include
        PRIVATE
        src
        )