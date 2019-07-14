# ALFA Dataset Python Tools

## Introduction

This directory contains the basic wrapper code written in C++'11 language to wrap our C++ Classes in Python and expose Classes and Methods to be able to work with them in Python 3.x. The code is cross-platform and has been tested on Linux, OSX and Windows platforms with the most common compilers (GCC, MSVC, Clang).

For more information about the dataset and the tools for the other programming languages, please refer to the *README.md* file provided in the parent folder of this repository.

For more information about the source code structure please refer to the *README.md* provided in the `alfa-cpp` folder.

## Building the code

We used *CMake* to build the Python wrapper files. In order to build Python wrapper files:

1. install *CMake* command-line tools:

```
#!bash

sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install cmake
```
2. Then you change directory to `alpha-python` in command line and run the following to build the library:

```
#!bash
cd alfa-dataset-tools/alfa-python
mkdir build
cd build
cmake ..
make
```
This should work if the default *CMake* configuration is Makefile. The resulted executable will be a `main` file in the `build` folder.

## Using the wrapper in Python

To use the wrapped classes in Python, copy the compiled file *alfa_python.so* (from your build directory) to where you wanna use it in Python. Then import the classes in your Python code. ex:

```
from alfa_python import Sequence
from alfa_python import Topic
```

Then you will have access to all corresponding methods and data members of these classes in Python.


## Citation
The tools and the dataset are provided with a publication. Please refer to the *README.md* file provided in the parent folder of this repository.

## Authors

* **Azarakhsh Keipour** - (keipour [at] cmu [dot] edu)
* **Mohammadreza Mousaei** - (mmousaei [at] cmu [dot] edu)
* **Sebastian Scherer** - (basti [at] cmu [dot] edu)

## License
[This software is BSD licensed.](http://opensource.org/licenses/BSD-3-Clause)

Copyright (c) 2019, Carnegie Mellon University
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## Acknowledgments

This work was supported through NASA Grant Number NNX17CL06C.
