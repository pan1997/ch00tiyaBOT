//
// Created by pankaj on 27/9/16.
//

#include "transpositionTable.h"
namespace TAK {
    int collisions;
    transpositionTableEntry *tr=nullptr;
    int currentGen;
    int dropped;
    size_t sizeOfTable;

    void transpositionTableInit() {
        sizeOfTable = 150000000;
        //sizeOfTable=20000000;
        if(tr)
            delete[]tr;
        tr = new transpositionTableEntry[sizeOfTable];
        currentGen = -1;
        //transpositionTable.reserve(40000000);
        collisions = 0;
        dropped = 0;
    }

    void displayTTinfo() {
#ifndef ASS
        std::cout << "TT has ? entries and " << collisions <<
        " collisions and "<<dropped<<" drops"<<100*dropped/(collisions+1)<<"%\n";
#else
        std::cerr << "TT has ? entries and " << collisions <<
        " collisions and " << dropped << " drops=(" << 100ULL * dropped / (collisions + 1) << "%)\n";
#endif
    }
}