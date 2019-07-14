%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% alfa.m - The Matlab library for working with the ALFA dataset.
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

classdef alfa
    methods(Static)

        %% This function loads the sequence from a given .mat file
        function Sequence = LoadSequence(filepath)
            % Turn the warning about the Python objects off
            warn_status = warning('off','MATLAB:Python:UnsupportedLoad');

            % Load the sequence
            Sequence = load(filepath);

            % Simplify the access to the data
            Sequence = Sequence.Sequence;

            % Return the warning status as before
            warning(warn_status);
        end
        
        %% Get the message from the topic and message indices
        function Message = GetMessage(Sequence, TopicIndex, MessageIndex)
            
            % Retrieve all the topic names
            fns = fieldnames(Sequence.Topics);

            % Find the topic we want
            Topic = Sequence.Topics.(fns{TopicIndex});

            % Retrieve the message from the topic
            Message = Topic(MessageIndex, :);
        end
        
        %% Get the duration of the sequence in seconds
        function Duration = GetSequenceDuration(Sequence)
            
            % Find the first message in the sequence
            first_msg_idx = Sequence.MessageList(1, :);
            first_msg = alfa.GetMessage(Sequence, first_msg_idx.TopicIndex, first_msg_idx.MessageIndex);
            
            % Find the last message in the sequence
            last_msg_idx = Sequence.MessageList(size(Sequence.MessageList, 1), :);
            last_msg = alfa.GetMessage(Sequence, last_msg_idx.TopicIndex, last_msg_idx.MessageIndex);
            
            % Get the time difference between the first and the last
            Duration = last_msg.time_recv - first_msg.time_recv;
        end
        
        %% This function prints some information about the Sequence
        
        
        
    end
end
