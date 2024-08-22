#ifndef FILEIO_H
#define FILEIO_H

#include <vector>
#include <string>
#include <thread>
#include "args.h"
#include "lps.h"
#include "helper.h"
#include "similarity_metrics.h"
#include "logging.h"

void save(args& arguments, std::vector<lcp::lps*>& cores);
void load(args& arguments, std::vector<lcp::lps*>& cores);
void read_from_file(args& arguments);
void read_cores(std::vector<args>& arguments);

#endif
