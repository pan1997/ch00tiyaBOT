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

        unsigned long long hash;

        int leftover_stones_white;
        int leftover_stones_black;
        int leftover_capstones_white;
        int leftover_capstones_black;

        void place(square s, peice p) {
            int r = getRow(s);
            int c = getCol(s);
            bs[r][c][height[r][c]++] = p;
            if (height[r][c] == 1)
                nempty--;
            if (!isCap(p)) if (color_of(p) == WHITE)
                leftover_stones_white--;
            else
                leftover_stones_black--;
            else if (color_of(p) == WHITE)
                leftover_capstones_white--;
            else
                leftover_capstones_black--;
            setTopbb(s, p);
        }

        void removeTop(square s) {
            int r = getRow(s);
            int c = getCol(s);
            height[r][c]--;
            if (height[r][c] == 0)
                nempty++;
            peice p = bs[r][c][height[r][c]];
            if (!isCap(p)) if (color_of(p) == WHITE)
                leftover_stones_white++;
            else
                leftover_stones_black++;
            else if (color_of(p) == WHITE)
                leftover_capstones_white++;
            else
                leftover_capstones_black++;
            unsetTopbb(s, p);
        }

        void setTopbb(square s, peice p) {
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

        void unsetTopbb(square s, peice p) {
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

        void xor_bitboard(square s, peice bb) {
            switch (bb) {
                case WHITE_FLAT:
                    WF ^= getBitboard(s);
                    break;
                case BLACK_FLAT:
                    BF ^= getBitboard(s);
                    break;
                case WHITE_STANDING:
                    WS ^= getBitboard(s);
                    break;
                case BLACK_STANDING:
                    BS ^= getBitboard(s);
                    break;
                case WHITE_CAP:
                    WC ^= getBitboard(s);
                    break;
                case BLACK_CAP:
                    BC ^= getBitboard(s);
                    break;
            }
        }

    public:
        boardstate() { clear(); }

        boardstate(const boardstate &b) {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++) {
                    height[i][j] = b.height[i][j];
                    for (int k = 0; k < height[i][j]; k++)
                        bs[i][j][k] = b.bs[i][j][k];
                }
            turn = b.turn;
            nempty = b.nempty;
            leftover_capstones_black = b.leftover_capstones_black;
            leftover_stones_white = b.leftover_stones_white;
            leftover_capstones_white = b.leftover_capstones_white;
            leftover_stones_black = b.leftover_stones_black;
            WF = b.WF;
            WC = b.WC;
            WS = b.WS;
            BF = b.BF;
            BC = b.BC;
            BS = b.BS;
            for (int i = 0; i < 8; i++) {
                group_count_W[i] = b.group_count_W[i];
                group_count_B[i] = b.group_count_B[i];
            }
            hash = b.hash;
        }

        void clear() {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++) {
                    bs[i][j][0] = NOP;
                    height[i][j] = 0;
                }
            turn = WHITE;
            nempty = n * n;
            switch (n) {
                case 4:
                    leftover_stones_black = leftover_stones_white = 15;
                    leftover_capstones_black = leftover_capstones_white = 0;
                    break;
                case 5:
                    leftover_stones_black = leftover_stones_white = 21;
                    leftover_capstones_black = leftover_capstones_white = 1;
                    break;
                case 6:
                    leftover_stones_black = leftover_stones_white = 30;
                    leftover_capstones_black = leftover_capstones_white = 1;
                    break;
                case 7:
                    leftover_stones_black = leftover_stones_white = 40;
                    leftover_capstones_black = leftover_capstones_white = 1;
                    break;
                case 8:
                    leftover_stones_black = leftover_stones_white = 50;
                    leftover_capstones_black = leftover_capstones_white = 2;
                    break;
            }
            WF = BF = WS = BS = WC = BC = 0;
            for (int i = 0; i < 8; i++)
                group_count_W[i] = group_count_B[i] = 0;
            hash = 0;
        }


        peice top(square s) const {
            int r = getRow(s);
            int c = getCol(s);
            return bs[r][c][height[r][c] - 1];
        }

        peice underTop(square s) const {
            int r = getRow(s);
            int c = getCol(s);
            return bs[r][c][height[r][c] - 2];
        }

        bool empty(square s) const {
            int r = getRow(s);
            int c = getCol(s);
            return r < n && c < n && r >= 0 && c >= 0 && height[r][c] == 0;
        }

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

        bool operator==(const boardstate &b) {
            if (hash != b.hash)
                return false;
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++) {
                    if (height[i][j] != b.height[i][j])
                        return false;
                    for (int k = 0; k < height[i][j]; k++)
                        if (bs[i][j][k] != b.bs[i][j][k])
                            return false;
                }
            return true;
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

        bool playMove(move m) {
            if (isPlaceMove(m)) {
                hash ^= zobristTable[getRow(m)][getCol(m)][getHeight(m)][getPlacePeice(m)];
                place(m, getPlacePeice(m));
                return false;
            }
            else {
                square s = m;
                direction d = getDirection(m);
                int pick = getPickCount(m);
                m >>= 12;
                unsetTopbb(s, top(s));
                height[getRow(s)][getCol(s)] -= pick;
                if (height[getRow(s)][getCol(s)] == 0)
                    nempty++;
                int index = height[getRow(s)][getCol(s)];
                square t = s;
                int count = 0;
                bool res = false;
                for (; pick > 0; count++) {
                    int drop = m & 7;
                    pick -= drop;
                    m >>= 3;
                    t = squareAt(t, d);
                    if (empty(t))
                        nempty--;
                    else {
                        if (pick == 0 && drop == 1 && isStanding(top(t))) {
                            flatten(t);
                            res = true;
                        }
                        unsetTopbb(t, top(t));
                    }
                    for (int i = 0; i < drop; i++) {
                        hash ^= zobristTable[getRow(s)][getCol(s)][index][bs[getRow(s)][getCol(s)][index]];
                        hash ^= zobristTable[getRow(t)][getCol(t)][height[getRow(t)][getCol(t)]][bs[getRow(s)][getCol(
                                s)][index]];
                        bs[getRow(t)][getCol(t)][height[getRow(t)][getCol(t)]++] = bs[getRow(s)][getCol(s)][index++];
                    }
                    //setTopbb(t, top(t));
                }
                d = direction(d ^ 1);
                for (; count >= 0; count--) {
                    if (!empty(t))
                        setTopbb(t, top(t));
                    t = squareAt(t, d);
                }
                return res;
            }
        }

        void undoMove(move m, bool lf = false) {
            if (isPlaceMove(m)) {
                removeTop(m);
                hash ^= zobristTable[getRow(m)][getCol(m)][getHeight(m)][getPlacePeice(m)];
            }
            else {
                square s = m;
                direction d = getDirection(m);
                int pick = getPickCount(m);
                //unsetTopbb(s,top(s));
                if (height[getRow(s)][getCol(s)] == 0)
                    nempty--;
                else unsetTopbb(s, top(s));
                m >>= 12;
                square t = s;
                int count;
                for (count = 0; pick > 0; count++) {
                    int drop = m & 7;
                    pick -= drop;
                    m >>= 3;
                    t = squareAt(t, d);
                    unsetTopbb(t, top(t));//xor_bitboard(t, top(t));//remove new
                    height[getRow(t)][getCol(t)] -= drop;
                    if (height[getRow(t)][getCol(t)] == 0)
                        nempty++;
                    //else setTopbb(t, top(t));//xor_bitboard(t, top(t));//add prev
                    for (int i = 0; i < drop; i++) {
                        hash ^= zobristTable[getRow(s)][getCol(s)][height[getRow(s)][getCol(s)]][bs[getRow(t)][getCol(
                                t)][
                                i + height[getRow(t)][getCol(t)]]];
                        hash ^= zobristTable[getRow(t)][getCol(t)][i + height[getRow(t)][getCol(t)]][bs[getRow(
                                t)][getCol(
                                t)][
                                i + height[getRow(t)][getCol(t)]]];
                        bs[getRow(s)][getCol(s)][height[getRow(s)][getCol(s)]++] = bs[getRow(t)][getCol(t)][i +
                                                                                                            height[getRow(
                                                                                                                    t)][getCol(
                                                                                                                    t)]];
                    }
                }
                d = direction(d ^ 1);
                square end = t;
                for (; count >= 0; count--) {
                    if (!empty(t))
                        setTopbb(t, top(t));
                    t = squareAt(t, d);
                }
                if (lf) {
                    liften(end);
                }
            }
        }

        void flipTurn() {
            if (turn == BLACK) turn = WHITE; else turn = BLACK;
            hash ^= white_to_move;
        }

        bool legal(move m) {
            if (isPlaceMove(m))
                return empty(m);
            else {
                return color_of(top(m)) == getTurn(); //&& getPickCount(m) >= getHeight(m);
            }
        }

        void printTo(std::ostream &o) const {
            int mh = 1;
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if (height[i][j] > mh)
                        mh = height[i][j];
            for (int i = 0; i < n; i++) {
                o << "  ";
                o << '+';
                for (int j = 0; j < n; j++) {
                    for (int k = 0; k < mh; k++)
                        o << '-';
                    o << '+';
                }
                o << '\n';
                o << n - i << ' ';
                o << '|';
                for (int j = 0; j < n; j++) {
                    int k = 0;
                    for (; k < height[i][j]; k++)
                        o << bs[i][j][k];
                    for (; k < mh; k++)
                        o << ' ';
                    o << '|';
                }
                o << '\n';
            }
            o << "  ";
            o << '+';
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < mh; k++)
                    o << '-';
                o << '+';
            }
            o << "\n  ";
            for (int i = 0; i < n; i++) {
                o << ' ';
                o << (char) (i + 'a');
                for (int k = 1; k < mh; k++)
                    o << ' ';
            }
            o << '\t' << (turn == WHITE ? "WHITE" : "BLACK") << " to move : empty squares " << nempty << ",left (" <<
            leftover_stones_white << ',' << leftover_stones_black << ")\n";
        }

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
}
#endif // BOARDSTATE_H
