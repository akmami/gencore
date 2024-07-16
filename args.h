#ifndef ARGS_H
#define ARGS_H

#include <string>
#include "program_mode.h"

struct args {
    program_mode mode;
    bool readCores;
    std::string infilename1;
    std::string infilename2;
    bool writeCores;
    std::string outfilename1;
    std::string outfilename2;
    int threadNumber;
    int lcpLevel;
};

#endif