//
// Created by pankaj on 25/9/16.
//
#include "evaluate.h"
namespace TAK {
    int scale = 100;
    int move_advantage = 50;
    int standingU = 50;
    int capstoneU = 100;
    int groupU[8];
    int FCaptureU=-50;
    int FReserveU=50;
    int SCaptureU=-25;
    int SReserveU=75;
    int CCaptureU=-25;
    int CReserveU=80;
    void initGroups(int n) {
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
                groupU[3] = 25;
                groupU[4] = 75;
                groupU[5] = scale * 2500;
                break;
            case 6:
                groupU[0] = 0;
                groupU[1] = 0;
                groupU[2] = 0;
                groupU[3] = 25;
                groupU[4] = 75;
                groupU[5] = 125;
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
