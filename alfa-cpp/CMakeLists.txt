cmake_minimum_required(VERSION 2.8.3)
project(faultdetection_dataset)

# Setup CMake to run unit tests
enable_testing()

## Compile as C++11
add_compile_options(-std=c++11)

# Find boost libraries
find_package(Boost COMPONENTS iostreams thread system program_options filesystem REQUIRED)
# Include headers
include_directories(${Boost_INCLUDE_DIRS}
                    include)

# Build libraries
add_library(error     src/error.cpp)
add_library(topic src/topic.cpp)

# Add example executable
add_executable(sample src/sample.cpp)

# Link libraries with executable
target_link_libraries(sample
                      topic
                      error
        		      ${Boost_LIBRARIES})

# Set output to bin folder
#set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Find all unit test files
#file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)

## Initialize each test for building
#foreach(testSrc ${TEST_SRCS})
#        #Extract the filename without an extension (NAME_WE)
#        get_filename_component(testName ${testSrc} NAME_WE)
#
#        #Add compile target
#        add_executable(${testName} ${testSrc})
#
#        # Link to Boost libraries, targets, and dependencies
#        target_link_libraries(${testName}
#                              gtsam
#                              localizer
#                              csvreader
#                              error
#                              interp
#                             linefit
#                              ${Boost_LIBRARIES})
#
#        # Set output to test/bin
#        set_target_properties(${testName} PROPERTIES
#            RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/test/bin)
#
#        #Finally add it to test execution -
#        add_test(NAME ${testName}
#                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/bin
#                 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/bin/${testName} )
#endforeach(testSrc)
