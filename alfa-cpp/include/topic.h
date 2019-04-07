/*  ***************************************************************************
*   topic.h - Header for working with topics of the ALFA dataset sequences.
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
#include "commons.h"

namespace alfa
{

class Topic2
{
public:
    // Structs, subclasses and type definitions
    typedef std::vector<std::string> VecString;

    struct HeaderType
    {
        int SequenceID;
        struct StampType { int Secs; int NanoSecs; } TimeStamp;
        std::string FrameID;
    };

    class Item
    {
    public:
        Commons::DateTime DateTime;
        HeaderType Header;
        VecString DataFields;
        friend std::ostream& operator<< (std::ostream& os, const Item& it)
        {
            os << it.ToString();
            return os;
        }

        std::string ToString(std::string separator = " | ") const
        {
            return ToString(5, 10, 9, 0, std::vector<int>(DataFields.size()));
        }

        std::string ToString(int l_seq, int l_secs, int l_nsecs, int l_frid, std::vector<int> l_fields, std::string separator = " | ") const
        {
            std::ostringstream oss;
            oss << DateTime << separator << 
                std::setw(l_seq) << Header.SequenceID << separator <<
                std::setw(l_secs) << Header.TimeStamp.Secs << separator << 
                std::setw(l_nsecs) << Header.TimeStamp.NanoSecs << separator << 
                std::setw(l_frid) << Header.FrameID;

            for (int i = 0; i < DataFields.size(); ++i)
                oss << separator << std::setw(l_fields[i]) << DataFields[i];
            
            return oss.str();
        }

        bool operator< (const Item &it) const
        {
            return (this->DateTime < it.DateTime);
        }

        Item& operator= (const Item &it)
        {
            // TODO
            return *this;
        }
    };

    // Class Data Members
    std::string Name;
    std::string FileName;
    VecString FieldLabels;
    VecString FieldDataTypes;
    std::vector<Item> DataItems;

    // Constructors & Deconstructors
    Topic2(std::string filename = "");

    // Member Functions
    bool ReadFromFile(std::string filename);
    void Print(int n_start = 0, int n_items = -1, std::string field_separator = " | ");
    bool IsInitialized();
    void Clear();

private:
    // Data Members
    bool is_initialized = false;

    // Member Functions
    Item TokensToItem(const VecString &tokens);

    // Maximum length of the data fields (for better printing)
    int len_seqid = 0, len_secs = 0, len_nsecs = 0, len_frameid = 0;
    std::vector<int> len_fields;
};



Topic2::Topic2(std::string filename)
{
    if (filename.compare("") != 0)
        ReadFromFile(filename);
}

bool Topic2::ReadFromFile(std::string filename)
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
        this->FieldLabels = Commons::Tokenize(line, Commons::CSVDelimiter);
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
        if (tokens.size() != this->FieldLabels.size())
        {
            std::cerr << "Error converting line #" << line_number << " of '" << filename << "'." << std::endl;
            continue;
        }
        this->DataItems.push_back(TokensToItem(tokens));
    }

    return IsInitialized();
}

void Topic2::Print(int n_start, int n_items, std::string field_separator)
{
    if (n_start < 0) return;

    if (n_items < 0) 
        n_items = DataItems.size();

    std::cout << "Index" << field_separator << std::endl;

    for (int i = n_start; (i < n_start + n_items) && (i < DataItems.size()); ++i)
        std::cout << std::setw(5) << i << field_separator << 
            DataItems[i].ToString(len_seqid, len_secs, len_nsecs, len_frameid, len_fields, field_separator) 
            << std::endl;
}

Topic2::Item Topic2::TokensToItem(const VecString &tokens)
{
    Item item;
    ".header.seq,.header.stamp.secs,.header.stamp.nsecs,.header.frame_id";
    for (int i = 0; i < FieldLabels.size(); ++i)
    {
        if (FieldLabels[i].compare("time") == 0)
            item.DateTime = Commons::DateTime::StringToTime(tokens[i], Commons::CSVDateTimeFormat);
        else if (FieldLabels[i].compare(".header.seq") == 0)
        {
            Commons::StringToInt(tokens[i], item.Header.SequenceID);
            if (tokens[i].length() > len_seqid) len_seqid = tokens[i].length();
        }
        else if (FieldLabels[i].compare(".header.stamp.secs") == 0)
        {
            Commons::StringToInt(tokens[i], item.Header.TimeStamp.Secs);
            if (tokens[i].length() > len_secs) len_secs = tokens[i].length();
        }
        else if (FieldLabels[i].compare(".header.stamp.nsecs") == 0)
        {
            Commons::StringToInt(tokens[i], item.Header.TimeStamp.NanoSecs);
            if (tokens[i].length() > len_nsecs) len_nsecs = tokens[i].length();
        }
        else if (FieldLabels[i].compare(".header.frame_id") == 0)
        {
            item.Header.FrameID = tokens[i];
            if (tokens[i].length() > len_frameid) len_frameid = tokens[i].length();
        }
        else
        {
            item.DataFields.push_back(tokens[i]);
            if (len_fields.size() < item.DataFields.size()) 
                len_fields.push_back(0);
            if (tokens[i].length() > len_fields[item.DataFields.size() - 1]) 
                len_fields[item.DataFields.size() - 1] = tokens[i].length();
        }
    }
    return item;
}

bool Topic2::IsInitialized()
{
    return is_initialized;
}

}
#endif