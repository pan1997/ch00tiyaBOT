//
// Created by pankaj on 27/9/16.
//

#include "transpositionTable.h"
namespace TAK {
    std::unordered_map<unsigned long long, transpositionTableEntry, hasher> transpositionTable;
    int collisions;

    void transpositionTableInit() {
        transpositionTable.reserve(20000000);
        collisions = 0;
    }

    void displayTTinfo() {
#ifndef ASS
        std::cout << "TT has " << transpositionTable.size() << " entries and " << collisions <<
        " collisions\n";
#else
        std::cerr << "TT has " << transpositionTable.size() << " entries and " << collisions <<
        " collisions\n";
#endif
    }
}
