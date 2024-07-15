#ifndef ARGS_H
#define ARGS_H

#include <string>

struct args {
    bool readCores;
    bool writeCores;
    std::string outfilename;
    std::string infilename1;
    std::string infilename2;
    int threadNumber;
    int lcpLevel;
};

#endif