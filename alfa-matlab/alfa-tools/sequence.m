%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% sequence.m - The class for working with the ALFA dataset sequences.
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

classdef sequence < handle
    properties
        MessageList table   % List of all indices of messages
        TopicNames cell     % List of all the topic names
        Topics struct       % Data of all the topics
        Name char           % The name of the sequence
    end
    
    properties (SetAccess = protected, GetAccess = protected)
        is_initialized = false;  % Keep if the sequence is initialized
    end
    
    methods
        %% The constructor accepting no parameter or the filename to load
        function obj = sequence(filepath)
            % Don't do anything if there is no input
            if nargin < 1
                return;
            end
            % Load the file into the sequence
            obj.LoadSequence(filepath);
        end

        
        %% This function loads the sequence from a given .mat file
        function LoadSequence(obj, filepath)
            % Turn the warning about the Python objects off
            warn_status = warning('off','MATLAB:Python:UnsupportedLoad');

            % Set the initialization status to default
            obj.is_initialized = false;

            try
                % Load the sequence
                data = load(filepath);

                % Copy the loaded data to the object
                obj.MessageList = data.Sequence.MessageList;
                obj.TopicNames = data.Sequence.TopicNames;
                obj.Topics = data.Sequence.Topics;
                obj.Name = data.Sequence.Name;
                obj.is_initialized = true;
            catch
                error(['Error loading file ' filename]);
            end
            % Return the warning status as before
            warning(warn_status);
        end
        
        %% This function checks if the sequence is initialized
        function is_initialized = IsInitialized(obj)
                is_initialized = obj.is_initialized;
        end
        
        %% Get the message from the topic and message indices
        function Message = GetMessageByIndex(obj, TopicIndex, MessageIndex)
            
            % Find the topic we want
            Topic = obj.GetTopicByIndex(TopicIndex);

            % Retrieve the message from the topic
            Message = Topic.GetMessage(MessageIndex);
        end
        
        %% Get the message from the topic and message indices
        function Message = GetMessageByTopicName(obj, TopicName, MessageIndex)
            
            % Find the topic we want
            Topic = obj.GetTopicByName(TopicName);

            % Retrieve the message from the topic
            Message = Topic.GetMessage(MessageIndex);
        end
        
        %% Get the topic from the topic index
        function Topic = GetTopicByIndex(obj, TopicIndex)
            
            % Retrieve all the topic names
            fns = fieldnames(obj.Topics);

            % Find the topic we want
            Topic = obj.GetTopicByName(fns{TopicIndex});
        end
        
        %% Get the topic from the topic name
        function Topic = GetTopicByName(obj, TopicName)
            Topic = topic(TopicName, obj.Topics.(TopicName));
        end
        
        %% Get the duration of the sequence in seconds
        function Duration = GetTotalDuration(obj)
            
            % Find the first message in the sequence
            first_msg_idx = obj.MessageList(1, :);
            first_msg = obj.GetMessageByIndex(first_msg_idx.TopicIndex, first_msg_idx.MessageIndex);
            
            % Find the last message in the sequence
            last_msg_idx = obj.MessageList(size(obj.MessageList, 1), :);
            last_msg = obj.GetMessageByIndex(last_msg_idx.TopicIndex, last_msg_idx.MessageIndex);
            
            % Get the time difference between the first and the last
            Duration = last_msg.TimeStamp - first_msg.TimeStamp;
        end
        
        %% Clear the sequence object
        function Clear(obj)
            obj.MessageList = table;
            obj.TopicNames = {};
            obj.Topics = struct([]);
            obj.Name = char;
            obj.is_initialized = false;
        end
        
        %% Get the normal flight duration in the sequence in seconds
        function Duration = GetNormalFlightDuration(obj)
            
            % Find the first fault message
            fault_ind = -1;
            for i = 1 : size(obj.MessageList, 1)
                if topic.IsFaultTopicName(obj.TopicNames{obj.MessageList.TopicIndex(i)})
                    fault_ind = i;
                    break;
                end
            end
            if fault_ind == -1
                Duration = obj.GetTotalDuration();
                return;
            end
            
            % Find the first message in the sequence
            first_msg_idx = obj.MessageList(1, :);
            first_msg = obj.GetMessageByIndex(first_msg_idx.TopicIndex, first_msg_idx.MessageIndex);
            
            % Find the first fault message in the sequence
            fault_msg_idx = obj.MessageList(fault_ind, :);
            fault_msg = obj.GetMessageByIndex(fault_msg_idx.TopicIndex, fault_msg_idx.MessageIndex);
            
            % Get the time difference between the first and the last
            Duration = fault_msg.TimeStamp - first_msg.TimeStamp;
        end
        
        %% This function prints some information about the Sequence
        function PrintBriefInfo(obj)
            
            % Exit if the sequence is not initialized
            if ~obj.IsInitialized()
                warning('The sequence is not initialized.');
                return;
            end
            
            % Print the sequence name and the number of messages
            fprintf('Sequence Name    : %s\n', obj.Name);
            fprintf('Total Messages   : %d\n', size(obj.MessageList, 1));
            
            % Print the total flight duration in the sequence
            fprintf('Total Duration   : %0.2f\n', obj.GetTotalDuration());

            % Print the normal flight duration before the fault happened in the sequence
            fprintf('Normal Flight    : %0.2f\n', obj.GetNormalFlightDuration());

            % List all the topics in the sequence
            fprintf('Sequence has %d Topics:\n', length(obj.TopicNames));
            for i = 1 : length(obj.TopicNames)
                topc = obj.GetTopicByIndex(i);
                fprintf('   %2d: ', i);
                if topc.IsFaultTopic
                    fprintf('*');
                end
                fprintf('%s (Size: %d)\n', obj.TopicNames{i}, size(topc.Data, 1));
            end
        end
        
        %% Find the start time of the sequence
        function time_stamp = GetStartTime(obj)
            % Find the first message in the sequence
            first_msg_idx = obj.MessageList(1, :);
            time_stamp = obj.GetMessageByIndex(first_msg_idx.TopicIndex, first_msg_idx.MessageIndex).TimeStamp;
        end
    end
end
