cmake_minimum_required(VERSION 2.8.3)
project(faultdetection_dataset)

# Setup CMake to run unit tests
enable_testing()

## Compile as C++11
add_compile_options(-std=c++11)

# Find boost libraries
find_package(Boost COMPONENTS iostreams thread system program_options filesystem REQUIRED)
# Include headers
include_directories(include)

# Build libraries
add_library(error     src/error.cpp)
add_library(topic src/topic.cpp)

# Add example executable
add_executable(main src/main.cpp)

# Link libraries with executable
target_link_libraries(main
                      topic
                      error)

# Set output to bin folder
#set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Find all unit test files
#file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)
