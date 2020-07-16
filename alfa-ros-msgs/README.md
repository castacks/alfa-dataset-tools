# ALFA Dataset ROS Message Definitions

## Introduction

This directory contains the ROS message definitions used in the AIR Lab Fault Detection (ALFA) Dataset `bag` files. 

For more information about the dataset and the tools for the other programming languages, please refer to the *README.md* file provided in the parent folder of this repository.

## Description of the files

In addition to this *README.md* file, the files in this directory are:

- *NavDataPair.msg*: A message type to keep a single pair of the commanded and measured values of a signal (e.g., roll) along with the header.

- *NavErrors.msg*: A message type to keep the altitude error, airspeed error, crosstrack error, and the distance to the next waypoint along with the header.

- *NavVector3.msg*: A message type to keep a pair of 3-D vectors of the desired and measured values of a 3-D signal (e.g., position) along with the header.

For more detailed information about the structure and data types of each data field, please refer to the files.

## Instructions

Many of the `bag` file topics in the dataset use the message types defined by `mavros` package. To properly see the messages in the topics you would need to install the package. Please follow [these instructions](https://github.com/mavlink/mavros) to install `mavros` package from source.

In the next step, assuming that `mavros` package is already available in a catkin workspace, the message files in this directory should be copied into the `mavros/mavros_msgs/msg` directory in the catkin workspace and their filenames should be manually added to the `mavros/mavros_msgs/CMakeLists.txt` file. 

After rebuilding the catkin workspace without errors and sourcing the workspace, you will be able to see the new message types in ROS.

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
