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
*   Last Modified: April 10, 2019
*   ***************************************************************************/

#ifndef ALFA_SEQUENCE_H
#define ALFA_SEQUENCE_H

#include <string>
#include <vector>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <queue>
#include <functional>
#include "commons.h"
#include "topic.h"

namespace alfa
{

// This class keeps the information of a dataset sequence
class Sequence
{
public:

    // Subclasses
    class MessageIndex 
    { 
    public:
        int TopicIdx; int MessageIdx; 
        MessageIndex(int topic_idx = -1, int message_idx = -1)
            : TopicIdx(topic_idx), MessageIdx(message_idx) {}
    };

    // Class Data Members
    std::string Name = "N/A";
    std::string DirectoryPath;
    std::vector<Topic> Topics;
    std::vector<MessageIndex> MessageIndexList;

    // Constructors & Deconstructors
    Sequence(const std::string &sequence_dir = "", const std::string &sequence_name = "N/A");

    // Member Functions
    bool LoadSequence(const std::string &sequence_dir, const std::string &sequence_name);
    bool IsInitialized() const;
    void Clear();
    Message GetMessage(size_t msg_idx);
    void PrintBriefInfo();

private:
    // Data Members
    bool is_initialized = false;

    // Member Functions
    std::string ExtractTopicName(const std::string &topic_filename);
    bool ExtractTopicNames(VecString &out_topic_files, VecString &out_topic_names);
    void CreateMessageList();
    bool CompareMessageIndices(MessageIndex msg1, MessageIndex msg2);
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
        std::string topic_full_filename = sequence_dir + topic_file_list[i] + "." + Commons::CSVFileExtension;
        Topics.push_back(Topic(topic_full_filename, topic_list[i]));
    }

    // Create the sorted message list of all the topics
    CreateMessageList();

    // Initialization done
    is_initialized = true;

    return IsInitialized();
}

// Returns the initialization status
bool Sequence::IsInitialized() const
{
    return is_initialized;
}

// Clear the entire sequence object
void Sequence::Clear()
{
    Name = "N/A";
    DirectoryPath = "";
    Topics.clear();
    MessageIndexList.clear();
    is_initialized = false;
}

// Get messages by index from the message collection sorted by the recording time
Message Sequence::GetMessage(size_t msg_idx)
{
    // Check if the index is in range
    if (msg_idx >= MessageIndexList.size())
        return Message();
    
    return Topics[MessageIndexList[msg_idx].TopicIdx].Messages[MessageIndexList[msg_idx].MessageIdx];
}

// Print some brief information like the number and names of topics, total messages, time, etc.
void Sequence::PrintBriefInfo()
{
    if (!IsInitialized())
    {
        std::cout << "Sequence is not initialized!" << std::endl;
        return;
    }

    std::cout << "Sequence Name    : " << Name << std::endl;
    std::cout << "Total Messages   : " << MessageIndexList.size() << std::endl;
    std::cout << "Sequence Duration: " << "" << std::endl;
    std::cout << Topics.size() << " Topics:" << std::endl;
    for (int i = 0; i < static_cast<int>(Topics.size()); ++i)
        std::cout << std::setw(2) << i << ": " << Topics[i].Name << " (Size: " << Topics[i].Messages.size() << ")" << std::endl;
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

// Merge all the messages in all the topics into MessageIndexList sorted by their recorded time
void Sequence::CreateMessageList()
{
    // Define a typedef for simplicity
    typedef std::pair<Message, int> KeyValuePair;

    // Initialize the list of the indices of current messages in the topic
    std::vector<int> curr_index(Topics.size(), 0);

    // Initialize the min heap using the first message of the topics
    std::priority_queue<KeyValuePair, std::vector<KeyValuePair>, std::greater<KeyValuePair> > min_heap;
    for (int i = 0; i < static_cast<int>(Topics.size()); ++i)
        if (!Topics[i].Messages.empty())
            min_heap.push(KeyValuePair(Topics[i].Messages[0], i));

    // Perform a process similar to merge sort of already sorted lists
    while (!min_heap.empty())
    {
        // Add the smallest message to the list
        int t_idx = min_heap.top().second;
        MessageIndexList.push_back(MessageIndex(t_idx, curr_index[t_idx]));
        
        // Remove the message from the heap
        min_heap.pop();

        // Add the new message from the topic
        ++curr_index[t_idx];
        if (curr_index[t_idx] < static_cast<int>(Topics[t_idx].Messages.size()))
            min_heap.push(KeyValuePair(Topics[t_idx].Messages[curr_index[t_idx]], t_idx));
    }
}

// Compare two message indices based on their actual message times, etc.
bool Sequence::CompareMessageIndices(MessageIndex msg1, MessageIndex msg2)
{
    return (Topics[msg1.TopicIdx].Messages[msg1.MessageIdx] < Topics[msg2.TopicIdx].Messages[msg2.MessageIdx]);
} 

}
#endif