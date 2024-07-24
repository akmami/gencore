#include "init.h"


void printUsage() {
    std::cout << "Usage: ./gencore [OPTIONS] [FILES]" << std::endl << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -r              Read cores from specified files (read mode)" << std::endl;
    std::cout << "                  Usage: ./gencore -r file1.cores file2.cores" << std::endl << std::endl;
    std::cout << "  [fa|fq|bam]     Execute program with specified files in the given format" << std::endl;
    std::cout << "                  Supported formats:" << std::endl;
    std::cout << "                    fa   Read from .fa files" << std::endl;
    std::cout << "                    fq   Read from .fq.gz files" << std::endl;
    std::cout << "                    bam  read from .bam files" << std::endl;
    std::cout << "                  Usage: ./gencore fa ref1.fa ref2.fa" << std::endl;
    std::cout << "                         ./gencore fq reads1.fq.gz reads2.fq.gz" << std::endl;
    std::cout << "                         ./gencore bam aln1.bam aln2.bam" << std::endl << std::endl;
    std::cout << "  -l [level]      Set lcp-level" << std::endl;
    std::cout << "                  Default: 4" << std::endl;
    std::cout << "                  Usage: ./gencore fa ref1.fa ref2.fa -l 4" << std::endl << std::endl;
    std::cout << "  -t [number]     Set number of threads" << std::endl;
    std::cout << "                  Default: 8" << std::endl;
    std::cout << "                  Only works when program is run with fq flag" << std::endl;
    std::cout << "                  Usage: ./gencore fq reads1.fq.gz reads2.fq.gz -t 4" << std::endl << std::endl;
    std::cout << "  -w [filenames]  Store cores processed from input files." << std::endl;
    std::cout << "                  Only works when program is not run with read mode" << std::endl;
    std::cout << "                  Usage: ./gencore fa reads1.fq.gz reads2.fq.gz -w reads1.cores,reads2.cores" << std::endl << std::endl;
};


void parse(int argc, char **argv, args& arguments1, args& arguments2) {

    if ( argc < 4 ) {
        printUsage();
        exit(1);
    }

    int index = 1;
    arguments1.readCores = false;
    arguments1.writeCores = false;
    arguments2.readCores = false;
    arguments2.writeCores = false;
    
    if ( strcmp(argv[index], "-r") == 0 ) {
        arguments1.readCores = true;
        arguments2.readCores = true;
        index++;
    }

    if (!arguments1.readCores) {
        if (index >= argc) {
            std::cerr << "Error: Missing program mode." << std::endl;
            exit(1);
        }
        if ( strcmp(argv[index], "fa") == 0 ) {
            arguments1.mode = FA;
            arguments2.mode = FA;
        } else if ( strcmp(argv[index], "fq") == 0 ) {
            arguments1.mode = FQ;
            arguments2.mode = FQ;
        } else if ( strcmp(argv[index], "bam") == 0 ) {
            arguments1.mode = BAM;
            arguments2.mode = BAM;
        } else {
            std::cerr << "Error: Invalid mode provided." << std::endl;
            exit(1);
        }
        index++;
    }

    if (index >= argc) {
        std::cerr << "Error: Missing filenames." << std::endl;
        exit(1);
    }
    std::string filename1(argv[index]);
    index++;
    arguments1.infilename = filename1;

    if (index >= argc) {
        std::cerr << "Error: Missing second filename." << std::endl;
        exit(1);
    }
    std::string filename2(argv[index]);
    index++;
    arguments2.infilename = filename2;

    arguments1.threadNumber = THREAD_NUMBER;
    arguments1.lcpLevel = 7;
    arguments2.threadNumber = THREAD_NUMBER;
    arguments2.lcpLevel = 7;

    while ( index < argc ) {
        if( strcmp(argv[index], "-t") == 0 ) {
            index++;
            if ( index >= argc ) {
                std::cerr << "Error: Missing value for thread number." << std::endl;
                exit(1);
            }
            try {
                arguments1.threadNumber = std::stoi(argv[index]);
                arguments2.threadNumber = std::stoi(argv[index]);
                if ( arguments1.threadNumber <= 0 ) {
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
                if ( std::stoi(argv[index]) < 0 ) {
                    throw std::invalid_argument("Invalid LCP level");
                }
                arguments1.lcpLevel = std::stoi(argv[index]);
                arguments2.lcpLevel = std::stoi(argv[index]);
            } catch ( const std::invalid_argument& e) {
                std::cerr << "Error: Invalid LCP level provided." << std::endl;
                exit(1);
            }
            index++;
        } else if( strcmp(argv[index], "-w") == 0 ) {
            index++;
            if ( index >= argc ) {
                std::cerr << "Error: Missing output files names." << std::endl;
                exit(1);
            }
            try {
                std::stringstream ss(argv[index]);
                if (!std::getline(ss, arguments1.outfilename, ',')) {
                    throw std::invalid_argument("Failed to read the first part of the string.");
                }
                if (!std::getline(ss, arguments2.outfilename, ',')) {
                    throw std::invalid_argument("Failed to read the second part of the string.");
                }
            } catch ( const std::invalid_argument& e) {
                std::cerr << "Error: Could not parse output filenames." << std::endl;
                exit(1);
            }
            index++;
        }
        
        else {
            std::cerr << "Invalid option provided: " << argv[index] << std::endl;
            index++;
        }
    }
}