#include <vector>
#include <iostream>
#include <thread>
#include <mutex>

#include "args.h"
#include "init.h"
#include "rfasta.h"
#include "rfastq.h"
#include "rbam.h"


int main(int argc, char **argv) {

    args arguments;
    parse(argc, argv, arguments);

    std::vector<uint> lcp_cores1, lcp_cores2;

    if ( arguments.readCores ) {
        // read cores which are stored in file.
    } else if ( arguments.mode == FA ) {
        read_fasta( arguments.infilename1, arguments, lcp_cores1 );
        read_fasta( arguments.infilename2, arguments, lcp_cores2 );
    } else if ( arguments.mode == FQ ) {
        read_fastq( arguments.infilename1, arguments, lcp_cores1 );
        read_fastq( arguments.infilename2, arguments, lcp_cores2 );
    } else if ( arguments.mode == BAM ) {
        read_bam( arguments.infilename1, arguments, lcp_cores1 );
        read_bam( arguments.infilename2, arguments, lcp_cores2 );
    } else {
        throw std::invalid_argument("Invalid program mode provided");
    }

    return 0;
};
