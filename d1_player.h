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
    int minimax(boardstate<n> &b, searchInfo *info, int d, int alpha, int beta) {
        info->nodes++;
        int neg = b.getTurn() == BLACK ? -1 : 1;
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
                            else if (d < info->depth_limit) {
                                ms = -minimax(b, info, d + 1, -alpha - 1, -alpha);
                                if (alpha < ms && ms < beta)
                                    ms = -minimax(b, info, d + 1, -beta, -alpha);
                            }
                            else ms = neg * evaluate(b);
                            b.undoMove(m);
                            b.flipTurn();
                            if (ms > alpha) {
                                bm = m;
                                alpha = ms;
                                if (alpha >= beta) {
                                    return alpha;
                                }
                            }
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
                                    else if (d < info->depth_limit) {
                                        ms = -minimax(b, info, d + 1, -alpha - 1, -alpha);
                                        if (alpha < ms && ms < beta)
                                            ms = -minimax(b, info, d + 1, -beta, -alpha);
                                    } else ms = neg * evaluate(b);
                                    //else ms = neg * evaluate(b);
                                    b.undoMove(m);
                                    b.flipTurn();
                                    if (ms > alpha) {
                                        bm = m;
                                        alpha = ms;
                                        if (alpha >= beta)
                                            return alpha;
                                    }
                                }
                            }
                    }
                }
        return alpha;
    }

#define depth_of_d1 5
#define tl 1000
    template<int n>
    move d1_getMove(boardstate<n> &b, int &max) {
        int neg = b.getTurn() == BLACK ? -1 : 1;
        move bm = -1;
        searchInfo info;
        info.nodes = 1;
        auto start = std::chrono::system_clock::now();
        for (int dl = 1; ; dl++) {
            max = -scale * 1000000;
            int alpha = max;
            int beta = -max;
            info.depth_limit=dl;
            bm=-1;
            for (int p = 1; p <= 3; p++)
                if (p != 3 || (b.getTurn() == WHITE ? b.getWhileCapLeft() : b.getBlackCapLeft()) > 0) {
                    for (int i = 0; i < n; i++)
                        for (int j = 0; j < n; j++)
                            if (b.empty(getSquare(i, j))) {
                                move m = construct_place_move(getSquare(i, j), (peice) (b.getTurn() | (p << 1)));
                                b.playMove(m);
                                b.flipTurn();
                                int ms;
                                if (b.end()) {
                                    ms = neg * terminalEval(b);
                                    //std::cout << 'E';
                                }
                                else if (dl > 1) {
                                    ms = -minimax(b, &info, 1, -alpha - 1, -alpha);
                                    if (alpha < ms && ms < beta)
                                        ms = -minimax(b, &info, 1, -beta, -alpha);
                                }
                                else ms = neg * evaluate(b);
                                //else ms = neg * evaluate(b);
                                if (ms > alpha) {
                                    bm = m;
                                    alpha = ms;
                                    max = alpha;
                                }
                                b.undoMove(m);
                                b.flipTurn();
                                //std::cout << '\t';
                                //printMove(std::cout, m);
                                //std::cout << "\tscore=" << ms << '\n';
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
                            for (int k = 0;
                                 k < n && (t != -1) && (b.empty(t) || isFlat(b.top(t))); t = squareAt(t, dir))
                                lr++; //capstone at end
                            for (int h = 1; h <= lh; h++)
                                for (int r = 1; r <= lr; r++) {
                                    for (int cnt = 0; cnt < count_slides[h][r]; cnt++) {
                                        move m = construct_move_move(getSquare(i, j), dir, h, slides[h][r][cnt]);
                                        b.playMove(m);
                                        b.flipTurn();
                                        int ms;
                                        if (b.end()) {
                                            ms = neg * terminalEval(b);
                                            //std::cout << 'E';
                                        }
                                        else if (dl > 1) {
                                            ms = -minimax(b, &info, 1, -alpha - 1, -alpha);
                                            if (alpha < ms && ms < beta)
                                                ms = -minimax(b, &info, 1, -beta, -alpha);
                                        }
                                        else ms = neg * evaluate(b);
                                        if (ms > alpha) {
                                            bm = m;
                                            alpha = ms;
                                            max = alpha;
                                        }
                                        b.undoMove(m);
                                        b.flipTurn();
                                        //std::cout << '\t';
                                        //printMove(std::cout, m);
                                        //std::cout << "\tscore=" << ms << '\n';
                                    }
                                }
                        }
                        //up donefor (int k = i - 1; k >= 0; k--)
                    }
            auto end = std::chrono::system_clock::now();
            int tm = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout<<dl<<'\t'<<alpha<<'\t';
            std::cout <<info.nodes<<" nodes @"<<info.nodes / (tm+1) <<" kNps["<<tm << " ms] ";
            std::cout << "EBF~=" << std::pow(info.nodes, 1.0 / dl) << " BM ";
            printMove(std::cout, bm);
            std::cout << '\n';
            if(tm>tl)
                break;
        }
        return bm;
    }
}
#endif //A3_D1_PLAYER_H
