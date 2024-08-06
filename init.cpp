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
    std::cout << "  -c              Execute program with a file that contains names of the input filenames" << std::endl;
    std::cout << "                  Usage: ./gencore -fa -c files.txt" << std::endl << std::endl;
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
    std::cout << "  -v              Verbose" << std::endl;
    std::cout << "                  Default: false" << std::endl;
    std::cout << "                  Usage: ./gencore fa reads1.fq.gz reads2.fq.gz -v" << std::endl << std::endl;
};


void parse(int argc, char **argv, std::vector<args>& arguments) {

    if ( argc < 4 ) {
        printUsage();
        exit(1);
    }

    int index = 1;
    bool readCores = false;
    bool writeCores = false;
    
    if ( strcmp(argv[index], "-r") == 0 ) {
        readCores = true;
        index++;
    }

    program_mode mode = FA;

    if (!readCores) {
        if (index >= argc) {
            std::cerr << "Error: Missing program mode." << std::endl;
            exit(1);
        }
        if ( strcmp(argv[index], "fa") == 0 ) {
            mode = FA;
        } else if ( strcmp(argv[index], "fq") == 0 ) {
            mode = FQ;
        } else if ( strcmp(argv[index], "bam") == 0 ) {
            mode = BAM;
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

    if ( strcmp(argv[index], "-c") == 0 ) {
        index++;

        if (index >= argc) {
            std::cerr << "Error: Missing filename." << std::endl;
            exit(1);
        }

        std::string filename(argv[index]), line;
        index++;

        std::fstream file;
        file.open( filename, std::ios::in );

        if ( file.is_open() ) {  
            while (getline(file, line)) {
                args ar;
                ar.infilename = line;
                arguments.push_back(ar);
            }
        }
        file.close();
    } else {
        std::string filename1(argv[index]);
        index++;
        
        args ar1;
        ar1.infilename = filename1;
        arguments.push_back(ar1);

        if (index >= argc) {
            std::cerr << "Error: Missing second filename." << std::endl;
            exit(1);
        }
        std::string filename2(argv[index]);
        index++;

        args ar2;
        ar2.infilename = filename2;
        arguments.push_back(ar2);
    }

    for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
        it->readCores = readCores;
        it->writeCores = writeCores;
        it->mode = mode;
        it->threadNumber = THREAD_NUMBER;
        it->lcpLevel = 7;
        it->verbose = VERBOSE;
    } 

    while ( index < argc ) {
        if( strcmp(argv[index], "-t") == 0 ) {
            index++;
            if ( index >= argc ) {
                std::cerr << "Error: Missing value for thread number." << std::endl;
                exit(1);
            }
            try {
                for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
                    it->threadNumber = std::stoi(argv[index]);
                }                
                if ( arguments[0].threadNumber <= 0 ) {
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
                for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
                    it->lcpLevel = std::stoi(argv[index]);
                } 
            } catch ( const std::invalid_argument& e) {
                std::cerr << "Error: Invalid LCP level provided." << std::endl;
                exit(1);
            }
            index++;
        } else if( strcmp(argv[index], "-w") == 0 ) {
            index++;
            for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
                it->writeCores = true;
            }
            if ( index >= argc ) {
                std::cerr << "Error: Missing output files names." << std::endl;
                exit(1);
            }
            try {
                std::stringstream ss(argv[index]);
                for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
                    if (!std::getline(ss, it->outfilename, ',')) {
                        throw std::invalid_argument("Failed to read the first part of the string.");
                    }
                }
            } catch ( const std::invalid_argument& e) {
                std::cerr << "Error: Could not parse output filenames." << std::endl;
                exit(1);
            }
            index++;
        } else if( strcmp(argv[index], "-v") == 0 ) {
            index++;
            for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
                it->verbose = true;
            }
        } else {
            std::cerr << "Invalid option provided: " << argv[index] << std::endl;
            index++;
        }
    }

   
    for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
        it->readCores && std::cout << "readCores: " << it->readCores << std::endl;
        !(it->readCores) && std::cout << "mode: " << it->mode << std::endl;
        std::cout << "infilename: " << it->infilename << std::endl;
        it->writeCores && std::cout << "writeCores: " << it->writeCores << std::endl;
        it->writeCores && std::cout << "outfilename: " << it->outfilename << std::endl;
        std::cout << "threadNumber: " << it->threadNumber << std::endl;
        std::cout << "lcpLevel: " << it->lcpLevel << std::endl;
        std::cout << std::endl;
    }
    
}
