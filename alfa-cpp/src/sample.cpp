/*    sample.cpp - Sample file to demonstrate how to use libraries and dataset files
 *                 
 *
 *    Carnegie Mellon University
 *    Authors: Mohammadreza Mousaei
 *    Contact: mmousaei@andrew.cmu.edu
 *
 */

 #include "error.h"
 #include "topic.h"
 #include <stdlib.h>
 #include <stdio.h>
 #include <iostream>
 #include <cstdio>
 #include <iostream>
 #include <math.h>
 #include <boost/program_options.hpp>
 #include <boost/filesystem.hpp>
 #include <cmath>
 #include <boost/tuple/tuple.hpp>

namespace pots = boost::program_options;

Error parse_inputs(std::string &data, int argc, char** argv);

int main(int argc, char** argv)
{

  // Var defenitions
  Error err;
  Topic topic;
  std::string dataPath;

  // Get the data directory from user
  err = parse_inputs(dataPath,argc,argv);  
  // Read the data from the given directory 
  topic.read(dataPath);

  // Print the data
  std::cout<<"--SAMPLE--"<<std::endl;
  for(int i = 0; i < topic.size; i++)
  {
    std::cout<<"Time in line "<<i<<" is: "<<topic.time[i]<<std::endl;
    std::cout<<"Header seq in line "<<i<<" is: "<<topic.header[i].seq<<std::endl;
    std::cout<<"Header sec timestamp in line "<<i<<" is: "<<topic.header[i].stamp.secs<<std::endl;
    std::cout<<"Header nsec timestamp in line "<<i<<" is: "<<topic.header[i].stamp.nsecs<<std::endl;
    std::cout<<"Header frame in line "<<i<<" is: "<<topic.header[i].frame<<std::endl;
    std::cout<<topic.data_labels[0]<<" in line "<<i<<" is: "<<topic.data[0][i]<<std::endl;
    std::cout<<topic.data_labels[1]<<" in line "<<i<<" is: "<<topic.data[1][i]<<std::endl;
  }

  return 0;
}


Error parse_inputs(std::string &data, int argc, char** argv) {
    // Parse command-line arguments
    pots::options_description desc("Allowed Options");
    bool debug;
    desc.add_options()
        ("help,h",   "Allowed Options:")
        ("data,d",   pots::value<std::string>(), "The path to archived bag data.");

    pots::variables_map vm;
    pots::store(pots::parse_command_line(argc, argv, desc),vm);
    pots::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return INVALID_CMD_INPUT;
    }

    try {
        data = vm["data"].as<std::string>();
    } catch ( const std::exception& e ) {
        std::cout << desc << "\n";
        return INVALID_CMD_INPUT;
    }

    return NO_ERROR;
}