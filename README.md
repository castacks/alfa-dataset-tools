# ALFA Dataset Tools

This repository is still under development...

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

For more information, please refer to each package about how to install and use them.

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
