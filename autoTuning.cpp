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
        int limit1=5000,limit2=5000;
        std::cout<<w1<<' '<<w2<<'\n';
        int increment=50;
        move m;
        for (int i = 0; ; i++) {
            //std::cout<<"------------------------------------------------------\n";
            //std::cerr << board << '\n';
            int mx = 0;
            if (i % 2 == 1) {
                std::cout<<"w- moving\n";
                setWeights(w2);

                int aim = limit2 / (std::min(board.getWhiteLeft(), board.getBlackLeft()) * 2 + 10);
                std::cout<<"aiming "<<aim<<" ms\n";
                auto start = std::chrono::system_clock::now();
                m = search(board, mx, aim);
                auto end = std::chrono::system_clock::now();
                //printMove(std::cout, m);
                limit2 -= std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                //std::cout << '\n';
                //std::cout.flush();
                limit2 += increment;
                //std::cerr<<"Left "<<limit<<" ms\n";
            }
            else {
                std::cout<<"w+ moving\n";
                setWeights(w1);

                int aim = limit1 / (std::min(board.getWhiteLeft(), board.getBlackLeft()) * 2 + 10);
                std::cout<<"aiming "<<aim<<" ms\n";
                auto start = std::chrono::system_clock::now();
                m = search(board, mx, aim);
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
                return 1-2*(i%2);
            }
        }
        return 0;
    }

    void tune() {
        /*
        initZobrist();
        initGroups(5);
        initSlides();
        initbasic(5);
        TAK::initCitadels();
        transpositionTableInit();

        int w=80;
        int delta=8;

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
            }
            std::cout<<score<<" score in favour of increment out of "<<40<<" games\n";
        }
         */
    }
}