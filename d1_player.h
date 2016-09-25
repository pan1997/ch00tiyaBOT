//
// Created by pankaj on 21/9/16.
//

#ifndef A3_D1_PLAYER_H
#define A3_D1_PLAYER_H

#include "basic.h"
#include "boardstate.h"
#include "evaluate.h"

namespace TAK {

    template<int n>int minimax(boardstate <n> &b, int d) {
        int max = -scale * 1000;
        int neg = b.getTurn() == BLACK ? -1 : 1;
        peice flat = b.getTurn() == WHITE ? WHITE_FLAT : BLACK_FLAT;
        move bm = -1;

        //first flat, then standing then cap
        for (int p = 1; p <= 3; p++) {
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
        return max;
    }

#define depth_of_d1 3
    template<int n>
    move d1_getMove(boardstate <n>&b, int &max) {
        max = -scale * 1000;
        int neg = b.getTurn() == BLACK ? -1 : 1;
        peice flat = b.getTurn() == WHITE ? WHITE_FLAT : BLACK_FLAT;
        move bm = -1;


        //first flat, then standing then cap
        for (int p = 1; p <= 3; p++) {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if (b.empty(getSquare(i, j))) {
                        move m = construct_place_move(getSquare(i, j), (peice) (b.getTurn() | (p << 1)));
                        b.playMove(m);
                        b.flipTurn();
                        int ms;
                        if (b.end())
                            ms = neg * terminalEval(b);
                        else ms=-minimax(b,depth_of_d1-1);
                        //else ms = neg * evaluate(b);
                        if (ms > max) {
                            bm = m;
                            max = ms;
                        }
                        b.undoMove(m);
                        b.flipTurn();
                    }
        }
        return bm;
    }
}

#endif //A3_D1_PLAYER_H
