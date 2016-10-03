//
// Created by pankaj on 25/9/16.
//
#include "evaluate.h"
namespace TAK {

    int scale = 100;
    int move_advantage = 50;
    int standingU = 50;
    int capstoneU = 96;
    int groupU[8];
    int FCaptureU=-55;
    int FReserveU=55;
    int SCaptureU=-20;
    int SReserveU=70;
    int CCaptureU=-25;
    int CReserveU=75;
    int citadel=5;
    int center=3;
    bitboard centerBoard;
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

    void initGroups(int n) {
        centerBoard=0;
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                if(i!=0&&i!=n-1&&j!=0&&j!=n-1)
                    centerBoard|=getBitboard(getSquare(i,j));
        switch (n) {
            case 4:
                groupU[0] = 0;
                groupU[1] = 0;
                groupU[2] = 50;
                groupU[3] = 100;
                groupU[4] = scale * 1600;
                break;
            case 5:
                groupU[0] = 0;
                groupU[1] = 0;
                groupU[2] = 0;
                groupU[3] = 22;
                groupU[4] = 85;
                groupU[5] = scale * 2500;
                break;
            case 6:
                groupU[0] = 0;
                groupU[1] = 0;
                groupU[2] = 0;
                groupU[3] = 25;
                groupU[4] = 75;
                groupU[5] = 100;
                groupU[6] = scale * 3600;
                break;
            case 7:
                groupU[0] = 0;
                groupU[1] = 0;
                groupU[2] = 0;
                groupU[3] = 25;
                groupU[4] = 70;
                groupU[5] = 90;
                groupU[6] = 120;
                groupU[7] = scale * 4900;
                break;
            case 8:
                groupU[0] = 0;
                groupU[1] = 0;
                groupU[2] = 0;
                groupU[3] = 25;
                groupU[4] = 50;
                groupU[5] = 75;
                groupU[6] = 90;
                groupU[7] = 125;
                groupU[7] = scale * 6400;
                break;
        }
    }
}
