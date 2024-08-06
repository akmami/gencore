#ifndef INIT_H
#define INIT_H

#include <sstream>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include "args.h"
#include "program_mode.h"

#ifndef THREAD_NUMBER
#define THREAD_NUMBER 8
#endif

#ifndef VERBOSE 
#define VERBOSE false
#endif

void printUsage();
void parse(int argc, char **argv, std::vector<args>& arguments);

#endif