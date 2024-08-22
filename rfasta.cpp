#include "rfasta.h"

void read_fastas( std::vector<args>& arguments ) {
    
    const size_t THREADNUMBER = arguments[0].threadNumber;
    std::vector<std::thread> threads;
    std::vector<args>::iterator current_argument = arguments.begin();

    while (current_argument < arguments.end()) {

        while (threads.size() < THREADNUMBER && current_argument < arguments.end()) {
            threads.emplace_back(read_fasta, std::ref(*current_argument));
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


void read_fasta( args& arguments ) {
    std::ostringstream ss;
    ss << std::this_thread::get_id();
    log(INFO, "Thread ID: %s started processing %s", ss.str().c_str(), arguments.infilename.c_str());

    std::fstream file;
    file.open( arguments.infilename, std::ios::in );

    int chrom_index = 0;
    std::string line;
    size_t genome_size = 0;

    std::vector<lcp::lps*> strs;

    // read file
    if ( file.is_open() ) {  

        std::string sequence, id;
        sequence.reserve(250000000);
        
        while (getline(file, line)) {

            if (line[0] == '>') {

                chrom_index++;

                // process previous chromosome before moving into new one
                if (sequence.size() != 0) {
                
                    lcp::lps* str = new lcp::lps(sequence);
                    str->deepen(arguments.lcpLevel);

                    strs.push_back(str);
                    genome_size += sequence.size();
                    
                    sequence.clear();
                }
                
                id = line.substr(1);
                continue;
            }
            else if (line[0] != '>'){
                sequence += line;
            }
        }
        
        arguments.size = genome_size;
        file.close();
    } else {
        log(ERROR, "Could not be able to open %s", arguments.infilename.c_str());
        exit(1);
    }
    
    flatten(strs, arguments.lcp_cores);

    if ( arguments.writeCores ) {
        save( arguments, strs );
    }

    for ( std::vector<lcp::lps*>::iterator it = strs.begin(); it != strs.end(); it++ ) {
        delete (*it);
    }

    strs.clear();

    generateMinhashSignature(arguments.lcp_cores);
};
