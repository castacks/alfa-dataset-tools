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
#include <ctime>
#include <algorithm>
#include "commons.h"
#include "data_item.h"

namespace alfa
{

class Topic
{
public:

    // Class Data Members
    std::string Name;
    std::string FileName;
    VecString FieldLabel;
    std::vector<DataItem> DataItems;

    // Constructors & Deconstructors
    Topic(std::string filename = "");

    // Member Functions
    bool ReadFromFile(std::string filename);
    void Print(int n_start = 0, int n_items = -1, std::string field_separator = " | ");
    void PrintHeader(std::string field_separator = " | ");
    bool IsInitialized();
    void Clear();

private:
    // Data Members
    bool is_initialized = false;

    // Member Functions
    DataItem TokensToItem(const VecString &tokens);
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

Topic::Topic(std::string filename)
{
    if (filename.compare("") != 0)
        ReadFromFile(filename);
}

bool Topic::ReadFromFile(std::string filename)
{
    this->FileName = filename;

    // Clear the previous data from the object
    //this->Clear();

    // Open the file
    std::ifstream ifs (filename);
    if (!ifs.is_open()) 
    {
        std::cerr << "Failed to open '" << filename << "' file." << std::endl;
        return false;
    }

    // Read the header line from the CSV file
    std::string line;
    if (std::getline(ifs, line))
    {
        this->orig_field_labels = Commons::Tokenize(line, Commons::CSVDelimiter);
    }
    else
    {
        std::cerr << "Error reading the header from '" << filename << "' file." << std::endl;
        return false;
    }

    // Read the data from the CSV file
    int line_number = 0;
    while (std::getline(ifs, line))
    {
        line_number++;
        auto tokens = Commons::Tokenize(line, Commons::CSVDelimiter);
        if (tokens.size() != this->orig_field_labels.size())
        {
            std::cerr << "Error converting line #" << line_number << " of '" << filename << "'." << std::endl;
            continue;
        }
        this->DataItems.push_back(TokensToItem(tokens));
    }

    // Postprocess the header labels
    ProcessHeader();

    // Initialization done
    is_initialized = true;

    return IsInitialized();
}

void Topic::Print(int n_start, int n_items, std::string field_separator)
{
    if (n_start < 0) return;

    if (n_items < 0) 
        n_items = DataItems.size();

    PrintHeader(field_separator);

    for (int i = n_start; (i < n_start + n_items) && (i < DataItems.size()); ++i)
        std::cout << field_separator << std::setw(hdr_ind.length()) << i << field_separator << 
            DataItems[i].ToString(len_seqid, len_secs, len_nsecs, len_frameid, len_fields, field_separator) 
            << field_separator << std::endl;
}

void Topic::PrintHeader(std::string field_separator)
{
    // Ignore if there are no data items
    if (DataItems.size() == 0) return;

    // Measure the length for the datetime string
    int len_datetime = DataItems[0].DateTime.ToString().length();

    // Measure the total line length
    int total_len = hdr_ind.length() + len_datetime + len_seqid + len_secs + len_nsecs + len_frameid;
    for (int i = 0; i < FieldLabel.size(); ++i)
        total_len += len_fields[i];
    total_len += (7 + FieldLabel.size()) * field_separator.length();

    // Print the index, time and the Header object
    std::cout << field_separator << hdr_ind << field_separator << std::setw(len_datetime) << hdr_datetime <<
            field_separator << std::setw(len_seqid) << hdr_seq << field_separator <<
            std::setw(len_secs) << hdr_secs << field_separator << std::setw(len_nsecs) << 
            hdr_nsecs << field_separator << std::setw(len_frameid) << hdr_frid;

    // Print the rest of the field labels
    for (int i = 0; i < FieldLabel.size(); ++i)
        std::cout << field_separator << std::setw(len_fields[i]) << FieldLabel[i];

    // Finish the line
    std::cout << field_separator << std::endl;

    // Print a line to separate labels from the data
    for (int i = 0; i < total_len; ++i)
        std::cout << '-';
    std::cout << std::endl;
}

bool Topic::IsInitialized()
{
    return is_initialized;
}

DataItem Topic::TokensToItem(const VecString &tokens)
{
    int l_seq = 0, l_secs = 0, l_nsecs = 0, l_frid = 0;
    std::vector<int> l_fields;

    // Convert the tokens to a data item
    DataItem item = DataItem::TokensToItem(tokens, orig_field_labels, l_seq, l_secs, l_nsecs, l_frid, l_fields);
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

    return item;
}

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
            FieldLabel.push_back(orig_field_labels[i].substr(1));
        else
            FieldLabel.push_back(orig_field_labels[i]);
    }

    // Update the minimum spaces needed for printing each field
    len_seqid = std::max(len_seqid, (int)hdr_seq.length());
    len_secs = std::max(len_secs, (int)hdr_secs.length());
    len_nsecs = std::max(len_nsecs, (int)hdr_nsecs.length());
    len_frameid = std::max(len_frameid, (int)hdr_frid.length());
    for (int i = 0; i < FieldLabel.size(); ++i)
        len_fields[i] = std::max(len_fields[i], (int)FieldLabel[i].length());
}

}
#endif