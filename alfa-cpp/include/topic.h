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
*   Last Modified: April 07, 2019
*   ***************************************************************************/

#ifndef ALFA_TOPIC_H
#define ALFA_TOPIC_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <iomanip>
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
    Topic(std::string filename = "", std::string topic_name = "N/A");

    // Member Functions
    bool ReadFromFile(std::string filename);
    int Print(int n_start = 0, int n_messages = -1, std::string field_separator = " | ");
    int PrintHeader(std::string field_separator = " | ");
    bool IsInitialized();
    void Clear();

private:
    // Data Members
    bool is_initialized = false;

    // Member Functions
    Message TokensToMessage(const VecString &tokens);
    void ProcessHeader();

    // Maximum length of the data fields (for better printing)
    int len_seqid = 0, len_secs = 0, len_nsecs = 0, len_frameid = 0;
    std::vector<int> len_fields;

    // Pre-processed field labels from the CSV file
    VecString orig_field_labels;

    // Header strings for printing
    const std::string hdr_ind = "Index", hdr_datetime = "Date/Time Stamp";
    const std::string hdr_seq = "SeqID", hdr_secs = "Secs", hdr_nsecs = "NanoSecs", hdr_frid = "Frame";
};

/******************************************************************************/
/************************** Function Definitions ******************************/
/******************************************************************************/

// Contructor function for Topic. Loads a CSV file containing an ALFA dataset topic.
Topic::Topic(std::string filename, std::string topic_name)
{
    // Assign the given topic name
    Name = topic_name;

    // Read the given CSV file
    if (filename.compare("") != 0)
        ReadFromFile(filename);
}

// Load a CSV file containing an ALFA dataset topic.
bool Topic::ReadFromFile(std::string filename)
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

        // Print an error if file is not formatted properly
        if (tokens.size() != this->orig_field_labels.size())
        {
            std::cerr << "Error converting line #" << line_number << " of '" << filename << "'." << std::endl;
            continue;
        }

        // Convert the tokens to a message and add to our collection
        this->Messages.push_back(TokensToMessage(tokens));
    }

    // Postprocess the header labels
    ProcessHeader();

    // Initialization done
    is_initialized = true;

    return IsInitialized();
}

// Print a specified number of messages. Also prints the header first. 
// Returns the number of messages printed.
int Topic::Print(int n_start, int n_messages, std::string field_separator)
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
    for (int i = n_start; (i < n_start + n_messages) && (i < Messages.size()); ++i)
    {
        std::cout << field_separator << std::setw(hdr_ind.length()) << i << field_separator << 
            Messages[i].ToString(len_seqid, len_secs, len_nsecs, len_frameid, len_fields, field_separator) 
            << field_separator << std::endl;
        printed_messages++;
    }

    // Print the number of printed messages
    return printed_messages;
}

// Print the topic header (message field labels).
// Returns the length of the header line printed.
int Topic::PrintHeader(std::string field_separator)
{
    // Ignore if there are no messages in the topic
    if (Messages.size() == 0) return 0;

    // Measure the length for the datetime string
    int len_datetime = Messages[0].DateTime.ToString().length();

    // Measure the total line length
    int total_len = hdr_ind.length() + len_datetime + len_seqid + len_secs + len_nsecs + len_frameid;
    for (int i = 0; i < FieldLabels.size(); ++i)
        total_len += len_fields[i];
    total_len += (7 + FieldLabels.size()) * field_separator.length();

    // Print the index, time and the Header object
    std::cout << field_separator << hdr_ind << field_separator << std::setw(len_datetime) << hdr_datetime <<
            field_separator << std::setw(len_seqid) << hdr_seq << field_separator <<
            std::setw(len_secs) << hdr_secs << field_separator << std::setw(len_nsecs) << 
            hdr_nsecs << field_separator << std::setw(len_frameid) << hdr_frid;

    // Print the rest of the field labels
    for (int i = 0; i < FieldLabels.size(); ++i)
        std::cout << field_separator << std::setw(len_fields[i]) << FieldLabels[i];

    // Finish the line
    std::cout << field_separator << std::endl;

    // Return the line length
    return total_len;
}

// Returns the initialization status
bool Topic::IsInitialized()
{
    return is_initialized;
}

// Clear the entire topic object
void Topic::Clear()
{
    Name = "";
    FileName = "";
    FieldLabels.clear();
    Messages.clear();
    is_initialized = false;
    len_seqid = 0; 
    len_secs = 0;
    len_nsecs = 0;
    len_frameid = 0;
    len_fields.clear();
    orig_field_labels.clear();
}

// Convert a vector of tokens to a message
Message Topic::TokensToMessage(const VecString &tokens)
{
    // Define temporary variables for header field lengths
    int l_seq = 0, l_secs = 0, l_nsecs = 0, l_frid = 0;
    std::vector<int> l_fields;

    // Convert the tokens to a message
    Message msg = Message::TokensToMessage(tokens, orig_field_labels, l_seq, l_secs, l_nsecs, l_frid, l_fields);

    // Update the field lengths in the messages
    len_seqid = std::max(len_seqid, l_seq);
    len_secs = std::max(len_secs, l_secs);
    len_nsecs = std::max(len_nsecs, l_nsecs);
    len_frameid = std::max(len_frameid, l_frid);
    for (int i = 0; i < l_fields.size(); ++i)
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
    for (int i = 0; i < orig_field_labels.size(); ++i)
    {
        // Ignore if it is for the timestamp of the header class
        if (orig_field_labels[i].compare("time") == 0
            || orig_field_labels[i].compare(".header.seq") == 0
            || orig_field_labels[i].compare(".header.stamp.secs") == 0
            || orig_field_labels[i].compare(".header.stamp.nsecs") == 0
            || orig_field_labels[i].compare(".header.frame_id") == 0)
            continue;

        // Remove the starting '.' if the label starts with it
        if (orig_field_labels[i][0] == '.')
            FieldLabels.push_back(orig_field_labels[i].substr(1));
        else
            FieldLabels.push_back(orig_field_labels[i]);
    }

    // Update the minimum spaces needed for printing each field
    len_seqid = std::max(len_seqid, (int)hdr_seq.length());
    len_secs = std::max(len_secs, (int)hdr_secs.length());
    len_nsecs = std::max(len_nsecs, (int)hdr_nsecs.length());
    len_frameid = std::max(len_frameid, (int)hdr_frid.length());
    for (int i = 0; i < FieldLabels.size(); ++i)
        len_fields[i] = std::max(len_fields[i], (int)FieldLabels[i].length());
}

}
#endif