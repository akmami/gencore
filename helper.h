#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <vector>
#include <algorithm>
#include "lps.h"

#ifndef BUFFERSIZE
#define BUFFERSIZE      100000
#endif

bool process( char *str );
bool process( std::string& str );
bool reverseComplement( std::string& sequence );
void flatten(std::vector<lcp::lps*>& strs, std::vector<uint>& lcp_cores);

#endif