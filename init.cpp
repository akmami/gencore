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
    std::cout << "  -i [filename]   Execute program with a file that contains names of the input filenames" << std::endl;
    std::cout << "                  Usage: ./gencore -fa -i files.txt" << std::endl << std::endl;
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
    std::cout << "  -o [filename]   Filename where output filenames of cores is given" << std::endl;
    std::cout << "                  Usage ./gencore fa -i infiles.txt -o outfiles.txt" << std::endl << std::endl;
    std::cout << "  -p [prefix]     Prefix for the output of the similarity matrices results'" << std::endl;
    std::cout << "                  Default: gc" << std::endl << std::endl;
    std::cout << "                  Usage ./gencore fa -i infiles.txt -o outfiles.txt -p primates" << std::endl << std::endl;
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
    size_t lcpLevel = 7;
    size_t threadNumber = THREAD_NUMBER;
    
    if ( strcmp(argv[index], "-r") == 0 ) {
        readCores = true;
        index++;
    }

    program_mode mode = FA;

    if (!readCores) {
        if (index >= argc) {
            log(ERROR, "Missing program mode.");
            exit(1);
        }
        if ( strcmp(argv[index], "fa") == 0 ) {
            mode = FA;
        } else if ( strcmp(argv[index], "fq") == 0 ) {
            mode = FQ;
        } else if ( strcmp(argv[index], "bam") == 0 ) {
            mode = BAM;
        } else {
            log(ERROR, "Invalid mode provided.");
            exit(1);
        }
        index++;
    }

    if (index >= argc) {
        log(ERROR, "Missing filenames.");
        exit(1);
    }

    if ( strcmp(argv[index], "-i") == 0 ) {
        index++;

        if (index >= argc) {
            log(ERROR, "Missing filename.");
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
        } else {
            log(ERROR, "Couldn't open %s", filename.c_str());
            exit(1);
        }
        file.close();

        if ( arguments.size() < 2 ) {
            log(ERROR, "There should be at least 2 file in %s", filename.c_str());
            exit(1);
        }
    } else {
        std::string filename1(argv[index]);
        index++;
        
        args ar1;
        ar1.infilename = filename1;
        arguments.push_back(ar1);

        if (index >= argc) {
            log(ERROR, "Missing second filename.");
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
        it->threadNumber = threadNumber;
        it->lcpLevel = lcpLevel;
        it->verbose = VERBOSE;
        it->prefix = PREFIX;
    } 

    while ( index < argc ) {
        if( strcmp(argv[index], "-t") == 0 ) {
            index++;
            if ( index >= argc ) {
                log(ERROR, "Missing value for thread number.");
                exit(1);
            }
            try {
                for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
                    it->threadNumber = std::stoi(argv[index]);
                }                
                if ( arguments[0].threadNumber <= 0 ) {
                    throw std::invalid_argument("Invalid thread number");
                }
                threadNumber = std::stoi(argv[index]);
            } catch (const std::invalid_argument& e) {
                log(ERROR, "Invalid thread number provided.");
                exit(1);
            }
            index++;
        } else if( strcmp(argv[index], "-l") == 0 ) {
            index++;
            if ( index >= argc ) {
                log(ERROR, "Missing value for LCP level.");
                exit(1);
            }
            try {
                if ( std::stoi(argv[index]) < 0 ) {
                    throw std::invalid_argument("Invalid LCP level");
                }
                for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
                    it->lcpLevel = std::stoi(argv[index]);
                } 
                lcpLevel = std::stoi(argv[index]);
            } catch ( const std::invalid_argument& e) {
                log(ERROR, "Invalid LCP level provided.");
                exit(1);
            }
            index++;
        } else if( strcmp(argv[index], "-w") == 0 ) {
            index++;
            for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
                it->writeCores = true;
            }
            if ( index >= argc ) {
                log(ERROR, "Missing output files names.");
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
                log(ERROR, "Could not parse output filenames.");
                exit(1);
            }
            index++;
        } else if( strcmp(argv[index], "-o") == 0 ) {            
            index++;

            if (index >= argc) {
                log(ERROR, "Missing filename.");
                exit(1);
            }

            std::string filename(argv[index]);
            index++;

            std::fstream file;
            file.open( filename, std::ios::in );

            if ( file.is_open() ) {  
                for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
                    if (!getline(file, it->outfilename)) {
                        log(ERROR, "Number of input file should match output filename count.");
                        exit(1);
                    }
                    it->writeCores = true;
                }
            } else {
                log(ERROR, "Couldn't open %s", filename.c_str());
                exit(1);
            }
            file.close();
        } else if( strcmp(argv[index], "-p") == 0 ) {            
            index++;

            if (index >= argc) {
                log(ERROR, "Missing prefix.");
                exit(1);
            }

            std::string prefix(argv[index]);
            index++;

            for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
                it->prefix = prefix;
            }
        } else if( strcmp(argv[index], "-v") == 0 ) {
            index++;
            for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
                it->verbose = true;
            }
        } else {
            log(ERROR, "Invalid option provided: %s", argv[index]);
            exit(1);
        }
    }

    if( readCores ) { 
        log(INFO, "Reading cores from file.");
    }
    log(INFO, "Thread number: %d", threadNumber);
    log(INFO, "LCP level: %d", lcpLevel);

    for ( std::vector<args>::iterator it = arguments.begin(); it < arguments.end(); it++ ) {
        log(INFO, "infilename: %s, outfilename: %s, prefix: %s", it->infilename.c_str(), it->outfilename.c_str(), it->prefix.c_str());
    }
    
}
