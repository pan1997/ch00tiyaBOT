//
// Created by pankaj on 20/9/16.
//

#include <iostream>
#include "basic.h"
#include "boardstate.h"
#include "evaluate.h"
#include "d1_player.h"
#include <stack>
#include <cstring>

template <int n>
void test(TAK::boardstate<n>&bs) {
    using namespace TAK;
    //bs.playMove(construct_place_move(getRandomEmptySquare(bs), BLACK_FLAT));
    std::cout << bs << '\n';
    std::cout << "ENTER black square:";
    int r, c, f;
    char tm[20];
    std::cin >> tm;
    bs.playMove(construct_place_move(readSquare(tm), BLACK_FLAT));
    std::cout << bs << '\n';
    std::cout << "ENTER white square:";
    std::cin >> tm;
    bs.playMove(construct_place_move(readSquare(tm), WHITE_FLAT));
    std::stack<move> moves;
    std::cout << "enter 0 for cpu white:";
    std::cin >> f;
    move m;
    for (int i = 0; ; i++) {
        std::cout<<"------------------------------------------------------\n";
        int mx = 0;
        if (i % 2 == f)
            m = d1_getMove(bs, mx);
        else {
            std::cout << "WAiting for move:";
            std::cin >> tm;
            m = readMove(tm, bs.getTurn());
        };
        moves.push(m);
        bs.playMove(m);
        bs.flipTurn();
        std::cout << bs << '\n';
        std::cout << "static eval:" << evaluate(bs) / (double) scale << " player eval:" << mx << '\n';
        if (bs.end()) {
            std::cout << "END\n";
            std::cout << "score " << terminalEvalVerbose(bs)<< '\n';
            return;
        }
    }
}

void testbug(TAK::boardstate<5> b){
    char mv[20][20];
    std::strcpy(mv[0],"e1");
    std::strcpy(mv[1],"a5");
    std::strcpy(mv[2],"Fb5");
    std::strcpy(mv[3],"Fc1");
    std::strcpy(mv[4],"Fc5");
    std::strcpy(mv[5],"Fd1");
    std::strcpy(mv[6],"Fd5");
    std::strcpy(mv[7],"Se5");
    std::strcpy(mv[8],"Fa4");
    std::strcpy(mv[9],"Fb1");
    std::strcpy(mv[10],"Fc2");

    b.playMove(TAK::construct_place_move(TAK::readSquare(mv[0]),TAK::BLACK_FLAT));
    std::cout<<b<<'\n';
    b.playMove(TAK::construct_place_move(TAK::readSquare(mv[1]),TAK::WHITE_FLAT));
    std::cout<<b<<'\n';

    for(int i=2;i<11;i++){
        b.playMove(TAK::readMove(mv[i],b.getTurn()));
        b.flipTurn();
        std::cout<<b<<'\n';
    }
    std::cout<<b<<'\n';
    int ms=0;
    TAK::move m=TAK::d1_getMove(b,ms);
    std::cout<<"done\n";
}

int main() {
    using namespace std;
    srand(time(NULL));
    TAK::initGroups(5);
    TAK::initSlides();
    TAK::initbasic(5);
    TAK::boardstate<5> board;
    test(board);
    //test3(board);
    //test_groups();
    //testbug(board);
    cout << "Hello, World!123" << endl;
    return 0;

}
