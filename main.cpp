#include <iostream>
#include "func.h"
#include "move_engine.h"
#include "board_constants.h"
#include "typedefs.h"

using namespace std;

int main() {
    cout << set_bits(7L) << endl;
    cout << leading_count(2L) << endl;
    cout << trailing_count_2(2L) << endl;
    wchar_t  b = 9818;
    cout << trailing_count(4L) << endl;
    wcout << b << endl;
    string x = "Hallo";
    cout << stoi(to_string(x[2])) << endl;
    cout << (1L << 2L) << endl;
    cout << (int)(x[1]) << endl;
    //cout << strncmp("Hello", "Hell", strlen("Hell")) << endl;
    cout << start("Hello", "Hell", strlen("Hell")) <<endl;
    cout << rook_left <<endl;
    print(static_cast<ULong>(0xA1100110A));
    cout << (knight_moves(static_cast<Board>(1),0)) << endl;
    cout << (char)(7+'a') <<endl;
    Chessboard brd = init_board();
    Moves m = moves(brd,1);
    cout << "Length of moves: " << m.length() << endl;
    cout << "Number of moves: " << m.length()/4 << endl;
    /*for(int move = 0; move < m.length(); move++){
        if (move % 4 == 0 && move != 0) printf("\n");
        if (move % 2 == 0 || move == 0) printf(" ");
        printf("%c", m[move]);
    }*/
    cout <<"Moves: " << m << endl;
    //print(brd.black_king);
    printf("\n");
    print(brd.black_pawn);
    //cout << "Knight moves: " << knight_moves(brd.white_knight,1) << endl;
    printf("\n\n");
    cout <<"Stringer: " << stringer(7+'a') << endl;
    print(brd.white_pawn);
    //cout <<string(sizeof(char),(7 / 8) + 'a')<<endl;
    cout << zo_rand << endl;
    cout << zo_rand << endl;
    cout << knight <<endl;
    cout << in_cell(brd.white_knight, 2) << endl;
    return 0;
}