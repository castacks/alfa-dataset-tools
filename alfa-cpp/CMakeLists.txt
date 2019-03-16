cmake_minimum_required(VERSION 2.8.3)
project(alfa_cpp_tools)

## Compile as C++11
add_compile_options(-std=c++11)

# Include headers
include_directories(include)

# Add example executable
add_executable(main 
    src/main.cpp 
    src/topic.cpp
    )

# Link libraries with executable
target_link_libraries(main)
