//
// Created by pankaj on 27/9/16.
//

#ifndef A3_SEARCH_H
#define A3_SEARCH_H

#define FREQ 2
#define MAX_TIME 120
//
// Created by pankaj on 21/9/16.
//
#include "basic.h"
#include "boardstate.h"
#include "evaluate.h"
#include <chrono>
#include <cmath>
#include "transpositionTable.h"
#include <thread>
#include <future>

namespace TAK {

#define NMP_ALLOWED

    struct searchInfo {
        int nodes;
        int qnodes;
        int ttcuts;
        int fatt;
        int fsucc;
        int depth_limit;
        bool stop;
        //turn square place or move
        long long history[2][2][64];
        //player place/move order
        square order[2][2][64];
    };

    enum node_type {
        PV_NODE, CUT_NODE, ALL_NODE
    };

    inline int qdepth(int dl) {
        return 5;
    }

    inline bool storehistory(int d) {
        return d > 2;
    }

    template<int n>
    int qsearch(boardstate<n> &b, searchInfo *info, int alpha, int beta, int lim) {
        info->qnodes++;
        transpositionTableEntry *transpositionTableEntry1 = getEntry(b, true);
        if (transpositionTableEntry1 != nullptr) {
            if (transpositionTableEntry1->depth >= lim - 100) {
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

        int neg = b.getTurn() == BLACK ? -1 : 1;
        move bm = -1;
        int alpha_backup = alpha;
        int stand_pat = neg * evaluate(b);
        if (stand_pat >= beta)
            return beta;
        if (stand_pat > alpha)
            alpha = stand_pat;

        if (transpositionTableEntry1 != nullptr) {//best move first
            bm = transpositionTableEntry1->bm;
            if (bm != -1&&b.legal(bm)) {
                int pn = (b.getGCW()[n - 1] - b.getGCB()[n - 1]);
                bool fl = b.playMove(bm);
                b.flipTurn();
                int ms;
                if (b.end())
                    ms = neg * terminalEval(b);
                else if ((b.getGCW()[n - 1] - b.getGCB()[n - 1]) != pn&&lim>=0)
                    ms = -qsearch(b, info, -beta, -alpha, lim - 1);
                else
                    ms = stand_pat;
                b.undoMove(bm, fl);
                b.flipTurn();
                if (ms > alpha)
                    alpha = ms;
            }
        }
        bool ff = isPlaceMove(bm);
        //first flat, then standing then cap
        for (int z = 0; z < 2; z++) {
            if (alpha < beta && ((z == 0) == ff))
                for (int p = 1; p <= 3; p++)
                    if (p != 2 && (p != 3 || (b.getTurn() == WHITE ? b.getWhileCapLeft() : b.getBlackCapLeft()) > 0)) {
                        for (int i = 0; i < n * n; i++)
                            if (b.empty(info->order[b.getTurn()][1][i])) {
                                move m = construct_place_move(info->order[b.getTurn()][1][i],
                                                              (peice) (b.getTurn() | (p << 1)));
                                if (bm == m)
                                    continue;
                                int pn = (b.getGCW()[n - 1] - b.getGCB()[n - 1]);
                                b.playMove(m);
                                b.flipTurn();
                                int ms;
                                if (b.end())
                                    ms = neg * terminalEval(b);
                                else if ((b.getGCW()[n - 1] - b.getGCB()[n - 1]) != pn && lim >= 0)
                                    ms = -qsearch(b, info, -beta, -alpha, lim - 1);
                                else
                                    ms = stand_pat;
                                b.undoMove(m);
                                b.flipTurn();
                                if (ms > alpha) {
                                    bm = m;
                                    alpha = ms;
                                    if (alpha >= beta)
                                        goto qos;
                                }
                            }
                    }
            if (alpha < beta && ((z == 1) == ff))
                for (int i = 0; i < n * n; i++)
                    if ((!b.empty(info->order[b.getTurn()][0][i])) &&
                        //b.getHeight(getSquare(i,j))>1 &&
                        color_of(b.top(info->order[b.getTurn()][0][i])) == b.getTurn()) {
                        int lh = b.getHeight(info->order[b.getTurn()][0][i]);
                        lh = std::min(lh, n);
                        for (int k = 0; k < 4; k++) {
                            direction dir = (direction) k;
                            int lr = 0;
                            square t = info->order[b.getTurn()][0][i];
                            t = squareAt(t, dir);
                            for (int k = 0;
                                 k < n && (t != -1) && (b.empty(t) || isFlat(b.top(t))); t = squareAt(t, dir))
                                lr++;
                            for (int h = 1; h <= lh; h++)
                                for (int r = 1; r <= lr; r++) {
                                    for (int cnt = 0; cnt < count_slides[h][r]; cnt++) {
                                        move m = construct_move_move(info->order[b.getTurn()][0][i], dir, h,
                                                                     slides[h][r][cnt]);
                                        if (bm == m)
                                            continue;
                                        int pn = (b.getGCW()[n - 1] - b.getGCB()[n - 1]);
                                        bool fl = b.playMove(m);
                                        b.flipTurn();
                                        int ms;
                                        if (b.end())
                                            ms = neg * terminalEval(b);
                                        else if ((b.getGCW()[n - 1] - b.getGCB()[n - 1]) != pn &&
                                                 lim >= 0)//&& b.getHeight(getSquare(i,j))>1)
                                            ms = -qsearch(b, info, -beta, -alpha, lim - 1);
                                        else
                                            ms = stand_pat;
                                        b.undoMove(m, fl);
                                        b.flipTurn();
                                        if (ms > alpha) {
                                            bm = m;
                                            alpha = ms;
                                            if (alpha >= beta)
                                                goto qos;
                                        }
                                    }
                                }
                            if (isCap(b.top(info->order[b.getTurn()][0][i])) && t != -1 && !b.empty(t) &&
                                isStanding(b.top(t))) {
                                lr++;
                                for (int h = lr; h <= lh; h++) {
                                    for (int r = lr; r <= lr; r++) {
                                        for (int cnt = 0; cnt < count_slides1[h][r]; cnt++) {
                                            move m = construct_move_move(info->order[b.getTurn()][0][i], dir, h,
                                                                         slides1[h][r][cnt]);
                                            if (bm == m)
                                                continue;
                                            int pn = (b.getGCW()[n - 1] - b.getGCB()[n - 1]);
                                            bool fl = b.playMove(m);
                                            b.flipTurn();
                                            int ms;
                                            if (b.end())
                                                ms = neg * terminalEval(b);
                                            else if ((b.getGCW()[n - 1] - b.getGCB()[n - 1]) != pn && lim >= 0)
                                                ms = -qsearch(b, info, -beta, -alpha, lim - 1);
                                            else
                                                ms = stand_pat;
                                            b.undoMove(m, fl);
                                            b.flipTurn();
                                            if (ms > alpha) {
                                                bm = m;
                                                alpha = ms;
                                                if (alpha >= beta) {
                                                    goto qos;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
        }
        qos:
        if (transpositionTableEntry1 != nullptr) {
            if (alpha < scale * 100)
                transpositionTableEntry1->depth = lim - 100;
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
    int minimax(boardstate<n> &b, searchInfo *info, int d, int alpha, int beta, node_type tp, bool in_nm,
                bool show_legal = false) {
        info->nodes++;
        bool clearbounds = false;
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
                clearbounds = true;
                //transpositionTableEntry1->lower_bound = std::numeric_limits<int>::min();
                //transpositionTableEntry1->upper_bound = std::numeric_limits<int>::max();
            }
        }

        if (info->stop)
            return alpha;

        bool extend = false;

        if (tp != PV_NODE) {
            bool pruned = false;
#ifdef NMP_ALLOWED
            if (info->depth_limit > d + 1 && !in_nm) {
                info->fatt++;
                b.flipTurn();
                int bound = beta - 2*move_advantage * n * n /
                                   (n * n + b.countEmpty()-2);
                int ms = -minimax(b, info, (info->depth_limit - d > 3) ? d + 3 : d + 1, -bound, -bound + 1,
                                  (tp == CUT_NODE) ? ALL_NODE : CUT_NODE, true);
                b.flipTurn();
                if (ms >= bound) {
                    info->fsucc++;
                    pruned = true;
                }
            }
#endif
            if (pruned)
                return beta;

            //futility pruning
            /*if (info->depth_limit == d + 1 &&
                transpositionTableEntry1 != nullptr &&
                transpositionTableEntry1->upper_bound != std::numeric_limits<int>::max() &&
                transpositionTableEntry1->upper_bound + scale < alpha) {
                return alpha;
            }*/

        }

        int neg = b.getTurn() == BLACK ? -1 : 1;
        move bm = -1;
        int alpha_backup = alpha;
        if (extend)
            info->depth_limit++;

        if (clearbounds) {
            transpositionTableEntry1->lower_bound = std::numeric_limits<int>::min();
            transpositionTableEntry1->upper_bound = std::numeric_limits<int>::max();
        }

        if (transpositionTableEntry1 != nullptr) {//best move first
            bm = transpositionTableEntry1->bm;
            if (bm != -1&&b.legal(bm)) {
                bool fl = b.playMove(bm);
                b.flipTurn();
                int ms;
                if (d < info->depth_limit)
                    ms = -minimax(b, info, d + 1, -beta, -alpha, tp, in_nm);
                else ms = -qsearch(b, info, -beta, -alpha, qdepth(info->depth_limit));//ms = neg * evaluate(b);
                b.undoMove(bm, fl);
                b.flipTurn();
                //if (fl)
                //    info->fsucc++;
                if (ms > alpha) {
                    if (storehistory(info->depth_limit - d))
                        info->history[b.getTurn()][isPlaceMove(bm) ? 1 : 0][bm & 63] +=
                                (info->depth_limit - d) * (info->depth_limit - d);
                    alpha = ms;
                }
            }
        }

        bool ff = isPlaceMove(bm);
        //first flat, then standing then cap
        for (int z = 0; z < 2; z++) {
            if (alpha < beta && ((z == 0) == (ff)))
                for (int p = 1; p <= 3; p++)
                    if (p != 2 && (p != 3 || (b.getTurn() == WHITE ? b.getWhileCapLeft() : b.getBlackCapLeft()) > 0)) {
                        for (int i = 0; i < n * n; i++)
                            if (b.empty(info->order[b.getTurn()][1][i])) {
                                move m = construct_place_move(info->order[b.getTurn()][1][i],
                                                              (peice) (b.getTurn() | (p << 1)));
                                if (bm == m)
                                    continue;
                                if (show_legal) {
                                    printMove(std::cerr, m);
                                    std::cerr << '\n';
                                }
                                b.playMove(m);
                                b.flipTurn();
                                int ms;
                                if (b.end())
                                    ms = neg * terminalEval(b);
                                else {
                                    if (d < info->depth_limit) {
                                        ms = -minimax(b, info, d + 1, -alpha - 1, -alpha,
                                                      (tp == CUT_NODE) ? ALL_NODE : CUT_NODE, in_nm);
                                        if (alpha < ms && ms < beta ||
                                            tp == PV_NODE && ms == beta && beta == alpha + 1) {
                                            if (ms == alpha + 1)
                                                ms = alpha;
                                            ms = -minimax(b, info, d + 1, -beta, -ms, tp, in_nm);
                                        }
                                    }
                                    else {
                                        ms = -qsearch(b, info, -alpha - 1, -alpha, qdepth(info->depth_limit));
                                        if (alpha < ms && ms < beta) {
                                            ms = -qsearch(b, info, -beta, -alpha, qdepth(info->depth_limit));
                                        }
                                    }
                                }
                                b.undoMove(m);
                                b.flipTurn();
                                if (ms > alpha) {
                                    bm = m;
                                    alpha = ms;
                                    if (alpha >= beta) {
                                        if (storehistory(info->depth_limit - d))
                                            info->history[b.getTurn()][1][m & 63] +=
                                                    (info->depth_limit - d) * (info->depth_limit - d);
                                        goto eos;
                                    }
                                }
                            }
                    }
            if (alpha < beta && ((z == 1) == ff))
                for (int i = 0; i < n * n; i++)
                    if (!b.empty(info->order[b.getTurn()][0][i]) &&
                        color_of(b.top(info->order[b.getTurn()][0][i])) == b.getTurn()) {
                        int lh = b.getHeight(info->order[b.getTurn()][0][i]);
                        lh = std::min(lh, n);
                        for (int k = 0; k < 4; k++) {
                            direction dir = (direction) k;
                            int lr = 0;
                            square t = info->order[b.getTurn()][0][i];
                            t = squareAt(t, dir);
                            for (int k = 0;
                                 k < n && (t != -1) && (b.empty(t) || isFlat(b.top(t))); t = squareAt(t, dir))
                                lr++;
                            for (int h = 1; h <= lh; h++)
                                for (int r = 1; r <= lr; r++) {
                                    for (int cnt = 0; cnt < count_slides[h][r]; cnt++) {
                                        move m = construct_move_move(info->order[b.getTurn()][0][i], dir, h,
                                                                     slides[h][r][cnt]);
                                        if (bm == m)
                                            continue;
                                        if (show_legal) {
                                            printMove(std::cerr, m);
                                            std::cerr << '\n';
                                        }
                                        bool fl = b.playMove(m);
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
                                        }
                                        else {
                                            ms = -qsearch(b, info, -alpha - 1, -alpha, qdepth(info->depth_limit));
                                            if (alpha < ms && ms < beta) {
                                                ms = -qsearch(b, info, -beta, -alpha, qdepth(info->depth_limit));
                                            }
                                        }
                                        b.undoMove(m, fl);
                                        b.flipTurn();
                                        if (ms > alpha) {
                                            bm = m;
                                            alpha = ms;
                                            if (alpha >= beta) {
                                                if (storehistory(info->depth_limit - d))
                                                    info->history[b.getTurn()][0][m & 63] +=
                                                            (info->depth_limit - d) * (info->depth_limit - d);
                                                goto eos;
                                            }
                                        }
                                    }
                                }
                            if (isCap(b.top(info->order[b.getTurn()][0][i])) && t != -1 && !b.empty(t) &&
                                isStanding(b.top(t))) {
                                lr++;
                                for (int h = lr; h <= lh; h++) {
                                    for (int r = lr; r <= lr; r++) {
                                        for (int cnt = 0; cnt < count_slides1[h][r]; cnt++) {
                                            move m = construct_move_move(info->order[b.getTurn()][0][i], dir, h,
                                                                         slides1[h][r][cnt]);
                                            if (bm == m)
                                                continue;
                                            if (show_legal) {
                                                printMove(std::cerr, m);
                                                std::cerr << '\n';
                                            }
                                            bool fl = b.playMove(m);
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
                                            }
                                            else {
                                                ms = -qsearch(b, info, -alpha - 1, -alpha,
                                                              qdepth(info->depth_limit));
                                                if (alpha < ms && ms < beta) {
                                                    ms = -qsearch(b, info, -beta, -alpha,
                                                                  qdepth(info->depth_limit));
                                                }
                                            }
                                            b.undoMove(m, fl);
                                            b.flipTurn();
                                            if (ms > alpha) {
                                                bm = m;
                                                alpha = ms;
                                                if (alpha >= beta) {
                                                    if (storehistory(info->depth_limit - d))
                                                        info->history[b.getTurn()][0][m & 63] +=
                                                                (info->depth_limit - d) * (info->depth_limit - d);
                                                    goto eos;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
        }
        if (alpha < beta)//standing stones last
            for (int p = 2; p < 3; p++) {
                for (int i = 0; i < n * n; i++)
                    if (b.empty(info->order[b.getTurn()][1][i])) {
                        move m = construct_place_move(info->order[b.getTurn()][1][i], (peice) (b.getTurn() | (p << 1)));
                        if (bm == m)
                            continue;
                        if (show_legal) {
                            printMove(std::cerr, m);
                            std::cerr << '\n';
                        }
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
                        else {
                            ms = -qsearch(b, info, -alpha - 1, -alpha, qdepth(info->depth_limit));
                            if (alpha < ms && ms < beta) {
                                ms = -qsearch(b, info, -beta, -alpha, qdepth(info->depth_limit));
                            }
                        }
                        b.undoMove(m);
                        b.flipTurn();
                        if (ms > alpha) {
                            bm = m;
                            alpha = ms;
                            if (alpha >= beta) {
                                info->history[b.getTurn()][0][m & 63] +=
                                        (info->depth_limit - d) * (info->depth_limit - d);
                                goto eos;
                            }
                        }
                    }
            }
        eos:
        if (extend)
            info->depth_limit--;
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
    void printpv(boardstate<n> &b,bool neg) {
        transpositionTableEntry *transpositionTableEntry1 = getEntry(b, false);
        if (transpositionTableEntry1 != nullptr && transpositionTableEntry1->bm != -1) {
#ifndef ASS
            printMove(std::cout, transpositionTableEntry1->bm);
            std::cout << ' ';
#else
            printMove(std::cerr, transpositionTableEntry1->bm);
            std::cerr << ' ';
#endif
            bool fl = b.playMove(transpositionTableEntry1->bm);
            b.flipTurn();
            printpv(b,neg);
            b.undoMove(transpositionTableEntry1->bm, fl);
            b.flipTurn();
        }
        else {
            int x = (neg?-1:1)*(b.end() ? terminalEval(b) : evaluate(b));
#ifndef ASS
            std::cout<<" #("<<x<<")";
#else
            std::cerr << " #(" << x << ")";
#endif
        }
    }

    extern searchInfo info;

    inline void initInfo(int n) {
        for (int i = 0; i < 64; i++)
            for (int k = 0; k < 2; k++) {
                info.history[k][0][i] = 0;
                info.history[k][1][i] = 0;
            }
        int x = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                info.order[0][0][x] = info.order[1][0][x] = getSquare(i, j);
                info.order[0][1][x] = info.order[1][1][x] = getSquare(i, j);
                x++;
            }

    }

    template<int n>
    move search(boardstate<n> &b, int &max, int Tlimit) {
        //int neg = b.getTurn() == BLACK ? -1 : 1;
        auto start = std::chrono::system_clock::now();
        move bm = -1;
        //searchInfo info;
        info.nodes = 0;
        info.ttcuts = 0;
        info.fatt = 0;
        info.fsucc = 0;
        info.qnodes = 0;
        for (int i = 0; i < 64; i++)
            for (int k = 0; k < 2; k++) {
                info.history[k][0][i] /= 8;
                info.history[k][1][i] /= 8;
            }
        info.stop = false;
        clearTable();
        boardstate<n> backup(b);
        int ms;
        int tm;
        double ebf;
        move pbm;
        for (int dl = 1; max < scale * 100 && max > -scale * 100; dl++) {
            max = -scale * 1000000;
            info.depth_limit = dl;
            int pn = info.nodes;
            int alpha = max;
            int beta = -max;
            //ms = minimax(b, &info, 1, alpha, beta, PV_NODE, false, false);
            //std::future<int> future=std::async(minimax,std::ref(b),&info,1,std::ref(alpha),std::ref(beta),PV_NODE,false);
            std::future<int> future = std::async(std::launch::async, [b, alpha, beta]() mutable {
                return minimax(b, &info, 1, alpha, beta, PV_NODE, false, false);
                //return mtdf(b,&info);
            });
            std::future_status status;
            int count = 0;
            do {
                status = future.wait_for(std::chrono::seconds(FREQ));
                if (status == std::future_status::timeout) {
                    tm = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now() - start).count();
                    std::cerr << dl <<"?\t[";
                    printpv(backup,b.getTurn()==BLACK);
                    std::cerr << "] (" << info.nodes << "," << info.qnodes << ") nodes @" <<
                    (info.qnodes + info.nodes) / (tm + 1) << " kNps[" << tm << " ms] ";
                    std::cerr << " " << (info.fsucc * 100 / (info.fatt + 1)) << "% of " << info.fatt << " nmt ";
                    displayTTinfo();
                }
                count++;
            } while (status != std::future_status::ready && count <MAX_TIME / FREQ);
            if (future.wait_for(std::chrono::seconds(2)) == std::future_status::timeout) {
                info.stop = true;
                std::cerr << "stopped";
                //future.wait_for(std::chrono::seconds());
            }
            ms = future.get();
            auto end = std::chrono::system_clock::now();
            bm = getEntry(b)->bm;
            if (!info.stop)
                pbm = bm;
            max = ms;
            tm = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
#ifndef ASS
            if(!info.stop){std::cout << dl << "\t[";
            printpv(b,b.getTurn()==BLACK);
            std::cout << "] (" << info.nodes << "," << info.qnodes << ") nodes @" <<
            (info.qnodes + info.nodes) / (tm + 1) << " kNps[" << tm << " ms] ";
            std::cout << "EBF~=" << (ebf = (info.nodes - pn) * 1.0 / (pn + 1));
            std::cout << "] fatt " << info.fatt << " fsucc " << info.fsucc << " ttcuts " << info.ttcuts << " ";
            displayTTinfo();
            }
#else
            if (!info.stop) {
                std::cerr << dl << "\t[";
                printpv(b,b.getTurn()==BLACK);
                std::cerr << "] (" << info.nodes << "," << info.qnodes << ") nodes @" <<
                (info.qnodes + info.nodes) / (tm + 1) << " kNps[" << tm << " ms] ";
                std::cerr << "EBF=" << (ebf = std::pow((info.nodes - pn), 1.0 / dl));
                std::cerr << " " << (info.fsucc * 100 / (info.fatt + 1)) << "% of " << info.fatt << " nmt ";
                displayTTinfo();
            }
#endif
            for (int i = 1; i < n * n; i++)
                for (int p = 0; p < 2; p++)
                    for (int t = 0; t < 2; t++) {
                        square s = info.order[p][t][i];
                        int x = info.history[p][t][s];
                        int j = i - 1;
                        for (; j >= 0 && info.history[p][t][info.order[p][t][j]] < x; j--) {
                            info.order[p][t][j + 1] = info.order[p][t][j];
                        }
                        info.order[p][t][j + 1] = s;
                    }
            if (tm * (ebf + 1) > Tlimit * 2 && dl > 2 && (pn > 100) || (tm * 3 > Tlimit)) {
                //if (dl>8) {
                break;
            }
        }
        return pbm;
    }
}
#endif //A3_SEARCH_H
