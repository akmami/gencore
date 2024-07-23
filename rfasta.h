#ifndef RFASTA_H
#define RFASTA_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "args.h"
#include "lps.h"
#include "similarity_metrics.h"

void flatten(std::vector<lcp::lps*>& strs, std::vector<uint>& lcp_cores);
void read_fasta(std::string filename, args& arguments, std::vector<uint>& lcp_cores);

#endif