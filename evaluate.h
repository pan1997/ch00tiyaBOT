//
// Created by pankaj on 20/9/16.
//

#ifndef A3_EVALUATE_H
#define A3_EVALUATE_H

#include "basic.h"
#include "boardstate.h"



namespace TAK {
    extern int scale;
    extern int move_advantage;
    extern int standingU;
    extern int capstoneU;
    extern int FCaptureU;
    extern int FReserveU;
    extern int SCaptureU;
    extern int SReserveU;
    extern int CCaptureU;
    extern int CReserveU;
    extern int Fattack;
    extern int Sattack;
    extern int Cattack;
    extern int groupU[8];

    void initGroups(int n);

    /*
     * flatstone importance increases as no.of empty squares decreases
     * flatstone count :scale
     * capsdtone count :capstoneU
     * standing stones :standingU
     * to move advantage :scale/2
     *
     * TODO
     * stacks
     * bonus for standing stone/capstone close to stacks
     * penalise flatstones near corners and boundries and reward those near center
     * roads
     */

    template<int n>
    int terminalEval(const boardstate<n> &b) {
        int score = 0;
        bool roadwin = false;
        int winner = -1;
        if (b.getGCW()[n] != 0 || b.getGCB()[n] != 0) {
            roadwin = true;
            //road
            if (b.getGCW()[n] == 0) {
                winner = BLACK;
                score = -1;
            }
            else if (b.getGCB()[n] == 0) {
                winner = WHITE;
                score = 1;
            }
            else if (b.getTurn() == BLACK) {//ie white has moved
                score = groupU[n];
                winner = WHITE;
            }
            else {
                winner = BLACK;
                score = -1;
            }
            score *= n * n;
        }
        int ns = popcnt(b.getWF() | b.getWC()) - popcnt(b.getBF() | b.getBC());//now capstone also counted
        if (!roadwin)
            winner = ns > 0 ? WHITE : ns < 0 ? BLACK : -1;
        score += ns;
        score += (winner == BLACK ? -b.getBlackLeft() : winner == WHITE ? b.getWhiteLeft() : 0);
        return score * scale * 100;
    }

    template<int n>
    int terminalEvalVerbose(const boardstate<n> &b) {
        int score = 0;
        bool roadwin = false;
        int winner = -1;
        if (b.getGCW()[n] != 0 || b.getGCB()[n] != 0) {
            roadwin = true;
            //road
            if (b.getGCW()[n] == 0) {
                winner = BLACK;
                score = -1;
            }
            else if (b.getGCB()[n] == 0) {
                winner = WHITE;
                score = 1;
            }
            else if (b.getTurn() == BLACK) {//ie white has moved
                score = groupU[n];
                winner = WHITE;
            }
            else {
                winner = BLACK;
                score = -1;
            }
            score *= n * n;
        }
        int ns = popcnt(b.getWF() | b.getWC()) - popcnt(b.getBF() | b.getBC());//now capstone also counted
        if (!roadwin)
            winner = ns > 0 ? WHITE : ns < 0 ? BLACK : -1;
        score += ns;
        score += (winner == BLACK ? -b.getBlackLeft() : winner == WHITE ? b.getWhiteLeft() : 0);
        if (score * (winner == BLACK ? -1 : 1) <= 0)
            std::cout << "Error " << score * winner << "\n";
        std::cout << (roadwin ? "R" : "F");
        std::cout << " winner :" << (winner == WHITE ? "WHITE" : winner == BLACK ? "BLACK" : "DRAW") << " own diff " <<
        ns << '\n';
        return score * scale * 100;
    }

    template<int n>
    int evaluateTop(const boardstate<n> &b) {
        return standingU * (popcnt(b.getWS()) - popcnt(b.getBS()));
    }

    template<int n>
    int evaluateTopFlat(const boardstate<n> &b) {
        return scale * (popcnt(b.getWF()) - popcnt(b.getBF())) + capstoneU * (popcnt(b.getWC()) - popcnt(b.getBC()));
    }

    template<int n>
    int evaluateGroups(const boardstate<n> &b) {
        int score = 0;
        for (int i = 0; i < n; i++)
            score += groupU[i] * (b.getGCW()[i] - b.getGCB()[i]);
        return score;
    }

    template<int n>
    int evaluateStacks(const boardstate<n> &b) {
        int score = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (b.getHeight(getSquare(i, j)) > 1) {
                    int cnt = b.countStacked(getSquare(i, j), std::min(n * n, b.getHeight(getSquare(i, j))),
                                             color_of(b.top(getSquare(i, j))));
                    int sign = (color_of(b.top(getSquare(i, j))) == WHITE ? 1 : -1);
                    if (isFlat(b.top(getSquare(i, j))))
                        score += ((std::min(n * n, b.getHeight(getSquare(i, j))) - cnt - 1) * FCaptureU +
                                  cnt * FReserveU) *
                                 sign;
                    else if (isCap(b.top(getSquare(i, j))))
                        score += ((std::min(n * n, b.getHeight(getSquare(i, j))) - cnt - 1) * CCaptureU +
                                  cnt * CReserveU) *
                                 sign;
                    else
                        score += ((std::min(n * n, b.getHeight(getSquare(i, j))) - cnt - 1) * SCaptureU +
                                  cnt * SReserveU) *
                                 sign;
                    /*if (isFlat(b.top(getSquare(i, j)))) {
                        score += sign * Fattack * (popcnt(neighbours(getBitboard(getSquare(i, j))) & b.getWF()) -
                                                   popcnt(neighbours(getBitboard(getSquare(i, j))) & b.getBF()));
                        score += sign * Sattack * (popcnt(neighbours(getBitboard(getSquare(i, j))) & b.getWS()) -
                                                   popcnt(neighbours(getBitboard(getSquare(i, j))) & b.getBS()));
                        score += sign * Cattack * (popcnt(neighbours(getBitboard(getSquare(i, j))) & b.getWC()) -
                                                   popcnt(neighbours(getBitboard(getSquare(i, j))) & b.getBC()));
                    }
                    else if (isStanding(b.top(getSquare(i, j)))) {
                        score += sign * Cattack * (popcnt(neighbours(getBitboard(getSquare(i, j))) & b.getWC()) -
                                                   popcnt(neighbours(getBitboard(getSquare(i, j))) & b.getBC()));
                    }*/
                }
        return score;
    }

    template<int n>
    int evaluate(const boardstate<n> &b) {
        int score =
                2 * n * n * (evaluateTopFlat(b) + (b.getTurn() == WHITE ? 1 : -1) * move_advantage) /
                (n * n + b.countEmpty());
        score += evaluateTop(b);
        score += evaluateGroups(b);
        score += evaluateStacks(b);
        return score;
    }
}
#endif //A3_EVALUATE_H
