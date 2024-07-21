#include "rbam.h"


void read_bam(std::string filename, args& arguments, std::vector<uint>& lcp_cores) {
    
    chtslib c(filename.c_str());

    bam1_t* aln = bam_init1();

    while( c.read(aln) > 0 ) {
        printf("%s\n", bam_get_qname(aln));
    }

    bam_destroy1(aln);
}