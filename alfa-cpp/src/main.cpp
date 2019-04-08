/*  ***************************************************************************
*   main.cpp - Demonstrates the usage of ALFA libraries and dataset.
*   
*   For more information about the dataset, please refer to:
*   http://theairlab.org/alfa-dataset
*
*   For more information about this project and the publications related to 
*   the dataset and this work, please refer to:
*   http://theairlab.org/fault-detection-project
*
*   Air Lab, Robotics Institute, Carnegie Mellon University
*
*   Authors: Azarakhsh Keipour, Mohammadreza Mousaei, Sebastian Scherer
*   Contact: keipour@cmu.edu
*
*   Last Modified: April 07, 2019
*   ***************************************************************************/

#include "topic.h"
#include "commons.h"
#include <iostream>
#include <string>

bool ParseCommandLine(int argc, char** argv, std::string &out_sequence_path, std::string &out_sequence_name);

int main(int argc, char** argv)
{
    // Read the dataset name/path from command-line arguments
    std::string sequenceDir, sequenceName;
    bool parsed = ParseCommandLine(argc, argv, sequenceDir, sequenceName);
    
    // Exit if the command line is not properly formatted
    if (!parsed) return 0;

    std::vector<std::string> FileList = alfa::Commons::GetFileList(sequenceDir);
    FileList = alfa::Commons::FilterFileList(FileList, "csv", true);
    for (int i = 0; i < FileList.size(); ++i)
        std::cout << FileList[i] << std::endl;

    // Read the data from the given directory 
    alfa::Topic topic(sequenceDir + sequenceName);

    // Print the first 10 data items
    topic.Print(0, 10);

    return 0;
}

// Parse command-line arguments
bool ParseCommandLine(int argc, char** argv, std::string &out_sequence_path, std::string &out_sequence_name) 
{
    if ((argc != 3) || (argv[argc-1] == NULL) || (argv[argc-1][0] == '-') ) 
    {
        // The input is in incorrect format
        std::cout << "Please provide the path and name of the sequence!" << std::endl;
        std::cout << "Usage (in Linux/Mac):" << std::endl;
        std::cout << "./main path/to/sequence/folder sequence_name" << std::endl;
        std::cout << "Usage (in Windows):" << std::endl;
        std::cout << "main.exe path\\to\\sequence\\folder sequence_name" << std::endl;
        return false;
    }

    // Extract the path and the sequence name
    out_sequence_path = std::string(argv[1]);
    out_sequence_name = std::string(argv[2]);

    // Add the path separator to the path
    if (out_sequence_path[out_sequence_path.length() - 1] != alfa::PathSeparator) 
        out_sequence_path += alfa::PathSeparator;

    return true;
}