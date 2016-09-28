//
// Created by pankaj on 27/9/16.
//

#ifndef A3_SEARCH_H
#define A3_SEARCH_H

//
// Created by pankaj on 21/9/16.
//
#include "basic.h"
#include "boardstate.h"
#include "evaluate.h"
#include <chrono>
#include <cmath>
#include "transpositionTable.h"
namespace TAK {


#define NMP_ALLOWED

    struct searchInfo {
        int nodes;
        int ttcuts;
        int fatt;
        int fsucc;
        int depth_limit;
    };

    enum node_type {
        PV_NODE, CUT_NODE, ALL_NODE
    };

    template<int n>
    int minimax(boardstate<n> &b, searchInfo *info, int d, int alpha, int beta, node_type tp, bool in_nm) {
        info->nodes++;
        transpositionTableEntry *transpositionTableEntry1 = getEntry(b, true);
        if (transpositionTableEntry1 != nullptr) {
            if (transpositionTableEntry1->depth >= info->depth_limit - d) {
                if (transpositionTableEntry1->lower_bound >= beta) {
                    info->ttcuts++;
                    return transpositionTableEntry1->lower_bound;
                }
                else if (transpositionTableEntry1->upper_bound <= alpha ||
                         transpositionTableEntry1->upper_bound == transpositionTableEntry1->lower_bound) {
                    info->ttcuts++;
                    return transpositionTableEntry1->upper_bound;
                }
                alpha = std::max(alpha, transpositionTableEntry1->lower_bound);
                beta = std::min(beta, transpositionTableEntry1->upper_bound);
            } else {
                transpositionTableEntry1->lower_bound = std::numeric_limits<int>::min();
                transpositionTableEntry1->upper_bound = std::numeric_limits<int>::max();
            }
        }


        if (tp != PV_NODE) {
            bool pruned = false;
#ifdef NMP_ALLOWED
            if (d > 2 && info->depth_limit > d + 1 && !in_nm) {
                info->fatt++;
                b.flipTurn();
                int bound=beta-scale*n*n/((n*n+b.countEmpty()));
                int ms = -minimax(b, info, (info->depth_limit-d>3)?d+3:d + 1, -bound, -bound+1, (tp == CUT_NODE) ? ALL_NODE : CUT_NODE, true);
                b.flipTurn();
                if (ms>=bound) {
                    info->fsucc++;
                    pruned = true;
                }
            }
#endif
            if (pruned)
                return beta;
        }

        int neg = b.getTurn() == BLACK ? -1 : 1;
        move bm = -1;
        int alpha_backup = alpha;

        if (transpositionTableEntry1 != nullptr) {//best move first
            bm = transpositionTableEntry1->bm;
            if (bm != -1) {
                bool fl = b.playMove(bm);
                b.flipTurn();
                int ms;
                if (d < info->depth_limit)
                    ms = -minimax(b, info, d + 1, -beta, -alpha, tp, in_nm);
                else ms = neg * evaluate(b);
                b.undoMove(bm, fl);
                b.flipTurn();
                if (fl)
                    info->fsucc++;
                if (ms > alpha)
                    alpha = ms;
            }
        }

        //first flat, then standing then cap
        if (alpha < beta)
            for (int p = 1; p <= 3; p++)
                if (p != 3 || (b.getTurn() == WHITE ? b.getWhileCapLeft() : b.getBlackCapLeft()) > 0) {
                    for (int i = 0; i < n; i++)
                        for (int j = 0; j < n; j++)
                            if (b.empty(getSquare(i, j))) {
                                move m = construct_place_move(getSquare(i, j), (peice) (b.getTurn() | (p << 1)));
                                if (bm == m)
                                    continue;
                                b.playMove(m);
                                b.flipTurn();
                                int ms;
                                if (b.end())
                                    ms = neg * terminalEval(b);
                                else if (d < info->depth_limit) {
                                    ms = -minimax(b, info, d + 1, -alpha - 1, -alpha,
                                                  (tp == CUT_NODE) ? ALL_NODE : CUT_NODE, in_nm);
                                    if (alpha < ms && ms < beta || tp == PV_NODE && ms == beta && beta == alpha + 1) {
                                        if (ms == alpha + 1)
                                            ms = alpha;
                                        ms = -minimax(b, info, d + 1, -beta, -ms, tp, in_nm);
                                    }
                                }
                                else ms = neg * evaluate(b);
                                b.undoMove(m);
                                b.flipTurn();
                                if (ms > alpha) {
                                    bm = m;
                                    alpha = ms;
                                    if (alpha >= beta) {
                                        goto eos;
                                    }
                                }
                            }
                }
        if (alpha < beta)
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
                                lr++;
                            for (int h = 1; h <= lh; h++)
                                for (int r = 1; r <= lr; r++) {
                                    for (int cnt = 0; cnt < count_slides[h][r]; cnt++) {
                                        move m = construct_move_move(getSquare(i, j), dir, h, slides[h][r][cnt]);
                                        if (bm == m)
                                            continue;
                                        b.playMove(m);
                                        b.flipTurn();
                                        int ms;
                                        if (b.end())
                                            ms = neg * terminalEval(b);
                                        else if (d < info->depth_limit) {
                                            ms = -minimax(b, info, d + 1, -alpha - 1, -alpha,
                                                          (tp == CUT_NODE) ? ALL_NODE : CUT_NODE, in_nm);
                                            if (alpha < ms && ms < beta ||
                                                tp == PV_NODE && ms == beta && beta == alpha + 1) {
                                                if (ms == alpha + 1)
                                                    ms = alpha;
                                                ms = -minimax(b, info, d + 1, -beta, -ms, tp, in_nm);
                                            }
                                        } else ms = neg * evaluate(b);
                                        //else ms = neg * evaluate(b);
                                        b.undoMove(m);
                                        b.flipTurn();
                                        if (ms > alpha) {
                                            bm = m;
                                            alpha = ms;
                                            if (alpha >= beta)
                                                goto eos;
                                        }
                                    }
                                }
                            if (isCap(b.top(getSquare(i, j))) && t != -1 && !b.empty(t) && isStanding(b.top(t))) {
                                //b.flatten(t);
                                lr++;
                                for (int h = lr; h <= lh; h++) {
                                    for (int r = lr; r <= lr; r++) {
                                        for (int cnt = 0; cnt < count_slides1[h][r]; cnt++) {
                                            move m = construct_move_move(getSquare(i, j), dir, h, slides1[h][r][cnt]);
                                            //printMove(std::cout,m);
                                            //std::cout<<'\n';
                                            if (bm == m)
                                                continue;
                                            //unsigned long long hbefor=b.getHash();
                                            bool fl = b.playMove(m);
                                            b.flipTurn();
                                            /*if(!fl){
                                                std::cout<<" not flattening\n";
                                                std::cout<<b<<'\n';
                                                printMove(std::cout,m);
                                                std::cout<<'\n'<<h<<r<<'\n';
                                                std::cout<<std::bitset<32>(slides1[h][r][cnt])<<'\n';
                                            }*/
                                            int ms;
                                            if (b.end())
                                                ms = neg * terminalEval(b);
                                            else if (d < info->depth_limit) {
                                                ms = -minimax(b, info, d + 1, -alpha - 1, -alpha,
                                                              (tp == CUT_NODE) ? ALL_NODE : CUT_NODE, in_nm);
                                                if (alpha < ms && ms < beta ||
                                                    tp == PV_NODE && ms == beta && beta == alpha + 1) {
                                                    if (ms == alpha + 1)
                                                        ms = alpha;
                                                    ms = -minimax(b, info, d + 1, -beta, -ms, tp, in_nm);
                                                }
                                            } else ms = neg * evaluate(b);
                                            b.undoMove(m, fl);
                                            b.flipTurn();
                                            //if(hbefor!=b.getHash()){
                                            //    std::cout<<" not resetting\n";
                                            //}
                                            if (ms > alpha) {
                                                bm = m;
                                                alpha = ms;
                                                if (alpha >= beta) {
                                                    b.liften(t);
                                                    goto eos;
                                                }
                                            }
                                        }
                                    }
                                }
                                //b.liften(t);
                            }
                        }
                    }

        eos:
        if (tp == CUT_NODE && alpha == alpha_backup)
            return alpha;
        if (transpositionTableEntry1 != nullptr) {
            if (alpha < scale * 100)
                transpositionTableEntry1->depth = info->depth_limit - d;
            else transpositionTableEntry1->depth = std::numeric_limits<int>::max();
            transpositionTableEntry1->bm = bm;
            if (alpha == alpha_backup)
                transpositionTableEntry1->upper_bound = alpha;
            else if (alpha >= beta)
                transpositionTableEntry1->lower_bound = alpha;
            else
                transpositionTableEntry1->lower_bound = transpositionTableEntry1->upper_bound = alpha;
        }
        return alpha;
    }

    template<int n>
    void printpv(boardstate<n> &b) {
        transpositionTableEntry *transpositionTableEntry1 = getEntry(b, false);
        if (transpositionTableEntry1 != nullptr && transpositionTableEntry1->bm != -1) {
            printMove(std::cout, transpositionTableEntry1->bm);
            std::cout << ' ';
            b.playMove(transpositionTableEntry1->bm);
            b.flipTurn();
            printpv(b);
            b.undoMove(transpositionTableEntry1->bm);
            b.flipTurn();
        }
    }

#define tl 3000

    template<int n>
    move d1_getMove(boardstate<n> &b, int &max) {
        //int neg = b.getTurn() == BLACK ? -1 : 1;
        move bm = -1;
        searchInfo info;
        info.nodes = 0;
        info.ttcuts = 0;
        info.fatt = 0;
        info.fsucc = 0;
        clearTable();
        auto start = std::chrono::system_clock::now();
        for (int dl = 1; max < scale * 100 && max > -scale * 100; dl++) {
            max = -scale * 1000000;
            int alpha = max;
            int beta = -max;
            info.depth_limit = dl;
            int pn = info.nodes;
            int ms = minimax(b, &info, 1, alpha, beta, PV_NODE, false);
            auto end = std::chrono::system_clock::now();
            bm = getEntry(b)->bm;
            max = ms;
            int tm = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
#ifndef ASS
            std::cout << dl << '\t' << ms << "\t pv [";
            printpv(b);
            std::cout << "] " << info.nodes << " nodes @" << info.nodes / (tm + 1) << " kNps[" << tm << " ms] ";
            std::cout << "EBF~=" << std::pow(info.nodes - pn, 1.0 / dl);
            std::cout << "] fatt " << info.fatt << " fsucc " << info.fsucc << " ttcuts " << info.ttcuts << " ";
            displayTTinfo();
#endif
            if (tm > tl)
                break;
        }
        return bm;
    }
}
#endif //A3_SEARCH_H
