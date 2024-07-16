#include <vector>
#include <iostream>
#include <thread>
#include <mutex>

#include "args.h"
#include "constant.h"
#include "init.h"
#include "rfasta.h"
#include "rfastq.h"


int main(int argc, char **argv) {

    args arguments = parse(argc, argv);

    // std::cout << "Intersection Size: " << intersection_size << std::endl;
    // std::cout << "Union Size: " << union_size << std::endl;
    // std::cout << "Same Core Count: " << same_count << std::endl;
    // std::cout << std::endl;
    // std::cout << "Jaccard Similarity: " << jaccard_similarity << std::endl;
    // std::cout << "Dice Similarity: " << dice_similarity << std::endl;
    // std::cout << "Distance Based Similarity: " << distance_similarity << std::endl;
    // std::cout << std::endl;

    return 0;
};
