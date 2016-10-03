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
        size_t index1 = h % sizeOfTable;
        transpositionTableEntry *ans1 = tr + index1;
        transpositionTableEntry *ans2 = tr + (hash2(b.getHash()) % sizeOfTable);
        if (ans1->hash == h) {
            ans1->generation = currentGen;
            return ans1;
        }
        else if (ans2->hash == h) {
            ans2->generation = currentGen;
            return ans2;
        }
        else if (ans1->generation < currentGen) {
            ans1->generation = currentGen;
            ans1->hash = h;
            ans1->WT = b.getWF();
            ans1->depth = std::numeric_limits<int>::min();
            ans1->bm = -1;
            return ans1;
        }
        else {
            collisions++;
            if (ans2->generation < currentGen) {
                ans2->generation = currentGen;
                ans2->hash = h;
                ans2->WT = b.getWF();
                ans2->depth = std::numeric_limits<int>::min();
                ans2->bm = -1;
                return ans2;
            }
            else {
                dropped++;
                return nullptr;
            }
        }
    }
    /*
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
        }
    }*/
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
