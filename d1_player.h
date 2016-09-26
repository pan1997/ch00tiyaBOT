//
// Created by pankaj on 21/9/16.
//

#ifndef A3_D1_PLAYER_H
#define A3_D1_PLAYER_H

#include "basic.h"
#include "boardstate.h"
#include "evaluate.h"
#include <chrono>
#include <cmath>
namespace TAK {

    struct searchInfo {
        int nodes;
        int depth_limit;
    };


    template<int n>
    int minimax(boardstate<n> &b, searchInfo *info, int d) {
        info->nodes++;
        //std::cout<<"minimax\n";
        int max = -scale * 1000000;
        int neg = b.getTurn() == BLACK ? -1 : 1;
        peice flat = b.getTurn() == WHITE ? WHITE_FLAT : BLACK_FLAT;
        move bm = -1;

        //first flat, then standing then cap
        for (int p = 1; p <= 3; p++)
            if (p != 3 || (b.getTurn() == WHITE ? b.getWhileCapLeft() : b.getBlackCapLeft()) > 0) {
                for (int i = 0; i < n; i++)
                    for (int j = 0; j < n; j++)
                        if (b.empty(getSquare(i, j))) {
                            move m = construct_place_move(getSquare(i, j), (peice) (b.getTurn() | (p << 1)));
                            b.playMove(m);
                            b.flipTurn();
                            int ms;
                            if (b.end())
                                ms = neg * terminalEval(b);
                            else if (d < info->depth_limit)
                                ms = -minimax(b, info, d + 1);
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
                    for (int k = 0; k < 4; k++) {
                        direction dir = (direction) k;
                        int lr = 0;
                        square t = getSquare(i, j);
                        t = squareAt(t, dir);
                        for (int k = 0; k < n && (t != -1) && (b.empty(t) || isFlat(b.top(t))); t = squareAt(t, dir))
                            lr++; //capstone at end
                        for (int h = 1; h <= lh; h++)
                            for (int r = 1; r <= lr; r++) {
                                for (int cnt = 0; cnt < count_slides[h][r]; cnt++) {
                                    move m = construct_move_move(getSquare(i, j), dir, h, slides[h][r][cnt]);
                                    b.playMove(m);
                                    b.flipTurn();
                                    int ms;
                                    if (b.end())
                                        ms = neg * terminalEval(b);
                                    else if (d < info->depth_limit)
                                        ms = -minimax(b, info, d + 1);
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
                    }
                }
        return max;
    }

#define depth_of_d1 3

    template<int n>
    move d1_getMove(boardstate<n> &b, int &max) {
        max = -scale * 1000000;
        int neg = b.getTurn() == BLACK ? -1 : 1;
        peice flat = b.getTurn() == WHITE ? WHITE_FLAT : BLACK_FLAT;
        move bm = -1;
        searchInfo info;
        info.depth_limit = depth_of_d1;
        info.nodes = 1;
        auto start = std::chrono::system_clock::now();
        int nl = 0;
        //first flat, then standing then cap
        for (int p = 1; p <= 3; p++)
            if (p != 3 || (b.getTurn() == WHITE ? b.getWhileCapLeft() : b.getBlackCapLeft()) > 0) {
                for (int i = 0; i < n; i++)
                    for (int j = 0; j < n; j++)
                        if (b.empty(getSquare(i, j))) {
                            nl++;
                            move m = construct_place_move(getSquare(i, j), (peice) (b.getTurn() | (p << 1)));
                            b.playMove(m);
                            b.flipTurn();
                            int ms;
                            if (b.end()) {
                                ms = neg * terminalEvalVerbose(b);
                                std::cout << 'E';
                            }
                            else if (depth_of_d1 > 1)
                                ms = -minimax(b, &info, 1);
                            else ms = neg * evaluate(b);
                            //else ms = neg * evaluate(b);
                            if (ms > max) {
                                bm = m;
                                max = ms;
                            }
                            b.undoMove(m);
                            b.flipTurn();
                            std::cout << '\t';
                            printMove(std::cout, m);
                            std::cout << "\tscore=" << ms << '\n';
                        }
            }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (!b.empty(getSquare(i, j)) && color_of(b.top(getSquare(i, j))) == b.getTurn()) {
                    int lh = b.getHeight(getSquare(i, j));
                    lh = std::min(lh, n);
                    for (int k = 0; k < 4; k++) {
                        direction dir = (direction) k;
                        int lr = 0;
                        square t = getSquare(i, j);
                        t = squareAt(t, dir);
                        for (int k = 0; k < n && (t != -1) && (b.empty(t) || isFlat(b.top(t))); t = squareAt(t, dir))
                            lr++; //capstone at end
                        for (int h = 1; h <= lh; h++)
                            for (int r = 1; r <= lr; r++) {
                                for (int cnt = 0; cnt < count_slides[h][r]; cnt++) {
                                    nl++;
                                    move m = construct_move_move(getSquare(i, j), dir, h, slides[h][r][cnt]);
                                    b.playMove(m);
                                    b.flipTurn();
                                    int ms;
                                    if (b.end()) {
                                        ms = neg * terminalEvalVerbose(b);
                                        std::cout << 'E';
                                    }
                                    else if (depth_of_d1 > 1)
                                        ms = -minimax(b, &info, 1);
                                    else ms = neg * evaluate(b);
                                    if (ms > max) {
                                        bm = m;
                                        max = ms;
                                    }
                                    b.undoMove(m);
                                    b.flipTurn();
                                    std::cout << '\t';
                                    printMove(std::cout, m);
                                    std::cout << "\tscore=" << ms << '\n';
                                }
                            }
                    }
                    //up donefor (int k = i - 1; k >= 0; k--)
                }
        auto end = std::chrono::system_clock::now();
        std::cout << info.nodes << " nodes searched " << nl << " legal moves :BEST ";
        printMove(std::cout, bm);
        std::cout << '\n';
        int tm = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << tm << " millis =" << info.nodes / tm << " kNps\n";
        std::cout << "EBF~=" << std::pow(info.nodes, 1.0 / depth_of_d1)<<'\n';
        return bm;
    }
}
#endif //A3_D1_PLAYER_H
