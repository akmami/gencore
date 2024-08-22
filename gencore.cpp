#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <iomanip>
#include <fstream>

#include "args.h"
#include "init.h"
#include "rfasta.h"
#include "rfastq.h"
#include "rbam.h"
#include "similarity_metrics.h"


int main(int argc, char **argv) {

    bool calculateSet = true;

    std::vector<args> arguments;
    parse(argc, argv, arguments);

    bool readCores = arguments[0].readCores;
    bool verbose = arguments[0].verbose;

    lcp::init_coefficients(verbose);

    if ( readCores ) {
        read_cores( arguments );
    } else {
        program_mode mode = arguments[0].mode;
        if ( mode == FA ) {
            read_fastas( arguments );
        } else if ( mode == FQ ) {
            for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
                read_fastq( *it );
            }
        } else if ( mode == BAM ) {
            for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
                read_bam( *it );
            }
        } else {
            throw std::invalid_argument("Invalid program mode provided");
            exit(1);
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
            
            std::vector<uint> set1, set2;
            std::vector<size_t> counts1, counts2;
            initializeSetAndCounts( it1->lcp_cores, set1, counts1 );
            initializeSetAndCounts( it2->lcp_cores, set2, counts2 );

            size_t interSize = 0, unionSize = 0;

            if ( calculateSet ) {
                calculateIntersectionAndUnionSizes( set1, set2, interSize, unionSize);
            }
            else {
                calculateIntersectionAndUnionSizes( it1->lcp_cores, it2->lcp_cores, interSize, unionSize);
            }
            
            double depth1 = 1;
            double depth2 = static_cast<double>(it2->size) / static_cast<double>(it1->size);
            
            double jaccard_similarity = calculateJaccardSimilarity(interSize, unionSize);
            double dice_similarity = calculateDiceSimilarity(interSize, it1->lcp_cores.size(), it2->lcp_cores.size());
            double distance_similarity = calculateNormalizedVectorSimilarity(set1, set2, depth1, depth2);

            if ( verbose ) {
                log(INFO, "Similarity metrics for %s and %s", it1->infilename.c_str(), it2->infilename.c_str());
                log(INFO, "Intersection Size: %d, Union Size: %d", interSize, unionSize);
                log(INFO, "Depth1: %f, Depth 2: %f", depth1, depth2);
            }

            dice[it1 - arguments.begin()][it2 - arguments.begin()] = dice_similarity;
            jaccard[it1 - arguments.begin()][it2 - arguments.begin()] = jaccard_similarity;
            distance[it1 - arguments.begin()][it2 - arguments.begin()] = distance_similarity;
            dice[it2 - arguments.begin()][it1 - arguments.begin()] = dice_similarity;
            jaccard[it2 - arguments.begin()][it1 - arguments.begin()] = jaccard_similarity;
            distance[it2 - arguments.begin()][it1 - arguments.begin()] = distance_similarity;
        }
    }

    std::fstream dice_out, jaccard_out, distance_out;
    dice_out.open( arguments[0].prefix + ".dice", std::ios::out );
    jaccard_out.open( arguments[0].prefix + ".jaccard", std::ios::out );
    distance_out.open( arguments[0].prefix + ".ns", std::ios::out );

    if ( dice_out.is_open() ) {  
        dice_out << std::setprecision(15);
        for(size_t i = 0; i < arguments.size(); i++ ) {
            for(size_t j = 0; j < arguments.size()-1; j++ ) {
                dice_out << dice[i][j] << ",";
            }
            dice_out << dice[i][arguments.size()-1] << std::endl;
        }
        dice_out.close();
    }
    
    if ( jaccard_out.is_open() ) {  
        jaccard_out << std::setprecision(15);
        for(size_t i = 0; i < arguments.size(); i++ ) {
            for(size_t j = 0; j < arguments.size()-1; j++ ) {
                jaccard_out << jaccard[i][j] << ",";
            }
            jaccard_out << jaccard[i][arguments.size()-1] << std::endl;
        }
        jaccard_out.close();
    }

    if ( distance_out.is_open() ) {  
        distance_out << std::setprecision(15);
        for(size_t i = 0; i < arguments.size(); i++ ) {
            for(size_t j = 0; j < arguments.size()-1; j++ ) {
                distance_out << distance[i][j] << ",";
            }
            distance_out << distance[i][arguments.size()-1] << std::endl;
        }
        distance_out.close();
    }

    return 0;
};
