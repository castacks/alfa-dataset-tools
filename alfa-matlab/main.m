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
% Last Modified: July 16, 2020
%
% Copyright (c) 2020 Carnegie Mellon University,
% Azarakhsh Keipour <keipour@cmu.edu>
%
% For License information please see the README file in the root directory.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Input .mat file
% Please change the path to a sequence .mat file from the dataset
filename = '/home/azarakhsh/rosbags/processed/carbonZ_2018-07-18-15-53-31_1_engine_failure/carbonZ_2018-07-18-15-53-31_1_engine_failure.mat';

%% Add the dataset tools library to the path
addpath('alfa-tools');

%% Load the sequence 
% Two ways to do load a sequence:
% 1 - Through the constructor:
Sequence = sequence(filename);
% 2- Loading after defining the object
% Sequence = sequence;
% Sequence.LoadSequence(filename);

%% Print brief information about the sequence
Sequence.PrintBriefInfo();

%% Plot the commanded vs. measured roll as an example
% Get the topic for the roll
roll_topic = Sequence.GetTopicByName('mavros_nav_info_roll');
% Get the start time to normalize times to start from zero
start_time = Sequence.GetStartTime();
% Normalize the time stamps in the topic
times = roll_topic.Data.time_recv - start_time;
% Plot the data
figure;
plot(times, roll_topic.Data.measured);
hold on; grid on;
plot(times, roll_topic.Data.commanded);
hold off
