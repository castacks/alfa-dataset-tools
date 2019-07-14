%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% main.m - Demonstrates the usage of ALFA libraries and dataset.
% 
% For more information about the dataset, please refer to:
% http://theairlab.org/alfa-dataset
%
% For more information about this project and the publications related to 
% the dataset and this work, please refer to:
% http://theairlab.org/fault-detection-project
%
% Air Lab, Robotics Institute, Carnegie Mellon University
%
% Authors: Azarakhsh Keipour, Mohammadreza Mousaei, Sebastian Scherer
% Contact: keipour@cmu.edu
%
% Last Modified: April 19, 2019
%
% Copyright (c) 2019 Carnegie Mellon University,
% Azarakhsh Keipour <keipour@cmu.edu>
%
% For License information please see the README file in the root directory.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Input .mat file
% Please change the path to a sequence .mat file from the dataset

filename = '/home/azarakhsh/rosbags/processed/carbonZ_2018-07-18-12-10-11_no_ground_truth/carbonZ_2018-07-18-12-10-11_no_ground_truth.mat';

%% Load the sequence
Sequence = alfa.LoadSequence(filename);

%% Print the duration of the flight
disp(alfa.GetSequenceDuration(Sequence));
