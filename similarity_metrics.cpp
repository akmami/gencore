#include "similarity_metrics.h"


void generateMinhashSignature( std::vector<uint>& hash_values ) {
    std::sort(hash_values.begin(), hash_values.end());
};


/**
 * @brief Populates vectors with unique LCP cores and their counts from a sorted vector of LCP cores.
 *
 * This function analyzes a sorted vector of LCP cores, identifying each unique core and
 * counting the number of occurrences of that core within the vector. The unique cores
 * and their counts are then stored in separate vectors. This operation is useful for
 * summarizing the distribution of LCP cores within a dataset, particularly in genomic
 * data analysis where understanding the frequency of certain sequences or patterns can
 * be critical.
 *
 * @param cores The input vector containing sorted LCP cores to be analyzed.
 * @param set An output vector that will contain all LCP cores once
 *                         identified in the input vector.
 * @param counts An output vector that will contain the counts of each unique
 *                        LCP core corresponding to the unique cores in `lcp_cores_set`.
 */
void initializeSetAndCounts( std::vector<uint>& cores, std::vector<uint>& set, std::vector<size_t>& counts ) {
    
    if( cores.empty() ) {
        return;
    }

    int distinct_cores = 1;

    for( size_t i = 1; i < cores.size(); i++ ) {
        if( cores[i] != cores[i - 1] ) {
            distinct_cores++;
        }

        if ( cores[i] > cores[i - 1] ) {
            log(ERROR, "Given LCP cores should be sorted.");
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


/**
 * @brief Calculates the intersection and union sizes between two sets of hashed LCP cores.
 *
 * Given two vectors containing hashed LCP cores from two genomes, this function calculates
 * the sizes of their intersection and union. These sizes are essential for computing
 * similarity metrics between the genomes.
 *
 * @param cores1 A vector containing the hashed LCP cores of the first genome.
 * @param cores2 A vector containing the hashed LCP cores of the second genome.
 * @param interSize A reference to a size_t variable to store the intersection size.
 * @param unionSize A reference to a size_t variable to store the union size.
 */
void calculateIntersectionAndUnionSizes( std::vector<uint>& cores1, std::vector<uint>& cores2, size_t& interSize, size_t& unionSize ) {

    std::vector<uint>::iterator it1 = cores1.begin(), it2 = cores2.begin();
    
    while ( it1 != cores1.end() && it2 != cores2.end() ) {
        if ( *it1 < *it2 ) {
            it1++;
        } else if ( *it1 > *it2 ) {
            it2++;
        } else {
            interSize++;
            it1++;
            it2++;
        }
        unionSize++;
    }

    // count the remaining elements in either vector
    if( it1 != cores1.end() ) {
        unionSize += (cores1.end() - it1);
    }
    else if( it2 != cores2.end() ) {
        unionSize += (cores2.end() - it2);
    }
};


/**
 * @brief Calculates the Jaccard similarity between two genomes.
 *
 * Computes the Jaccard similarity metric based on the intersection and union sizes
 * of hashed LCP cores from two genomes. This metric provides a measure of similarity
 * in terms of shared genomic features.
 *
 * @param interSize The size of the intersection between the two sets of cores.
 * @param unionSize The size of the union between the two sets of cores.
 * @return The Jaccard similarity coefficient as a double.
 */
double calculateJaccardSimilarity( size_t interSize, size_t unionSize ) {
    return static_cast<double>(interSize) / static_cast<double>(unionSize);
};


/**
 * @brief Calculates the Dice similarity between two genomes.
 *
 * Computes the Dice similarity metric based on the intersection size of hashed LCP cores
 * and the sizes of individual core sets from two genomes. This metric is another measure
 * of similarity focusing on shared genomic features.
 *
 * @param interSize The size of the intersection between the two sets of cores.
 * @param first The size of the first set of cores.
 * @param second The size of the second set of cores.
 * @return The Dice similarity coefficient as a double.
 */
double calculateDiceSimilarity( size_t interSize, size_t first, size_t second ) {
    return 2 * static_cast<double>(interSize) / ( static_cast<double>(first) + static_cast<double>(second) );
};


/**
 * @brief Calculates a similarity score between two sets of hashed LCP cores.
 *
 * This function compares two sets of hashed LCP cores, representing genomic sequences
 * analyzed at specified depths, to compute a similarity score. The score is based on
 * the normalized Manhattan distance between the weighted counts of matching LCP cores,
 * adjusted to reflect similarity. This approach considers both the presence and abundance
 * of LCP cores in relation to the analysis depths, offering a nuanced similarity measure
 * suitable for genomic comparisons.
 *
 * @param set1 First set of hashed LCP cores.
 * @param set2 Second set of hashed LCP cores.
 * @param depth1 Depth of analysis for the first set of cores.
 * @param depth2 Depth of analysis for the second set of cores.
 * @return A double representing the similarity score, ranging from 0 (no similarity) to 1 (identical).
 */
double calculateNormalizedVectorSimilarity( std::vector<uint>& set1, std::vector<uint>& set2, double depth1, double depth2 ) {
    
    double numerator = 0.0, denominator = 0.0;
    size_t i = 0, j = 0;

    while( i < set1.size() && j < set2.size() ) {
        if( set1[i] == set2[j] ) {
            numerator += abs(set1[i] * depth2 - set2[j] * depth1);
            denominator += (set1[i] * depth2 + set2[j] * depth1);
            i++;
            j++;
        } else if (set1[i] < set2[j]) {
            numerator += set1[i] * depth2;
            denominator += set1[i] * depth2;
            i++;
        } else {
            numerator += set2[j] * depth1;
            denominator += set2[j] * depth1;
            j++;
        }
    }

    while( i < set1.size() ) {
        numerator += set1[i] * depth2;
        denominator += set1[i] * depth2;
        i++;
    }

    while( j < set2.size() ) {
        numerator += set2[j] * depth1;
        denominator += set2[j] * depth1;
        j++;
    }

    // check for division by zero before returning the result
    return denominator != 0.0 ? 1 - numerator / denominator : 0.0;
};