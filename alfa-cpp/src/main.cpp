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
*   Last Modified: April 16, 2019
*   ***************************************************************************/

#include <iostream>
#include <string>
#include "sequence.h"
#include "commons.h"

bool ParseCommandLine(int argc, char** argv, std::string &out_sequence_path, std::string &out_sequence_name);
void PrintHelpMessage();
void PrintProjectInfo();

int main(int argc, char** argv)
{
    // Read the dataset name/path from command-line arguments
    std::string sequenceDir, sequenceName;
    bool parsed = ParseCommandLine(argc, argv, sequenceDir, sequenceName);
    
    // Exit if the command line is not properly formatted
    if (!parsed) return 0;

    // Print a message about the project
    PrintProjectInfo();
    std::cout << std::endl;

    // Read the sequence from the given directory 
    alfa::Sequence sequence(sequenceDir, sequenceName);

    // Print some information about the sequence
    sequence.PrintBriefInfo();
    std::cout << std::endl;

    // Find the index for a sample topic
    int mytopic_idx = sequence.FindTopicIndex("mavros-nav_info-roll");

    // Print 10 messages starting from number 20 from this topic
    if (mytopic_idx != -1)
    {
        std::cout << "Messages from topic " << sequence.Topics[mytopic_idx].Name << ":" << std::endl;
        std::cout << std::endl;
        sequence.Topics[mytopic_idx].Print(20, 10);
        std::cout << std::endl;
    }

    // Print the info for the first 10 messages in the whole sequence
    std::cout << "Info on the first 10 messages in the sequence:" << std::endl;
    for (int i = 0; (i < 10) && (i < (int)sequence.MessageIndexList.size()); ++i)
    {
        int topic_idx = sequence.MessageIndexList[i].TopicIdx;
        int message_idx = sequence.MessageIndexList[i].MessageIdx;
        std::cout << std::setw(2) << i << " | Time: " << sequence.GetMessage(i).DateTime <<
            " | Topic: " << sequence.Topics[topic_idx].Name << std::endl;
    }
    std::cout << std::endl;

    // Print the first fault message
    int fault_msg_idx = sequence.FindFirstFaultMessage();
    int fault_topic_idx = sequence.MessageIndexList[fault_msg_idx].TopicIdx;
    std::cout << "The first fault message in the sequence is from '" << sequence.Topics[fault_topic_idx].Name << "' topic." << std::endl;
    std::cout << "The fault happens after " << sequence.GetMessage(fault_msg_idx).DateTime - sequence.GetMessage(0).DateTime << " seconds." << std::endl;
    sequence.Topics[fault_topic_idx].Print(0, 1);
    std::cout << std::endl;

    // Retrieve the measured rolls (10 first messages)
    int rolltopic_idx = sequence.FindTopicIndex("mavros-nav_info-roll");
    auto rolls = sequence.Topics[rolltopic_idx].GetFieldsAsDouble("measured", 0, 10);
    // for (int i = 0; i < (int)rolls.size(); ++i)
    //     std::cout << "Roll " << i << ": " << 

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
    if (out_sequence_path.empty() || out_sequence_path[out_sequence_path.length() - 1] != alfa::Commons::FilePathSeparator) 
        out_sequence_path += alfa::Commons::FilePathSeparator;

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

// Print information about the project
void PrintProjectInfo()
{
    std::cout << "************************************************************************" << std::endl; 
    std::cout << "* Thank you for using the ALFA dataset!                                *" << std::endl;
    std::cout << "* The following is an example of using the provided alpha-cpp code.    *" << std::endl;
    std::cout << "* Please contact us about any questions or to report any bugs.         *" << std::endl;
    std::cout << "************************************************************************" << std::endl; 
    std::cout << "* For more information about the dataset, please refer to:             *" << std::endl;
    std::cout << "* http://theairlab.org/alfa-dataset                                    *" << std::endl;
    std::cout << "*                                                                      *" << std::endl;
    std::cout << "* For more information about this project and the publications related *" << std::endl;
    std::cout << "* to the dataset and this work, please refer to:                       *" << std::endl;
    std::cout << "* http://theairlab.org/fault-detection-project                         *" << std::endl;
    std::cout << "*                                                                      *" << std::endl;
    std::cout << "* Air Lab, Robotics Institute, Carnegie Mellon University              *" << std::endl;
    std::cout << "*                                                                      *" << std::endl;
    std::cout << "* Authors: Azarakhsh Keipour, Mohammadreza Mousaei, Sebastian Scherer  *" << std::endl;
    std::cout << "* Contact: keipour@cmu.edu                                             *" << std::endl;
    std::cout << "* **********************************************************************" << std::endl; 
}