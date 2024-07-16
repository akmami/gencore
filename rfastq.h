#ifndef RFASTQ_H
#define RFASTQ_H

#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <string>
#include "lps.h"
#include "helper.h"
#include "constant.h"
#include "utils/GzFile.hpp"
#include "utils/ThreadSafeQueue.hpp"

std::mutex results_mutex; // mutex for protecting access to the results vector

struct Task {
    std::string read;
};

void process_read( ThreadSafeQueue<Task>& task_queue, std::vector<uint>& lcp_cores, int lcp_level );
void read_fastq( const char* filename, GzFile& infile, int lcp_level, std::vector<uint>& lcp_cores, size_t& read_count, size_t& total_length, size_t thread_number );

#endif