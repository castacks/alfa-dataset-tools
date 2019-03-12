/*    error.h - definition for processing errors
 *
 *    Carnegie Mellon University
 *    Authors: Mohammadreza Mousaei
 *    Contact: mmousaei@andrew.cmu.edu
 *
 */

#ifndef ERROR_H
#define ERROR_H

#include <iostream>

#define BOLDBLUE "\033[1m\033[94m"
#define BOLDRED  "\033[1m\033[31m"
#define BOLDYEL  "\033[1m\033[93m"
#define ENDC     "\033[0m"

#define DEBUG

enum Error {
    NO_ERROR               = 0,
    NO_TIME_DATA           = 1,
    NO_HEADER_DATA         = 2,
    READ_TIME_FAIL         = 3,
    READ_FRAME_FAIL        = 4,
    INVALID_CMD_INPUT      = 5,
    NO_DATA_DIRECTORY      = 6,
    FAILED_FILE_OPEN       = 7,
    READ_FAIL              = 8
};

static const char* Error_Message[] = {
    "No error",
    "No timestamp data found, aborting!",
    "No header data found!",
    "Failed to read timestamp data, aborting!",
    "Failed to read frame data!",
    "Incorrect command call.",
    "Archived data directory does not exists.",
    "Failed to open file.",
    "Failed to read file.",
};

// Prototype for error reporting
void showErr(Error *err);

// Prototype for warning reporting
void showWarn(Error *err);

#endif
