//
// Created by pankaj on 20/9/16.
//

#ifndef BASIC_H
#define BASIC_H
#include <iostream>
namespace TAK {


#define row 255ULL
#define col 72340172838076673ULL

    enum player {
        BLACK = 0, WHITE = 1
    };
    enum direction {
        UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3
    };
    enum peice {
        NOP = 0, WHITE_FLAT = 3, BLACK_FLAT = 2, WHITE_STANDING = 5, BLACK_STANDING = 4, WHITE_CAP = 7, BLACK_CAP = 6
    };

    /*
     * 3 bits + 3 bits=6 bits
     */
    typedef int square;

    /*
     * 6 bits start square
     * 100 if place
     * rest bits peice placed
     * 0xx if move
     * next 3 bits denote pick count
     * next 3n bits denote drop counts
     * max board size 8*8
     */
    typedef int move;

    typedef unsigned long long bitboard;

    inline move construct_place_move(square s, peice p) {
        return s | (p << 9) | 64;
    }

    move construct_move_move(square s, direction d, int pick, int *drop);

    inline direction getDirection(move m) {
        return (direction) ((m >> 7) & 3);
    }

    inline int getPickCount(move m) {
        return (m >> 9) & 7;
    }

    inline peice getPlacePeice(move m) {
        return (peice) (m >> 9);
    }

    inline bool isPlaceMove(move m) {
        return (m & 64) != 0;
    }

    inline int getRow(square s) {
        return s & 7;
    }

    inline int getCol(square s) {
        return (s >> 3) & 7;
    }

    inline player color_of(peice p) {
        return (player) (p & 1);
    }

    inline square getSquare(int r, int c) {
        return r | (c << 3);
    }

    inline bool isFlat(peice p) {
        return (p & 6) == 2;
    }

    inline bool isStanding(peice p) {
        return (p & 6) == 4;
    }

    inline bool isCap(peice p) {
        return (p & 6) == 6;
    }

    inline square squareAt(square s, direction d) {
        switch (d) {
            case UP:
                return s - 1;
            case DOWN:
                return s + 1;
            case LEFT:
                return s - 8;
            case RIGHT:
                return s + 8;
        }
        return 0;
    }


    void printMove(std::ostream &, move);

    void printSquare(std::ostream &, square);

    std::ostream &operator<<(std::ostream &, peice);

    inline bitboard getBitboard(square a) {
        return 1ULL << (getRow(a) * 8 + getCol(a));
    }

    int popcnt(bitboard i);

    inline bitboard neighbours(bitboard b) {
        return (b << 1) | (b >> 1) | (b << 8) | (b >> 8);
    }
    int countRows(bitboard b,int n);
    int countCols(bitboard b,int n);
    bitboard group(bitboard b,bitboard start);
    //std::ostream&operator<<(std::ostream&,move m);

    extern int ***slides;


    template<int n>
    class boardstate;
}
#endif // BASIC_H
