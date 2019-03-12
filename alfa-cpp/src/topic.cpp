/*    topic.cpp - class source for topic class that reads data from
 *                    archived data files (csv)
 *
 *    Carnegie Mellon University
 *    Authors: Mohammadreza Mousaei
 *    Contact: mmousaei@andrew.cmu.edu
 *
 */

#include "topic.h"

Topic::~Topic() {}

Error Topic::read(std::string fname) 
{
    // Open file stream to given csv
    std::ifstream f(fname);
    if( !f.is_open() ) {
        return FAILED_FILE_OPEN;
    }
    // Define line data for string and double types
    std::vector<std::string> splitString;
    std::vector<std::string> splitString2;

    std::string line = "";
    double res;

    // Logic for whether line width matches what we expect, based on first line
    bool validLine = false;
    // Initialize loop counter
    uint32_t lcount = 0;
    // Initialize column counter
    size_t ccount = 0;
    // Initialize # of levels in data-structure
    size_t levvount = 0;
    // Initialize time index
    int timeind;
    // Initialize header indexs
    std::vector<int> headerind;
    // Initialize data indexs
    std::vector<int> dataind;
    // Initialize data indexs
    bool isThereTime = false;
    // Remove first line as it contains text descriptions of column contents
    getline(f,line);
    boost::split(splitString, line, boost::is_any_of(","));
    ccount = splitString.size();
    // Process description text to identify indexes for processing each colomn
    for(int i = 0; i < ccount; i++)
    {
        boost::split(splitString2, splitString[i], boost::is_any_of("."));
        if(!splitString[i].compare("time"))
        {
            timeind = i;
            isThereTime = true;
        }
        else if(splitString2.size() > 0)
        {
            if(!splitString2[1].compare("header"))
            {
                headerind.push_back(i);
            }
            else
            {
                dataind.push_back(i);
                data_labels.push_back(splitString[i]);
            }
        }
        else
        {
            dataind.push_back(i);
            data_labels.push_back(splitString[i]);
        }
    }

    if(!isThereTime)
    {
        return NO_TIME_DATA;
    }
        
    // Clear old data
    header.clear();
    time.clear();
    data.clear();
    // Iterate over all lines and extract data
    while( getline(f,line) && lcount < MAX_LINE_ITER ) 
    {
        // Split string via comma delimitation
        boost::split(splitString, line, boost::is_any_of(","));
        // Initialize column count based on first line
        if(lcount == 0) 
        {
            // Save column count
            ccount = splitString.size();
        }

        // Enforce column uniformity
        if( splitString.size() != ccount ) 
        {
            validLine = false;
        } 
        else 
        {
            validLine = true;
        }

        if( validLine ) 
        {
            std::vector<int>::iterator it;
            Header headertmp;
            Stamp stamptmp;
            // Iterate over all split strings and convert to doubles
            for( size_t i = 0; i < splitString.size(); i++ ) 
            {
                // Push time
                if(i == timeind)
                {
                    time.push_back(splitString[i]);
                }
                // Find if index belongs to header indexs
                it = std::find(headerind.begin(), headerind.end(), i);
                // Push header
                if(it != headerind.end())
                {
                    if((it - headerind.begin()) == 0) 
                    {
                        headertmp.seq = std::stod(splitString[i]);
                    }
                    if((it - headerind.begin()) == 1) 
                    {
                        stamptmp.secs = std::stoi(splitString[i]);
                    }
                    if((it - headerind.begin()) == 2) 
                    {
                        stamptmp.nsecs = std::stoi(splitString[i]);
                        headertmp.stamp = stamptmp;
                    }
                    if((it - headerind.begin()) == 3) 
                    {
                        headertmp.frame = splitString[i];
                        header.push_back(headertmp);
                    }
                }
                // Find if index belongs to data indexs
                it = std::find(dataind.begin(), dataind.end(), i);
                data.resize(dataind.size());
                // Push data
                if(it != dataind.end())
                {
                    data[(it - dataind.begin())].push_back(std::stod(splitString[i]));
                }
            }
        }
        // Incrememnt loop count
        lcount++;
    }
    // Close file
    f.close();
    if(!headerind.size())
    {
        return NO_HEADER_DATA;
    }
    if(header.size() == time.size())
    {
        size = header.size();
    }
    else
    {
        return READ_FAIL;
    }
    
    return NO_ERROR;
}

Error Topic::read(std::string fname, std::vector<std::vector<std::string>> *data) {
    // Open file stream to given csv
    std::ifstream f(fname);
    if( !f.is_open() ) {
        return FAILED_FILE_OPEN;
    }
    // Define line data for string and double types
    std::vector<std::string> splitString;
    std::string line = "";

    // Logic for whether line width matches what we expect, based on first line
    bool validLine = false;
    // Initialize loop counter
    uint32_t lcount = 0;
    // Initialize column counter
    size_t ccount = 0;

    // Remove first line as it contains text descriptions of column contents
    getline(f,line);
    // Iterate over all lines and extract data
    while( getline(f,line) && lcount < MAX_LINE_ITER ) {
        // Split string via comma delimitation
        boost::split(splitString, line, boost::is_any_of(","));
        // Initialize column count based on first line
        if(lcount == 0) {
            data->resize(splitString.size());
            // Save column count
            ccount = splitString.size();
        }

        // Enforce column uniformity
        if( splitString.size() != ccount ) {
            validLine = false;
        } else {
            validLine = true;
        }

        if( validLine ) {
            // Iterate over all split strings and convert to doubles
            for( size_t i = 0; i < splitString.size(); i++ ) {
                // Push data into column
                data->at(i).push_back(splitString[i]);
            }
        }
        // Incrememnt loop count
        lcount++;
    }
    // Close file
    f.close();

    return NO_ERROR;
}

Error Topic::writeLog(std::string fname, std::vector<std::vector<double>> data) {

    // Open file stream to given csv
    std::ofstream f(fname);
    // if( !f.is_open() ) {
    //     return FAILED_WRITE_LOG;
    // }

    // Iterate over rows
    for(int i = 0; i < data.size(); i++) {
        // Iterate over columns
        for(int j = 0; j < data.at(0).size(); j++) {
            f << (data)[i][j];
            // Add comma or endline at end of row
            if( j == data.at(0).size()-1 ) {
                f << std::endl;
            } else {
                f << ",";
            }
        }
    }

    return NO_ERROR;
}
