#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <algorithm>

#ifndef BUFFERSIZE
#define BUFFERSIZE      100000
#endif

bool process( char *str );
bool process( std::string& str );
bool reverseComplement( std::string& sequence );

#endif