#include "similarity_metrics.h"


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
 * @param lcp_cores The input vector containing sorted LCP cores to be analyzed.
 * @param unique_lcp_cores An output vector that will contain all unique LCP cores
 *                         identified in the input vector.
 * @param lcp_cores_count An output vector that will contain the counts of each unique
 *                        LCP core corresponding to the unique cores in `unique_lcp_cores`.
 */
void fillValuesAndCounts( std::vector<uint>& lcp_cores, std::vector<size_t>& unique_lcp_cores, std::vector<size_t>& lcp_cores_count ) {
    
    if( lcp_cores.empty() ) {
        return;
    }

    int unique_lcp_core_count = 1;

    for( size_t i = 1; i < lcp_cores.size(); i++ ) {
        if( lcp_cores[i] != lcp_cores[i - 1] ) {
            unique_lcp_core_count++;
        }
    }
    
    // pre-allocate memory for efficiency
    unique_lcp_cores.reserve( unique_lcp_core_count ); 
    lcp_cores_count.reserve( unique_lcp_core_count );
    
    unique_lcp_cores.push_back(lcp_cores[0]);
    size_t currentCount = 1;
    
    for (size_t i = 1; i < lcp_cores.size(); ++i) {
        if (lcp_cores[i] == lcp_cores[i - 1]) {
            currentCount++;
        } else {
            lcp_cores_count.push_back(currentCount);
            unique_lcp_cores.push_back(lcp_cores[i]);
            currentCount = 1;
        }
    }

    // add the count for the last element
    lcp_cores_count.push_back(currentCount); 
};


void generateMinhashSignature( std::vector<uint>& hash_values ) {
    std::sort(hash_values.begin(), hash_values.end());
};


/**
 * @brief Calculates the intersection and union sizes between two sets of hashed LCP cores.
 *
 * Given two vectors containing hashed LCP cores from two genomes, this function calculates
 * the sizes of their intersection and union. These sizes are essential for computing
 * similarity metrics between the genomes.
 *
 * @param hashed_cores1 A vector containing the hashed LCP cores of the first genome.
 * @param hashed_cores2 A vector containing the hashed LCP cores of the second genome.
 * @param intersection_size A reference to a size_t variable to store the intersection size.
 * @param union_size A reference to a size_t variable to store the union size.
 */
void calculateIntersectionAndUnionSizes( std::vector<uint>& hashed_cores1, std::vector<uint>& hashed_cores2, size_t& intersection_size, size_t& union_size ) {

    std::vector<uint>::iterator it1 = hashed_cores1.begin(), it2 = hashed_cores2.begin();
    
    while ( it1 != hashed_cores1.end() && it2 != hashed_cores2.end() ) {
        if ( *it1 < *it2 ) {
            it1++;
        } else if ( *it1 > *it2 ) {
            it2++;
        } else {
            intersection_size++;
            it1++;
            it2++;
        }
        union_size++;
    }

    // count the remaining elements in either vector
    while ( it1 != hashed_cores1.end() ) {
        it1++;
        union_size++;
    }
    while ( it2 != hashed_cores2.end() ) {
        it2++;
        union_size++;
    }
};


/**
 * @brief Calculates the Jaccard similarity between two genomes.
 *
 * Computes the Jaccard similarity metric based on the intersection and union sizes
 * of hashed LCP cores from two genomes. This metric provides a measure of similarity
 * in terms of shared genomic features.
 *
 * @param intersection_size The size of the intersection between the two sets of cores.
 * @param union_size The size of the union between the two sets of cores.
 * @return The Jaccard similarity coefficient as a double.
 */
double calculateJaccardSimilarity( size_t intersection_size, size_t union_size ) {
    return static_cast<double>(intersection_size) / static_cast<double>(union_size);
};


/**
 * @brief Calculates the Dice similarity between two genomes.
 *
 * Computes the Dice similarity metric based on the intersection size of hashed LCP cores
 * and the sizes of individual core sets from two genomes. This metric is another measure
 * of similarity focusing on shared genomic features.
 *
 * @param intersection_size The size of the intersection between the two sets of cores.
 * @param first_size The size of the first set of cores.
 * @param second_size The size of the second set of cores.
 * @return The Dice similarity coefficient as a double.
 */
double calculateDiceSimilarity( size_t intersection_size, size_t first_size, size_t second_size ) {
    return 2 * static_cast<double>(intersection_size) / ( static_cast<double>(first_size) + static_cast<double>(second_size) );
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
 * @param hashed_cores1 First set of hashed LCP cores.
 * @param hashed_cores2 Second set of hashed LCP cores.
 * @param depth_1 Depth of analysis for the first set of cores.
 * @param depth_2 Depth of analysis for the second set of cores.
 * @param same_count Reference to store the count of matching cores across both sets.
 * @return A double representing the similarity score, ranging from 0 (no similarity) to 1 (identical).
 */
double calculateDistanceSimilarity( std::vector<uint>& hashed_cores1, std::vector<uint>& hashed_cores2, double depth_1, double depth_2, size_t& same_count ) {

    std::vector<size_t> unique_lcp_cores_1, unique_lcp_cores_2;
    std::vector<size_t> lcp_cores_count_1, lcp_cores_count_2;

    fillValuesAndCounts(hashed_cores1, unique_lcp_cores_1, lcp_cores_count_1);
    fillValuesAndCounts(hashed_cores2, unique_lcp_cores_2, lcp_cores_count_2);
    
    double numerator = 0.0;
    double denominator = 0.0;
    size_t i = 0, j = 0;

    same_count = 0;

    int diff = 0;

    while( i < unique_lcp_cores_1.size() && j < unique_lcp_cores_2.size() ) {
        if( unique_lcp_cores_1[i] == unique_lcp_cores_2[j] ) {
            diff = lcp_cores_count_1[i] * depth_2 - lcp_cores_count_2[j] * depth_1;
            numerator += diff >= 0 ? diff : -diff;
            denominator += (lcp_cores_count_1[i] * depth_2 + lcp_cores_count_2[j] * depth_1);
            same_count += lcp_cores_count_1[i] < lcp_cores_count_2[j] ? lcp_cores_count_1[i] : lcp_cores_count_2[j];
            i++;
            j++;
        } else if (unique_lcp_cores_1[i] < unique_lcp_cores_2[j]) {
            numerator += lcp_cores_count_1[i] * depth_2;
            denominator += lcp_cores_count_1[i] * depth_2;
            i++;
        } else {
            numerator += lcp_cores_count_2[j] * depth_1;
            denominator += lcp_cores_count_2[j] * depth_1;
            j++;
        }
    }

    while( i < unique_lcp_cores_1.size() ) {
        numerator += lcp_cores_count_1[i] * depth_2;
        denominator += lcp_cores_count_1[i] * depth_2;
        i++;
    }

    while( j < unique_lcp_cores_2.size() ) {
        numerator += lcp_cores_count_2[j] * depth_1;
        denominator += lcp_cores_count_2[j] * depth_1;
        j++;
    }

    // check for division by zero before returning the result
    return denominator != 0.0 ? 1 - numerator / denominator : 0.0;
};