#ifndef INIT_H
#define INIT_H

#include <sstream>
#include <cstring>
#include <iostream>
#include "args.h"
#include "program_mode.h"

#ifndef THREAD_NUMBER
#define THREAD_NUMBER 8
#endif

void printUsage();
void parse(int argc, char **argv, args& arguments1, args& arguments2);

#endif