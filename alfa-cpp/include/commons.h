/*  ***************************************************************************
*   Commons.h - Header for Commons for ALFA dataset libraries.
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

#ifndef ALFA_Commons_H
#define ALFA_Commons_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

namespace alfa
{

class Commons
{
public:
    // Data Members
    static const char CSVDelimiter;
    static const char FilePathSeparator;
    static const std::string CSVDateTimeFormat;

    // Member Functions
    static std::vector<std::string> Tokenize(const std::string input, const char delim);
    static bool StringToInt(std::string str, int &out_number);

    // Subclasses
    class DateTime
    {
    public: 
        int Year = 0, Month = 0, Day = 0, Hour = 0, Minute = 0, Second = 0, Microsecond = 0;

        friend std::ostream& operator<< (std::ostream& os, const DateTime &dt)
        {
            char buffer [30];
            sprintf (buffer, "%04d/%02d/%02d %02d:%02d:%02d.%06d", dt.Year, dt.Month, dt.Day, dt.Hour, dt.Minute, dt.Second, dt.Microsecond);
            os << buffer;
            return os;
        }

        bool operator< (const DateTime &dt) const
        {
            if (Year < dt.Year) return true;
            if (Year > dt.Year) return false;

            if (Month < dt.Month) return true;
            if (Month > dt.Month) return false;
            
            if (Day < dt.Day) return true;
            if (Day > dt.Day) return false;
            
            if (Hour < dt.Hour) return true;
            if (Hour > dt.Hour) return false;
            
            if (Minute < dt.Minute) return true;
            if (Minute > dt.Minute) return false;

            if (Second < dt.Second) return true;
            if (Second > dt.Second) return false;

            if (Microsecond < dt.Microsecond) return true;
            return false;
        }

        DateTime& operator= (const DateTime &dt)
        {
            Year = dt.Year;
            Month = dt.Month;
            Day = dt.Day;
            Hour = dt.Hour;
            Minute = dt.Minute;
            Second = dt.Second;
            Microsecond = dt.Microsecond;
            return *this;
        }

        static DateTime StringToTime(std::string strdatetime, std::string format)
        {
            DateTime dt;
            
            !sscanf(strdatetime.c_str(), format.c_str(), 
                        &dt.Year, &dt.Month, &dt.Day, &dt.Hour, &dt.Minute, &dt.Second, &dt.Microsecond);
            
            return dt;
        }
    };

};
const char Commons::CSVDelimiter = ',';

const char PathSeparator = 
#if defined _WIN32 || defined __CYGWIN__
    '\\';
#else
    '/';
#endif

const std::string Commons::CSVDateTimeFormat = "%d/%d/%d/%d:%d:%d.%d";


// Break a string into smaller tokens using a delimiter
std::vector<std::string> Commons::Tokenize(const std::string input, const char delim)
{
    // Vector of string to save tokens 
    std::vector<std::string> tokens;
      
    std::istringstream iss(input);
    std::string tempstr;

    // Break into the tokens
    while (std::getline(iss, tempstr, delim))
        tokens.push_back(tempstr);

    return tokens;
}

bool Commons::StringToInt(std::string str, int &out_number)
{
    char *endptr;
    long value = std::strtol(str.c_str(), &endptr, 10);
    if (*endptr == '\0')
    {
        out_number = (int)value;
        return true;
    }
    return false;
}


}
#endif