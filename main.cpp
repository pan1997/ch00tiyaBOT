//
// Created by pankaj on 20/9/16.
//

#include <iostream>
#include "basic.h"
#include "boardstate.h"
#include "evaluate.h"
#include "d1_player.h"
#include <stack>

void test(TAK::boardstate<5>&bs) {
    using namespace TAK;
    bs.playMove(construct_place_move(getRandomEmptySquare(bs), BLACK_FLAT));
    std::cout << bs << '\n';
    std::cout << "ENTER flat:";
    int r, c, f;
    std::cin >> r >> c;
    bs.playMove(construct_place_move(getSquare(r, c), WHITE_FLAT));
    std::stack<move> moves;
    move m;
    for (; !bs.end();) {
        std::cout << bs << '\n';
        std::cout << "eval:" << evaluate(bs) / (double) scale << '\n';
        int mx = 0;
        m = d1_getMove(bs, mx);
        moves.push(m);
        bs.playMove(m);
        bs.flipTurn();
        std::cout << bs << '\n';
        std::cout << "eval:" << evaluate(bs) / (double) scale<<' ';
        printMove(std::cout,m);
        std::cout<<" ret d1->" << mx << '\n';
        if (bs.end()) {
            std::cout << "FLAT END\n";
            return;
        }
        square in;
        std::cout << "Enter square r c:";
        if (std::cin >> r) {
            std::cin >> c;
            in = getSquare(r, c);
            if (bs.empty(in)) {
                std::cout << "Enter flat:";
                std::cin >> f;
                printMove(std::cout, construct_place_move(in, f ? BLACK_FLAT : BLACK_STANDING));
                std::cout << '\n';
                bs.playMove(m = construct_place_move(in, f ? BLACK_FLAT : BLACK_STANDING));
                moves.push(m);
            } else {
                std::cout << "Enter direction:";
                int dir;
                std::cin >> dir;
                std::cout << "Enter pick, drops:";
                int pick;
                std::cin >> pick;
                std::cout << "picked here " << pick << "\n";
                int tmp[5];
                int cnt = 0;
                for (int i = 0; cnt < pick; i++) {
                    std::cin >> tmp[i];
                    cnt += tmp[i];
                }
                printMove(std::cout, construct_move_move(in, (direction) dir, pick, tmp));
                std::cout << '\n';
                bs.playMove(m = construct_move_move(in, (direction) dir, pick, tmp));
                moves.push(m);
            }
            bs.flipTurn();
        } else {
            std::string s;
            std::cin>>s;
            std::cout << "unoding ";
            printMove(std::cout, moves.top());
            bs.undoMove(moves.top());
            bs.flipTurn();
            moves.pop();
        }
    }
}
/*
void test_groups() {
    using namespace TAK;
    bitboard b = 8799704242778;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
            std::cout << ((b & getBitboard(getSquare(i, j))) != 0);
        std::cout << '\n';
    }
    int r, c;
    //std::cout<<"Enter rc:";
    //std::cin>>r>>c;
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            if (b & getBitboard(getSquare(r, c))) {
                bitboard g = group(b, getBitboard(getSquare(r, c)));
                //bitboard g=neighbours(getBitboard(getSquare(r,c)));
                //std::cout << "r,c:" << r << c << '\n';
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++)
                        std::cout << (i == r && j == c);
                    std::cout << ' ';
                    for (int j = 0; j < 8; j++)
                        std::cout << ((g & getBitboard(getSquare(i, j))) != 0);
                    std::cout << '\n';
                }
                std::cout<<"count "<<countRows(g,8)<<countCols(g,8)<<'\n';
                std::cout<<'\n';
            }
}*/

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
    cout << "Hello, World!123" << endl;
    return 0;

}
