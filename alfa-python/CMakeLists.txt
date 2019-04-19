cmake_minimum_required(VERSION 2.8)
include(ExternalProject)

# Adding customized cmake module for building boost
list(APPEND CMAKE_MODULE_PATH  "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(alfa-python)

  # Find default python libraries and interpreter
  find_package(PythonInterp REQUIRED)
  find_package(PythonLibs REQUIRED)
  include(BuildBoost) # Custom module

  include_directories(${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/../alfa-cpp/include/")
  link_directories(${Boost_LIBRARY_DIR})

  # Build and link the alfa_python module
  add_library(alfa_python SHARED alfa_python.cpp)
  target_link_libraries(alfa_python ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
  add_dependencies(alfa_python Boost)

  # Tweaks the name of the library to match what Python expects
  set_target_properties(alfa_python PROPERTIES SUFFIX .so)
  set_target_properties(alfa_python PROPERTIES PREFIX "")

  # # Build and link the alfa_python_auto module
  # add_library(alfa_python_auto SHARED alfa_python_auto.cpp)
  # target_link_libraries(alfa_python_auto ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
  # add_dependencies(alfa_python_auto Boost)

  # # Tweaks the name of the library to match what Python expects
  # set_target_properties(alfa_python_auto PROPERTIES SUFFIX .so)
  # set_target_properties(alfa_python_auto PROPERTIES PREFIX "")
