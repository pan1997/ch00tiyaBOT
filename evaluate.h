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
    extern int standingU; //1
    extern int capstoneU; //2
    extern int FCaptureU; //3
    extern int FReserveU; //4
    extern int SCaptureU; //5
    extern int SReserveU; //6
    extern int CCaptureU; //7
    extern int CReserveU; //8
    extern int groupU[8];
    extern int citadel;   //9
    extern int center;    //10
    extern int underCap;
    extern int endgameCutoff;
    extern bitboard citadels[7][7];
    extern bitboard centerBoard;
    extern bitboard allBoard;
    extern int potential;

    void initGroups(int n);

    void initCitadels();

    /*
     * flatstone importance increases as no.of empty squares decreases
     * flatstone count :scale
     * capsdtone count :capstoneU
     * standing stones :standingU
     * to move advantage :scale/2
     * stacks
     * roads
     * penalise flatstones near corners and boundries and reward those near center
     *
     * TODO
     * bonus for standing stone/capstone close to stacks
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
                winner = WHITE;
                score=1;
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
        return standingU * (popcnt(b.getWS()) - popcnt(b.getBS())) +
               capstoneU * (popcnt(b.getWC()) - popcnt(b.getBC()));
    }

    template<int n>
    int evaluateTopFlat(const boardstate<n> &b) {
        return scale * (popcnt(b.getWF()) - popcnt(b.getBF()));
    }

    template<int n>
    int evaluateGroups(const boardstate<n> &b) {
        int score = 0;
        for (int i = 0; i < n; i++)
            score += groupU[i] * (b.getGCW()[i] - b.getGCB()[i]);
        return score;
    }

    template<int n>
    int evaluateCitadels(const boardstate<n> &b) {
        bitboard W = b.getWF() | b.getWC();
        bitboard B = b.getBF() | b.getBC();
        int lim = n - 1;
        int score = 0;
        for (int i = 0; i < lim; i++)
            for (int j = 0; j < lim; j++)
                if ((W & citadels[i][j]) == citadels[i][j])
                    score += citadel;
                else if ((B & citadels[i][j]) == citadels[i][j])
                    score -= citadel;
        return score;
    }

    template<int n>
    int evaluateStacks(const boardstate<n> &b) {
        int score = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                square s=getSquare(i,j);
                if (b.getHeight(s) > 1) {
                    int cnt = b.countStacked(s, b.getHeight(s),color_of(b.top(s)));
                    int sign = (color_of(b.top(s)) == WHITE ? 1 : -1);
                    if (isFlat(b.top(s)))
                        score += //((std::min(n * n, b.getHeight(getSquare(i, j))) - cnt - 1) * FCaptureU +
                                ((b.getHeight(s) - cnt - 1) * FCaptureU + cnt * FReserveU) * sign;
                    else if (isCap(b.top(s)))
                        score += //((std::min(n * n, b.getHeight(getSquare(i, j))) - cnt - 1) * CCaptureU +
                                ((b.getHeight(s) - cnt - 1) * CCaptureU + cnt * CReserveU) * sign;
                    else
                        score += //((std::min(n * n, b.getHeight(getSquare(i, j))) - cnt - 1) * SCaptureU +
                                ((b.getHeight(s) - cnt - 1) * SCaptureU + cnt * SReserveU) * sign;
                    if (isCap(b.top(s)) && b.getHeight(s) > 1) {
                        if (color_of(b.underTop(s)) == BLACK)
                            score -= underCap;
                        else score += underCap;
                    }
                    cnt = b.countStacked(s, std::min(b.getHeight(s), n), color_of(b.top(s)));
                    if(cnt>0) {
                        bitboard bd = b.getBC() | b.getBF() | b.getBS() | b.getWC() | b.getWF() | b.getWS();
                        bd = ~bd;
                        if (sign > 0)
                            bd = bd | b.getBF();
                        else
                            bd = bd | b.getWF();
                        int mx = 0;
                        for (int d = 0; d < 4; d++) {
                            square q = squareAt(s, (direction) d);
                            if (q != -1 && (b.empty(q) || isFlat(b.top(q)))) {
                                int t = popcnt(spread[i][j][cnt][d] & bd);
                                //std::cout << i << " " << j << " spread in " << d << " is " << t << '\n';
                                if (t > mx)
                                    mx = t;
                            }
                        }
                        score += potential * mx * sign;
                    }
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
            }
        return score;
    }

    template<int n>
    int evaluateCenter(const boardstate<n> &b) {
        return center * (popcnt((b.getWF() | b.getWC()) & centerBoard) - popcnt((b.getBF() | b.getBC()) & centerBoard));
    }

    /*
    template<int n>
    int evaluateInfluence(const boardstate<n> &b) {
        bitboard W = b.getWC() | b.getWF() | b.getWS();
        bitboard B = b.getBF() | b.getBS() | b.getBC();
        bitboard winf = (neighbours(W) | W) & allBoard;
        bitboard binf = (neighbours(B) | B) & allBoard;

        /*printBitboard(std::cout, W);
        printBitboard(std::cout, B);
        printBitboard(std::cout, winf);
        printBitboard(std::cout, binf);
        std::cout << "White inf empty " << popcnt(winf & ~(W | B)) << '\n';
        std::cout << "Black inf empty " << popcnt(binf & ~(W | B)) << '\n';
        std::cout << "White inf flat " << popcnt(winf & (b.getWF() | b.getBF())) << '\n';
        std::cout << "Black inf flat " << popcnt(binf & (b.getWF() | b.getBF())) << '\n';

        //return emptyInfluence * (popcnt(winf & ~(W | B)) - popcnt(binf & ~(W | B)));
        //flatInfluence * (popcnt(winf & (b.getWF() | b.getBF())) - popcnt(binf & (b.getWF() | b.getBF())));
    }*/

    template<int n>
    void countThreats(const boardstate<n> &b, int &wc, int &bc) {
        bitboard W = b.getWC() | b.getWF();
        bitboard B = b.getBF() | b.getBC();
        bitboard empty = ~(W | B | b.getBS() | b.getWS());
        wc = bc = 0;
        bitboard s, grp;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                s = getBitboard(getSquare(i, j)) & empty;
                if (s != 0) {
                    grp = group(W | s, s);
                    if (countRows(grp, n) == n || countCols(grp, n) == n)
                        wc++;
                    grp = group(B | s, s);
                    if (countRows(grp, n) == n || countCols(grp, n) == n)
                        bc++;
                }
            }
    }


    //empty oppflat oppwall oppcap mywall
    extern int weights[8][5];

    template<int n>int evaluate2(const boardstate<n>&b) {
        int score = 0;
        int origr,origc;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                square s = getSquare(i, j);
                bitboard bd = neighbours(getBitboard(s));
                //bd=b.join(bd,color_of(b.top(s)));
                if (b.empty(s) || color_of(b.top(s)) == BLACK) {
                    bitboard gr = b.join(bd, WHITE, origr, origc);// | getBitboard(s);
                    if (gr != 0) {
                        gr |= getBitboard(s);
                        int r = countRows(gr, n);
                        int c = countCols(gr, n);
                        if (r <= origr)
                            r = 0;
                        if (c <= origc)
                            c = 0;
                        if (r != 0 || c != 0) {
                            //printBitboard(std::cout,gr);
                            //std::cout<<"Added\n";
                            if (b.empty(s)) {
                                score += weights[r][0];
                                score += weights[c][0];
                            }
                            else if (isFlat(b.top(s))) {
                                score += weights[r][1];
                                score += weights[c][1];
                            }
                            else if (isStanding(b.top(s))) {
                                score += weights[r][2];
                                score += weights[c][2];
                            }
                            else if (isCap(b.top(s))) {
                                score += weights[r][3];
                                score += weights[c][3];
                            }
                        }
                    }
                }
                if (b.empty(s) || color_of(b.top(s)) == WHITE) {
                    bitboard gr = b.join(bd, BLACK, origr,origc);// | getBitboard(s);
                    if (gr != 0) {
                        gr|=getBitboard(s);
                        int r = countRows(gr, n);
                        int c = countCols(gr, n);
                        if (r <= origr)
                            r = 0;
                        if (c <= origc)
                            c = 0;
                        if (r != 0 || c != 0) {
                            //printBitboard(std::cout,gr);
                            //std::cout<<"subtracted\n";
                            if (b.empty(s)) {
                                score -= weights[r][0];
                                score -= weights[c][0];
                            }
                            else if (isFlat(b.top(s))) {
                                score -= weights[r][1];
                                score -= weights[c][1];
                            }
                            else if (isStanding(b.top(s))) {
                                score -= weights[r][2];
                                score -= weights[c][2];
                            }
                            else if (isCap(b.top(s))) {
                                score -= weights[r][3];
                                score -= weights[c][3];
                            }
                        }
                    }
                }
            }
        return score;
    }

    //extern double r;

    template<int n>
    int evaluate(const boardstate<n> &b, int x) {
        int factor = std::min(std::min(b.getWhiteLeft(), b.getBlackLeft()), endgameCutoff);
        int score = x + (b.getTurn() == WHITE ? 1 : -1) * move_advantage;
        //(3 * endgameCutoff - 2 * factor) * (x + (b.getTurn() == WHITE ? 1 : -1) * move_advantage) /
        //endgameCutoff;
        score = (2 * endgameCutoff - factor) * (score) / endgameCutoff;
        /*
        5 * n * n * (evaluateTopFlat(b) + (b.getTurn() == WHITE ? 1 : -1) * move_advantage) /
        (3 * n * n + 2*b.countEmpty());
         */
        score += evaluateTop(b);
        score += evaluateGroups(b);
        score += evaluateStacks(b);
        //score += evaluateCitadels(b);
        //score += evaluateInfluence(b);
        score += evaluateCenter(b);
        score+=evaluate2(b);
        return score;
    }
}
#endif //A3_EVALUATE_H
