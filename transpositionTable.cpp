//
// Created by pankaj on 27/9/16.
//

#include "transpositionTable.h"
namespace TAK {
    std::unordered_map<unsigned long long, transpositionTableEntry> transpositionTable;
    int collisions;

    void transpositionTableInit() {
        transpositionTable.reserve(20000000);
        collisions = 0;
    }

    void displayTTinfo() {
        std::cout << "TT has " << transpositionTable.size() << " entries and " << collisions <<
        " collisions\n";
    }
}