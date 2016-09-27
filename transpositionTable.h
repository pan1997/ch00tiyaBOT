//
// Created by pankaj on 27/9/16.
//

#ifndef A3_TRANSPOSITIONTABLE_H
#define A3_TRANSPOSITIONTABLE_H

#include <limits>
#include <unordered_map>
#include "boardstate.h"
namespace TAK {
    class transpositionTableEntry {
    public:
        transpositionTableEntry() : lower_bound(std::numeric_limits<int>::min()),
                                    upper_bound(std::numeric_limits<int>::max()), depth(0), bm(-1), WT(0) { }

        int lower_bound;
        int upper_bound;
        int depth;
        move bm;
        unsigned long long WT;
    };

    struct hasher {
        std::size_t operator()(const unsigned long long &l) const {
            return (size_t) (l * 1610612741);
        }
    };

    extern std::unordered_map<unsigned long long, transpositionTableEntry, hasher> transpositionTable;
    extern int collisions;

    template<int n>
    transpositionTableEntry *getEntry(const boardstate<n> &b, bool create_if_absent = false) {
        if (transpositionTable.count(b.getHash())) {
            transpositionTableEntry *ans = &transpositionTable[b.getHash()];
            if (b.getWF() == ans->WT)
                return ans;
            else {
                collisions++;
                return nullptr;
            }
        }
        else {
            if (create_if_absent) {
                transpositionTableEntry *ans = &transpositionTable[b.getHash()];
                ans->WT = b.getWF();
                return ans;
            } else {
                return nullptr;
            }
        }
    }

    inline void clearTable() {
        transpositionTable.clear();
    }

    void transpositionTableInit();

    void displayTTinfo();
}

#endif //A3_TRANSPOSITIONTABLE_H
