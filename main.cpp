#include <iostream>
#include "func.h"
#include "move_engine.h"
#include "board_constants.h"
#include "negamax.h"
#include "ratings.h"
#include "typedefs.h"

using namespace std;

int main() {
    move_counter = 0;
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
    Chessboard* brd = new Chessboard;
    *brd = init_board();
    init_zo();
    player = white;
    /**Moves m = moves(*brd,player);
    cout << "Length of moves: " << m.length() << endl;
    cout << "Number of moves: " << m.length()/4 << endl;
    /*for(int move = 0; move < m.length(); move++){
        if (move % 4 == 0 && move != 0) printf("\n");
        if (move % 2 == 0 || move == 0) printf(" ");
        printf("%c", m[move]);
    }
    cout <<"Moves: " << m << endl;*/
    //print(brd.black_king);
    printf("\n");
    print(brd->black_pawn);
    //cout << "Knight moves: " << knight_moves(brd.white_knight,1) << endl;
    printf("\n\n");
    cout <<"Stringer: " << stringer(7+'a') << endl;
    print(brd->white_pawn);
    //cout <<string(sizeof(char),(7 / 8) + 'a')<<endl;
    cout << zo_rand << endl;
    cout << zo_rand << endl;
    cout << knight <<endl;
    cout << in_cell(brd->white_knight, 1) << endl;
    cout << Z.table[1][pawn][white] << endl;
    short arr[] = {1,2,3,4};
    reverse(arr,0,3);
    for(int i = 0; i < 4; i++){
        cout << arr[i] << " ";
    }
    cout << "Chessboard test: " <<endl;
    print_cb(*brd);
    cout << "FEN test: " <<endl;
    Chessboard FEN_Test = FEN("rn1qkb1r/ppp2ppp/4pn2/3p1b2/3P1B2/2N1P3/PPP2PPP/R2QKBNR w KQkq - 2 5");
    print_cb(FEN_Test);
    Moves test_move = "a2a4";
    /*cout << "Unit in " << test_move.substr(0,2)  << " is " << unit(*brd,test_move.substr(0,2))
         << " which is in " << *get_board(brd, unit(*brd,test_move.substr(0,2)))<< " board"<< endl;
    cout << "Unit in " << test_move.substr(2,2)  << " is " << unit(*brd,test_move.substr(2,2))
         << " which is in " << *get_board(brd, unit(*brd,test_move.substr(2,2)))<< " board"<< endl;*/
    print(*get_board(brd,'r'));
    move_piece(*brd,&brd->white_pawn,test_move,' ');
    move_piece(*brd,&brd->black_rook,test_move,' ');
    player != player;
    print(*get_board(brd,'r'));
    print_cb(*brd);
    //Moves* s = slice(m);
    /*for(int i = 0; i < m.length()/4; i++){
        cout << s[i] <<endl;
    }*/
    Moves m2 = moves(FEN_Test,player);
    //sortedMoves(FEN_Test,m2);
    //sortedMoves(*brd,m);
    /**Node test_node = ab_search(INT_MIN,INT_MAX,2,*brd,"");
    cout << "Alpha: " << test_node.alpha <<endl;
    cout << "Beta: " << test_node.beta <<endl;
    cout << "Color: " << test_node.color <<endl;
    cout << "Depth: " << test_node.depth <<endl;
    cout << "Move: " << test_node.move <<endl;
    cout << "Rating: " << test_node.rating <<endl;*/
    /**string input = "position fen rwrwew";
    cout << input.substr(input.find("fen ")+4) <<endl;*/
    string again = "";
    Chessboard* brd2 = new Chessboard;
    *brd2 = init_board();
    cout << brd2->white_knight << endl;
    Moves mv = "";
    player = white;
    //player = black;
    while(again != "N" || again != "n"){
        print_cb(*brd2);
        if(player == white){
            cout << "White's move" <<endl;
        }else{
            cout << "Black's move" <<endl;
        }
        cout << "Enter your move: ";
        getline(cin, mv);
        while(mv.empty()){
            cout << "Failed to process move, enter your move again: ";
            getline(cin, mv);
        }
        if(mv.find("?") != string::npos){
            Moves poss = "";
            switch(mv[0]){
                /**case 'R':
                case 'r':
                    poss = player == white ? rook_moves(brd2->white_rook,0) : rook_moves(brd2->black_rook,0);
                    break;
                case 'Q':
                case 'q':
                    poss = player == white ? queen_moves(brd2->white_queen,0) : queen_moves(brd2->black_queen,0);
                    break;
                case 'K':
                case 'k':
                    poss = player == white ? king_moves(brd2->white_king,0) : king_moves(brd2->black_king,0);
                    break;
                case 'B':
                case 'b':
                    poss = player == white ? bishop_moves(brd2->white_bishop,0) : bishop_moves(brd2->black_bishop,0);
                    break;
                case 'N':
                case 'n':
                    poss = player == white ? knight_moves(brd2->white_knight,0) : knight_moves(brd2->black_knight,0);
                    break;
                case 'P':
                case 'p':
                    poss = player == white ?
                           white_pawn_moves(brd2->white_pawn,brd2->black_pawn,brd2->en_passant) :
                           black_pawn_moves(brd2->black_pawn,brd2->white_pawn,brd2->en_passant);
                    break;*/
                default:
                    poss = moves(*brd2,player);
                    break;
            }
            cout << "Possible moves (" << poss.length()/4 << "): " << poss <<endl;
            cout << "Enter your move: ";
            getline(cin, mv);
            while(mv.empty()){
                cout << "Failed to process move, enter your move again: ";
                getline(cin, mv);
            }
        }
        play(brd2,mv);
        //play(brd2,ab_search(INT_MIN,INT_MAX,2,*brd,"").move);
        player = black;
        //cout << ab_search(INT_MIN,INT_MAX,2,*brd,mv).move << endl;
        cout << albe_search(INT_MIN,INT_MAX,2,*brd,"") << endl;
        cout << endl;
        cout << "Do you wish to make further moves? [Y]\\n: ";
        getline(cin, again);
        if(again.empty()) again =  "Y";
    }
    return 0;
}