#include "init.h"


void printUsage() {
    std::cout << "Usage: ./gencore [OPTIONS] [FILES]" << std::endl << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -r              Read cores from specified files" << std::endl;
    std::cout << "                  Usage: ./gencore -r file1.cores file2.cores" << std::endl << std::endl;
    std::cout << "  -w [fa|fq|bam]  Write cores to specified files in the given format" << std::endl;
    std::cout << "                  Supported formats:" << std::endl;
    std::cout << "                    fa   Write cores to .fa files" << std::endl;
    std::cout << "                    fq   Write cores to .fq.gz files" << std::endl;
    std::cout << "                    bam  Write cores to .bam files" << std::endl;
    std::cout << "                  Usage: ./gencore -w fa file1.fa file2.fa" << std::endl;
    std::cout << "                         ./gencore -w fq file1.fq.gz file2.fq.gz" << std::endl;
    std::cout << "                         ./gencore -w bam file1.bam file2.bam" << std::endl << std::endl;
    std::cout << "  -l [level]      Set lcp-level" << std::endl;
    std::cout << "                  Usage: ./gencore -l level" << std::endl << std::endl;
    std::cout << "  -t [number]     Set number of threads" << std::endl;
    std::cout << "                  Usage: ./gencore -t number" << std::endl << std::endl;
};


args& parse(int argc, char **argv) {

    if ( argc < 3 ) {
        std::cerr << "Usage: program [-r|-w] <filename1> <filename2> [-t <thread_number>] [-l <lcp_level>]" << std::endl;
        exit(1);
    }

    args arguments;

    int index = 1;
    arguments.readCores = false;
    arguments.writeCores = false;
    if ( strcmp(argv[1], "-r") == 0 ) {
        arguments.readCores = true;
        index++;
    } else if ( strcmp(argv[1], "-w") == 0 ) {
        arguments.writeCores = true;
        index++;
        if (index >= argc) {
            std::cerr << "Error: Missing output filename." << std::endl;
            exit(1);
        }
        std::string outfilename(argv[index]);
        index++;
        arguments.outfilename = outfilename;
    }

    if (index >= argc) {
        std::cerr << "Error: Missing filenames." << std::endl;
        exit(1);
    }
    std::string filename1(argv[index]);
    index++;
    arguments.infilename1 = filename1;

    if (index >= argc) {
        std::cerr << "Error: Missing second filename." << std::endl;
        exit(1);
    }
    std::string filename2(argv[index]);
    index++;
    arguments.infilename2 = filename2;

    arguments.threadNumber = THREAD_NUMBER;
    arguments.lcpLevel = 7;

    while ( index < argc ) {
        if( strcmp(argv[index], "-t") == 0 ) {
            index++;
            if ( index >= argc ) {
                std::cerr << "Error: Missing value for thread number." << std::endl;
                exit(1);
            }
            try {
                arguments.threadNumber = std::stoi(argv[index]);
                if ( arguments.threadNumber <= 0 ) {
                    throw std::invalid_argument("Invalid thread number");
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid thread number provided." << std::endl;
                exit(1);
            }
            index++;
        } else if( strcmp(argv[index], "-l") == 0 ) {
            index++;
            if ( index >= argc ) {
                std::cerr << "Error: Missing value for LCP level." << std::endl;
                exit(1);
            }
            try {
                arguments.lcpLevel = std::stoi(argv[index]);
                if ( arguments.lcpLevel < 0 ) {
                    throw std::invalid_argument("Invalid LCP level");
                }
            } catch ( const std::invalid_argument& e) {
                std::cerr << "Error: Invalid LCP level provided." << std::endl;
                exit(1);
            }
            index++;
        } else {
            std::cerr << "Invalid option provided: " << argv[index] << std::endl;
            index++;
        }
    }

    return arguments;
}