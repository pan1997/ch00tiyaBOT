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

    extern std::unordered_map<unsigned long long, transpositionTableEntry> transpositionTable;
    extern int collisions;

    template<int n>
    transpositionTableEntry *getEntry(const boardstate<n> &b,bool create_if_absent=false) {
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
    inline void clearTable(){
        transpositionTable.clear();
    }
    template<int n>
    void updateEntry(const boardstate<n> &b, int l, int u, int d) {
        if (transpositionTable.count(b.getHash())) {
            transpositionTableEntry *ans = &transpositionTable[b.getHash()];
            if (b.getWF() == ans->WT) {
                ans->lower_bound = l;
                ans->upper_bound = u;
                ans->depth = d;
            }
            else {
                collisions++;
            }
        }
        else {
            transpositionTableEntry *ans = &transpositionTable[b.getHash()];
            ans->WT = b.getWF();
            ans->lower_bound = l;
            ans->upper_bound = u;
            ans->depth = d;
        }
    }
    void transpositionTableInit();
    void displayTTinfo();
}

#endif //A3_TRANSPOSITIONTABLE_H
