#include "fileio.h"


void save(std::string filename, std::vector<lcp::lps*>& cores) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cerr << "Error opening file for writing" << std::endl;
        return;
    }

    std::cout << "Saving cores to file " << filename << std::endl;

    size_t size = cores.size();

    out.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for ( std::vector<lcp::lps*>::iterator it = cores.begin(); it != cores.end(); it++ ) {
        (*it)->write(out);
    }
};


void load(std::string filename, std::vector<lcp::lps*>& cores) {
    std::ifstream in(filename, std::ios::binary);

    if (!in) {
        std::cerr << "Error opening file for reading" << std::endl;
        return;
    }

    std::cout << "Loading cores from file " << filename << std::endl;

    size_t size;

    in.read(reinterpret_cast<char*>(&size), sizeof(size));

    cores.reserve(size);

    for ( size_t i = 0; i < size; i++ ) {
        lcp::lps *str = new lcp::lps(in);
        cores.push_back(str);
    }
};


void read_from_file( args& arguments) {

    std::vector<lcp::lps*> strs;

    load( arguments.infilename, strs );

    flatten( strs, arguments.lcp_cores );
};
