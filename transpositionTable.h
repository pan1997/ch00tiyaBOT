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
                                    upper_bound(std::numeric_limits<int>::max()), depth(0), bm(-1), generation(-1),
                                    hash(0), WT(0) { }

        int lower_bound;
        int upper_bound;
        int depth;
        move bm;
        int generation;
        unsigned long long hash;
        unsigned long long WT;
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
    transpositionTableEntry *getEntry(const boardstate<n> &b, bool create_if_absent = false) {
        size_t h = hash1(b.getHash());
        size_t index = h % sizeOfTable;
        transpositionTableEntry *ans = tr + index;
        if (ans->hash == h) {
            ans->generation = currentGen;
            return ans;
        }
        else if (ans->generation < currentGen) {
            ans->generation = currentGen;
            ans->hash = h;
            ans->WT = b.getWF();
            ans->depth = std::numeric_limits<int>::min();
            ans->bm = -1;
            return ans;
        } else{
            collisions++;
            index=(index+hash2(b.getHash()))%sizeOfTable;
            ans=tr+index;
            if(ans->hash==h){
                ans->generation=currentGen;
                return ans;
            }
            else if (ans->generation < currentGen) {
                ans->generation = currentGen;
                ans->hash = h;
                ans->WT = b.getWF();
                ans->depth = std::numeric_limits<int>::min();
                ans->bm = -1;
                return ans;
            }
            else dropped++;
            return nullptr;
        }

        /*if (transpositionTable.count(b.getHash())) {
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
        }*/
    }

    inline void clearTable() {
        currentGen++;
        collisions=0;
        dropped=0;
        //transpositionTable.clear();
    }

    void transpositionTableInit();

    void displayTTinfo();
}

#endif //A3_TRANSPOSITIONTABLE_H
