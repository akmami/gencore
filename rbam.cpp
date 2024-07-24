#include "rbam.h"


void read_bam( args& arguments ) {
    
    chtslib c(arguments.infilename.c_str());

    bam1_t* aln = bam_init1();

    while( c.read(aln) > 0 ) {
        printf("%s\n", bam_get_qname(aln));
    }

    bam_destroy1(aln);
}