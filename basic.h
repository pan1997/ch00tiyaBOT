//
// Created by pankaj on 20/9/16.
//

#ifndef BASIC_H
#define BASIC_H
#include <iostream>
#define row 255ULL
#define col 72340172838076673ULL

#define ASS

namespace TAK {
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
     * if move & (1<<31)!=0, then also flattening move
     * max board size 8*8
     */
    typedef int move;

    typedef unsigned long long bitboard;

    inline move construct_place_move(square s, peice p) {
        return s | (p << 9) | 64;
    }

    move construct_move_move(square s, direction d, int pick, int *drop);

    move construct_move_move(square s, direction d, int pick, int drops);

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

    void initbasic(int bsize);

    extern int squareAtLim;

    inline square squareAt(square s, direction d) {
        switch (d) {
            case UP:
                return ((s & 7) != 0) ? s - 1 : -1;
            case DOWN:
                return ((s & 7) != squareAtLim) ? s + 1 : -1;
            case LEFT:
                return ((s & 56) != 0) ? s - 8 : -1;
            case RIGHT:
                return ((s & 56) != (squareAtLim << 3)) ? s + 8 : -1;
        }
        return 0;
    }

    void printMove(std::ostream &, move);

    void printSquare(std::ostream &, square);

    std::ostream &operator<<(std::ostream &, peice);

    inline bitboard getBitboard(square a) {
        if (a == -1)
            return 0ULL;
        else
            return 1ULL << (getRow(a) * 8 + getCol(a));
    }

    int popcnt(bitboard i);

    inline bitboard neighbours(bitboard b) {
#ifdef ALLOWED_8
        return ((b & ~(col << 7)) << 1) | ((b & ~col) >> 1) | (b << 8) | (b >> 8);
#else
        return (b<<1)|(b>>1)|(b<<8)|(b>>8);
#endif
    }

    int countRows(bitboard b, int n);

    int countCols(bitboard b, int n);

    bitboard group(bitboard b, bitboard start);

    extern int slides[9][9][35];
    extern int slides1[9][9][35];
    extern int count_slides[9][9];
    extern int count_slides1[9][9];

    void initSlides();

    move readMove(char *ch, player turn);

    inline square readSquare(char *ch) {
        return getSquare(squareAtLim - (ch[1] - '1'), ch[0] - 'a');
    }

    inline void printBitboard(std::ostream&o,bitboard b) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++)
                o << ((b & getBitboard(getSquare(i, j)))!=0);
            o << '\n';
        }
        o<<'\n';
    }

    template<int n>
    class boardstate;

    extern unsigned long long zobristTable[8][8][3 * 8 * 8 / 2][8];
    extern unsigned long long white_to_move;

    void initZobrist();

    extern bitboard spread[8][8][8][4];

    void initSpread();
}
#endif // BASIC_H
