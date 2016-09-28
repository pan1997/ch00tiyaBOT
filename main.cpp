//
// Created by pankaj on 20/9/16.
//

#include <iostream>
#include "basic.h"
#include "boardstate.h"
#include "evaluate.h"
#include "search.h"
#include <stack>
#include <cstring>
#include <bitset>

template <int n>
void test(TAK::boardstate<n>&bs) {
    using namespace TAK;
    //bs.playMove(construct_place_move(getRandomEmptySquare(bs), BLACK_FLAT));
    std::cout << bs << '\n';
    std::cout << "ENTER black square:";
    int f;
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
    std::cout << bs << '\n';
    move m;
    for (int i = 0; ; i++) {
        std::cout<<"------------------------------------------------------\n";
        int mx = 0;
        if (i % 2 == f) {
            std::cout<<std::bitset<64>(bs.getHash())<<'\n';
            int limit=3000;
            m = search(bs, mx,limit);
            std::cout<<std::bitset<64>(bs.getHash())<<'\n';
        }
        else {
            std::cout << "WAiting for move:";
            std::cin >> tm;
            if(strcmp(tm,"undo")==0){
                i-=3;
                bs.undoMove(moves.top());
                bs.flipTurn();
                moves.pop();
                bs.undoMove(moves.top());
                bs.flipTurn();
                moves.pop();
                std::cout<<bs<<'\n';
                continue;
            }
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
    std::strcpy(mv[10],"Fa1");
    std::strcpy(mv[11],"1b1<1");
    std::strcpy(mv[12],"Fa2");
    std::strcpy(mv[13],"2a1+2");
    std::strcpy(mv[14],"Fa1");
    std::strcpy(mv[15],"Cd4");
    std::strcpy(mv[16],"1a1+1");


    b.playMove(TAK::construct_place_move(TAK::readSquare(mv[0]),TAK::BLACK_FLAT));
    std::cout<<b<<'\n';
    b.playMove(TAK::construct_place_move(TAK::readSquare(mv[1]),TAK::WHITE_FLAT));
    std::cout<<b<<'\n';

    for(int i=2;i<17;i++){
        std::cout<<"Move "<<mv[i]<<'\n';
        std::cout<<std::bitset<64>(b.getHash())<<'\n';
        b.playMove(TAK::readMove(mv[i],b.getTurn()));
        b.undoMove(TAK::readMove(mv[i],b.getTurn()));
        std::cout<<std::bitset<64>(b.getHash())<<'\n';
        b.playMove(TAK::readMove(mv[i],b.getTurn()));
        b.flipTurn();
        std::cout<<b<<'\n';
    }

    std::cout<<b<<'\n';
    int ms=0;
    ms=TAK::evaluateStacks(b);
    //TAK::move m=TAK::d1_getMove(b,ms);
    std::cout<<ms<<" done\n";
}

template <int n> void assignment(TAK::boardstate<n> board,int p,int limit) {
    using namespace TAK;
    initZobrist();
    initGroups(n);
    initSlides();
    initbasic(n);
    transpositionTableInit();
    p-=1;
    char tm[50];
    if(p==0) {
        std::strcpy(tm,"Fa1");
        std::cout <<tm<<"\n";
        board.playMove(construct_place_move(readSquare(tm+1), BLACK_FLAT));
        std::cin>>tm;
        board.playMove(construct_place_move(readSquare(tm+1), WHITE_FLAT));
    }
    else{
        std::cin>>tm;
        board.playMove(construct_place_move(readSquare(tm+1), BLACK_FLAT));
        if(std::strcmp(tm,"Fa1")!=0)
            std::strcpy(tm,"Fa1");
        else std::strcpy(tm,"Fe1");
        std::cout <<tm<<"\n";
        board.playMove(construct_place_move(readSquare(tm+1), WHITE_FLAT));
    }
    move m;
    for (int i = 0; ; i++) {
        //std::cout<<"------------------------------------------------------\n";
        std::cerr<<board<<'\n';
        int mx = 0;
        if (i % 2 == p) {
            int aim=limit/(board.getWhiteLeft()*3+20);
            std::cerr<<"aiming "<<aim<<" ms\n";
            m = search(board, mx,aim);
            limit+=aim;
            printMove(std::cout,m);
            std::cout<<'\n';
            std::cerr<<"Left "<<limit<<" ms\n";
        }
        else {
            std::cin >> tm;
            m = readMove(tm, board.getTurn());
        };
        board.playMove(m);
        board.flipTurn();
        if (board.end()) {
            return;
        }
    }
}

int main() {
    using namespace std;
    srand(time(NULL));
#ifndef ASS
    TAK::initZobrist();
    TAK::initGroups(5);
    TAK::initSlides();
    TAK::initbasic(5);
    TAK::transpositionTableInit();
    TAK::boardstate<5> board;
    test(board);
    //test3(board);
    //test_groups();
    //testbug(board);
#endif
#ifdef ASS
    cerr<<"ch00tiyaBOT\n";
    int p,n,lim;
    cerr<<"Enter p n lim\n";
    cin>>p>>n>>lim;
    lim*=1000;
    switch(n){
        case 3:assignment(TAK::boardstate<3>(),p,lim);break;
        case 4:assignment(TAK::boardstate<4>(),p,lim);break;
        case 5:assignment(TAK::boardstate<5>(),p,lim);break;
        case 6:assignment(TAK::boardstate<6>(),p,lim);break;
        case 7:assignment(TAK::boardstate<7>(),p,lim);break;
        case 8:assignment(TAK::boardstate<8>(),p,lim);break;
    }
#endif
    //cout << "Hello, World!123" << endl;
    return 0;

}
