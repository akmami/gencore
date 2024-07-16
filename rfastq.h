#ifndef RFASTQ_H
#define RFASTQ_H

#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <string>
#include "lps.h"
#include "helper.h"
#include "utils/GzFile.hpp"
#include "utils/ThreadSafeQueue.hpp"

#ifndef BUFFERSIZE
#define BUFFERSIZE      100000
#endif

#ifndef MERGE_CORE_THRESHOLD
#define MERGE_CORE_THRESHOLD 200
#endif

struct Task {
    std::string read;
};

void process_read( ThreadSafeQueue<Task>& task_queue, std::vector<uint>& lcp_cores, int lcp_level );
void read_fastq( const char* filename, GzFile& infile, int lcp_level, std::vector<uint>& lcp_cores, size_t& read_count, size_t& total_length, size_t thread_number );

#endif