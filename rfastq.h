#ifndef RFASTQ_H
#define RFASTQ_H

#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <string>
#include "args.h"
#include "lps.h"
#include "similarity_metrics.h"
#include "helper.h"
#include "utils/GzFile.hpp"
#include "utils/ThreadSafeQueue.hpp"

#ifndef MERGE_CORE_THRESHOLD
#define MERGE_CORE_THRESHOLD 200
#endif

struct Task {
    std::string read;
};

void process_read( ThreadSafeQueue<Task>& task_queue, std::vector<uint>& lcp_cores, int lcp_level );
void read_fastq( args& arguments );

#endif