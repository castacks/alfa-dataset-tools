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
*   Last Modified: April 01, 2019
*   ***************************************************************************/

#include "topic.h"
#include "topic2.h"
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
    std::string pth = "/home/azarakhsh/ALFA-Dataset Tools/alfa-cpp/csv/sample.csv";
    alfa::Topic2 ttt(pth);
    std::cout << ttt.DataItems.size() << std::endl;
    for (int i = 0; i < 10 /*ttt.DataItems.size()*/; ++i)
    {
        std::cout << ttt.DataItems[i] << std::endl;
    }

    // Read the dataset name/path from command-line arguments
    std::string sequenceName;
    sequenceName = ParseCommandLine(argc, argv);

    // Exit if the sequence name is not read correctly
    if (sequenceName.empty()) return 0;

    // Read the data from the given directory 
    Topic topic;
    topic.read(sequenceName);

    // Print the data
    std::cout<<"--SAMPLE--"<<std::endl;
    for(int i = 0; i < 1/*topic.size*/; i++)
    {
        std::cout<<"Time in line "<<i<<" is: "<<topic.time[i]<<std::endl;
        std::cout<<"Header seq in line "<<i<<" is: "<<topic.header[i].seq<<std::endl;
        std::cout<<"Header sec timestamp in line "<<i<<" is: "<<topic.header[i].stamp.secs<<std::endl;
        std::cout<<"Header nsec timestamp in line "<<i<<" is: "<<topic.header[i].stamp.nsecs<<std::endl;
        std::cout<<"Header frame in line "<<i<<" is: "<<topic.header[i].frame<<std::endl;
        std::cout<<topic.data_labels[0]<<" in line "<<i<<" is: "<<topic.data[0][i]<<std::endl;
        std::cout<<topic.data_labels[1]<<" in line "<<i<<" is: "<<topic.data[1][i]<<std::endl;
    }

    return 0;
}

// Parse command-line arguments
std::string ParseCommandLine(int argc, char** argv) 
{
    if ((argc != 3) || (argv[argc-1] == NULL) || (argv[argc-1][0] == '-') ) 
    {
        // The input is in incorrect format
        std::cout << "Please provide the path and name of the dataset!" << std::endl;
        std::cout << "Usage (in Linux/Mac):" << std::endl;
        std::cout << "./main path/to/dataset/folder sequence_name" << std::endl;
        std::cout << "Usage (in Windows):" << std::endl;
        std::cout << "main.exe path\\to\\dataset\\folder sequence_name" << std::endl;
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