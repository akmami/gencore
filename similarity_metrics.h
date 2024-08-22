#ifndef SIMILARITY_METRICS_H
#define SIMILARITY_METRICS_H

#include <vector>
#include <iterator>
#include <algorithm>
#include "logging.h"


void generateMinhashSignature( std::vector<uint>& hash_values );
void initializeSetAndCounts( std::vector<uint>& lcp_cores, std::vector<uint>& set, std::vector<size_t>& counts );
void calculateIntersectionAndUnionSizes( std::vector<uint>& cores1, std::vector<uint>& cores2, size_t& interSize, size_t& unionSize );
double calculateJaccardSimilarity( size_t interSize, size_t unionSize );
double calculateDiceSimilarity( size_t interSize, size_t first, size_t second );
double calculateNormalizedVectorSimilarity( std::vector<uint>& cores1, std::vector<uint>& cores2, double depth1, double depth2 );

#endif