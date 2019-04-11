/*  ***************************************************************************
*   message.h - Header for working with data items (messages) in ALFA topics.
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

#ifndef ALFA_MESSAGE_H
#define ALFA_MESSAGE_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "commons.h"

namespace alfa
{

// This class keeps the information of a single message (data item) in a topic
class Message
{
public:

    // Local struct definitions
    struct HeaderType           // Structure for the message headers
    {
        int SequenceID = -1;
        long long int Stamp;
        std::string FrameID = "N/A";
    };
    
    // Data Members
    alfa::DateTime DateTime;    // Recorded Timestamp
    HeaderType Header;          // Message Header
    VecString Fields;           // Message Fields

    // Member Functions
    std::string ToString(bool has_header = true, std::string separator = " | ") const;
    std::string ToString(int l_seq, int l_stamp, int l_frid, std::vector<int> l_fields, bool has_header = true, std::string separator = " | ") const;
    bool operator< (const Message &msg) const;
    bool operator> (const Message &msg) const;
    bool operator== (const Message &msg) const;
    bool operator!= (const Message &msg) const;
    static Message TokensToMessage(const VecString &tokens, const VecString &field_labels);
    static Message TokensToMessage(const VecString &tokens, const VecString &field_labels, int &out_len_seqid,
            int &out_len_stamp, int &out_len_frameid, std::vector<int> &out_len_fields);
};

// Overload the << operator for Message
std::ostream& operator<< (std::ostream& os, const Message& msg)
{
    // Write to the stream using the default values for field sizes
    os << msg.ToString();
    return os;
}

/******************************************************************************/
/************************** Function Definitions ******************************/
/******************************************************************************/

// Convert Message to string, using the default values for fiels sizes
std::string Message::ToString(bool has_header, std::string separator) const
{
    return ToString(5, 10, 0, std::vector<int>(Fields.size()), has_header, separator);
}

// Convert Message to string, given the minimum spacing for each field member
std::string Message::ToString(int l_seq, int l_stamp, int l_frid, 
        std::vector<int> l_fields, bool has_header, std::string separator) const
{
    // Create an output string stream
    std::ostringstream oss;

    // Write the time and the header (if it has one) in the string stream
    oss << DateTime;
    if (has_header)
        oss << separator << std::setw(l_seq) << Header.SequenceID << separator <<
            std::setw(l_stamp) << Header.Stamp << separator << 
            std::setw(l_frid) << Header.FrameID;

    // Write the fields in the string stream
    for (int i = 0; i < (int)Fields.size(); ++i)
        oss << separator << std::setw(l_fields[i]) << Fields[i];
    
    // Convert the string stream to a string and return
    return oss.str();
}

// Overload the < operator for Message
bool Message::operator< (const Message &msg) const
{
    // Compare the recorded time of the messages
    if (this->DateTime < msg.DateTime) return true;
    if (this->DateTime > msg.DateTime) return false;
    
    // Compare the sequence id of the messages
    if (this->Header.SequenceID < msg.Header.SequenceID) return true;
    if (this->Header.SequenceID > msg.Header.SequenceID) return false;

    // Compare the nanoseconds of the header time stamp
    if (this->Header.Stamp < msg.Header.Stamp) return true;
    if (this->Header.Stamp > msg.Header.Stamp) return false;

    // Compare the other fields
    for (int i = 0; i < (int)std::min(this->Fields.size(), msg.Fields.size()); ++i)
    {
        if (this->Fields[i] < msg.Fields[i]) return true;
        if (this->Fields[i] > msg.Fields[i]) return false;
    }

    // See if they have the same number of fields
    if (this->Fields.size() < msg.Fields.size())
        return true;
    
    return false;
}

// Overload the > operator for Message
bool Message::operator> (const Message &msg) const
{
    return (msg < *this);
}

// Overload the == operator for Message
bool Message::operator== (const Message &msg) const
{
    if (this->DateTime != msg.DateTime) return false;
    if (this->Header.SequenceID != msg.Header.SequenceID) return false;
    if (this->Header.Stamp != msg.Header.Stamp) return false;

    // See if they have the same number of fields
    if (this->Fields.size() != msg.Fields.size()) return false;

    // Compare the fields
    for (int i = 0; i < (int)this->Fields.size(); ++i)
        if (this->Fields[i] != msg.Fields[i]) return false;

    return true;
}

// Overload the != operator for Message
bool Message::operator!= (const Message &msg) const
{
    return !(msg == *this);
}

// Convert a token collection to Message object
Message Message::TokensToMessage(const VecString &tokens, const VecString &field_labels)
{
    int len_seqid = 0, len_stamp = 0, len_frameid = 0;
    std::vector<int> len_fields;
    return TokensToMessage(tokens, field_labels, len_seqid, len_stamp, len_frameid, len_fields);
}

// Convert a token collection to Message object and output the string sizes of the fields
Message Message::TokensToMessage(const VecString &tokens, const VecString &field_labels, int &out_len_seqid, 
            int &out_len_stamp, int &out_len_frameid, std::vector<int> &out_len_fields)
{
    Message msg;
    out_len_seqid = 0; out_len_stamp = 0; out_len_frameid = 0;
    out_len_fields.clear();

    // Check the type of the current token (time, header, etc.)
    for (int i = 0; i < (int)field_labels.size(); ++i)
    {
        if (field_labels[i].compare("%time") == 0)                                              // If it is timestamp
            msg.DateTime = DateTime::EpochStringToTime(tokens[i]);
        else if (field_labels[i].compare(Commons::CSVFieldsPrefix + "header.seq") == 0)         // If it is sequence id
        {
            Commons::StringToInt(tokens[i], msg.Header.SequenceID);
            out_len_seqid = tokens[i].length();
        }
        else if (field_labels[i].compare(Commons::CSVFieldsPrefix + "header.stamp") == 0)       // If it is header stamp
        {
            Commons::StringToLongLong(tokens[i], msg.Header.Stamp);
            out_len_stamp = tokens[i].length();
        }
        else if (field_labels[i].compare(Commons::CSVFieldsPrefix + "header.frame_id") == 0)     // If it is frame id
        {
            msg.Header.FrameID = tokens[i];
            out_len_frameid = tokens[i].length();
        }
        else                                                                                      // If it is any other field
        {
            msg.Fields.push_back(tokens[i]);
            out_len_fields.push_back(tokens[i].length());
        }
    }
    return msg;
}

}
#endif