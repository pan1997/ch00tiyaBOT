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
        if (b.getGCW()[n] != 0 || b.getGCW()[n] != 0) {
            //road
            if (b.getGCW()[n] == 0)
                score = -groupU[n];
            else if (b.getGCB()[n] == 0)
                score = groupU[n];
            else if (b.getTurn() == BLACK)//ie white has moved
                score = groupU[n];
            else score = -groupU[n];
        }
        int ns = popcnt(b.getWF() | b.getWC()) - popcnt(b.getBF() | b.getBC());//now capstone also counted
        score += ns * scale * 100;
        score += ((b.getTurn() == WHITE) ? -b.getBlackLeft() : b.getWhiteLeft()) * scale * 100;
        return score;
    }

    template<int n>
    int evaluateTop(const boardstate<n> &b) {
        return scale * (popcnt(b.getWF()) - popcnt(b.getBF())) + standingU * (popcnt(b.getWS()) - popcnt(b.getBS())) +
               capstoneU * (popcnt(b.getWC()) - popcnt(b.getBC()));
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
        return 0;
    }

    template<int n>
    int evaluate(const boardstate<n> &b) {
        int score = 2 * n * evaluateTop(b) / (n * n + b.countEmpty());
        score += evaluateGroups(b);
        score += evaluateStacks(b);
        if (b.getTurn() == WHITE)
            score += move_advantage;
        else
            score -= move_advantage;
        return score;
    }
}
#endif //A3_EVALUATE_H
