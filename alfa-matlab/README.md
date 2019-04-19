# ALFA Dataset MATLAB Tools

## Introduction

This directory contains the basic code written in MATLAB language to work with the AIR Lab Fault Detection (ALFA) Dataset. The code is cross-platform and has been tested on Linux, OSX and Windows platforms.

For more information about the dataset and the tools for the other programming languages, please refer to the *README.md* file provided in the parent folder of this repository.

## Description of the files

In addition to this *README.md* file, the files in this project are:

- *main.m*: An example file showing some of the capablities of the library. It is suggested that you start from here to learn how to load a sequence and work with the dataset.

- *alfa.m*: A file that defines a number of useful functions to work with the sequences, including loading the sequence, retrieving the messages, getting some useful information such as the sequence duration, the flight time before the fault happened, and the fault information. 

## Running the code
Change the `filename` in the `main.m` file to the desired sequence `.mat` file. Then running the code should show some information about the sequence and its topics.

## Citation
The tools and the dataset are provided with a publication. Please refer to the *README.md* file provided in the parent folder of this repository.

## Authors

* **Azarakhsh Keipour** - (keipour [at] cmu [dot] edu)
* **Mohammadreza Mousaei** - (mmousaei [at] cmu [dot] edu)
* **Sebastian Scherer** - (basti [at] cmu [dot] edu)

## License


## Acknowledgments

This work was supported through NASA Grant Number NNX17CL06C.
