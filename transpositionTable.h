//
// Created by pankaj on 27/9/16.
//

#ifndef A3_TRANSPOSITIONTABLE_H
#define A3_TRANSPOSITIONTABLE_H

#include <limits>
#include <unordered_map>
#include "boardstate.h"
namespace TAK {
    extern int scale;

    class transpositionTableEntry {
    public:
        transpositionTableEntry() : lower_bound(std::numeric_limits<int>::min()),
                                    upper_bound(std::numeric_limits<int>::max()), depth(-1), bm(-1), generation(-1),
                                    hash(0) { }

        int lower_bound;
        int upper_bound;
        int depth;
        move bm;
        int generation;
        unsigned long long hash;
        //unsigned long long WT;
    };

    struct hasher {
        std::size_t operator()(const unsigned long long &l) const {
            return (size_t) ((l >> 32) ^ (l * 1610612741));
        }
    };

    inline size_t hash1(const unsigned long long &l) {
        return (size_t) ((l >> 32) ^ (l * 1610612741));
    }

    inline size_t hash2(const unsigned long long &l) {
        return (size_t) (l ^ ((l >> 32) * 1610612741));
    }

    //extern std::unordered_map<unsigned long long, transpositionTableEntry, hasher> transpositionTable;
    extern transpositionTableEntry *tr;
    extern int collisions;
    extern int dropped;
    extern int currentGen;
    extern size_t sizeOfTable;

    template<int n>
    transpositionTableEntry *getEntry(const boardstate<n> &b, bool create_if_absent = true) {
        unsigned long long h = b.getHash();
        transpositionTableEntry *ans1 = tr + (hash1(h) % sizeOfTable);
        if (ans1->hash == h) {
            ans1->generation = currentGen;
            return ans1;
        }
        transpositionTableEntry *ans2 = tr + (hash2(h) % sizeOfTable);
        if (ans2->hash == h) {
            ans2->generation = currentGen;
            return ans2;
        }
        if (ans1->generation < currentGen) {
            ans1->generation = currentGen;
            ans1->hash = h;
            ans1->depth = std::numeric_limits<int>::min();
            ans1->bm = -1;
            return ans1;
        }
        if (ans2->generation < currentGen) {
            ans2->generation = currentGen;
            ans2->hash = h;
            ans2->depth = std::numeric_limits<int>::min();
            ans2->bm = -1;
            return ans2;
        }
        collisions++;
        if (ans1->depth < 0 && create_if_absent) {
            ans1->generation = currentGen;
            ans1->hash = h;
            ans1->depth = std::numeric_limits<int>::min();
            ans1->bm = -1;
            return ans1;
        }
        if (ans2->depth < 0 && create_if_absent) {
            ans2->generation = currentGen;
            ans2->hash = h;
            ans2->depth = std::numeric_limits<int>::min();
            ans2->bm = -1;
            return ans2;
        }
        dropped++;
        return nullptr;
    }

    inline void clearTable() {
        currentGen++;
        collisions = 0;
        dropped = 0;
    }

    void transpositionTableInit();

    void displayTTinfo();
}
#endif //A3_TRANSPOSITIONTABLE_H
