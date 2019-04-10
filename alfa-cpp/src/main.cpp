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

#include <iostream>
#include <string>
#include "sequence.h"
#include "commons.h"

bool ParseCommandLine(int argc, char** argv, std::string &out_sequence_path, std::string &out_sequence_name);
void PrintHelpMessage();

int main(int argc, char** argv)
{
    // Read the dataset name/path from command-line arguments
    std::string sequenceDir, sequenceName;
    bool parsed = ParseCommandLine(argc, argv, sequenceDir, sequenceName);
    
    // Exit if the command line is not properly formatted
    if (!parsed) return 0;

    // Read the sequence from the given directory 
    alfa::Sequence sequence(sequenceDir, sequenceName);

    // Print the first 10 data items
    //seque.Print(0, 10);

    return 0;
}

// Parse command-line arguments
bool ParseCommandLine(int argc, char** argv, std::string &out_sequence_path, std::string &out_sequence_name) 
{
    // Check the number and the format of the inputs
    if ((argc != 2) || (argv[argc-1] == NULL) || (argv[argc-1][0] == '-') ) 
    {
        PrintHelpMessage();
        return false;
    }

    // Extract the path and the sequence name
    std::string bag_path = std::string(argv[1]);
    std::string extension;
    bool extracted = alfa::Commons::ExtractFilenameAndExtension(bag_path, out_sequence_name, extension, out_sequence_path);

    // Check that the file exists and extension is correct
    if (!extracted || (extension != "bag"))
    {
        PrintHelpMessage();
        return false;
    }

    // Add the path separator to the path
    if (out_sequence_path.empty() || out_sequence_path[out_sequence_path.length() - 1] != alfa::PathSeparator) 
        out_sequence_path += alfa::PathSeparator;

    return true;
}

// Print a message for the user about the command line input format
void PrintHelpMessage()
{
    std::cout << "Please provide the path to the sequence bag file!" << std::endl;
    std::cout << "Usage (in Linux/Mac):" << std::endl;
    std::cout << "./main path/to/sequence/bagfile.bag" << std::endl;
    std::cout << "Usage (in Windows):" << std::endl;
    std::cout << "main.exe path\\to\\sequence\\bagfile.bag" << std::endl;
}