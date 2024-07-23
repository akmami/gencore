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

    args arguments;
    parse(argc, argv, arguments);

    std::vector<uint> lcp_cores_1, lcp_cores_2;

    if ( arguments.readCores ) {
        // read cores which are stored in file.
    } else if ( arguments.mode == FA ) {
        read_fasta( arguments.infilename1, arguments, lcp_cores_1 );
        read_fasta( arguments.infilename2, arguments, lcp_cores_2 );
    } else if ( arguments.mode == FQ ) {
        read_fastq( arguments.infilename1, arguments, lcp_cores_1 );
        read_fastq( arguments.infilename2, arguments, lcp_cores_2 );
    } else if ( arguments.mode == BAM ) {
        read_bam( arguments.infilename1, arguments, lcp_cores_1 );
        read_bam( arguments.infilename2, arguments, lcp_cores_2 );
    } else {
        throw std::invalid_argument("Invalid program mode provided");
    }

    size_t intersection_size = 0, union_size = 0, same_count;

    calculateIntersectionAndUnionSizes(lcp_cores_1, lcp_cores_2, intersection_size, union_size);

    double jaccard_similarity = calculateJaccardSimilarity(intersection_size, union_size);
    double dice_similarity = calculateDiceSimilarity(intersection_size, lcp_cores_1.size(), lcp_cores_2.size());
    double distance_similarity = calculateDistanceSimilarity(lcp_cores_1, lcp_cores_2, 1, 1, same_count);

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
