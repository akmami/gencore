#include "similarity_metrics.h"


void calculateIntersectionAndUnionSizes( const struct targs& argument1, const struct targs& argument2, const struct pargs& program_arguments, size_t& interSize, size_t& unionSize ) {

    std::vector<uint32_t>::const_iterator core1 = argument1.cores.begin(), core2 = argument2.cores.begin();
    std::vector<size_t>::const_iterator count1 = argument1.counts.begin(), count2 = argument2.counts.begin();
    interSize = 0;
    unionSize = 0;

    while ( core1 != argument1.cores.end() && core2 != argument2.cores.end() ) {
        if ( *core1 < *core2 ) {
            core1++;
            count1++;
        } else if ( *core1 > *core2 ) {
            core2++;
            count2++;
        } else {
            if ( program_arguments.type == SET ) {
                interSize++;
            } else {
                interSize += std::min(*count1, *count2);
            }
            core1++;
            count1++;
            core2++;
            count2++;
        }

        if ( program_arguments.type == SET ) {
            unionSize++;
        } else {
            unionSize += std::max(*count1, *count2);
        }
    }

    // count the remaining elements in either vector
    if ( program_arguments.type == SET ) {
        if( count1 != argument1.counts.end() ) {
            unionSize += argument1.counts.end() - count1;
        }
        else if( count2 != argument2.counts.end() ) {
            unionSize += argument2.counts.end() - count2;
        }
    } else {
        while ( count1 != argument1.counts.end() ) {
            unionSize += *count1;
            count1++;
        }
        while ( count2 != argument2.counts.end() ) {
            unionSize += *count2;
            count2++;
        }
    }
};


double calculateJaccardSimilarity( size_t interSize, size_t unionSize ) {
    return static_cast<double>(interSize) / static_cast<double>(unionSize);
};


double calculateDiceSimilarity( const size_t interSize, const struct targs& argument1, const struct targs& argument2, const struct pargs& program_arguments ) {
    
    double size1 = 0, size2 = 0;

    if ( program_arguments.type == SET ) {
        size1 += argument1.cores.size();
        size2 += argument2.cores.size();
    } else {
        
        std::vector<size_t>::const_iterator count1 = argument1.counts.begin(), count2 = argument2.counts.begin();
        while ( count1 != argument1.counts.end() ) {
            size1 += (*count1);
            count1++;
        }
        while ( count2 != argument2.counts.end() ) {
            size2 += (*count2);
            count2++;
        }
        
    }
    
    return 2 * static_cast<double>(interSize) / ( size1 + size2 );
};


double calculateNormalizedVectorSimilarity( const struct targs& argument1, const struct targs& argument2 ) {
    
    std::vector<uint32_t>::const_iterator core1 = argument1.cores.begin(), core2 = argument2.cores.begin();
    std::vector<size_t>::const_iterator count1 = argument1.counts.begin(), count2 = argument2.counts.begin();
    
    double numerator = 0.0, denominator = 0.0;
    double depth1 = 1, depth2 = static_cast<double>(argument2.size) / static_cast<double>(argument1.size);
    
    while ( core1 != argument1.cores.end() && core2 != argument2.cores.end() ) {
        if ( *core1 < *core2 ) {
            numerator += (*count1) * depth2;
            denominator += (*count1) * depth2;
            core1++;
            count1++;
        } else if ( *core1 > *core2 ) {
            numerator += (*count2) * depth1;
            denominator += (*count2) * depth1;
            core2++;
            count2++;
        } else {
            numerator += abs((*count1) * depth2 - (*count2) * depth1);
            denominator += ((*count1) * depth2 + (*count2) * depth1);
            core1++;
            count1++;
            core2++;
            count2++;
        }
    }

    while( count1 < argument1.counts.end() ) {
        numerator += (*count1) * depth2;
        denominator += (*count1) * depth2;
        count1++;
    }

    while( count2 < argument2.counts.end() ) {
        numerator += (*count2) * depth1;
        denominator += (*count2) * depth1;
        count2++;
    }

    // check for division by zero before returning the result
    return denominator != 0.0 ? 1 - numerator / denominator : 0.0;
};