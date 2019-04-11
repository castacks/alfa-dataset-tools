# ALFA Dataset C++ Tools

## Introduction

This directory contains the basic code written in C++'11 language to work with the AIR Lab Fault Detection (ALFA) Dataset. The code is cross-platform and has been tested on Linux, OSX and Windows platforms with the most common compilers (GCC, MSVC, Clang).

For more information about the dataset and the tools for the other programming languages, please refer to the README file provided in the parent folder of this repository.

## Description of the files

In addition to this README file, the files in this project are:

- *src/main.cpp*: An example file showing some of the capablities of the library. It is suggested that you start from here to learn how to load a sequence and work with the dataset.

- *include/sequence.h*: A header file that defines a container class for a sequence. Each sequence is a collection of topics and each topic is a collection of messages. This header allows to load the whole sequence from the disk, go over topics, find a topic, iterate through all the messages in the sequence based on their time, etc. 
Additionally, it provides some useful information, such as the sequence duration, the flight time before error happened, and error information. 

- *include/topic.h*: A header file that defines a container class for a topic. Each topic is a collection of messages. This header allows to load a topic from the disk, go over the messages, checking the type of the topic (fault ground truth topic), printing the messages with their field labels, etc.

- *include/message.h*: A header file that defines a container class for a message. Each message has the recording time, may have a header (which includes the message's sequence id, epoch time and frame id) and the list of the other fields.

- *include/commons.h*: A header file contains the common functionalities between the above headers, including a class for DateTime, functions for converting strings to integers, cross-platform file and directory operations, etc.

- *CMakeLists.txt*: It contains a set of directives and instructions for the CMake build system describing the project's source files and targets. Is only used if you are planning to use CMake to build the system.

## Building the code

There are two ways to build the provided code:

- Directly using the command line compiler commands
- *(Recommended)* Using the *CMake* build system (also can create the project files for XCode and Visual Studio).

Depending on which way you want to use for the compilation, please follow the relevant subsection below.

### Using the CMake build system
*[CMake](https://cmake.org)* is a cross-platform open-source family of tools designed to build, test and package software. The binaries for the latest version can be downloaded from [here](https://cmake.org/download/) for Linux, Mac OS X and Windows. 

Some helpful OS and IDE-specific instructions follow.

#### Ubuntu Linux
As an alternative to installing the binaries from the website, you can run the following commands:

```
#!bash
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install cmake
```
Then go the `alpha-cpp` directory in command line and run the following commands to build the library:

```
#!bash
mkdir build
cd build
cmake ..
make
```
#### Microsoft Windows
#### Mac OS

### Using the compiler

#### Ubuntu Linux
#### Microsoft Windows
#### Mac OS

## Running the code
To run the main code:

```
    ./sample PATH_TO_BAG_FILE (e.g. path/sample.bag)
```

## Citation

### Tools and Dataset
The tools and the dataset are provided with the following paper. Please cite the following publicarion if you use either the tools or the dataset:

**BibTeX:** \
@article{keipour:dataset:2019, \
author={Azarakhsh Keipour and Mohammadreza Mousaei and Sebastian Scherer}, \
title={ALFA: A Dataset for UAV Fault and Anomaly Detection}, \
journal = {The International Journal of Robotics Research}, \
volume = {}, \
number = {}, \
pages = {}, \
year = {In press}, \
}

**IEEE Style:** \
A.  Keipour,  M.  Mousaei,  and  S.  Scherer,  “ALFA:  A  dataset  for  UAV fault  and  anomaly  detection,” *The  International  Journal of  Robotics Research*, In press.

### Portions of Dataset
Some portions of the dataset are released in the following publication. If you use any of those sequences, please also cite this paper:

**BibTeX:** \
@inproceedings{keipour:detection:2019, \
author={Azarakhsh Keipour and Mohammadreza Mousaei and Sebastian Scherer}, \
booktitle={2019 IEEE International Conference on Robotics and Automation (ICRA)}, \
title={Automatic Real-time Anomaly Detection for Autonomous Aerial Vehicles}, \
year={2019}, \
month={May}, \
pages={forthcoming} \
}

**IEEE Style:** \
A.  Keipour,  M.  Mousaei,  and  S.  Scherer,  “Automatic Real-time Anomaly Detection for Autonomous Aerial Vehicles,” in *2019 IEEE International Conference on Robotics and Automation (ICRA)*, May 2019, forthcoming.


## Authors

* **Azarakhsh Keipour** - (keipour [at] cmu [dot] edu)
* **Mohammadreza Mousaei** - [mmousaei](https://github.com/mmousaei)

## License


## Acknowledgments

This work was supported through NASA Grant Number NNX17CL06C.