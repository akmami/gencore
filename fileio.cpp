#include "fileio.h"


void save( const struct targs& arguments, const std::vector<lcp::lps*>& cores ) {
    std::ofstream out(arguments.outFileName, std::ios::binary);
    if (!out) {
        log(ERROR, "Error opening file for writing %s", arguments.inFileName.c_str());
        return;
    }

    log(INFO, "Saving cores to file %s", arguments.outFileName.c_str());

    size_t core_size = cores.size();
    size_t genome_size = arguments.size;

    out.write(reinterpret_cast<const char*>(&core_size), sizeof(core_size));
    out.write(reinterpret_cast<const char*>(&genome_size), sizeof(genome_size));

    for ( std::vector<lcp::lps*>::const_iterator it = cores.begin(); it != cores.end(); it++ ) {
        (*it)->write(out);
    }

    out.close();
};


void load( struct targs& thread_arguments, struct pargs& program_arguments, std::vector<lcp::lps*>& cores ) {
    std::ifstream in(thread_arguments.inFileName, std::ios::binary);

    if (!in) {
        log(ERROR, "Error opening file for reading %s", thread_arguments.inFileName.c_str());
        return;
    }
    
    size_t core_size, genome_size;

    in.read(reinterpret_cast<char*>(&core_size), sizeof(core_size));
    in.read(reinterpret_cast<char*>(&genome_size), sizeof(genome_size));

    cores.reserve(core_size);
    thread_arguments.size = genome_size;

    for ( size_t i = 0; i < core_size; i++ ) {
        lcp::lps *str = new lcp::lps(in);
        str->deepen(program_arguments.lcpLevel);

        cores.push_back(str);
    }

    in.close();
};


void read_from_file( struct targs& thread_arguments, struct pargs& program_arguments ) {

    // get thread id
    std::ostringstream ss;
    ss << std::this_thread::get_id();

    // log initiation of reading fasta
    log(INFO, "Thread ID: %s started loading %s", ss.str().c_str(), thread_arguments.inFileName.c_str());

    // load lcp cores
    std::vector<lcp::lps*> strs;
    load( thread_arguments, program_arguments, strs );

    // log ending of processing fasta
    log(INFO, "Thread ID: %s ended loading %s", ss.str().c_str(), thread_arguments.inFileName.c_str());

    // get lcp core hashes
    std::vector<uint32_t> lcp_core_hashes;
    flatten(strs, lcp_core_hashes);

    // delete lcp cores
    for ( std::vector<lcp::lps*>::iterator it = strs.begin(); it != strs.end(); it++ ) {
        delete (*it);
    }
    strs.clear();

    // set lcp cores and counts to arguments
    generateSignature( lcp_core_hashes );
    initializeSetAndCounts( lcp_core_hashes, thread_arguments.cores, thread_arguments.counts );
};


void read_cores( std::vector<struct targs>& thread_arguments, struct pargs& program_arguments ) {

    std::vector<std::thread> threads;
    std::vector<struct targs>::iterator current_argument = thread_arguments.begin();

    while (current_argument < thread_arguments.end()) {

        while (threads.size() < program_arguments.threadNumber && current_argument < thread_arguments.end()) {
            threads.emplace_back(read_from_file, std::ref(*current_argument), std::ref(program_arguments));
            current_argument++;
        }

        for ( std::vector<std::thread>::iterator it = threads.begin(); it != threads.end(); ) {
            if (it->joinable()) {
                it->join();
                it = threads.erase(it);
            } else {
                it++;
            }
        }
    }

    for (std::vector<std::thread>::iterator it = threads.begin(); it != threads.end(); it++ ) {
        it->join();
    }
};
