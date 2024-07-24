#include "rfastq.h"


std::mutex results_mutex; // mutex for protecting access to the results vector

/**
 * @brief Processes genomic reads from a queue, extracts LCP cores, and aggregates results in a shared vector.
 *
 * This function runs in a worker thread and is responsible for processing genomic reads
 * retrieved from a thread-safe queue. For each read, it computes the LCP cores at a specified
 * LCP level, processes the reverse complement of the read, computes its LCP cores, and then
 * aggregates these cores in a shared vector. The function ensures thread-safe access to the
 * shared vector using mutexes and minimizes locking overhead by merging local results into
 * the shared vector periodically.
 *
 * @param task_queue The thread-safe queue from which tasks (genomic reads) are retrieved.
 * @param lcp_cores A shared vector to store the labels of LCP cores extracted from the reads.
 * @param lcp_level The depth of analysis for extracting LCP cores from the reads.
 */
void process_read( ThreadSafeQueue<Task>& task_queue, std::vector<uint>& lcp_cores, int lcp_level ) {
    std::vector<uint> local_cores;
    Task task;

    auto merge_results = [&]() {
        std::lock_guard<std::mutex> lock(results_mutex);
        lcp_cores.insert(lcp_cores.end(), local_cores.begin(), local_cores.end());
        local_cores.clear();
    };

    while ( task_queue.pop(task) || !task_queue.isFinished() ) {

        lcp::lps *lcp = new lcp::lps(task.read);
        lcp->deepen(lcp_level);
        
        for ( std::vector<lcp::core*>::iterator it = lcp->cores.begin(); it != lcp->cores.end(); it++ ) {
            local_cores.push_back( (*it)->label );
        }

        delete lcp;
        lcp = NULL;

        reverseComplement(task.read);

        lcp = new lcp::lps(task.read);
        lcp->deepen(lcp_level);
        
        for ( std::vector<lcp::core*>::iterator it = lcp->cores.begin(); it != lcp->cores.end(); it++ ) {
            local_cores.push_back( (*it)->label );
        }

        delete lcp;

        // periodically merge results to the main vector to reduce locking overhead
        if (local_cores.size() >= MERGE_CORE_THRESHOLD) {
            merge_results();
        }
    }

    // merge any remaining results after all tasks are processed
    if (!local_cores.empty()) {
        merge_results();
    }
};


/**
 * @brief Processes a genome file to extract LCP cores using multiple threads.
 *
 * This function reads genomic sequences from a specified file and distributes the processing
 * tasks among several worker threads. Each thread computes LCP cores for the sequences at a
 * given LCP level and aggregates these cores into a shared vector. The function tracks the
 * total number of reads processed and their combined length. It ensures efficient and
 * thread-safe handling of genomic data, leveraging parallel processing to enhance performance.
 *
 * @param filename Path to the input file containing genomic sequences.
 * @param infile Reference to an open GzFile object for reading the input file.
 * @param lcp_level The depth of LCP analysis for extracting cores from sequences.
 * @param lcp_cores A reference to a shared vector where extracted LCP cores are aggregated.
 * @param read_count Reference to a variable that will hold the total number of processed reads.
 * @param total_length Reference to a variable that will hold the combined length of all reads.
 * @param thread_number The number of worker threads to use for processing.
 */
void read_fastq( args& arguments ) {

    GzFile infile( arguments.infilename.c_str(), "rb" );

    ThreadSafeQueue<Task> task_queue;
    std::vector<std::thread> workers;

    // start worker threads
    for (size_t i = 0; i < arguments.threadNumber; ++i) {
        workers.emplace_back(process_read, std::ref(task_queue), std::ref(arguments.lcp_cores), arguments.lcpLevel);
    }

    std::cout << "Processing is started for " << arguments.infilename << std::endl;
    
    // variables
    char buffer[BUFFERSIZE];

    // read file
    std::string sequence;
        
    sequence.reserve(BUFFERSIZE);

    while (true) {
        if ( infile.gets(buffer) == Z_NULL) {
            // End of file or an error
            if ( ! infile.eof() ) {
                std::cerr << "Error reading file." << std::endl;
            }
            break;
        }

        infile.gets(buffer);
        sequence.append(buffer);

        while ( !task_queue.isAvailable() );

        // assign read to tasks queue
        Task task;
        task.read = std::string(buffer);
        task_queue.push(task);

        infile.gets(buffer);
        infile.gets(buffer);
    }

    task_queue.markFinished();

    // wait for all worker threads to complete
    for (std::vector<std::thread>::iterator it = workers.begin(); it != workers.end(); it++ ) {
        if ((*it).joinable()) {
            (*it).join();
        }
    }

    generateMinhashSignature(arguments.lcp_cores);
};