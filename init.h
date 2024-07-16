#ifndef INIT_H
#define INIT_H

#include <cstring>
#include <iostream>
#include "args.h"
#include "constant.h"

void printUsage();
args& parse(int argc, char **argv);

#endif