//
// Created by pankaj on 21/9/16.
//

#include "basic.h"
#include <iostream>
#include <bitset>
#include <random>
#include <functional>
namespace TAK {
    int slides[9][9][35];
    int count_slides[9][9];
    int slides1[9][9][35];
    int count_slides1[9][9];

    void append_splits(int i, int j, int *splits) {
        slides[i][j][count_slides[i][j]] = 0;
        int left = i;
        for (; left > 0;) {
            slides[i][j][count_slides[i][j]] = ((slides[i][j][count_slides[i][j]] << 3) | (*splits));
            left -= *(splits++);
        }
        count_slides[i][j]++;
    }

    void append_splits1(int i, int j, int *splits) {
        slides1[i][j][count_slides1[i][j]] = 0;
        int left = i;
        for (; left > 0;) {
            slides1[i][j][count_slides1[i][j]] = ((slides1[i][j][count_slides1[i][j]] << 3) | (*splits));
            left -= *(splits++);
        }
        count_slides1[i][j]++;
    }

    void split_rec(int n, int k, int left, int d, int *splits) {
        if (d == k) {
            splits[d - 1] = left;
            append_splits(n, k, splits);
            if (*splits == 1) {
                //for(int l=0;l<d;l++)
                //    std::cout<<splits[l];
                //std::cout<<'\n';
                append_splits1(n, k, splits);
            }
        } else {
            for (int i = 1; left - i >= k - d; i++) {
                splits[d - 1] = i;
                split_rec(n, k, left - i, d + 1, splits);
            }
        }
    }

    void initSlides() {
        for (int i = 0; i <= 8; i++)
            for (int j = 0; j <= 8; j++)
                count_slides[i][j] = 0;
        int tmp[9];
        for (int n = 1; n <= 8; n++)
            for (int r = 1; r <= n; r++)
                split_rec(n, r, n, 1, tmp);
    }

    void printSquare(std::ostream &o, square s) {
        o << (char) (getCol(s) + 'a') << squareAtLim + 1 - getRow(s);
    }

    void printMove(std::ostream &o, move m) {
        if (isPlaceMove(m)) {
            peice p = getPlacePeice(m);
            if (isFlat(p))
                o << 'F';
            else if (isCap(p))
                o << 'C';
            else o << 'S';
            printSquare(o, m);
        } else {
            int p;
            o << (p = getPickCount(m));
            printSquare(o, m);
            switch (getDirection(m)) {
                case UP:
                    o << '+';
                    break;
                case DOWN:
                    o << '-';
                    break;
                case LEFT:
                    o << '<';
                    break;
                case RIGHT:
                    o << '>';
                    break;
            }
            m >>= 12;
            while (p > 0) {
                p -= (m & 7);
                o << (m & 7);
                m >>= 3;
            }
        }
    }

    int popcnt(bitboard i) {
        i = i - ((i >> 1) & 0x5555555555555555UL);
        i = (i & 0x3333333333333333UL) + ((i >> 2) & 0x3333333333333333UL);
        return (int) ((((i + (i >> 4)) & 0xF0F0F0F0F0F0F0FUL) * 0x101010101010101UL) >> 56);
    }

    std::ostream &operator<<(std::ostream &o, peice p) {
        switch (p) {
            case BLACK_FLAT:
                o << 'f';
                break;
            case WHITE_FLAT:
                o << 'F';
                break;
            case BLACK_CAP:
                o << 'b';
                break;
            case WHITE_CAP:
                o << 'W';
                break;
            case BLACK_STANDING:
                o << 's';
                break;
            case WHITE_STANDING:
                o << 'S';
                break;
        }
        return o;
    }

    bitboard group(bitboard b, bitboard start) {
        bitboard ne = start & b;
        do {
            start = ne;
            ne = start | (b & neighbours(start));
        } while (ne != start);
        return start;
    }

    move construct_move_move(square s, direction d, int pick, int *drop) {
        move ans = s | (d << 7);
        ans |= (pick << 9);
        int l = 12;
        while (pick > 0) {
            ans |= ((*drop) << l);
            l += 3;
            pick -= *drop;
            drop++;
        }
        return ans;
    }

    move construct_move_move(square s, direction d, int pick, int drops) {
        move ans = s | (d << 7);
        ans |= (pick << 9);
        ans |= (drops << 12);
        return ans;
    }

    int countRows(bitboard b, int n) {
        int cnt = 0;
        for (int i = 0; i < n; i++)
            if (b & (row << (i << 3)))
                cnt++;
        return cnt;
    }

    int countCols(bitboard b, int n) {
        int cnt = 0;
        for (int i = 0; i < n; i++)
            if (b & (col << i))
                cnt++;

        return cnt;
    }

    int squareAtLim;

    void initbasic(int n) {
        squareAtLim = n - 1;
    }

    move readMove(char *ch, player turn) {
        if (ch[0] <= '9' && ch[0] >= '0') {
            int list[8];
            int pick = ch[0] - '0';
            direction d;
            square start = readSquare(ch + 1);
            switch (ch[3]) {
                case '>':
                    d = RIGHT;
                    break;
                case '<':
                    d = LEFT;
                    break;
                case '+':
                    d = UP;
                    break;
                case '-':
                    d = DOWN;
                    break;
            }
            int s = pick;
            ch += 4;
            for (int i = 0; s > 0; i++)
                s -= (list[i] = (*ch++) - '0');
            return construct_move_move(start, d, pick, list);
        } else {
            switch (ch[0]) {
                case 'C':
                    return construct_place_move(readSquare(ch + 1), peice(6 | turn));
                case 'S':
                    return construct_place_move(readSquare(ch + 1), peice(4 | turn));
                default:
                    return construct_place_move(readSquare(ch + 1), peice(2 | turn));
            }
        }
    }

    unsigned long long zobristTable[8][8][64][8];
    unsigned long long white_to_move;

    void initZobrist() {
        std::default_random_engine generator;
        std::uniform_int_distribution<unsigned long long> dist(0, std::numeric_limits<unsigned long long>::max());
        auto rnd = std::bind(dist, generator);
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                for (int k = 0; k < 64; k++)
                    for (int l = 0; l < 8; l++)
                        zobristTable[i][j][k][l] = rnd();
        //std::cout<<zobristTable[i][j][k][l]<<'\n';
        white_to_move = rnd();
    }
}
