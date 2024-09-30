#include "helper.h"


bool reverseComplement( std::string& sequence ) {
    
    // reverse the sequence
    std::reverse( sequence.begin(), sequence.end() );
    
    // replace each nucleotide with its complement
    std::transform( sequence.begin(), sequence.end(), sequence.begin(),
                   [](char nucleotide) -> char {
                       switch (nucleotide) {
                           case 'A': return 'T';
                           case 'T': return 'A';
                           case 'G': return 'C';
                           case 'C': return 'G';
                           default: return nucleotide;
                       }
                   });
    
    return true;
};


void flatten(std::vector<lcp::lps*>& strs, std::vector<uint32_t>& lcp_cores) {
    
    size_t size = 0;
    
    for(std::vector<lcp::lps*>::iterator it_str = strs.begin(); it_str != strs.end(); it_str++) {
        size += (*it_str)->cores->size();
    }

    lcp_cores.reserve(size);

    for(std::vector<lcp::lps*>::iterator it_str = strs.begin(); it_str != strs.end(); it_str++) {
        for ( std::vector<lcp::core*>::iterator it_lcp = (*it_str)->cores->begin(); it_lcp != (*it_str)->cores->end(); it_lcp++ ) {
            lcp_cores.push_back( (*it_lcp)->label );
        }
    }
};


void generateSignature( std::vector<uint32_t>& hash_values ) {
    std::sort(hash_values.begin(), hash_values.end());
};


void initializeSetAndCounts( std::vector<uint32_t>& cores, std::vector<uint32_t>& set, std::vector<size_t>& counts ) {
    
    if( cores.empty() ) {
        return;
    }

    int distinct_cores = 1;

    for( size_t i = 1; i < cores.size(); i++ ) {
        if( cores[i] != cores[i - 1] ) {
            distinct_cores++;
        }

        if ( cores[i] < cores[i - 1] ) {
            log(ERROR, "Given LCP cores should be sorted.");
            exit(1);
        }
    }
    
    // pre-allocate memory for efficiency
    set.reserve( distinct_cores ); 
    counts.reserve( distinct_cores );
    
    set.push_back(cores[0]);
    size_t count = 1;
    
    for (size_t i = 1; i < cores.size(); ++i) {
        if (cores[i] == cores[i - 1]) {
            count++;
        } else {
            counts.push_back(count);
            set.push_back(cores[i]);
            count = 1;
        }
    }

    // add the count for the last element
    counts.push_back(count); 
};