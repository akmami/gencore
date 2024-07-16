#include "rfasta.h"


void flatten(std::vector<lcp::lps*>& strs, std::vector<uint>& lcp_cores) {
    for(std::vector<lcp::lps*>::iterator it_str = strs.begin(); it_str != strs.end(); it_str++) {
        for ( std::vector<lcp::core*>::iterator it_lcp = (*it_str)->cores.begin(); it_lcp != (*it_str)->cores.end(); it_lcp++ ) {
            lcp_cores.push_back( (*it_lcp)->label );
        }
    }
};


void read_fasta(std::string filename, std::vector<uint>& lcp_cores) {
    
    std::fstream file;
    file.open(filename, std::ios::in);

    int chrom_index = 0;
    std::string line;

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
                    
                    for ( int i = 1; i < LCP_LEVEL; i++ ) {

                        str->deepen();
                    }

                    std::cout << "Processing is done for " << id << std::endl;
                    std::cout << "Length of the processed sequence: " << sequence.size() << std::endl;
                    std::cout << std::endl;
                    
                    strs.push_back(str);
                    
                    sequence.clear();
                }
                
                id = line.substr(1);
                std::cout << "Processing started for " << id << std::endl;
                continue;
                    
            }
            else if (line[0] != '>'){
                sequence += line;
            }
        }

        file.close();
    }
    
    flatten(strs, lcp_cores);
};