//
// Created by pankaj on 21/9/16.
//

#ifndef A3_D1_PLAYER_H
#define A3_D1_PLAYER_H

#include "basic.h"
#include "boardstate.h"
#include "evaluate.h"

namespace TAK {

    template<int n>
    int minimax(boardstate <n> &b, int d) {
        //std::cout<<"minimax\n";
        int max = -scale * 1000;
        int neg = b.getTurn() == BLACK ? -1 : 1;
        peice flat = b.getTurn() == WHITE ? WHITE_FLAT : BLACK_FLAT;
        move bm = -1;

        //first flat, then standing then cap
        for (int p = 1; p <= 3; p++)
            if (p != 3 || (b.getTurn() == WHITE ? b.getWC() : b.getBC()) > 0) {
                for (int i = 0; i < n; i++)
                    for (int j = 0; j < n; j++)
                        if (b.empty(getSquare(i, j))) {
                            move m = construct_place_move(getSquare(i, j), (peice) (b.getTurn() | (p << 1)));
                            b.playMove(m);
                            b.flipTurn();
                            int ms;
                            if (b.end())
                                ms = neg * terminalEval(b);
                            else if (d > 1)
                                ms = -minimax(b, d - 1);
                            else ms = neg * evaluate(b);
                            if (ms > max) {
                                bm = m;
                                max = ms;
                            }
                            b.undoMove(m);
                            b.flipTurn();
                        }
            }
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (!b.empty(getSquare(i, j)) && color_of(b.top(getSquare(i, j))) == b.getTurn()) {
                    int lh = b.getHeight(getSquare(i, j));
                    lh = std::min(lh, n);
                    bool iscap = isCap(b.top(getSquare(i, j)));

                    for (int k = 0; k < 4; k++) {
                        direction dir = (direction) k;
                        int lr = 0;
                        square t = getSquare(i, j);
                        t = squareAt(t, dir);
                        for (int k = 0; k < n && (t != 0) && (b.empty(t) || (!isCap(b.top(t)) &&
                                                                             (!isStanding(b.top(t)) ||
                                                                              iscap))); t = squareAt(t, dir))
                            lr++;
                        //for (int k = i - 1; k >= 0; k--)
                        //    if (b.empty(getSquare(k, j)) ||
                        //        (!isCap(b.top(getSquare(k, j))) && (!isStanding(b.top(getSquare(k, j))) || iscap)))
                        //        lr++;
                        //we can lift at most lh peices and drop till at most lr places in UP
                        for (int h = 1; h <= lh; h++)
                            for (int r = 1; r <= lr; r++) {
                                for (int cnt = 0; cnt < count_slides[h][r]; cnt++) {
                                    move m = construct_move_move(getSquare(i, j), dir, h, slides[h][r][cnt]);
                                    b.playMove(m);
                                    b.flipTurn();
                                    //std::cout<<"after move ::";
                                    //printMove(std::cout,m);
                                    //std::cout<<'\n'<<b<<'\n';
                                    int ms;
                                    if (b.end())
                                        ms = neg * terminalEval(b);
                                    else if (d > 1)
                                        ms = -minimax(b, d - 1);
                                    else ms = neg * evaluate(b);
                                    //else ms = neg * evaluate(b);
                                    if (ms > max) {
                                        bm = m;
                                        max = ms;
                                    }
                                    b.undoMove(m);
                                    b.flipTurn();
                                    //std::cout<<"after reset move ::";
                                    //printMove(std::cout,m);
                                    //std::cout<<'\n'<<b<<'\n';
                                }
                            }
                    }
                    //up donefor (int k = i - 1; k >= 0; k--)
                }
        return max;
    }

#define depth_of_d1 3

    template<int n>
    move d1_getMove(boardstate <n> &b, int &max) {
        max = -scale * 1000;
        int neg = b.getTurn() == BLACK ? -1 : 1;
        peice flat = b.getTurn() == WHITE ? WHITE_FLAT : BLACK_FLAT;
        move bm = -1;


        //first flat, then standing then cap
        for (int p = 1; p <= 3; p++)
            if (p != 3 || (b.getTurn() == WHITE ? b.getWC() : b.getBC()) > 0) {
                for (int i = 0; i < n; i++)
                    for (int j = 0; j < n; j++)
                        if (b.empty(getSquare(i, j))) {
                            move m = construct_place_move(getSquare(i, j), (peice) (b.getTurn() | (p << 1)));
                            b.playMove(m);
                            b.flipTurn();
                            int ms;
                            if (b.end())
                                ms = neg * terminalEval(b);
                            else if (depth_of_d1 > 1)
                                ms = -minimax(b, depth_of_d1 - 1);
                            else ms = neg * evaluate(b);
                            //else ms = neg * evaluate(b);
                            if (ms > max) {
                                bm = m;
                                max = ms;
                            }
                            b.undoMove(m);
                            b.flipTurn();
                        }
            }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (!b.empty(getSquare(i, j)) && color_of(b.top(getSquare(i, j))) == b.getTurn()) {
                    int lh = b.getHeight(getSquare(i, j));
                    lh = std::min(lh, n);
                    bool iscap = isCap(b.top(getSquare(i, j)));

                    for (int k = 0; k < 4; k++) {
                        direction dir = (direction) k;
                        int lr = 0;
                        square t = getSquare(i, j);
                        t = squareAt(t, dir);
                        for (int k = 0; k < n && (t != 0) && (b.empty(t) || (!isCap(b.top(t)) &&
                                                                             (!isStanding(b.top(t)) ||
                                                                              iscap))); t = squareAt(t, dir))
                            lr++;
                        //for (int k = i - 1; k >= 0; k--)
                        //    if (b.empty(getSquare(k, j)) ||
                        //        (!isCap(b.top(getSquare(k, j))) && (!isStanding(b.top(getSquare(k, j))) || iscap)))
                        //        lr++;
                        //we can lift at most lh peices and drop till at most lr places in UP
                        for (int h = 1; h <= lh; h++)
                            for (int r = 1; r <= lr; r++) {
                                for (int cnt = 0; cnt < count_slides[h][r]; cnt++) {
                                    move m = construct_move_move(getSquare(i, j), dir, h, slides[h][r][cnt]);
                                    b.playMove(m);
                                    b.flipTurn();
                                    //std::cout<<"after move ::";
                                    //printMove(std::cout,m);
                                    //std::cout<<'\n'<<b<<'\n';
                                    int ms;
                                    if (b.end())
                                        ms = neg * terminalEval(b);
                                    else if (depth_of_d1 > 1)
                                        ms = -minimax(b, depth_of_d1 - 1);
                                    else ms = neg * evaluate(b);
                                    //else ms = neg * evaluate(b);
                                    if (ms > max) {
                                        bm = m;
                                        max = ms;
                                    }
                                    b.undoMove(m);
                                    b.flipTurn();
                                    //std::cout<<"after reset move ::";
                                    //printMove(std::cout,m);
                                    //std::cout<<'\n'<<b<<'\n';
                                }
                            }
                    }
                    //up donefor (int k = i - 1; k >= 0; k--)
                }
        return bm;
    }
}

#endif //A3_D1_PLAYER_H
