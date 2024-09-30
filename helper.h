#ifndef HELPER_H
#define HELPER_H

#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include "logging.h"
#include "lps.h"

#ifndef BUFFERSIZE
#define BUFFERSIZE      100000
#endif


/**
 * @brief Generates the reverse complement of a DNA sequence.
 *
 * This function takes a DNA sequence as input, reverses it, and then replaces each
 * nucleotide with its complement (A <-> T, C <-> G). This operation is commonly used
 * in bioinformatics for DNA sequence analysis, especially when working with palindromic
 * sequences or when preparing to align sequence reads from both strands.
 *
 * @param sequence The DNA sequence to be reversed and complemented.
 * @return Returns a new string containing the reverse complement of the input sequence.
 */
bool reverseComplement( std::string& sequence );

/**
 * @brief Flattens a collection of locally parsed strings into a single vector of core labels.
 * 
 * This function processes a vector of pointers to `lcp::lps` (Locally Consisted Parsing) structures
 * and extracts the labels from their associated cores, appending them to the output vector `lcp_cores`.
 * The resulting flattened vector contains the labels of all `core` elements, maintaining the same 
 * order as they appear in the original structures.
 * 
 * @param strs A reference to a vector of pointers to `lcp::lps` structures, each containing a collection of cores.
 * @param lcp_cores A reference to a vector of 32-bit unsigned integers that will be populated with core labels.
 *                  This vector is modified in-place, and its capacity is pre-allocated based on the total number 
 *                  of cores across all locally parsed strings.
 */
void flatten(std::vector<lcp::lps*>& strs, std::vector<uint32_t>& lcp_cores);

/**
 * @brief Sorts the provided vector of hash values in ascending order.
 * 
 * This function modifies the input vector `hash_values` by sorting it in-place
 * using the standard library's `std::sort` algorithm. The resulting vector 
 * will contain the same values arranged in ascending order.
 * 
 * @param hash_values A reference to a vector of 32-bit unsigned integers 
 *        representing hash values. The vector is sorted in-place.
 */
void generateSignature( std::vector<uint32_t>& hash_values );

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
 *                        LCP core corresponding to the unique cores in `set`.
 */
void initializeSetAndCounts( std::vector<uint32_t>& lcp_cores, std::vector<uint32_t>& set, std::vector<size_t>& counts );

#endif