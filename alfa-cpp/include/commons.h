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
*   Last Modified: April 16, 2019
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
		static const std::string CSVFieldsPrefix;
		static const std::string FaultTopicPrefix;

		// Member Functions
		static std::vector<std::string> Tokenize(const std::string &input, const char delim);
		static bool StringToInt(const std::string &str, int &out_number);
		static bool StringToLong(const std::string &str, long &out_number);
		static bool StringToLongLong(const std::string &str, long long &out_number);
		static bool StringToDouble(const std::string &str, double &out_number);
		static bool StringToLongDouble(const std::string &str, long double &out_number);
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
	const char Commons::FilePathSeparator =
#if defined _WIN32 || defined __CYGWIN__
		'\\';
#else
		'/';
#endif

	// The prefix added to all the field labels in the CSV file
	const std::string Commons::CSVFieldsPrefix = "field.";

	// The prefix of the failure topics in the sequences
	const std::string Commons::FaultTopicPrefix = "failure_status";

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

	// Convert a string to a long integer type. Returns false if the string is not exactly a long integer.
	bool Commons::StringToLong(const std::string &str, long &out_number)
	{
		// Convert to long long integer first
		long long temp;
		if (!StringToLongLong(str, temp)) return false;

		// If successfull, convert to integer
		out_number = (long)temp;
		return true;
	}

	// Convert a string to an integer. Returns false if the string is not exactly an integer.
	bool Commons::StringToInt(const std::string &str, int &out_number)
	{
		// Convert to long long integer first
		long long temp;
		if (!StringToLongLong(str, temp)) return false;

		// If successfull, convert to integer
		out_number = (int)temp;
		return true;
	}

	// Convert a string to a long long integer. Returns false if the string is not exactly a long long integer.
	bool Commons::StringToLongLong(const std::string &str, long long &out_number)
	{
		char *endptr;
		long long value = std::strtoll(str.c_str(), &endptr, 10);

		// If the conversion is not successful
		if (*endptr != '\0') return false;

		// Otherwise, set the output variable
		out_number = value;
		return true;
	}

	// Convert a string to a double. Returns false if the string is not exactly a double.
	bool Commons::StringToDouble(const std::string &str, double &out_number)
	{
		// Convert to long double first
		long double temp;
		if (!StringToLongDouble(str, temp)) return false;

		// If successfull, convert to double
		out_number = (double)temp;
		return true;
	}

	// Convert a string to a long double. Returns false if the string is not exactly a long double.
	bool Commons::StringToLongDouble(const std::string &str, long double &out_number)
	{
		char *endptr;
		long long value = std::strtold(str.c_str(), &endptr);

		// If the conversion is not successful
		if (*endptr != '\0') return false;

		// Otherwise, set the output variable
		out_number = value;
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
			do { file_list.push_back(data.cFileName); } while (FindNextFile(hFind, &data) != 0);
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
			int file_pos = out_filename.find_last_of(FilePathSeparator);
			if (file_pos == std::string::npos) file_pos = -1;

			// Extract the file name
			out_filename = out_filename.substr(file_pos + 1);

			// Extract the directory
			out_directory = file_path.substr(0, file_pos);

			return true;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			return false;
		}
	}

	// Return the list of files in the input list that have the desired extension
	VecString Commons::FilterFileList(const VecString &file_list, const std::string &extension, const bool remove_extension)
	{
		VecString filtered_list;
		for (int i = 0; i < (int)file_list.size(); ++i)
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
		int Year = 0, Month = 0, Day = 0, Hour = 0, Minute = 0, Second = 0, Nanosecond = 0;

		// Member Functions
		bool operator< (const DateTime &dt) const;
		bool operator> (const DateTime &dt) const;
		bool operator== (const DateTime &dt) const;
		bool operator!= (const DateTime &dt) const;
		static DateTime StringToTime(const std::string &strdatetime, const std::string &format);
		static DateTime EpochStringToTime(const std::string &epoch);
		std::string ToString() const;
		double operator-(const DateTime &dt) const;
	};

	// Overload the << operator for DateTime
	std::ostream& operator<< (std::ostream& os, const DateTime &dt)
	{
		os << dt.ToString();
		return os;
	}

	/******************************************************************************/
	/********************** DateTime Function Definitions *************************/
	/******************************************************************************/

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

		if (Nanosecond < dt.Nanosecond) return true;
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
		if (Nanosecond != dt.Nanosecond) return false;
		return true;
	}

	// Overload the != operator for DateTime
	bool DateTime::operator!= (const DateTime &dt) const
	{
		return !(dt == *this);
	}

	// Overload the - operator for DateTime to calculate the duration between two DateTimes in seconds.
	double DateTime::operator-(const DateTime &dt) const
	{
		// Convert DateTime to the standard time representation
		struct std::tm a = { this->Second, this->Minute, this->Hour, this->Day, this->Month, this->Year - 1900 };
		struct std::tm b = { dt.Second, dt.Minute, dt.Hour, dt.Day, dt.Month, dt.Year - 1900 };
		std::time_t x = std::mktime(&a);
		std::time_t y = std::mktime(&b);

		// Check if conversion was successful
		if (x == (std::time_t)(-1) || y == (std::time_t)(-1))
			return 0;

		// Calculate the difference in seconds (also add the Nanoseconds)
		double difference = std::difftime(x, y) + ((double)(this->Nanosecond - dt.Nanosecond)) / 1e9;

		return difference;
	}

	// Convert a given string to a DateTime object given the specified format
	DateTime DateTime::StringToTime(const std::string &strdatetime, const std::string &format)
	{
		DateTime dt;

		// Parse the string using the format
		sscanf(strdatetime.c_str(), format.c_str(),
			&dt.Year, &dt.Month, &dt.Day, &dt.Hour, &dt.Minute, &dt.Second, &dt.Nanosecond);

		return dt;
	}

	// Convert a given UNIX epoch string in nanoseconds to a DateTime object
	DateTime DateTime::EpochStringToTime(const std::string &epoch)
	{
		DateTime dt;

		// Get the nanoseconds substring (last 9 digits)
		std::string nano_str;
		if (epoch.size() < 9)
			nano_str = epoch;
		else
			nano_str = epoch.substr(epoch.size() - 9, 9);

		// Get the rest of the string (seconds in epoch)
		std::string sec_str = epoch.substr(0, epoch.size() - nano_str.size());

		// Read the nanoseconds
		if (!Commons::StringToInt(nano_str, dt.Nanosecond))
			return DateTime();

		// Get the epoch seconds
		long long secs;
		if (!Commons::StringToLongLong(sec_str, secs))
			return DateTime();

		// Convert the seconds to time_t structure
		std::time_t time = secs;

		// Convert time_t structure to DateTime
		std::tm *temp_tm;

		//Set local dst fields of struct tm ?
		std::time_t tvsec = std::time(NULL);
		temp_tm = localtime(&tvsec);

		// Perform the conversion
		temp_tm = localtime(&time);
		dt.Year = 1900 + temp_tm->tm_year;
		dt.Month = temp_tm->tm_mon + 1;
		dt.Day = temp_tm->tm_mday;
		dt.Hour = temp_tm->tm_hour;
		dt.Minute = temp_tm->tm_min;
		dt.Second = temp_tm->tm_sec;

		return dt;
	}

	// Convert DateTime object to string
	std::string DateTime::ToString() const
	{
		// Write the date and time in a character array
		char buffer[30];
		sprintf(buffer, "%04d/%02d/%02d %02d:%02d:%02d.%09d", Year, Month, Day, Hour, Minute, Second, Nanosecond);
		return buffer;
	}

}
#endif