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
*   Last Modified: April 08, 2019
*   ***************************************************************************/

#ifndef ALFA_SEQUENCE_H
#define ALFA_SEQUENCE_H

#include <string>
#include <vector>
#include <iostream>
#include <cctype>
#include <algorithm>
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
    std::string DirectoryPath;
    std::vector<Topic> Topics;

    // Constructors & Deconstructors
    Sequence(const std::string &sequence_dir = "", const std::string &sequence_name = "N/A");

    // Member Functions
    bool LoadSequence(const std::string &sequence_dir, const std::string &sequence_name);
    //int Print(int n_start = 0, int n_messages = -1, std::string field_separator = " | ");
    //int PrintHeader(std::string field_separator = " | ");
    bool IsInitialized() const;
    void Clear();

private:
    // Data Members
    bool is_initialized = false;

    // Member Functions
    void MergeTopics();
    std::string ExtractTopicName(const std::string &topic_filename);
    bool ExtractTopicNames(VecString &out_topic_files, VecString &out_topic_names);
};

/******************************************************************************/
/************************** Function Definitions ******************************/
/******************************************************************************/

// Contructor function for Sequence. Loads all CSV files of an ALFA dataset sequence.
Sequence::Sequence(const std::string &sequence_dir, const std::string &sequence_name)
{
    // Load the sequence if the path is provided
    if (!sequence_dir.empty())
        LoadSequence(sequence_dir, sequence_name);
}

// Load all the topic files in a sequence
bool Sequence::LoadSequence(const std::string &sequence_dir, const std::string &sequence_name)
{
    // Save the given directory and sequence name
    DirectoryPath = sequence_dir;
    Name = sequence_name;

    // Extract the list of all the topic names and topic filenames
    VecString topic_list, topic_file_list;
    if (ExtractTopicNames(topic_file_list, topic_list) == false)
    {
        // Output error if no topics are found
        std::cerr << "No topic files found at '" << sequence_dir << "' directory." << std::endl;
        return false;
    }

    // Load all the topics
    for (int i = 0; i < static_cast<int>(topic_list.size()); ++i)
    {
        std::cout << "Loading topic: " << topic_list[i] << std::endl;
        std::string topic_full_filename = sequence_dir + topic_file_list[i] + "." + Commons::CSVFileExtension;
        Topics.push_back(Topic(topic_full_filename, topic_list[i]));
    }

    // Initialization done
    is_initialized = true;

    return IsInitialized();
}

// Returns the initialization status
bool Sequence::IsInitialized() const
{
    return is_initialized;
}

/******************************************************************************/
/*********************** Local Function Definitions ***************************/
/******************************************************************************/

// Extract the topic name from its filename removing the sequence name from it.
// Assumes that the topic file name starts with the sequence name followed by
// a connecting character and then the topic name.
std::string Sequence::ExtractTopicName(const std::string &topic_filename)
{
    std::string topic_name;
    
    // Return if the filename is smaller than the sequence name
    if (topic_filename.size() < this->Name.size() + 1) return "";

    // Return if the beginning of the filename does not match the sequence name
    if (topic_filename.substr(0, this->Name.size()) != this->Name) return "";

    // Remove the connecting character between the topic and sequence names 
    int start_pos = this->Name.size();
    if (!isalnum(topic_filename[start_pos])) 
        ++start_pos; 

    // Extract and return the topic name
    return topic_filename.substr(start_pos);
}

// Extract the topic names and filenames given the sequence directory and sequence name
bool Sequence::ExtractTopicNames(VecString &out_topic_files, VecString &out_topic_names)
{
    // Clear the output variables
    out_topic_files.clear();
    out_topic_names.clear();

    // Extract the list of all the CSV files in the directory
    VecString dir_file_list = Commons::FilterFileList(Commons::GetFileList(DirectoryPath), Commons::CSVFileExtension, true);

    // Sort the file list alphabetically
    std::sort(dir_file_list.begin(), dir_file_list.end());

    // Extract the topic names from their file names
    for (int i = 0; i < static_cast<int>(dir_file_list.size()); ++i)
    {
        std::string topic_name = ExtractTopicName(dir_file_list[i]);
        if (!topic_name.empty())
        {
            out_topic_files.push_back(dir_file_list[i]);
            out_topic_names.push_back(topic_name);
        }
    }

    // Check if the topic files were successfully extracted
    if (out_topic_names.empty()) return false;

    return true;
}


}
#endif