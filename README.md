# ALFA Dataset Tools

## Introduction

This repository contains the basic tools to work with the AIR Lab Fault Detection (ALFA) Dataset. ALFA dataset contains autonomous flight sequences of a UAV plane with eight different types of fault happening during the flights and is provided to help advance fault and anomaly detection and isolation research. 

For more information about the dataset, please refer to [http://theairlab.org/alfa-dataset](http://theairlab.org/alfa-dataset).

For more information about this project and the publications related to the dataset and this work, please refer to [http://theairlab.org/fault-detection-project](http://theairlab.org/fault-detection-project).

## Description of Tools

These tool packages are provided for reading and working with the dataset:

- *alfa-cpp*: The tools to read, filter and iterate through the sequences in C++'11 language.

- *alfa-python*: The tools to read, filter and iterate through the sequences in Python 3.x language.

- *alfa-matlab*: The tools to read, filter and iterate through the sequences in MATLAB development environment.

- *alfa-evaluate*: The tools for evaluation of new methods with the provided dataset. The code is written in C++'11 language, but examples are provided in C++'11, Python 3.x and MATLAB on how to evaluate the methods in those languages using these tools. 

- *alfa-ros-messages*: The custom ROS message definitions for topics in the `bag` files. The message definitions are only provided for working with the `bag` files and are not required for other dataset file types.

For more information, please refer to each individual package about how to install and use them.

## Citation

### Tools and Dataset
The tools and the dataset are provided with the following paper ([available on arXiv](https://arxiv.org/abs/1907.06268)). Please cite the following publicarion if you use either the tools or the dataset:

**BibTeX:** \
@article{keipour:dataset:2020, \
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
Some portions of the dataset are released in the following publication ([available on arXiv](https://arxiv.org/abs/1907.00511)). If you use any of those sequences, please also cite this paper:

**BibTeX:** \
@inproceedings{keipour:detection:2019, \
author={Azarakhsh Keipour and Mohammadreza Mousaei and Sebastian Scherer}, \
booktitle={2019 IEEE International Conference on Robotics and Automation (ICRA)}, \
title={Automatic Real-time Anomaly Detection for Autonomous Aerial Vehicles}, \
year={2019}, \
month={May}, \
pages={5679-5685} \
doi={10.1109/ICRA.2019.8794286} \
}

**IEEE Style:** \
A. Keipour, M. Mousaei, and S. Scherer, “Automatic Real-time Anomaly Detection for Autonomous Aerial Vehicles,” in *2019 IEEE International Conference on Robotics and Automation (ICRA)*, May 2019, pp.5679-5685. doi: 10.1109/ICRA.2019.8794286. 

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
