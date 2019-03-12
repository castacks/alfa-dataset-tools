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

#include "error.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cerrno>
#include <cstdio>
#include <inttypes.h>
#include <stdexcept>
#include <algorithm>

#include <boost/algorithm/string.hpp>

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
    
    int size;
    std::vector<std::string> time;
    std::vector<Header> header;
    std::vector<std::vector<double>> data;
    std::vector<std::string> data_labels;

    Error read(std::string fname);
    Error read(std::string fname, std::vector<std::vector<std::string>> *data);
    Error writeLog(std::string fname, std::vector<std::vector<double>> data);

private:
    std::string delim;
}; // Topic

#endif
