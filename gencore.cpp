#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <iomanip>

#include "args.h"
#include "init.h"
#include "rfasta.h"
#include "rfastq.h"
#include "rbam.h"
#include "similarity_metrics.h"


int main(int argc, char **argv) {

    std::vector<args> arguments;
    parse(argc, argv, arguments);
    lcp::init_coefficients(true);

    for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
        if ( it->readCores ) {
            read_from_file( *it );
        } else if ( it->mode == FA ) {
            read_fasta( *it );
        } else if ( it->mode == FQ ) {
            read_fastq( *it );
        } else if ( it->mode == BAM ) {
            read_bam( *it );
        } else {
            throw std::invalid_argument("Invalid program mode provided");
        }
    }

    double jaccard[arguments.size()][arguments.size()];
    double dice[arguments.size()][arguments.size()];
    double distance[arguments.size()][arguments.size()];
    
    for (size_t i = 0; i < arguments.size(); i++ ) {
        jaccard[i][i] = 1;
        dice[i][i] = 1;
        distance[i][i] = 1;
    }

    for ( std::vector<args>::iterator it1 = arguments.begin(); it1 < arguments.end(); it1++ ) { 
        for ( std::vector<args>::iterator it2 = it1+1; it2 < arguments.end(); it2++ ) {
            
            size_t intersection_size = 0, union_size = 0, same_count;

            calculateIntersectionAndUnionSizes( it1->lcp_cores, it2->lcp_cores, intersection_size, union_size);

            double jaccard_similarity = calculateJaccardSimilarity(intersection_size, union_size);
            double dice_similarity = calculateDiceSimilarity(intersection_size, it1->lcp_cores.size(), it2->lcp_cores.size());
            double distance_similarity = calculateDistanceSimilarity(it1->lcp_cores, it2->lcp_cores, 1, 1, same_count);
            
            std::cout << "Similarity metrics for " << it1->infilename << " and " << it2->infilename << std::endl;

            std::cout << "Intersection Size: " << intersection_size << std::endl;
            std::cout << "Union Size: " << union_size << std::endl;
            std::cout << "Same Core Count: " << same_count << std::endl;
            std::cout << std::endl;

            std::cout << "Jaccard Similarity: " << jaccard_similarity << std::endl;
            std::cout << "Dice Similarity: " << dice_similarity << std::endl;
            std::cout << "Distance Based Similarity: " << distance_similarity << std::endl;
            std::cout << std::endl;

            dice[it1 - arguments.begin()][it2 - arguments.begin()] = dice_similarity;
            jaccard[it1 - arguments.begin()][it2 - arguments.begin()] = jaccard_similarity;
            distance[it1 - arguments.begin()][it2 - arguments.begin()] = distance_similarity;
            dice[it2 - arguments.begin()][it1 - arguments.begin()] = dice_similarity;
            jaccard[it2 - arguments.begin()][it1 - arguments.begin()] = jaccard_similarity;
            distance[it2 - arguments.begin()][it1 - arguments.begin()] = distance_similarity;
        }
    }

    std::cout << std::setprecision(15);
    
    std::cout << "Dice Matrix" << std::endl;
    for(size_t i = 0; i < arguments.size(); i++ ) {
        for(size_t j = 0; j < arguments.size()-1; j++ ) {
            std::cout << dice[i][j] << ",";
        }
        std::cout << distance[i][arguments.size()-1] << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Jaccard Matrix" << std::endl;
    for(size_t i = 0; i < arguments.size(); i++ ) {
        for(size_t j = 0; j < arguments.size()-1; j++ ) {
            std::cout << jaccard[i][j] << ",";
        }
        std::cout << distance[i][arguments.size()-1] << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Distance Based Matrix" << std::endl;
    for(size_t i = 0; i < arguments.size(); i++ ) {
        for(size_t j = 0; j < arguments.size()-1; j++ ) {
            std::cout << distance[i][j] << ",";
        }
        std::cout << distance[i][arguments.size()-1] << std::endl;
    }
    std::cout << std::endl;

    return 0;
};
