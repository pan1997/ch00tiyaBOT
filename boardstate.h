//
// Created by pankaj on 20/9/16.
//

#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include <iostream>
#include <set>
#include"basic.h"
namespace TAK {

    template<int n>
    class boardstate {
        player turn;
        bitboard WF, BF, WS, BS, WC, BC;
        peice bs[n][n][3 * n * n / 2];
        int height[n][n];
        int nempty;
        int group_count_W[8];
        int group_count_B[8];
        //std::set<bitboard> WG,BG;
        //bitboard groups[n][n];//which group does this square belong to
        unsigned long long hash;

        int leftover_stones_white;
        int leftover_stones_black;
        int leftover_capstones_white;
        int leftover_capstones_black;

        inline void place(square s, peice p);

        inline void removeTop(square s);

        inline void setTopbb(square s, peice p) {
            if (s == -1) {
                std::cout << "settopbb -1\n";
            }
            if (isCap(p) || isFlat(p)) {
                bitboard g1 = group((color_of(p) == WHITE) ? (WF | WC) : (BF | BC), getBitboard(squareAt(s, UP)));
                if (g1 != 0) {
                    if (color_of(p) == WHITE)
                        group_count_W[countRows(g1, n)]--;
                    else group_count_B[countRows(g1, n)]--;
                    if (color_of(p) == WHITE)
                        group_count_W[countCols(g1, n)]--;
                    else group_count_B[countCols(g1, n)]--;
                }

                bitboard g2 = group((color_of(p) == WHITE) ? (WF | WC) : (BF | BC), getBitboard(squareAt(s, DOWN)));
                if (g2 != 0 && g2 != g1) {
                    if (color_of(p) == WHITE)
                        group_count_W[countRows(g2, n)]--;
                    else group_count_B[countRows(g2, n)]--;
                    if (color_of(p) == WHITE)
                        group_count_W[countCols(g2, n)]--;
                    else group_count_B[countCols(g2, n)]--;
                }

                bitboard g3 = group((color_of(p) == WHITE) ? (WF | WC) : (BF | BC), getBitboard(squareAt(s, LEFT)));
                if (g3 != 0 && g3 != g1 && g3 != g2) {
                    if (color_of(p) == WHITE)
                        group_count_W[countRows(g3, n)]--;
                    else group_count_B[countRows(g3, n)]--;
                    if (color_of(p) == WHITE)
                        group_count_W[countCols(g3, n)]--;
                    else group_count_B[countCols(g3, n)]--;
                }

                bitboard g4 = group((color_of(p) == WHITE) ? (WF | WC) : (BF | BC), getBitboard(squareAt(s, RIGHT)));
                if (g4 != 0 && g4 != g1 && g4 != g2 && g4 != g3) {
                    if (color_of(p) == WHITE)
                        group_count_W[countRows(g4, n)]--;
                    else group_count_B[countRows(g4, n)]--;
                    if (color_of(p) == WHITE)
                        group_count_W[countCols(g4, n)]--;
                    else group_count_B[countCols(g4, n)]--;
                }

                xor_bitboard(s, p);

                bitboard gr = group((color_of(p) == WHITE) ? (WF | WC) : (BF | BC), getBitboard(s));
                if (color_of(p) == WHITE)
                    group_count_W[countRows(gr, n)]++;
                else group_count_B[countRows(gr, n)]++;
                if (color_of(p) == WHITE)
                    group_count_W[countCols(gr, n)]++;
                else group_count_B[countCols(gr, n)]++;
            } else xor_bitboard(s, p);
        }

        inline void unsetTopbb(square s, peice p) {
            if (s == -1) {
                std::cout << "settopbb -1\n";
            }
            if (isFlat(p) || isCap(p)) {
                bitboard gr = group((color_of(p) == WHITE) ? (WF | WC) : (BF | BC), getBitboard(s));
                if (color_of(p) == WHITE)
                    group_count_W[countRows(gr, n)]--;
                else group_count_B[countRows(gr, n)]--;
                if (color_of(p) == WHITE)
                    group_count_W[countCols(gr, n)]--;
                else group_count_B[countCols(gr, n)]--;

                xor_bitboard(s, p);

                bitboard g1 = group((color_of(p) == WHITE) ? (WF | WC) : (BF | BC), getBitboard(squareAt(s, UP)));
                if (g1 != 0) {
                    if (color_of(p) == WHITE)
                        group_count_W[countRows(g1, n)]++;
                    else group_count_B[countRows(g1, n)]++;
                    if (color_of(p) == WHITE)
                        group_count_W[countCols(g1, n)]++;
                    else group_count_B[countCols(g1, n)]++;
                }

                bitboard g2 = group((color_of(p) == WHITE) ? (WF | WC) : (BF | BC), getBitboard(squareAt(s, DOWN)));
                if (g2 != 0 && g2 != g1) {
                    if (color_of(p) == WHITE)
                        group_count_W[countRows(g2, n)]++;
                    else group_count_B[countRows(g2, n)]++;
                    if (color_of(p) == WHITE)
                        group_count_W[countCols(g2, n)]++;
                    else group_count_B[countCols(g2, n)]++;
                }

                bitboard g3 = group((color_of(p) == WHITE) ? (WF | WC) : (BF | BC), getBitboard(squareAt(s, LEFT)));
                if (g3 != 0 && g3 != g1 && g3 != g2) {
                    if (color_of(p) == WHITE)
                        group_count_W[countRows(g3, n)]++;
                    else group_count_B[countRows(g3, n)]++;
                    if (color_of(p) == WHITE)
                        group_count_W[countCols(g3, n)]++;
                    else group_count_B[countCols(g3, n)]++;
                }

                bitboard g4 = group((color_of(p) == WHITE) ? (WF | WC) : (BF | BC), getBitboard(squareAt(s, RIGHT)));
                if (g4 != 0 && g4 != g1 && g4 != g2 && g4 != g3) {
                    if (color_of(p) == WHITE)
                        group_count_W[countRows(g4, n)]++;
                    else group_count_B[countRows(g4, n)]++;
                    if (color_of(p) == WHITE)
                        group_count_W[countCols(g4, n)]++;
                    else group_count_B[countCols(g4, n)]++;
                }
            }
            else xor_bitboard(s, p);
        }

        void xor_bitboard(square s, peice bb);

    public:
        boardstate();

        peice top(square s) const;

        bool empty(square s) const;

        int countEmpty() const { return nempty; }

        bitboard getWS() const { return WS; }

        bitboard getBS() const { return BS; }

        bitboard getWF() const { return WF; }

        bitboard getWC() const { return WC; }

        bitboard getBF() const { return BF; }

        bitboard getBC() const { return BC; }

        //flattens the top standing stone at s
        inline void flatten(square s) {
            peice p = top(s);
            unsetTopbb(s, p);
            setTopbb(s, color_of(p) == BLACK ? BLACK_FLAT : WHITE_FLAT);
            hash ^= zobristTable[getRow(s)][getCol(s)][height[getRow(s)][getCol(s)] - 1][bs[getRow(s)][getCol(s)][
                    height[getRow(s)][getCol(s)] - 1]];
            bs[getRow(s)][getCol(s)][height[getRow(s)][getCol(s)] - 1] = color_of(p) == BLACK ? BLACK_FLAT : WHITE_FLAT;
            hash ^= zobristTable[getRow(s)][getCol(s)][height[getRow(s)][getCol(s)] - 1][bs[getRow(s)][getCol(s)][
                    height[getRow(s)][getCol(s)] - 1]];
        }

        //lifts the flat stone on n
        inline void liften(square s) {
            peice p = top(s);
            unsetTopbb(s, p);
            setTopbb(s, color_of(p) == BLACK ? BLACK_STANDING : WHITE_STANDING);
            hash ^= zobristTable[getRow(s)][getCol(s)][height[getRow(s)][getCol(s)] - 1][bs[getRow(s)][getCol(s)][
                    height[getRow(s)][getCol(s)] - 1]];
            bs[getRow(s)][getCol(s)][height[getRow(s)][getCol(s)] - 1] =
                    color_of(p) == BLACK ? BLACK_STANDING : WHITE_STANDING;
            hash ^= zobristTable[getRow(s)][getCol(s)][height[getRow(s)][getCol(s)] - 1][bs[getRow(s)][getCol(s)][
                    height[getRow(s)][getCol(s)] - 1]];
        }

        //does not include top. no. of peices of color p
        int countStacked(square s, int k, player p) const {
            int r = getRow(s);
            int c = getCol(s);
            int i = height[r][c] - k - 1;
            int cnt = 0;
            for (int j = 1; j < k; j++)
                if (color_of(bs[r][c][j + i]) == p)
                    cnt++;
            return cnt;
        }

        int getWhiteLeft() const { return leftover_stones_white; }

        int getWhileCapLeft() const { return leftover_capstones_white; }

        int getBlackCapLeft() const { return leftover_capstones_black; }

        int getBlackLeft() const { return leftover_stones_black; }

        const int *getGCW() const { return group_count_W; }

        const int *getGCB() const { return group_count_B; }

        bool end() const {
            return nempty == 0 || leftover_stones_black == 0 || leftover_stones_white == 0 || group_count_W[n] != 0 ||
                   group_count_B[n] != 0;
        }

        player getTurn() const { return turn; }

        int getHeight(square s) const {
            return height[getRow(s)][getCol(s)];
        }

        bool playMove(move m);

        void undoMove(move m, bool liften = false);

        void flipTurn() {
            if (turn == BLACK) turn = WHITE; else turn = BLACK;
            hash ^= white_to_move;
        }

        void printTo(std::ostream &o) const;

        unsigned long long getHash() const { return hash; }
    };


    template<int n>
    std::ostream &operator<<(std::ostream &o, const boardstate<n> &b) {
        b.printTo(o);
        return o;
    }

    template<int n>
    square getRandomEmptySquare(const boardstate<n> &b) {
        int nempty = 0;
        square ans = -1;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (b.empty(getSquare(i, j))) {
                    nempty++;
                    if (rand() % nempty == 0)
                        ans = getSquare(i, j);
                }
        return ans;
    }

    template
    class boardstate<5>;

    template
    class boardstate<4>;

    template
    class boardstate<6>;

    template
    class boardstate<7>;

    template
    class boardstate<8>;

    template
    class boardstate<3>;
}
#endif // BOARDSTATE_H
