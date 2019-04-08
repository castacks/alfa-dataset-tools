/*  ***************************************************************************
*   sequence.h - Header for working with the sequences of ALFA dataset.
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

#ifndef ALFA_SEQUENCE_H
#define ALFA_SEQUENCE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "commons.h"
#include "topic.h"

namespace alfa
{

// This class keeps the information of a single topic in a dataset sequence
class Sequence
{
public:

    // Class Data Members
    std::string Name = "N/A";
    std::string FileName;
    std::vector<Topic> Topics;

    // Constructors & Deconstructors
    Sequence(std::string sequence_path = "", std::string sequence_name = "N/A");

    // Member Functions
    bool LoadSequence(std::string filename);
    //int Print(int n_start = 0, int n_messages = -1, std::string field_separator = " | ");
    //int PrintHeader(std::string field_separator = " | ");
    bool IsInitialized();
    void Clear();

private:
    // Data Members
    bool is_initialized = false;

    // Member Functions
    void MergeTopics();
};

/******************************************************************************/
/************************** Function Definitions ******************************/
/******************************************************************************/

// Contructor function for Sequence. Loads all CSV files of an ALFA dataset sequence.
Sequence::Sequence(std::string sequence_path, std::string sequence_name)
{
    // Assign the given sequence name
    Name = sequence_name;

}


}
#endif