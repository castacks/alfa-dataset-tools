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

const char PathSeparator = 
#if defined _WIN32 || defined __CYGWIN__
    '\\';
#else
    '/';
#endif

std::string ParseCommandLine(int argc, char** argv);

int main(int argc, char** argv)
{
    // Read the dataset name/path from command-line arguments
    std::string sequenceName;
    sequenceName = ParseCommandLine(argc, argv);

    // Exit if the sequence name is not read correctly
    if (sequenceName.empty()) return 0;

    // Read the data from the given directory 
    alfa::Topic2 topic(sequenceName);
    
    // Print the number of data items read and the first 10 items
    std::cout << topic.DataItems.size() << std::endl;
    topic.Print(0, 10);

    return 0;
}

// Parse command-line arguments
std::string ParseCommandLine(int argc, char** argv) 
{
    if ((argc != 3) || (argv[argc-1] == NULL) || (argv[argc-1][0] == '-') ) 
    {
        // The input is in incorrect format
        std::cout << "Please provide the path and name of the sequence!" << std::endl;
        std::cout << "Usage (in Linux/Mac):" << std::endl;
        std::cout << "./main path/to/sequence/folder sequence_name" << std::endl;
        std::cout << "Usage (in Windows):" << std::endl;
        std::cout << "main.exe path\\to\\sequence\\folder sequence_name" << std::endl;
        return "";
    }

    // Extract the path and the sequence name
    std::string path = std::string(argv[1]);
    std::string sequenceName = std::string(argv[2]);

    // Add the path separator to the path
    if (path[path.length() - 1] != PathSeparator) 
        path += PathSeparator;

    return path + sequenceName;
}