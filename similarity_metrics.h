#ifndef SIMILARITY_METRICS_H
#define SIMILARITY_METRICS_H

#include <vector>
#include <iterator>
#include <algorithm>

void fillValuesAndCounts( std::vector<uint>& lcp_cores, std::vector<size_t>& unique_lcp_cores, std::vector<size_t>& lcp_cores_count );
void generateMinhashSignature( std::vector<uint>& hash_values );
void calculateIntersectionAndUnionSizes( std::vector<uint>& hashed_cores1, std::vector<uint>& hashed_cores2, size_t& intersection_size, size_t& union_size );
double calculateJaccardSimilarity( size_t intersection_size, size_t union_size );
double calculateDiceSimilarity( size_t intersection_size, size_t first_size, size_t second_size );
double calculateDistanceSimilarity( std::vector<uint>& hashed_cores1, std::vector<uint>& hashed_cores2, double depth_1, double depth_2, size_t& same_count );

#endif