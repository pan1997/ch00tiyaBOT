//
// Created by pankaj on 25/9/16.
//
#include "evaluate.h"
#include "search.h"

namespace TAK {
    int scale = 100;
    int move_advantage = 50;
    int standingU = 52;
    int capstoneU = 90;
    int groupU[8];
    int FCaptureU = -45;
    int FReserveU = 50;
    int SCaptureU = -30;
    int SReserveU = 70;
    int CCaptureU = -30;
    int CReserveU = 65;
    //int placeThreat=44;
    int citadel = 0;
    int center = 7;
    //int emptyInfluence = -2;//drop protected squares
    //int flatInfluence = 0;
    int underCap = 14;
    int endgameCutoff = 9;

    int weights[8][5];
    bitboard centerBoard;
    bitboard allBoard;
    bitboard citadels[7][7];

    void initCitadels() {
        int cnt = 0;
        for (int i = 0; i < 7; i++)
            for (int j = 0; j < 7; j++) {
                citadels[i][j] = getBitboard(getSquare(i, j)) | getBitboard(getSquare(i + 1, j)) |
                                 getBitboard(getSquare(i, j + 1)) | getBitboard(getSquare(i + 1, j + 1));
                //std::cout<<std::bitset<64>(citadels[i][j])<<'\n';
            }
    }

    //empty oppflat oppwall oppcap mywall

    void initGroups(int n) {
        centerBoard = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                if (i != 0 && i != n - 1 && j != 0 && j != n - 1)
                    centerBoard |= getBitboard(getSquare(i, j));
                allBoard |= getBitboard(getSquare(i, j));
            }
        switch (n) {
            case 4:
                groupU[0] = 0;
                groupU[1] = 0;
                groupU[2] = 15;
                groupU[3] = 60;
                groupU[4] = scale * 1600;
                weights[0][0] = weights[0][1] = weights[0][2] = weights[0][3] = weights[0][4] = 0;
                weights[1][0] = weights[1][1] = weights[1][2] = weights[1][3] = weights[1][4] = 0;
                weights[2][0] = weights[2][1] = weights[2][2] = weights[2][3] = weights[2][4] = 0;
                weights[3][0] = weights[3][1] = weights[3][2] = weights[3][3] = weights[3][4] = 0;
                weights[4][0] = 85;
                weights[4][1] = 21;
                weights[4][2] = 2;
                weights[4][3] = 0;
                weights[4][4] = 0;
                break;
            case 5:
                groupU[0] = 0;
                groupU[1] = 0;
                groupU[2] = 5;
                groupU[3] = 25;
                groupU[4] = 72;
                groupU[5] = scale * 2500;
                weights[0][0] = weights[0][1] = weights[0][2] = weights[0][3] = weights[0][4] = 0;
                weights[1][0] = weights[1][1] = weights[1][2] = weights[1][3] = weights[1][4] = 0;
                weights[2][0] = weights[2][1] = weights[2][2] = weights[2][3] = weights[2][4] = 0;
                weights[3][0] = 1;
                weights[3][1] = weights[3][2] = weights[3][3] = weights[3][4] = 0;
                weights[4][0] = 5;
                weights[4][1] = 2;
                weights[4][2] = 1;
                weights[4][3] = 0;
                weights[4][4] = 0;
                weights[5][0] = 65;
                weights[5][1] = 19;
                weights[5][2] = 1;
                weights[5][3] = 0;
                weights[5][4] = 4;
                /*
                 * groupU[0] = 0
                groupU[1] = 0;
                groupU[2] = 5;
                groupU[3] = 25;
                groupU[4] = 58;
                groupU[5] = scale * 2500;
                weights[0][0]=weights[0][1]=weights[0][2]=weights[0][3]=weights[0][4]=0;
                weights[1][0]=weights[1][1]=weights[1][2]=weights[1][3]=weights[1][4]=0;
                weights[2][0]=weights[2][1]=weights[2][2]=weights[2][3]=weights[2][4]=0;
                weights[3][0]=weights[3][1]=weights[3][2]=weights[3][3]=weights[3][4]=0;
                weights[4][0]=5;
                weights[4][1]=2;
                weights[4][2]=1;
                weights[4][3]=0;
                weights[4][4]=0;
                weights[5][0]=85;
                weights[5][1]=28;
                weights[5][2]=2;
                weights[5][3]=0;
                weights[5][4]=4;
                 */
                break;
            case 6:
                groupU[0] = 0;
                groupU[1] = 0;
                groupU[2] = 2;
                groupU[3] = 5;
                groupU[4] = 25;
                groupU[5] = 60;
                groupU[6] = scale * 3600;
                weights[0][0] = weights[0][1] = weights[0][2] = weights[0][3] = weights[0][4] = 0;
                weights[1][0] = weights[1][1] = weights[1][2] = weights[1][3] = weights[1][4] = 0;
                weights[2][0] = weights[2][1] = weights[2][2] = weights[2][3] = weights[2][4] = 0;
                weights[3][0] = weights[3][1] = weights[3][2] = weights[3][3] = weights[3][4] = 0;
                weights[4][0] = weights[4][1] = weights[4][2] = weights[4][3] = weights[4][4] = 0;
                weights[5][0] = 5;
                weights[5][1] = 2;
                weights[5][2] = 1;
                weights[5][3] = 0;
                weights[5][4] = 0;
                weights[6][0] = 68;
                weights[6][1] = 32;
                weights[6][2] = 2;
                weights[6][3] = 0;
                weights[6][4] = 4;
                break;
            case 7:
                groupU[0] = 0;
                groupU[1] = 0;
                groupU[2] = 0;
                groupU[3] = 5;
                groupU[4] = 15;
                groupU[5] = 25;
                groupU[6] = 60;
                groupU[7] = scale * 4900;
                weights[0][0] = weights[0][1] = weights[0][2] = weights[0][3] = weights[0][4] = 0;
                weights[1][0] = weights[1][1] = weights[1][2] = weights[1][3] = weights[1][4] = 0;
                weights[2][0] = weights[2][1] = weights[2][2] = weights[2][3] = weights[2][4] = 0;
                weights[3][0] = weights[3][1] = weights[3][2] = weights[3][3] = weights[3][4] = 0;
                weights[4][0] = weights[4][1] = weights[4][2] = weights[4][3] = weights[4][4] = 0;
                weights[5][0] = weights[5][1] = weights[5][2] = weights[5][3] = weights[5][4] = 0;
                weights[6][0] = 5;
                weights[6][1] = 2;
                weights[6][2] = 1;
                weights[6][3] = 0;
                weights[6][4] = 0;
                weights[7][0] = 68;
                weights[7][1] = 32;
                weights[7][2] = 2;
                weights[7][3] = 0;
                weights[7][4] = 4;
                break;
            case 8:
                groupU[0] = 0;
                groupU[1] = 0;
                groupU[2] = 0;
                groupU[3] = 5;
                groupU[4] = 15;
                groupU[5] = 30;
                groupU[6] = 45;
                groupU[7] = 85;
                groupU[7] = scale * 6400;
                break;
        }
    }

    searchInfo info;
}
