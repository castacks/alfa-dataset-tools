%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% data_field.m - The class for working with the ALFA dataset messages.
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

classdef data_field
    properties
        TimeStamp double        % Time stamp for the message (in seconds)
        Fields table            % List of all the data fields in the message
    end
    
    properties (SetAccess = protected, GetAccess = public)
        FieldNames cell = {};   % List of all the field names in the message
    end
    
    methods
        
        %% Constructor for the message
        function obj = data_field(table_row)
            obj.Fields = table_row;
            obj.TimeStamp = table_row.time_recv;
        end
        
        %% Print the single message with its headers (field titles)
        function Print(obj)
            disp(obj.Fields);
        end
        
        %% Get all the field names as a cell list
        function value = get.FieldNames(obj)
            value = obj.Fields.Properties.VariableNames;
        end
    end
    
end
