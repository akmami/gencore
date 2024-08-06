#include "fileio.h"


void save(args& arguments, std::vector<lcp::lps*>& cores) {
    std::ofstream out(arguments.outfilename, std::ios::binary);
    if (!out) {
        std::cerr << "Error opening file for writing" << std::endl;
        return;
    }

    std::cout << "Saving cores to file " << arguments.outfilename << std::endl;

    size_t size = cores.size();

    out.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for ( std::vector<lcp::lps*>::iterator it = cores.begin(); it != cores.end(); it++ ) {
        (*it)->write(out);
    }
};


void load(args& arguments, std::vector<lcp::lps*>& cores) {
    std::ifstream in(arguments.infilename, std::ios::binary);

    if (!in) {
        std::cerr << "Error opening file for reading" << std::endl;
        return;
    }

    std::cout << "Loading cores from file " << arguments.infilename << std::endl;

    size_t size;

    in.read(reinterpret_cast<char*>(&size), sizeof(size));

    cores.reserve(size);

    for ( size_t i = 0; i < size; i++ ) {
        lcp::lps *str = new lcp::lps(in);
        str->deepen(arguments.lcpLevel);

        cores.push_back(str);
    }
};


void read_from_file( args& arguments ) {

    std::vector<lcp::lps*> strs;

    load( arguments, strs );

    flatten( strs, arguments.lcp_cores );

    generateMinhashSignature(arguments.lcp_cores);
};
