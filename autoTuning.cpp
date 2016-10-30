//
// Created by pankaj on 3/10/16.
//

#include "autoTuning.h"
#include "basic.h"
#include "boardstate.h"
#include "search.h"
#include "evaluate.h"
namespace TAK {
    int game(boardstate<5>&board,int w1, int w2) {
        //int limit1 = 10000, limit2 = 10000;
        int limit1=0,limit2=0;
        std::cout << w1 << ' ' << w2 << '\n';
        int increment = 0;
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
                int aim = limit2 / (std::min(board.getWhiteLeft(), board.getBlackLeft()) * 2 + 10);
                std::cout << "aiming " << aim << " ms\n";
                auto start = std::chrono::system_clock::now();
                m = search(board, mx, aim, 2000);
                auto end = std::chrono::system_clock::now();
                //printMove(std::cout, m);
                limit2 -= std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                //std::cout << '\n';
                //std::cout.flush();
                limit2 += increment;
                //std::cerr<<"Left "<<limit<<" ms\n";
            }
            else {
                std::cout << w1 << " moving\n";
                setWeights(w1);

                //r=w1>0?1:1.5;
                //r=w1/(1.0+w1);

                int aim = limit1 / (std::min(board.getWhiteLeft(), board.getBlackLeft()) * 2 + 10);
                std::cout << "aiming " << aim << " ms\n";
                auto start = std::chrono::system_clock::now();
                m = search(board, mx, aim, 2000);
                auto end = std::chrono::system_clock::now();
                //printMove(std::cout, m);
                limit1 -= std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                //std::cout << '\n';
                //std::cout.flush();
                limit1 += increment;
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
    void tune() {

        initZobrist();
        initGroups(5);
        initSlides();
        initbasic(5);
        TAK::initInfo(5);
        TAK::initCitadels();
        transpositionTableInit();

        int w=13;
        int delta=1;

        for(int n=0;n<1;n++) {

            int score=0;

            for (int i = 0; i < 20; i++) {
                boardstate<5> b;
                square s1,s2;
                b.playMove(construct_place_move(s1=getRandomEmptySquare(b), BLACK_FLAT));
                b.playMove(construct_place_move(s2=getRandomEmptySquare(b), WHITE_FLAT));
                int res=game(b,w-delta,w+delta);
                transpositionTableInit();
                std::cout<<res<<" won\n";
                score-=res;
                boardstate<5> b1;
                b.playMove(construct_place_move(s1, BLACK_FLAT));
                b.playMove(construct_place_move(s2, WHITE_FLAT));
                res=game(b1,w+delta,w-delta);
                transpositionTableInit();
                std::cout<<res<<" won\n";
                score+=res;



                std::cout<<score<<" score in favour of increment out of "<<i*2+2<<" games\n";

            }
            std::cout<<score<<" score in favour of increment out of "<<40<<" games\n";
        }

    }
}