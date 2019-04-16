/*  ***************************************************************************
*   topic.h - Header for working with the topics of ALFA dataset sequences.
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

#ifndef ALFA_TOPIC_H
#define ALFA_TOPIC_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <iomanip>
#include <map>
#include <algorithm>
#include "commons.h"
#include "message.h"

namespace alfa
{

// This class keeps the information of a single topic in a dataset sequence
class Topic
{
public:

    // Class Data Members
    std::string Name = "N/A";
    std::string FileName;
    VecString FieldLabels;
    std::vector<Message> Messages;

    // Constructors & Deconstructors
    Topic(const std::string &filename = "", const std::string &topic_name = "N/A");

    // Member Functions
    bool ReadFromFile(const std::string &filename);
    int Print(int n_start = 0, int n_messages = -1, const std::string &field_separator = " | ") const;
    int PrintHeader(const std::string &field_separator = " | ") const;
    bool IsInitialized() const;
    bool IsFaultTopic();
    bool HasHeaderField();
    int FindLabelIndex(const std::string &label);
    void Clear();

    std::vector<DateTime> GetTimes(int start_msg_index = 0, int n_messages = -1);
    std::vector<Message::HeaderType> GetHeaders(int start_msg_index = 0, int n_messages = -1);

    std::vector<std::string> GetFieldsAsString(const std::string &field_label, int start_msg_index = 0, int n_messages = -1);
    std::vector<std::string> GetFieldsAsString(int field_index, int start_msg_index = 0, int n_messages = -1);

    std::vector<int> GetFieldsAsInt(const std::string &field_label, int start_msg_index = 0, int n_messages = -1);
    std::vector<int> GetFieldsAsInt(int field_index, int start_msg_index = 0, int n_messages = -1);

    std::vector<long long> GetFieldsAsLongLong(const std::string &field_label, int start_msg_index = 0, int n_messages = -1);
    std::vector<long long> GetFieldsAsLongLong(int field_index, int start_msg_index = 0, int n_messages = -1);

    std::vector<double> GetFieldsAsDouble(const std::string &field_label, int start_msg_index = 0, int n_messages = -1);
    std::vector<double> GetFieldsAsDouble(int field_index, int start_msg_index = 0, int n_messages = -1);

    std::vector<long double> GetFieldsAsLongDouble(const std::string &field_label, int start_msg_index = 0, int n_messages = -1);
    std::vector<long double> GetFieldsAsLongDouble(int field_index, int start_msg_index = 0, int n_messages = -1);

private:
    // Member Functions
    Message TokensToMessage(const VecString &tokens);
    void ProcessHeader();

    // Data Members

    // Table of the message labels
    std::map<std::string, int> labels_map;

    // Is the topic initialized or not
    bool is_initialized = false;

    // Is the topic a fault topic
    bool is_fault_topic = false;

    // Maximum length of the data fields (for better printing)
    int len_seqid = 0, len_stamp = 0, len_frameid = 0;
    std::vector<int> len_fields;

    // Pre-processed field labels from the CSV file
    VecString orig_field_labels;

    // Header strings for printing
    const std::string hdr_ind = "Index", hdr_datetime = "Date/Time Stamp";
    const std::string hdr_seq = "SeqID", hdr_stamp = "Time Stamp", hdr_frid = "Frame";

    // Keep if the topic has header field
    bool has_header = false;
};

/******************************************************************************/
/************************** Function Definitions ******************************/
/******************************************************************************/

// Contructor function for Topic. Loads a CSV file containing an ALFA dataset topic.
Topic::Topic(const std::string &filename, const std::string &topic_name)
{
    // Assign the given topic name
    Name = topic_name;

    // Read the given CSV file
    if (!filename.empty())
        ReadFromFile(filename);
}

// Load a CSV file containing an ALFA dataset topic.
bool Topic::ReadFromFile(const std::string &filename)
{
    // Keep the topic name
    std::string topic_name = Name;

    // Clear the previous data from the object
    this->Clear();

    // Save the filename and topic name
    this->FileName = filename;
    this->Name = topic_name;

    // Open the CSV file
    std::ifstream ifs (filename);

    // Print an error if file did not open properly
    if (!ifs.is_open())
    {
        std::cerr << "Failed to open '" << filename << "' file." << std::endl;
        return false;
    }

    // Read the header line from the CSV file
    std::string line;
    if (std::getline(ifs, line))
        this->orig_field_labels = Commons::Tokenize(line, Commons::CSVDelimiter);
    else // Print an error if the file is not formatted properly
    {
        std::cerr << "Error reading the header from '" << filename << "' file." << std::endl;
        return false;
    }

    // Read the data from the CSV file
    int line_number = 0;
    while (std::getline(ifs, line))
    {
        line_number++;

        // Break the line to tokens
        auto tokens = Commons::Tokenize(line, Commons::CSVDelimiter);

        // Add empty tokens if the line did not include all the fields
        while (tokens.size() < this->orig_field_labels.size())
            tokens.push_back("");

        // Print an error and stop operation if file is not formatted properly
        if (tokens.size() > this->orig_field_labels.size())
        {
            std::cerr << "Error converting line #" << line_number << " of '" << filename << "'. Skipping this topic!" << std::endl;
            break;
        }

        // Convert the tokens to a message and add to our collection
        this->Messages.push_back(TokensToMessage(tokens));
    }

    // Postprocess the header labels
    ProcessHeader();

    // It is not a fault topic if the topic name is shorter than the fault prefix
    if (this->Name.length() >= Commons::FaultTopicPrefix.length()) 
        // Check if the prefix of topic name is the fault prefix
        is_fault_topic = (this->Name.substr(0, Commons::FaultTopicPrefix.length()) == Commons::FaultTopicPrefix);

    // Initialization done
    is_initialized = true;

    return IsInitialized();
}

// Print a specified number of messages. Also prints the header first. 
// Returns the number of messages printed.
int Topic::Print(int n_start, int n_messages, const std::string &field_separator) const
{
    // Return if the start index is negative
    if (n_start < 0) return 0;

    // If the number of messages is negative, print all the messages
    if (n_messages < 0)
        n_messages = Messages.size();

    // Print the header first. Puts separators between each two fields.
    int header_length = PrintHeader(field_separator);

    // Print a line to separate labels from the data
    std::cout << std::string(header_length, '-') << std::endl;


    // Print all the messages. Puts separators between each two fields.
    int printed_messages = 0;
    for (int i = n_start; (i < n_start + n_messages) && (i < (int)Messages.size()); ++i)
    {
        std::cout << field_separator << std::setw(hdr_ind.length()) << i << field_separator << 
            Messages[i].ToString(len_seqid, len_stamp, len_frameid, len_fields, has_header, field_separator) 
            << field_separator << std::endl;
        printed_messages++;
    }

    // Print the number of printed messages
    return printed_messages;
}

// Print the topic header (message field labels).
// Returns the length of the header line printed.
int Topic::PrintHeader(const std::string &field_separator) const
{
    // Ignore if there are no messages in the topic
    if (Messages.size() == 0) return 0;

    // Measure the length for the datetime string
    int len_datetime = Messages[0].DateTime.ToString().length();

    // Measure the total line length
    int total_len = hdr_ind.length() + len_datetime;
    if (has_header) total_len += len_seqid + len_stamp + len_frameid;
    for (int i = 0; i < (int)FieldLabels.size(); ++i)
        total_len += len_fields[i];
    total_len += (3 + FieldLabels.size()) * field_separator.length();
    if (has_header)
        total_len += 3 * field_separator.length();

    // Print the index, time and the Header object (if it has one)
    std::cout << field_separator << hdr_ind << field_separator << std::setw(len_datetime) << hdr_datetime;
    if (has_header)
        std::cout << field_separator << std::setw(len_seqid) << hdr_seq << field_separator <<
            std::setw(len_stamp) << hdr_stamp << field_separator << std::setw(len_frameid) << hdr_frid;

    // Print the rest of the field labels
    for (int i = 0; i < (int)FieldLabels.size(); ++i)
        std::cout << field_separator << std::setw(len_fields[i]) << FieldLabels[i];

    // Finish the line
    std::cout << field_separator << std::endl;

    // Return the line length
    return total_len;
}

// Returns the initialization status
bool Topic::IsInitialized() const
{
    return is_initialized;
}

// Returns true if the current topic is a fault topic
bool Topic::IsFaultTopic()
{
    return is_fault_topic;
}

bool Topic::HasHeaderField()
{
    return has_header;
}

// Clear the entire topic object
void Topic::Clear()
{
    Name = "";
    FileName = "";
    FieldLabels.clear();
    Messages.clear();
    is_initialized = false;
    is_fault_topic = false;
    len_seqid = 0; 
    len_stamp = 0;
    len_frameid = 0;
    len_fields.clear();
    orig_field_labels.clear();
    has_header = false;
    labels_map.clear();
}

// Find the index of a given field label (case sensitive)
int Topic::FindLabelIndex(const std::string &label)
{
    std::map<std::string, int>::iterator it = labels_map.find(label);

    // Return -1 if not found
    if (it == labels_map.end()) return -1;

    return it->second;        
}

// Retrieve the DateTime of a desired number of messages starting from the desired index
std::vector<DateTime> Topic::GetTimes(int start_msg_index, int n_messages)
{
    // Initialize the output
    std::vector<DateTime> vec_output;

    // Return if the start index is negative
    if (start_msg_index < 0) return vec_output;

    // If the number of messages is negative, use all the messages
    if (n_messages < 0)
        n_messages = Messages.size();

    // Add the datetimes to the output vector
    for (int i = start_msg_index; i < (int)Messages.size(); ++i)
        vec_output.push_back(Messages[i].DateTime);

    return vec_output;
}

// Retrieve the Header of a desired number of messages starting from the desired index
std::vector<Message::HeaderType> Topic::GetHeaders(int start_msg_index, int n_messages)
{
    // Initialize the output
    std::vector<Message::HeaderType> vec_output;

    // Return if the start index is negative
    if (start_msg_index < 0) return vec_output;

    // If the number of messages is negative, use all the messages
    if (n_messages < 0)
        n_messages = Messages.size();

    // Add the headers to the output vector
    for (int i = start_msg_index; i < (int)Messages.size(); ++i)
        vec_output.push_back(Messages[i].Header);

    return vec_output;
}

// Retrieve the fields of a desired number of messages starting from the desired index
std::vector<std::string> Topic::GetFieldsAsString(int field_index, int start_msg_index, int n_messages)
{
    // Initialize the output
    std::vector<std::string> vec_output;

    // Print error if the field index is negative
    if (field_index < 0)
    {
        std::cerr << "GetFieldsAsString Error! Field index is negative." << std::endl;
        return vec_output;
    }

    // Print error if the start index is negative
    if (start_msg_index < 0)
    {
        std::cerr << "GetFieldsAsString Error! Starting index is negative." << std::endl;
        return vec_output;
    }

    // If the number of messages is negative, use all the messages
    if (n_messages < 0)
        n_messages = Messages.size();

    // Add the fields to the output vector
    for (int i = start_msg_index; i < (int)Messages.size(); ++i)
        vec_output.push_back(Messages[i].Fields[field_index]);

    return vec_output;
}

// Retrieve the fields of a desired number of messages starting from the desired index
std::vector<std::string> Topic::GetFieldsAsString(const std::string &field_label, int start_msg_index, int n_messages)
{
    // Find the field index
    int field_index = FindLabelIndex(field_label);

    // Print error if the field name is not found
    if (field_index < 0)
    {
        std::cerr << "GetFieldsAsString Error! '" << field_label << "' field not found." << std::endl;
        return std::vector<std::string>();
    }

    // Return the desired output
    return GetFieldsAsString(field_index, start_msg_index, n_messages);
}

// Retrieve the fields of a desired number of messages starting from the desired index
std::vector<int> Topic::GetFieldsAsInt(int field_index, int start_msg_index, int n_messages)
{
    // Initialize the output
    std::vector<int> vec_output;

    // Print error if the field index is negative
    if (field_index < 0)
    {
        std::cerr << "GetFieldsAsInt Error! Field index is negative." << std::endl;
        return vec_output;
    }

    // Print error if the start index is negative
    if (start_msg_index < 0)
    {
        std::cerr << "GetFieldsAsInt Error! Starting index is negative." << std::endl;
        return vec_output;
    }

    // If the number of messages is negative, use all the messages
    if (n_messages < 0)
        n_messages = Messages.size();

    // Add the fields to the output vector
    for (int i = start_msg_index; i < (int)Messages.size(); ++i)
    {
        int temp = 0;
        Commons::StringToInt(Messages[i].Fields[field_index], temp);
        vec_output.push_back(temp);
    }

    return vec_output;
}

// Retrieve the fields of a desired number of messages starting from the desired index
std::vector<int> Topic::GetFieldsAsInt(const std::string &field_label, int start_msg_index, int n_messages)
{
    // Find the field index
    int field_index = FindLabelIndex(field_label);

    // Print error if the field name is not found
    if (field_index < 0)
    {
        std::cerr << "GetFieldsAsInt Error! '" << field_label << "' field not found." << std::endl;
        return std::vector<int>();
    }

    // Return the desired output
    return GetFieldsAsInt(field_index, start_msg_index, n_messages);
}

// Retrieve the fields of a desired number of messages starting from the desired index
std::vector<long long> Topic::GetFieldsAsLongLong(int field_index, int start_msg_index, int n_messages)
{
    // Initialize the output
    std::vector<long long> vec_output;

    // Print error if the field index is negative
    if (field_index < 0)
    {
        std::cerr << "GetFieldsAsLongLong Error! Field index is negative." << std::endl;
        return vec_output;
    }

    // Print error if the start index is negative
    if (start_msg_index < 0)
    {
        std::cerr << "GetFieldsAsLongLong Error! Starting index is negative." << std::endl;
        return vec_output;
    }

    // If the number of messages is negative, use all the messages
    if (n_messages < 0)
        n_messages = Messages.size();

    // Add the fields to the output vector
    for (int i = start_msg_index; i < (int)Messages.size(); ++i)
    {
        long long temp = 0;
        Commons::StringToLongLong(Messages[i].Fields[field_index], temp);
        vec_output.push_back(temp);
    }

    return vec_output;
}

// Retrieve the fields of a desired number of messages starting from the desired index
std::vector<long long> Topic::GetFieldsAsLongLong(const std::string &field_label, int start_msg_index, int n_messages)
{
    // Find the field index
    int field_index = FindLabelIndex(field_label);

    // Print error if the field name is not found
    if (field_index < 0)
    {
        std::cerr << "GetFieldsAsLongLong Error! '" << field_label << "' field not found." << std::endl;
        return std::vector<long long>();
    }

    // Return the desired output
    return GetFieldsAsLongLong(field_index, start_msg_index, n_messages);
}

// Retrieve the fields of a desired number of messages starting from the desired index
std::vector<double> Topic::GetFieldsAsDouble(int field_index, int start_msg_index, int n_messages)
{
    // Initialize the output
    std::vector<double> vec_output;

    // Print error if the field index is negative
    if (field_index < 0)
    {
        std::cerr << "GetFieldsAsDouble Error! Field index is negative." << std::endl;
        return vec_output;
    }

    // Print error if the start index is negative
    if (start_msg_index < 0)
    {
        std::cerr << "GetFieldsAsDouble Error! Starting index is negative." << std::endl;
        return vec_output;
    }

    // If the number of messages is negative, use all the messages
    if (n_messages < 0)
        n_messages = Messages.size();

    // Add the fields to the output vector
    for (int i = start_msg_index; i < (int)Messages.size(); ++i)
    {
        double temp = 0;
        Commons::StringToDouble(Messages[i].Fields[field_index], temp);
        vec_output.push_back(temp);
    }

    return vec_output;
}

// Retrieve the fields of a desired number of messages starting from the desired index
std::vector<double> Topic::GetFieldsAsDouble(const std::string &field_label, int start_msg_index, int n_messages)
{
    // Find the field index
    int field_index = FindLabelIndex(field_label);

    // Print error if the field name is not found
    if (field_index < 0)
    {
        std::cerr << "GetFieldsAsLongDouble Error! '" << field_label << "' field not found." << std::endl;
        return std::vector<double>();
    }

    // Return the desired output
    return GetFieldsAsDouble(field_index, start_msg_index, n_messages);
}

// Retrieve the fields of a desired number of messages starting from the desired index
std::vector<long double> Topic::GetFieldsAsLongDouble(int field_index, int start_msg_index, int n_messages)
{
    // Initialize the output
    std::vector<long double> vec_output;

    // Print error if the field index is negative
    if (field_index < 0)
    {
        std::cerr << "GetFieldsAsLongDouble Error! Field index is negative." << std::endl;
        return vec_output;
    }

    // Print error if the start index is negative
    if (start_msg_index < 0)
    {
        std::cerr << "GetFieldsAsLongDouble Error! Starting index is negative." << std::endl;
        return vec_output;
    }

    // If the number of messages is negative, use all the messages
    if (n_messages < 0)
        n_messages = Messages.size();

    // Add the fields to the output vector
    for (int i = start_msg_index; i < (int)Messages.size(); ++i)
    {
        long double temp = 0;
        Commons::StringToLongDouble(Messages[i].Fields[field_index], temp);
        vec_output.push_back(temp);
    }

    return vec_output;
}

// Retrieve the fields of a desired number of messages starting from the desired index
std::vector<long double> Topic::GetFieldsAsLongDouble(const std::string &field_label, int start_msg_index, int n_messages)
{
    // Find the field index
    int field_index = FindLabelIndex(field_label);

    // Print error if the field name is not found
    if (field_index < 0)
    {
        std::cerr << "GetFieldsAsLongDouble Error! '" << field_label << "' field not found." << std::endl;
        return std::vector<long double>();
    }

    // Return the desired output
    return GetFieldsAsLongDouble(field_index, start_msg_index, n_messages);
}

/******************************************************************************/
/*********************** Local Function Definitions ***************************/
/******************************************************************************/

// Convert a vector of tokens to a message
Message Topic::TokensToMessage(const VecString &tokens)
{
    // Define temporary variables for header field lengths
    int l_seq = 0, l_stamp = 0, l_frid = 0;
    std::vector<int> l_fields;

    // Convert the tokens to a message
    Message msg = Message::TokensToMessage(tokens, orig_field_labels, l_seq, l_stamp, l_frid, l_fields);

    // Update the field lengths in the messages
    len_seqid = std::max(len_seqid, l_seq);
    len_stamp = std::max(len_stamp, l_stamp);
    len_frameid = std::max(len_frameid, l_frid);
    for (int i = 0; i < (int)l_fields.size(); ++i)
    {
        if (i == len_fields.size())
            len_fields.push_back(l_fields[i]);
        else
            len_fields[i] = std::max(len_fields[i], l_fields[i]);
    }

    // Return the new message
    return msg;
}

// Postprocess the header of the CSV file (remove time, etc. from labels).
void Topic::ProcessHeader()
{
    // Iterate through all the column labels read from file
    for (int i = 0; i < (int)orig_field_labels.size(); ++i)
    {
        // Ignore if it is for the timestamp
        if (orig_field_labels[i].compare("%time") == 0) continue;

        // Ignore if it is for the header fields, also remember we have a header
        if (orig_field_labels[i].compare(Commons::CSVFieldsPrefix + "header.seq") == 0
            || orig_field_labels[i].compare(Commons::CSVFieldsPrefix + "header.stamp") == 0
            || orig_field_labels[i].compare(Commons::CSVFieldsPrefix + "header.frame_id") == 0)
        {
            has_header = true;
            continue;
        }


        std::string new_field_label = orig_field_labels[i];
        // Remove the starting prefix if the label starts with it
        if (orig_field_labels[i].substr(0, Commons::CSVFieldsPrefix.size()) == Commons::CSVFieldsPrefix)
            new_field_label = orig_field_labels[i].substr(Commons::CSVFieldsPrefix.size());

        // Add the label to the label table and the vector
        FieldLabels.push_back(new_field_label);
        this->labels_map.insert(std::make_pair(new_field_label, FieldLabels.size() - 1));
    }

    // Update the minimum spaces needed for printing each field
    len_seqid = std::max(len_seqid, (int)hdr_seq.length());
    len_stamp = std::max(len_stamp, (int)hdr_stamp.length());
    len_frameid = std::max(len_frameid, (int)hdr_frid.length());
    for (int i = 0; i < (int)FieldLabels.size(); ++i)
    {
        if ((int)len_fields.size() < (i + 1)) break;
        len_fields[i] = std::max(len_fields[i], (int)FieldLabels[i].length());
    }
}

}
#endif