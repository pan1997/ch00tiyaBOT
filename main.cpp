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
#include <sstream>

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
        std::cout << "------------------------------------------------------\n";
        int mx = 0;
        if (i % 2 == f) {
            std::cout << std::bitset<64>(bs.getHash()) << '\n';
            int limit = 3000;
            m = search(bs, mx, limit);
            std::cout << std::bitset<64>(bs.getHash()) << '\n';
        }
        else {
            std::cout << "WAiting for move:";
            std::cin >> tm;
            if (strcmp(tm, "undo") == 0) {
                i -= 3;
                bs.undoMove(moves.top());
                bs.flipTurn();
                moves.pop();
                bs.undoMove(moves.top());
                bs.flipTurn();
                moves.pop();
                std::cout << bs << '\n';
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
            std::cout << "score " << terminalEvalVerbose(bs) << '\n';
            return;
        }
    }
}

void testbug(TAK::boardstate<5> b) {
    //std::string game="e1 a5 Fb1 Fc1 Fc5 Fd1 Fd5 Se5 Fa4 Fb1 Fa1 1b1<1 Fa2 2a1+2 Fa1 Sb2";
    std::string game = "a5 a1 Fb1 Fb5 Cc1 Cb2 Fd1 1b2-1 ";
    game += "Fa2 Sc2 Fb2 Fb3 Fe2 1b3-1 ";
    game += "Fa3 Fa4 Sb4 Fe1 Fc3 Fd4 Fe3 ";
    game += "Fc4 Sc5 1c4>1 Fe4 Fc4 Sd5 1c4>1 ";
    game += "Fd3 Fc4 1d3+1 Fe5 Fd3 1c4>1 1d3+1 Fc4 ";
    game += "1e4<1 1c4>1 Fe4 Sc4 1e4<1 1c4>1 1b4-1";


    std::string game1 = "a5 a1 Fa2 Fb5 Fb2 Sc2 Cc1 Fe1 Fd1 ";
    game1 += "Fe5 Fe2 Fd4 Fe3 Fc4 Fa3 1c4>1 Sc5 Fe4 Sd5 1e4<1 ";
    game1 += "Fc4 Fe4 1c4>1 1e4<1 Fc4 Fe4 1c4>1 Fa4 Fc4 Fb3 ";
    game1 += "1c4>1 1b3-1 Fb3 1e4<1 1b3-1 Ce4 Fc4 Fd3 1c4>1 ";
    game1 += "1d3+1 Fc3 1e4<1 Fb4 1d4>1 Fc4 1e4<1 Fb3 1d4>1 1b3>1 1e4<1 ";

    std::string game2 = "e1 a5 Fb5 Cb4 Fa2 Fe3 Fa3 Fa4 Fc4 Fc5 Fd5 Fd4 ";
    game2 += "Fe5 1b4>1 1a5-1 Fe2 1b5>1 2c4+2 Ca5";
    std::stringstream moves(game2);
    //std::strcpy(mv[16],"1a1+1");


    char tm[20];
    moves >> tm;
    b.playMove(TAK::construct_place_move(TAK::readSquare(tm), TAK::BLACK_FLAT));
    std::cout << b << '\n';
    moves >> tm;
    b.playMove(TAK::construct_place_move(TAK::readSquare(tm), TAK::WHITE_FLAT));
    std::cout << b << '\n';

    for (int i = 2; moves >> tm; i++) {
        std::cout << "Move " << tm << '\n';
        std::cout << std::bitset<64>(b.getHash()) << '\n';
        b.playMove(TAK::readMove(tm, b.getTurn()));
        b.undoMove(TAK::readMove(tm, b.getTurn()));
        std::cout << std::bitset<64>(b.getHash()) << '\n';
        b.playMove(TAK::readMove(tm, b.getTurn()));
        b.flipTurn();
        std::cout << b << '\n';
    }
    //b.flipTurn();
    std::cout << b << '\n';
    int ms = 0, mse = 0;
    mse = TAK::evaluate(b);
    TAK::move m = TAK::search(b, ms, 15000);
    std::cout << ms << ' ' << mse << " done\n";
    std::cout << b << '\n';
}

template <int n> void assignment(TAK::boardstate<n> board,int p,int limit,int increment=0) {
    using namespace TAK;
    initZobrist();
    initGroups(n);
    initSlides();
    initbasic(n);
    TAK::initCitadels();
    transpositionTableInit();
    p -= 1;
    char tm[50];
    std::string mv = "Fa";
    tm[1] = 0;
    tm[0] = '0' + n;
    mv.append(tm);
    if (p == 0) {
        std::strcpy(tm, mv.c_str());
        std::cout << tm << "\n";
        board.playMove(construct_place_move(readSquare(tm + 1), BLACK_FLAT));
        std::cin >> tm;
        board.playMove(construct_place_move(readSquare(tm + 1), WHITE_FLAT));
    }
    else {
        std::cin >> tm;
        board.playMove(construct_place_move(readSquare(tm + 1), BLACK_FLAT));
        if (std::strcmp(tm, mv.c_str()) != 0)
            std::strcpy(tm, mv.c_str());
        else std::strcpy(tm, "Fa1");
        std::cout << tm << "\n";
        board.playMove(construct_place_move(readSquare(tm + 1), WHITE_FLAT));
    }
    move m;
    for (int i = 0; ; i++) {
        //std::cout<<"------------------------------------------------------\n";
        std::cerr << board << '\n';
        int mx = 0;
        if (i % 2 == p) {
            int aim = limit / (std::min(board.getWhiteLeft(), board.getBlackLeft()) * 2 + 10);
            //std::cerr<<"aiming "<<aim<<" ms\n";
            auto start = std::chrono::system_clock::now();
            m = search(board, mx, aim);
            auto end = std::chrono::system_clock::now();
            printMove(std::cout, m);
            limit -= std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << '\n';
            std::cout.flush();
            limit += increment;
            //std::cerr<<"Left "<<limit<<" ms\n";
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
    TAK::initCitadels();
    TAK::boardstate<5> board;
    //assignment(board,1,480000);
    //test(board);
    //test3(board);
    //test_groups();
    testbug(board);
#endif
#ifdef ASS
    cerr << "ch00tiyaBOT 1.3\n";
    int p, n, lim;
    cerr << "Enter p n lim\n";
    cin >> p >> n >> lim;
    int increment = 8 * 1000;
    lim *= 1000;
    switch (n) {
        case 3:
            assignment(TAK::boardstate<3>(), p, lim, increment);
            break;
        case 4:
            assignment(TAK::boardstate<4>(), p, lim, increment);
            break;
        case 5:
            assignment(TAK::boardstate<5>(), p, lim, increment);
            break;
        case 6:
            assignment(TAK::boardstate<6>(), p, lim, increment);
            break;
        case 7:
            assignment(TAK::boardstate<7>(), p, lim, increment);
            break;
        case 8:
            assignment(TAK::boardstate<8>(), p, lim, increment);
            break;
    }
#endif
    //cout << "Hello, World!123" << endl;
    return 0;

}
