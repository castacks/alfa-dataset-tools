/*  ***************************************************************************
*   data_item.h - Header for working with data items in ALFA dataset topics.
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

#ifndef ALFA_DATAITEM_H
#define ALFA_DATAITEM_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "commons.h"

namespace alfa
{

// This class keeps the information of a single message (data item) in a topic
class DataItem
{
public:

    // Local struct definitions
    struct HeaderType           // Structure for the message headers
    {
        int SequenceID = -1;
        struct StampType { int Secs = -1; int NanoSecs = -1; } TimeStamp;
        std::string FrameID = "N/A";
    };
    
    // Data Members
    alfa::DateTime DateTime;    // Recorded Timestamp
    HeaderType Header;          // Message header
    VecString Fields;           // Message fields

    // Member Functions
    std::string ToString(std::string separator = " | ") const;
    std::string ToString(int l_seq, int l_secs, int l_nsecs, int l_frid, std::vector<int> l_fields, std::string separator = " | ") const;
    bool operator< (const DataItem &it) const;
    static DataItem TokensToItem(const VecString &tokens, const VecString &field_labels);
    static DataItem TokensToItem(const VecString &tokens, const VecString &field_labels, int &out_len_seqid,
            int &out_len_secs, int &out_len_nsecs, int &out_len_frameid, std::vector<int> &out_len_fields);
};

// Overload the << operator for DataItem
std::ostream& operator<< (std::ostream& os, const DataItem& it)
{
    // Write to the stream using the default values for field sizes
    os << it.ToString();
    return os;
}

/******************************************************************************/
/************************** Function Definitions ******************************/
/******************************************************************************/

// Convert DataItem to string, using the default values for fiels sizes
std::string DataItem::ToString(std::string separator) const
{
    return ToString(5, 10, 9, 0, std::vector<int>(Fields.size()));
}

// Convert DataItem to string, given the minimum spacing for each field member
std::string DataItem::ToString(int l_seq, int l_secs, int l_nsecs, int l_frid, 
        std::vector<int> l_fields, std::string separator) const
{
    // Create an output string stream
    std::ostringstream oss;

    // Write the time and the header in the string stream
    oss << DateTime << separator << 
        std::setw(l_seq) << Header.SequenceID << separator <<
        std::setw(l_secs) << Header.TimeStamp.Secs << separator << 
        std::setw(l_nsecs) << Header.TimeStamp.NanoSecs << separator << 
        std::setw(l_frid) << Header.FrameID;

    // Write the fields in the string stream
    for (int i = 0; i < Fields.size(); ++i)
        oss << separator << std::setw(l_fields[i]) << Fields[i];
    
    // Convert the string stream to a string and return
    return oss.str();
}

// Overload the < operator for DataItem
bool DataItem::operator< (const DataItem &it) const
{
    // Compare only using the recorded time of the items
    return (this->DateTime < it.DateTime);
}

// Convert a token collection to DataItem object
DataItem DataItem::TokensToItem(const VecString &tokens, const VecString &field_labels)
{
    int len_seqid = 0, len_secs = 0, len_nsecs = 0, len_frameid = 0;
    std::vector<int> len_fields;
    return TokensToItem(tokens, field_labels, len_seqid, len_secs, len_nsecs, len_frameid, len_fields);
}

// Convert a token collection to DataItem object and output the string sizes of the fields
DataItem DataItem::TokensToItem(const VecString &tokens, const VecString &field_labels, int &out_len_seqid, 
            int &out_len_secs, int &out_len_nsecs, int &out_len_frameid, std::vector<int> &out_len_fields)
{
    DataItem item;
    out_len_seqid = 0; out_len_secs = 0; out_len_nsecs = 0; out_len_frameid = 0;
    out_len_fields.clear();

    // Check the type of the current token (time, header, etc.)
    for (int i = 0; i < field_labels.size(); ++i)
    {
        if (field_labels[i].compare("time") == 0)                       // If it is timestamp
            item.DateTime = DateTime::StringToTime(tokens[i], Commons::CSVDateTimeFormat);
        else if (field_labels[i].compare(".header.seq") == 0)           // If it is sequence id
        {
            Commons::StringToInt(tokens[i], item.Header.SequenceID);
            out_len_seqid = tokens[i].length();
        }
        else if (field_labels[i].compare(".header.stamp.secs") == 0)    // If it is header seconds
        {
            Commons::StringToInt(tokens[i], item.Header.TimeStamp.Secs);
            out_len_secs = tokens[i].length();
        }
        else if (field_labels[i].compare(".header.stamp.nsecs") == 0)   // If it is header nanoseconds
        {
            Commons::StringToInt(tokens[i], item.Header.TimeStamp.NanoSecs);
            out_len_nsecs = tokens[i].length();
        }
        else if (field_labels[i].compare(".header.frame_id") == 0)      // If it is frame id
        {
            item.Header.FrameID = tokens[i];
            out_len_frameid = tokens[i].length();
        }
        else                                                            // If it is any other field
        {
            item.Fields.push_back(tokens[i]);
            out_len_fields.push_back(tokens[i].length());
        }
    }
    return item;
}

}
#endif