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
*   Last Modified: April 01, 2019
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
            os << it.DateTime << " | " << std::setw(5) << it.Header.SequenceID << " | " <<
                std::setw(10) << it.Header.TimeStamp.Secs << " | " << std::setw(9) << 
                it.Header.TimeStamp.NanoSecs << std::setw(0) << " | " << it.Header.FrameID;

            for (int i = 0; i < it.DataFields.size(); ++i)
                os << " | " << it.DataFields[i];
            return os;
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
    int NumOfFields = 0;
    std::vector<Item> DataItems;

    // Constructors & Deconstructors
    Topic2(std::string filename = "");

    // Member Functions
    bool ReadFromFile(std::string filename);
    bool IsInitialized();
    void Clear();

private:
    // Data Members
    bool is_initialized = false;

    // Member Functions
    Item TokensToItem(const VecString &tokens);

    // Maximum length of the data fields (for better printing)
    int len_seqid = 0, len_secs = 0, len_nsecs = 0, len_frameid;
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
            Commons::StringToInt(tokens[i], item.Header.TimeStamp.Secs);
        else if (FieldLabels[i].compare(".header.stamp.nsecs") == 0)
            Commons::StringToInt(tokens[i], item.Header.TimeStamp.NanoSecs);
        else if (FieldLabels[i].compare(".header.frame_id") == 0)
            item.Header.FrameID = tokens[i];
        else
            item.DataFields.push_back(tokens[i]);        
    }
    return item;
}

bool Topic2::IsInitialized()
{
    return is_initialized;
}

}
#endif