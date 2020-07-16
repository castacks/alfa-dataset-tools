%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% topic.m - The class for working with the ALFA dataset topics.
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

classdef topic
    properties (SetAccess = protected, GetAccess = public)
        IsFaultTopic = false;   % Keeps if the topic is the fault ground truth
        Data table              % The data in the topic
        Name char = '';         % The topic name
    end
    
    methods
        
        %% Constructor for the topic
        function obj = topic(name, table_rows)
            obj.Data = table_rows;
            obj.Name = name;
            obj = obj.UpdateTopic();
        end
        
        %% Print the topic with headers and data
        function Print(obj)
            disp(obj.Data);
        end
        
        %% Extract a message from the topic using its index
        function Message = GetMessage(obj, MessageIndex)
            Message = data_field(obj.Data(MessageIndex, :));
        end
    end
    
    methods (Access = protected)
        
        %% Update the topic's fault status
        function obj = UpdateTopic(obj)
            obj.IsFaultTopic = topic.IsFaultTopicName(obj.Name);
        end
    end
    
    methods (Static)
        
        %% Check if a given topic name matches the fault topic name pattern
        function is_fault_topic_name = IsFaultTopicName(str)
            is_fault_topic_name = strncmp(str, 'failure_status', 14);
        end
    end
    
end

