#include "fileio.h"


void save(args& arguments, std::vector<lcp::lps*>& cores) {
    std::ofstream out(arguments.outfilename, std::ios::binary);
    if (!out) {
        log(ERROR,  "Error opening file for writing %s", arguments.infilename.c_str());
        return;
    }

    log(INFO, "Saving cores to file %s", arguments.outfilename.c_str());

    size_t core_size = cores.size();
    size_t genome_size = arguments.size;

    out.write(reinterpret_cast<const char*>(&core_size), sizeof(core_size));
    out.write(reinterpret_cast<const char*>(&genome_size), sizeof(genome_size));

    for ( std::vector<lcp::lps*>::iterator it = cores.begin(); it != cores.end(); it++ ) {
        (*it)->write(out);
    }

    out.close();
};


void load(args& arguments, std::vector<lcp::lps*>& cores) {
    std::ifstream in(arguments.infilename, std::ios::binary);

    if (!in) {
        log(ERROR,  "Error opening file for reading %s", arguments.infilename.c_str());
        return;
    }

    log(INFO, "Loading cores from file %s", arguments.infilename.c_str());

    size_t core_size, genome_size;

    in.read(reinterpret_cast<char*>(&core_size), sizeof(core_size));
    in.read(reinterpret_cast<char*>(&genome_size), sizeof(genome_size));

    cores.reserve(core_size);
    arguments.size = genome_size;

    for ( size_t i = 0; i < core_size; i++ ) {
        lcp::lps *str = new lcp::lps(in);
        str->deepen(arguments.lcpLevel);

        cores.push_back(str);
    }

    in.close();
};


void read_from_file( args& arguments ) {

    std::vector<lcp::lps*> strs;

    load( arguments, strs );

    flatten( strs, arguments.lcp_cores );

    generateMinhashSignature(arguments.lcp_cores);

    for ( std::vector<lcp::lps*>::iterator it = strs.begin(); it != strs.end(); it++ ) {
        delete (*it);
    }

    strs.clear();
};


void read_cores( std::vector<args>& arguments ) {

    const size_t THREADNUMBER = arguments[0].threadNumber;
    std::vector<std::thread> threads;
    std::vector<args>::iterator current_argument = arguments.begin();

    while (current_argument < arguments.end()) {

        while (threads.size() < THREADNUMBER && current_argument < arguments.end()) {
            threads.emplace_back(read_from_file, std::ref(*current_argument));
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
