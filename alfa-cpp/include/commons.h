/*  ***************************************************************************
*   Commons.h - Header for common stuff for ALFA dataset libraries.
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

#ifndef ALFA_Commons_H
#define ALFA_Commons_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <algorithm>

// Define different headers for Windows and Unix-based systems
#if defined _WIN32 || defined __CYGWIN__
	#define NOMINMAX 
	#include <windows.h>
#else
    #include <dirent.h>
#endif


namespace alfa
{

// Typedefs
typedef std::vector<std::string> VecString;

class Commons
{
public:
    // Data Members
    static const char CSVDelimiter;
    static const std::string CSVFileExtension;
    static const char FilePathSeparator;
    static const std::string CSVDateTimeFormat;

    // Member Functions
    static std::vector<std::string> Tokenize(const std::string &input, const char delim);
    static bool StringToInt(const std::string &str, int &out_number);
    static VecString GetFileList(const std::string &dir_path);
    static VecString FilterFileList(const VecString &file_list, const std::string &extension, const bool remove_extension = false);
    static bool ExtractFilenameAndExtension(const std::string &file_path, std::string &out_filename, std::string &out_extension, std::string &out_directory);
};

/******************************************************************************/
/********************** Commons Function Definitions **************************/
/******************************************************************************/

// The delimiter used in the topic CSV files to separate the data fields
const char Commons::CSVDelimiter = ',';

// The extension for the CSV files (normally should be just 'csv')
const std::string Commons::CSVFileExtension = "csv";

// The OS-specific separator for the file paths
const char PathSeparator = 
#if defined _WIN32 || defined __CYGWIN__
    '\\';
#else
    '/';
#endif

// The format of the datetime in the topic CSV files
const std::string Commons::CSVDateTimeFormat = "%d/%d/%d/%d:%d:%d.%d";


// Break a string into smaller tokens using a delimiter
std::vector<std::string> Commons::Tokenize(const std::string &input, const char delim)
{
    // Vector of string to save tokens 
    std::vector<std::string> tokens;
      
    // Define the input string stream and the temporary string
    std::istringstream iss(input);
    std::string tempstr;

    // Break into the tokens
    while (std::getline(iss, tempstr, delim))
        tokens.push_back(tempstr);

    return tokens;
}

// Convert a string to an integer type. Returns false if the string is not exactly an integer.
bool Commons::StringToInt(const std::string &str, int &out_number)
{
    char *endptr;
    long value = std::strtol(str.c_str(), &endptr, 10);
    
    // if the conversion is not successful
    if (*endptr != '\0')
        return false;

    // Otherwise, convert to integer
    out_number = (int)value;
    return true;
}

// Get the list of files in a given directory path
VecString Commons::GetFileList(const std::string &dir_path)
{
    VecString file_list;

    // Treat Windows and Unix differently
    #if defined _WIN32 || defined __CYGWIN__    // Windows implementation to obtain the list of files
        std::string pattern(dir_path);
        pattern.append("\\*");
        WIN32_FIND_DATA data;
        HANDLE hFind;
        if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) 
        {
            do { file_list.push_back(data.cFileName); } 
            while (FindNextFile(hFind, &data) != 0);
            FindClose(hFind);
        }
    #else                                       // POSIX implementation to obtain the list of files
        // Open the directory file
        DIR* dirp = opendir(dir_path.c_str());

        // If successfully opened
        if (dirp)
        {
            struct dirent * dp;

            // Read all the filenames and add them to the vector
            while ((dp = readdir(dirp)) != NULL)
                file_list.push_back(dp->d_name);
            
            // Close the directory file
            closedir(dirp);
        }
    #endif
    return file_list;
}

// Return the filename, directory and the extension from the file path
bool Commons::ExtractFilenameAndExtension(const std::string &file_path, std::string &out_filename, 
    std::string &out_extension, std::string &out_directory)
{
    // Initialize the outputs
    out_filename = "";
    out_extension = "";

    try
    {
        // Find the file extension position
        int ext_pos = file_path.find_last_of(".");

        // Extract the file extension and convert to lower case
        out_extension = file_path.substr(ext_pos + 1);
        std::transform(out_extension.begin(), out_extension.end(), out_extension.begin(), ::tolower);

        // Remove the extension
        out_filename = file_path.substr(0, ext_pos);

        // Find the filename position
        int file_pos = out_filename.find_last_of(PathSeparator);
        if (file_pos == std::string::npos) file_pos = -1;

        // Extract the file name
        out_filename = out_filename.substr(file_pos + 1);

        // Extract the directory
        out_directory = file_path.substr(0, file_pos);

        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

// Return the list of files in the input list that have the desired extension
VecString Commons::FilterFileList(const VecString &file_list, const std::string &extension, const bool remove_extension)
{
    VecString filtered_list;
    for (int i = 0; i < static_cast<int>(file_list.size()); ++i)
    {
        // Find the file extension position
        int ext_pos = file_list[i].find_last_of(".");

        // Extract the file extension and convert to lower case
        std::string file_ext = file_list[i].substr(ext_pos + 1);
        std::transform(file_ext.begin(), file_ext.end(), file_ext.begin(), ::tolower);

        // Convert the input extension to lower case
        std::string lower_ext(extension);
        std::transform(lower_ext.begin(), lower_ext.end(), lower_ext.begin(), ::tolower);

        // Check the extension ignoring the cases
        if (file_list[i].substr(ext_pos + 1) == extension)
        {
            // Remove the extension if desired
            if (remove_extension)
                filtered_list.push_back(file_list[i].substr(0, ext_pos));
            else
                filtered_list.push_back(file_list[i]);
        }
    }
    return filtered_list;
}

/******************************************************************************/
/********************** DateTime Class Definition *****************************/
/******************************************************************************/

class DateTime
{
public: 
    // Data Members
    int Year = 0, Month = 0, Day = 0, Hour = 0, Minute = 0, Second = 0, Microsecond = 0;

    // Member Functions
    bool operator< (const DateTime &dt) const;
    bool operator> (const DateTime &dt) const;
    bool operator== (const DateTime &dt) const;
    bool operator!= (const DateTime &dt) const;
    static DateTime StringToTime(const std::string &strdatetime, const std::string &format);
    std::string ToString() const;
};

/******************************************************************************/
/********************** DateTime Function Definitions *************************/
/******************************************************************************/

// Overload the << operator for DateTime
std::ostream& operator<< (std::ostream& os, const DateTime &dt)
{
    os << dt.ToString();
    return os;
}

// Overload the < operator for DateTime
bool DateTime::operator< (const DateTime &dt) const
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

// Overload the > operator for DateTime
bool DateTime::operator> (const DateTime &dt) const
{
    return (dt < *this);
}

// Overload the == operator for DateTime
bool DateTime::operator== (const DateTime &dt) const
{
    if (Year != dt.Year) return false;
    if (Month != dt.Month) return false;
    if (Day != dt.Day) return false;
    if (Hour != dt.Hour) return false;
    if (Minute != dt.Minute) return false;
    if (Second != dt.Second) return false;
    if (Microsecond != dt.Microsecond) return false;
    return true;
}

// Overload the != operator for DateTime
bool DateTime::operator!= (const DateTime &dt) const
{
    return !(dt == *this);
}

// Convert a given string to a DateTime object given the specified format
DateTime DateTime::StringToTime(const std::string &strdatetime, const std::string &format)
{
    DateTime dt;
    
    // Parse the string using the format
    sscanf(strdatetime.c_str(), format.c_str(), 
                &dt.Year, &dt.Month, &dt.Day, &dt.Hour, &dt.Minute, &dt.Second, &dt.Microsecond);
    
    return dt;
}

// Convert DateTime object to string
std::string DateTime::ToString() const
{
    // Write the date and time in a character array
    char buffer [30];
    sprintf(buffer, "%04d/%02d/%02d %02d:%02d:%02d.%06d", Year, Month, Day, Hour, Minute, Second, Microsecond);
    return buffer;
}    

}
#endif