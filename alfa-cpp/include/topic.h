/*    topic.h - class definition for topic class that reads data from
 *              archived data files (csv)
 *
 *    Carnegie Mellon University
 *    Authors: Mohammadreza Mousaei
 *    Contact: mmousaei@andrew.cmu.edu
 *
 */

#ifndef TOPIC_H
#define TOPIC_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cerrno>
#include <cstdio>
#include <inttypes.h>
#include <stdexcept>
#include <algorithm>

#define MAX_LINE_ITER 100000

class Topic {

public:
    Topic(std::string delim = ",") :
              delim(delim)
    {}

    ~Topic();

    struct Stamp
    {
        int secs;
        int nsecs;
    };
    
    struct Header
    {
        double seq;
        Stamp stamp;
        std::string frame;
    };
    
    bool ReadTopicFromFile(std::string filename);

    int size;
    std::vector<std::string> time;
    std::vector<Header> header;
    std::vector<std::vector<double>> data;
    std::vector<std::string> data_labels;

    bool read(std::string fname);
    bool read(std::string fname, std::vector<std::vector<std::string>> *data);
    bool writeLog(std::string fname, std::vector<std::vector<double>> data);

private:
    std::vector<std::string> Tokenize(const std::string input, const char delim);
    std::string delim;
}; // Topic

#endif
