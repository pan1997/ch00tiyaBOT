//
// Created by pankaj on 20/9/16.
//

#include "boardstate.h"
#include "basic.h"
namespace TAK {

    template<int n>
    void boardstate<n>::place(square s, peice p) {
        int r = getRow(s);
        int c = getCol(s);
        if (r >= n || c >= n) {
            std::cout << "OUT of bounds.\n";
        }
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

    template<int n>
    void boardstate<n>::removeTop(square s) {
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

    template<int n>
    boardstate<n>::boardstate() {
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
    }

    template<int n>
    peice boardstate<n>::top(square s) const {
        int r = getRow(s);
        int c = getCol(s);
        return bs[r][c][height[r][c] - 1];
    }

    template<int n>
    bool boardstate<n>::empty(square s) const {
        int r = getRow(s);
        int c = getCol(s);
        return r < n && c < n && r >= 0 && c >= 0 && height[r][c] == 0;
    }

    template<int n>
    void boardstate<n>::xor_bitboard(square s, peice bb) {
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

    template<int n>
    void boardstate<n>::printTo(std::ostream &o) const {
        int mh = 1;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (height[i][j] > mh)
                    mh = height[i][j];
        for (int i = 0; i < n; i++) {
            o << '+';
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < mh; k++)
                    o << '-';
                o << '+';
            }
            o << '\n';
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
        o << '+';
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < mh; k++)
                o << '-';
            o << '+';
        }
        o << (turn == WHITE ? "WHITE" : "BLACK") << " to move : empty squares " << nempty << "\n";

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++)
                o << ((WF & getBitboard(getSquare(i, j))) != 0);
            o << ' ';
            for (int j = 0; j < 8; j++)
                o << ((BF & getBitboard(getSquare(i, j))) != 0);
            o << ' ';
            for (int j = 0; j < 8; j++)
                o << ((WS & getBitboard(getSquare(i, j))) != 0);
            o << ' ';
            for (int j = 0; j < 8; j++)
                o << ((BS & getBitboard(getSquare(i, j))) != 0);
            o << ' ';
            for (int j = 0; j < 8; j++)
                o << ((WC & getBitboard(getSquare(i, j))) != 0);
            o << ' ';
            for (int j = 0; j < 8; j++)
                o << ((BC & getBitboard(getSquare(i, j))) != 0);
            o << '\n';
        }
        for(int i=0;i<8;i++){
            o<<group_count_W[i]<<'\t';
        }
        o<<'\n';
        for(int i=0;i<8;i++){
            o<<group_count_B[i]<<'\t';
        }
        o<<'\n';

    }

    template<int n>
    void boardstate<n>::playMove(move m) {
        if (isPlaceMove(m))
            place(m, getPlacePeice(m));
        else {
            square s = m;
            direction d = getDirection(m);
            int pick = getPickCount(m);
            m >>= 12;
            unsetTopbb(s, top(s));
            //xor_bitboard(s, top(s));//remove prev
            height[getRow(s)][getCol(s)] -= pick;
            //std::cout << "picked " << pick << "\n";
            if (height[getRow(s)][getCol(s)] == 0)
                nempty++;
            else setTopbb(s, top(s));
            // xor_bitboard(s, top(s));//add new
            int index = height[getRow(s)][getCol(s)];
            square t = s;
            for (; pick > 0;) {
                int drop = m & 7;
                //std::cout << "dropped " << drop << '\n';
                pick -= drop;
                m >>= 3;
                t = squareAt(t, d);
                if (empty(t))
                    nempty--;
                else unsetTopbb(t, top(t));
                //xor_bitboard(t, top(t));//remove prev
                for (int i = 0; i < drop; i++)
                    bs[getRow(t)][getCol(t)][height[getRow(t)][getCol(t)]++] = bs[getRow(s)][getCol(s)][index++];
                setTopbb(t, top(t));
                //xor_bitboard(t, top(t));//add new
            }
        }
    }

    template<int n>
    void boardstate<n>::undoMove(move m) {
        if (isPlaceMove(m))
            removeTop(m);
        else {
            square s = m;
            direction d = getDirection(m);
            int pick = getPickCount(m);
            if (height[getRow(s)][getCol(s)] == 0)
                nempty--;
            else unsetTopbb(s, top(s));//xor_bitboard(s, top(s));//remove new
            m >>= 12;
            square t = s;
            while (pick > 0) {
                int drop = m & 7;
                pick -= drop;
                m >>= 3;
                t = squareAt(t, d);
                unsetTopbb(t, top(t));//xor_bitboard(t, top(t));//remove new
                height[getRow(t)][getCol(t)] -= drop;
                if (height[getRow(t)][getCol(t)] == 0)
                    nempty++;
                else setTopbb(t, top(t));//xor_bitboard(t, top(t));//add prev
                for (int i = 0; i < drop; i++)
                    bs[getRow(s)][getCol(s)][height[getRow(s)][getCol(s)]++] = bs[getRow(t)][getCol(t)][i +
                                                                                                        height[getRow(
                                                                                                                t)][getCol(
                                                                                                                t)]];
            }
            setTopbb(s, top(s));
        }
    }
}