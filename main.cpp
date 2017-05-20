#include <iostream>
#include "func.h"
#include "move_engine.h"
#include "board_constants.h"
#include "negamax.h"
#include "ratings.h"
#include <sys/time.h>
#include <signal.h>
#include "typedefs.h"
using namespace std;

struct timeval S, F;
static int volatile play_sig = 0;
Moves ai;
pthread_t ser;
Chessboard *brd2 = new Chessboard;

double exec_time(struct timeval S, struct timeval F){
    return (double) (F.tv_usec - S.tv_usec) / 1000000 + (double) (F.tv_sec - S.tv_sec);
}

void *search(void *args) {
    Chessboard *board = (Chessboard *) args;
    Moves *best = new Moves;
    for (int i = 2; i <= 5; i++) {
        if(!play_sig){
            gettimeofday(&S,NULL);
            ai = albe_search_sig(INT_MIN, INT_MAX, i, i,*board, " ", play_sig);
            gettimeofday(&F,NULL);
            *best = ai;
            cout << "Found at move at depth " << i << " which is " << ai << " in "
                 << exec_time(S,F) << " seconds" <<endl;
        }
    }
    pthread_exit(best);
}

Moves iter_deep(Chessboard *board, pthread_t ser) {
    void *move;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_create(&ser, &attr, search, board);
    pthread_join(ser, &move);
    return *(Moves *) move;
}

void play_now(int signum){
    cout << "Play now invoked! " <<endl;
    play_sig = 1;
    cout << "Terminating thread" <<endl;
    pthread_cancel(ser);
    if(ai.empty()){
        Moves all = moves(*brd2,bot);
        //ai = sortedMoves(*brd2,all)[rand()%(all.length()/4)];
        ai = sortedMoves(*brd2,all)[0];
    }

}

int main() {
    signal(SIGINT, play_now);
    move_counter = 0;
    cout << set_bits(7L) << endl;
    cout << leading_count(2L) << endl;
    cout << trailing_count_2(2L) << endl;
    wchar_t b = 9818;
    cout << trailing_count(4L) << endl;
    wcout << b << endl;
    string x = "Hallo";
    cout << stoi(to_string(x[2])) << endl;
    cout << (1L << 2L) << endl;
    cout << (int) (x[1]) << endl;
    //cout << strncmp("Hello", "Hell", strlen("Hell")) << endl;
    cout << start("Hello", "Hell", strlen("Hell")) << endl;
    cout << rook_left << endl;
    print(static_cast<ULong>(0xA1100110A));
    cout << (knight_moves(static_cast<Board>(1), 0)) << endl;
    cout << (char) (7 + 'a') << endl;
    Chessboard *brd = new Chessboard;
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
    cout << "Stringer: " << stringer(7 + 'a') << endl;
    print(brd->white_pawn);
    //cout <<string(sizeof(char),(7 / 8) + 'a')<<endl;
    cout << zo_rand << endl;
    cout << zo_rand << endl;
    cout << knight << endl;
    cout << in_cell(brd->white_knight, 1) << endl;
    cout << Z.table[1][pawn][white] << endl;
    short arr[] = {1, 2, 3, 4};
    reverse(arr, 0, 3);
    for (int i = 0; i < 4; i++) {
        cout << arr[i] << " ";
    }
    cout << "Chessboard test: " << endl;
    print_cb(*brd);
    cout << "FEN test: " << endl;
    Chessboard FEN_Test = FEN("rn1qkb1r/ppp2ppp/4pn2/3p1b2/3P1B2/2N1P3/PPP2PPP/R2QKBNR w KQkq - 2 5");
    print_cb(FEN_Test);
    Moves test_move = "a2a4";
    /*cout << "Unit in " << test_move.substr(0,2)  << " is " << unit(*brd,test_move.substr(0,2))
         << " which is in " << *get_board(brd, unit(*brd,test_move.substr(0,2)))<< " board"<< endl;
    cout << "Unit in " << test_move.substr(2,2)  << " is " << unit(*brd,test_move.substr(2,2))
         << " which is in " << *get_board(brd, unit(*brd,test_move.substr(2,2)))<< " board"<< endl;*/
    print(*get_board(brd, 'r'));
    move_piece(*brd, &brd->white_pawn, test_move, ' ');
    move_piece(*brd, &brd->black_rook, test_move, ' ');
    player != player;
    print(*get_board(brd, 'r'));
    print_cb(*brd);
    //Moves* s = slice(m);
    /*for(int i = 0; i < m.length()/4; i++){
        cout << s[i] <<endl;
    }*/
    Moves m2 = moves(FEN_Test, player);
    //sortedMoves(FEN_Test,m2);
    //sortedMoves(*brd,m);
    Chessboard* tb = new Chessboard;
    *tb = init_null();
    tb->black_king = 0x1000000000000000;
    tb->white_king = 0x10;
    human = black;
    bot != human;
    player = human;
    Node test_node = ab_search(INT_MIN,INT_MAX,2,*tb,"");//Node test_node = ab_search(INT_MIN,INT_MAX,2,*brd,"");
    cout << "Alpha: " << test_node.alpha <<endl;
    cout << "Beta: " << test_node.beta <<endl;
    cout << "Color: " << test_node.color <<endl;
    cout << "Depth: " << test_node.depth <<endl;
    cout << "Move: " << test_node.move <<endl;
    cout << "Rating: " << test_node.rating <<endl;
    /**string input = "position fen rwrwew";
    cout << input.substr(input.find("fen ")+4) <<endl;*/
    print(0x1818000000);
    print(0x3C3C3C3C0000);
    string again = "";
    *brd2 = init_board();
    cout << brd2->white_knight << endl;
    Moves mv = "";
    string color = " ";
    player = white;
    //player = black;
    bool flag = true;
    bot = !player;
    iter_deep(brd2, ser);
    cout << "AI move was: " << ai << endl;
    while (again != "N" || again != "n") {
        if (flag) {
            print_cb(*brd2);
            cout << "Please choose your color? [W]\\B ";
            getline(cin,color);
            if(color.empty()) color == "W";
            while (color != "W" && color != "w" && color != "b" && color != "B") {
                cout << "Invalid selection, choose your color again: ";
                getline(cin,color);
            }
            flag = false;
        }
        if(color == "W" || color == "w") human = white;
        if(color == "B" || color == "b") human = black;
        bot = !human;
        if (human == white) {
            cout << "White's move" << endl;
        } else {
            gettimeofday(&S,NULL);
            ai = albe_search(INT_MIN, INT_MAX, MAX_DEPTH, *brd2, "");
            /**Node n = ab_search(INT_MIN, INT_MAX, MAX_DEPTH, *brd2, "");
            play(brd2, n.move);
            cout << "Move: " << n.move << endl;*/
            gettimeofday(&F,NULL);
            play(brd2, ai);
            print_cb(*brd2);
            cout << "Black's move" << endl;
        }
        cout << "Enter your move: ";
        getline(cin, mv);
        while (mv.empty()) {
            cout << "Failed to process move, enter your move again: ";
            getline(cin, mv);
        }
        while (moves(*brd2, human).find(mv) == string::npos || mv.length() != 4) {
            if (mv.find("?") != string::npos) {
                Moves poss = moves(*brd2, human);
                cout << "Possible moves (" << poss.length() / 4 << "): " << endl;
                for(int i = 0; i < poss.length(); i+=4){
                    cout << (i/4)+1 << ": " << poss.substr(i,4) << ". ";
                }
                cout << endl;
                cout << "Enter your move: ";
                getline(cin, mv);
            }else {
                cout << "Invalid move, enter your move again: ";
                getline(cin, mv);
            }
        }
        while (mv.empty()) {
            cout << "Failed to process move, enter your move again: ";
            getline(cin, mv);
        }
        if(human == white){
            play(brd2, mv);
            gettimeofday(&S,NULL);
            //play(brd2, albe_search(INT_MIN, INT_MAX, MAX_DEPTH, *brd2, ""));
            //play(brd2, ab_search(INT_MIN, INT_MAX, MAX_DEPTH, *brd2, "").move);
            ai = albe_search(INT_MIN, INT_MAX, MAX_DEPTH, *brd2, "");
            gettimeofday(&F,NULL);
            play(brd2, ai);
            print_cb(*brd2);
        }else{
            play(brd2, mv);
            print_cb(*brd2);
        }
        //cout << albe_search(INT_MIN,INT_MAX,2,*brd2,"") << endl;
        //cout << danger(*brd2,bot) << endl;
        print(danger(*brd2,bot));
        cout << move_counter << endl;
        cout << "Execution time  is " << exec_time(S,F) << " seconds" <<endl;
        cout << "Do you wish to make further moves? [Y]\\n: ";
        getline(cin, again);
        if (again.empty()) again = "Y";
    }
    return 0;
}