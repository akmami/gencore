#ifndef RFASTA_H
#define RFASTA_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "lps.h"

#define LCP_LEVEL 7

void flatten(std::vector<lcp::lps*>& strs, std::vector<uint>& lcp_cores);
void read_fasta(std::string filename, std::vector<uint>& lcp_cores);

#endif