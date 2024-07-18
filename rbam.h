#ifndef RFASTA_H
#define RFASTA_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "lps.h"
#include "chtslib.h"

void read_bam(std::string filename, std::vector<uint>& lcp_cores);

#endif