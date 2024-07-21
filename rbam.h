#ifndef RBAM_H
#define RBAM_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "args.h"
#include "lps.h"
#include "chtslib.h"

void read_bam(std::string filename, args& arguments, std::vector<uint>& lcp_cores);

#endif