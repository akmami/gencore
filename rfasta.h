#ifndef RFASTA_H
#define RFASTA_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include "args.h"
#include "lps.h"
#include "helper.h"
#include "fileio.h"
#include "similarity_metrics.h"
#include "logging.h"

void read_fastas( std::vector<args>& arguments );
void read_fasta( args& arguments );

#endif