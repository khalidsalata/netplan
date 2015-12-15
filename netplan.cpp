#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "UndirectedGraph.hpp"

/**
 * Entry point into the netplan program.
 *
 * Usage:
 *   ./netplan infile
 *
 */

using namespace std;

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " infile" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::ifstream in(argv[1]);
    if (!in) {
        std::cerr << "Unable to open file for reading." << std::endl;
        return EXIT_FAILURE;
    }

    if(in.peek() == std::ifstream::traits_type::eof()){
      for(int i = 0; i < 6; i++){
        cout << 0 << endl;}
      return EXIT_SUCCESS;
    }

    
    UndirectedGraph* ug = new UndirectedGraph();

    int totalCost = ug->getTotalCost(in);
    cout << totalCost << endl;
    in.close();
    in.open(argv[1]);
    string starter = ug->build(in);
 

    int cheapCost = ug->getCheapestNetwork();
    cout << cheapCost << endl;

    int difference = totalCost - cheapCost;
    cout << difference << endl;

    int totalTransit = ug->findTotalTransit();

    cout << totalTransit << endl;

    int MSTTransit = ug->findMSTTransit();

    cout << MSTTransit << endl;

    cout << MSTTransit - totalTransit << endl;

    return EXIT_SUCCESS;
}
