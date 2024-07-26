#include <vector>
#include <iostream>
#include <thread>
#include <mutex>

#include "args.h"
#include "init.h"
#include "rfasta.h"
#include "rfastq.h"
#include "rbam.h"
#include "similarity_metrics.h"


int main(int argc, char **argv) {

    args arguments1, arguments2;
    parse(argc, argv, arguments1, arguments2);
    lcp::init_coefficients(true);

    if ( arguments1.readCores ) {
        read_from_file( arguments1 );
        read_from_file( arguments2 );
    } else if ( arguments1.mode == FA ) {
        read_fasta( arguments1 );
        read_fasta( arguments2 );
    } else if ( arguments1.mode == FQ ) {
        read_fastq( arguments1 );
        read_fastq( arguments2 );
    } else if ( arguments1.mode == BAM ) {
        read_bam( arguments1 );
        read_bam( arguments2 );
    } else {
        throw std::invalid_argument("Invalid program mode provided");
    }

    size_t intersection_size = 0, union_size = 0, same_count;

    calculateIntersectionAndUnionSizes( arguments1.lcp_cores, arguments2.lcp_cores, intersection_size, union_size);

    double jaccard_similarity = calculateJaccardSimilarity(intersection_size, union_size);
    double dice_similarity = calculateDiceSimilarity(intersection_size, arguments1.lcp_cores.size(), arguments2.lcp_cores.size());
    double distance_similarity = calculateDistanceSimilarity(arguments1.lcp_cores, arguments2.lcp_cores, 1, 1, same_count);

    std::cout << "Intersection Size: " << intersection_size << std::endl;
    std::cout << "Union Size: " << union_size << std::endl;
    std::cout << "Same Core Count: " << same_count << std::endl;
    std::cout << std::endl;

    std::cout << "Jaccard Similarity: " << jaccard_similarity << std::endl;
    std::cout << "Dice Similarity: " << dice_similarity << std::endl;
    std::cout << "Distance Based Similarity: " << distance_similarity << std::endl;
    std::cout << std::endl;

    return 0;
};
