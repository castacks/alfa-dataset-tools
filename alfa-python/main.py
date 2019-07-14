#  ***************************************************************************
#   main.cpy - Demonstrates the usage of ALFA libraries and dataset.
#
#   For more information about the dataset, please refer to:
#   http://theairlab.org/alfa-dataset
#
#   For more information about this project and the publications related to
#   the dataset and this work, please refer to:
#   http://theairlab.org/fault-detection-project
#
#   Air Lab, Robotics Institute, Carnegie Mellon University
#
#   Authors: Azarakhsh Keipour, Mohammadreza Mousaei, Sebastian Scherer
#   Contact: keipour@cmu.edu
#
#   Last Modified: April 16, 2019
#
#   Copyright (c) 2019 Carnegie Mellon University,
#   Azarakhsh Keipour <keipour@cmu.edu>
#
#   For License information please see the README file in the root directory.
#
#   ***************************************************************************
from alfa_python import Sequence
# from alfa_python import Commons

parsed = ParseCommandLine(sys.argc, sys.argv, sequenceDir, sequenceName)




def ParseCommandLine(argc, argv, out_sequence_path, out_sequence_name):

    #  Check the number and the format of the inputs
    if ((argc != 2) or (argv[argc-1] == NULL) or (argv[argc-1][0] == '-') ):
    
        PrintHelpMessage()
        return false
    

    #  Extract the path and the sequence name
    bag_path = argv[1]
    extracted = Commons.ExtractFilenameAndExtension(bag_path, out_sequence_name, extension, out_sequence_path)

    #  Check that the file exists and extension is correct
    if (not extracted or (extension != "bag")):
    
        PrintHelpMessage()
        return false
    

    #  Add the path separator to the path
    if (not out_sequence_path or out_sequence_path[out_sequence_path.length() - 1] != Commons.FilePathSeparator) 
        out_sequence_path += Commons.FilePathSeparator

    return true
