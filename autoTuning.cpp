//
// Created by pankaj on 3/10/16.
//

#include "autoTuning.h"
#include "basic.h"
#include "boardstate.h"
#include "search.h"
#include "evaluate.h"
namespace TAK {

    void setWeights(int i);
    int game(boardstate<5>&board,int w1, int w2) {
        //int limit1 = 10000, limit2 = 10000;
        int limit1=0,limit2=0;
        std::cout << w1 << ' ' << w2 << '\n';
        int increment = 100;
        move m;
        for (int i = 0; ; i++) {
            //std::cout<<"------------------------------------------------------\n";
            //std::cerr << board << '\n';
            int mx = 0;
            if (i % 2 == 1) {
                std::cout << w2 << " moving\n";
                setWeights(w2);
                //r=w2>0?1:1.5;
                //r=w2/(1.0+w2);
                int aim = 2*increment;//limit2 / (std::min(board.getWhiteLeft(), board.getBlackLeft()) * 2 + 10);
                //std::cout << "aiming " << aim << " ms\n";
                //auto start = std::chrono::system_clock::now();
                transpositionTableInit();
                m = search(board, mx, aim, aim);
                //auto end = std::chrono::system_clock::now();
                //printMove(std::cout, m);
                //limit2 -= std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                //std::cout << '\n';
                //std::cout.flush();
                //limit2 = std::max(limit2,0)+increment;
                //std::cerr<<"Left "<<limit<<" ms\n";
            }
            else {
                std::cout << w1 << " moving\n";
                setWeights(w1);

                //r=w1>0?1:1.5;
                //r=w1/(1.0+w1);

                int aim = 2*increment;//limit1 / (std::min(board.getWhiteLeft(), board.getBlackLeft()) * 2 + 10);
                //std::cout << "aiming " << aim << " ms\n";
                //auto start = std::chrono::system_clock::now();
                transpositionTableInit();
                m = search(board, mx, aim, aim);
                //auto end = std::chrono::system_clock::now();
                //printMove(std::cout, m);
                //limit1 -= std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                //std::cout << '\n';
                //std::cout.flush();
                //limit1= std::max(limit1,0)+increment;
            };
            board.playMove(m);
            board.flipTurn();
            if (board.end()) {
                return terminalEval(board)>0?1:-1;//1 - 2 * (i % 2);
            }
            if(i>200)
                return 0;
        }
        return 0;
    }

    double r;


    void setWeights(int i) {
        //emptyInfluence = i;
        //weights[5][0]=i;
        //groupU[3]=i;

        //standingU=i;
        //CCaptureU=-i;
        //SReserveU=i;
        //underCap=i;
        //groupU[3]=i;
        weights[5][0]=i;
        //weights[5][1]=i;
        //center=i;
        //CReserveU=i;
        //scale=100+i;
        //capstoneU=i;
        //endgameCutoff=i;
        //move_advantage=45+i;
        //groupU[4]=i;
        //placeThreat=i;
        //move_advantage=i;
        //scale=100-i;
        //capstoneU=i;
    }
    void tune() {

        initZobrist();
        initGroups(5);
        initSlides();
        initbasic(5);
        TAK::initInfo(5);
        TAK::initCitadels();
        transpositionTableInit();

        int w=65;
        int delta=2;

        for(int n=0;n<1;n++) {

            int score=0;

            for (int i = 0; i < 20; i++) {
                boardstate<5> b;
                square s1,s2;
                b.playMove(construct_place_move(s1=getRandomEmptySquare(b), BLACK_FLAT));
                b.playMove(construct_place_move(s2=getRandomEmptySquare(b), WHITE_FLAT));
                int res=game(b,w-delta,w+delta);
                std::cout<<res<<" won\n";
                score-=res;
                boardstate<5> b1;
                b.playMove(construct_place_move(s1, BLACK_FLAT));
                b.playMove(construct_place_move(s2, WHITE_FLAT));
                res=game(b1,w+delta,w-delta);
                std::cout<<res<<" won\n";
                score+=res;



                std::cout<<score<<" score in favour of increment out of "<<i*2+2<<" games\n";

            }
            std::cout<<score<<" score in favour of increment out of "<<40<<" games\n";
        }

    }
}