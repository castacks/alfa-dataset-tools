# ALFA Dataset C++ Tools

## Introduction

This directory contains the basic code written in C++'11 language to work with the AIR Lab Fault Detection (ALFA) Dataset. The code is cross-platform and has been tested on Linux, OSX and Windows platforms with the most common compilers (GCC, MSVC, Clang).

For more information about the dataset and the tools for the other programming languages, please refer to the *README.md* file provided in the parent folder of this repository.

## Description of the files

In addition to this *README.md* file, the files in this project are:

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

*Cmake* allows creating the Makefiles or project files for many IDEs, including Visual Studio, XCode, Eclipse and CodeBlocks among many others. We recommend using the GUI CMake tool for the first time making the project files. A bit outdated but otherwise nice tutorial on *CMake* can be found [here](https://www.johnlamp.net/cmake-tutorial.html). The [official documentation](https://cmake.org/documentation) provides more updated information.

For each Operating System and depending on your applications, there are faster ways to get the project running. For example, in Linux you can run the following commands to install *CMake* command-line tools:

```
#!bash

sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install cmake
```
Then you change directory to `alpha-cpp` in command line and run the following to build the library:

```
#!bash

mkdir build
cd build
cmake ..
make
```
This should work if the default *CMake* configuration is Makefile. The resulted executable will be a `main` file in the `build` folder.

### Using the compiler
As mentioned above, *CMake* tool is very simple and helpful for making a project for your favorite IDE or Make system (Visual Studio, Makefile, etc.). An alternative is to compile the project directly to build the executable file. Depending on the choice of the compiler, the commands for compiling will be very different. However, once you learn the necessary commands, the process is not necessarily hard. Just remember that the code is written in C++'11 and the compiler should be aware of this.

An example command for compiling the code in Ubuntu Linux using the GNU C++ Compiler (g++) is:

```
#!bash

g++ -std=c++11 -I./include ./src/main.cpp -o ./main
```

You should run this command from the `alpha-cpp` directory. If you are getting an error about g++ command not being available, you would need to install the `build-essential` package.

```
#!bash

sudo apt-get update
sudo apt-get install build-essential
```

## Running the code
The executable file generated from the previous step is `main` in Linux/UNIX and Mac OS, and `main.exe` in Windows. To run the executable, change your current directory to the one containing the executable file. You need to pass the path to the desired `.bag` file containging the sequence as an argument. 

For example, in Ubuntu Linux, assuming that your executable is in `~/alpha-dataset-tools/alpha-cpp/build` and your desired sequence is in `~/alpha-dataset/processed/my_sequence/my_sequence.bag`. Then the commands will be:

```
#!bash

cd ~/alpha-dataset-tools/alpha-cpp/build
./main ~/alpha-dataset/processed/my_sequence/my_sequence.bag
```

## Citation
The tools and the dataset are provided with a publication. Please refer to the *README.md* file provided in the parent folder of this repository.

## Authors

* **Azarakhsh Keipour** - (keipour [at] cmu [dot] edu)
* **Mohammadreza Mousaei** - (mmousaei [at] cmu [dot] edu)
* **Sebastian Scherer** - (basti [at] cmu [dot] edu)

## License


## Acknowledgments

This work was supported through NASA Grant Number NNX17CL06C.
