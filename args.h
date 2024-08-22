#ifndef ARGS_H
#define ARGS_H

#include <string>
#include <vector>
#include "program_mode.h"

struct args {
    program_mode mode;
    bool readCores;
    std::string infilename;
    bool writeCores;
    std::string outfilename;
    std::string prefix;
    size_t threadNumber;
    size_t lcpLevel;
    std::vector<uint> lcp_cores;
    size_t size;
    bool verbose;
};

#endif