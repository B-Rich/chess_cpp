#include <iostream>
#include "func.h"
#include "move_engine.h"
#include "board_constants.h"
#include "negamax.h"
#include "ratings.h"
#include <sys/time.h>
#include <io.h>
#include <fcntl.h>
#include <signal.h>
#include "typedefs.h"

using namespace std;

struct timeval S, F;
static int volatile play_sig = 0;
Moves ai;
pthread_t ser;
Chessboard *chess = new Chessboard;

double exec_time(struct timeval S, struct timeval F) {
    return (double) (F.tv_usec - S.tv_usec) / 1000000 + (double) (F.tv_sec - S.tv_sec);
}

void *search(void *args) {
    Chessboard *board = (Chessboard *) args;
    Moves *best = new Moves;
    for (int i = 2; i <= 5; i++) {
        if (!play_sig) {
            gettimeofday(&S, NULL);
            ai = albe_search_sig(INT_MIN, INT_MAX, i, i, *board, " ", play_sig);
            gettimeofday(&F, NULL);
            *best = ai;
            cout << "Found at move at depth " << i << " which is " << ai << " in "
                 << exec_time(S, F) << " seconds" << endl;
        }
    }
    pthread_exit(best);
}

Moves iter_deep(Chessboard *board, pthread_t ser) {
    void *move;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_create(&ser, &attr, search, board);
    pthread_join(ser, &move);
    return *(Moves *) move;
}

void play_now(int signum) {
    cout << "Play now invoked! " << endl;
    play_sig = 1;
    cout << "Terminating thread" << endl;
    pthread_cancel(ser);
    if (ai.empty()) {
        Moves all = moves(*chess, bot);
        ai = sortedMoves(*chess, all)[rand() % (all.length() / 4)];
        //ai = sortedMoves(*brd2,all)[0];
    }

}

int main() {
    //ios::sync_with_stdio(false);
    signal(SIGINT, play_now);
    move_counter = 0;
    string again = "", choice = "", game = "";
    //cout << brd2->white_knight << endl;
    Moves mv = "";
    string color = " ";
    //player = white;
    //player = black;
    bool flag = true, diff = false;
    //bot = !player;
    //iter_deep(brd2, ser);
    //_setmode( _fileno(stdout),_O_WTEXT);
    //cout << "AI move was: " << ai << endl;
    cout << "Welcome to Oberon Chess Engine" <<endl;
    cout << "------------------------------" <<endl;
    cout << endl;
    cout << endl;
    player = white;
    while (again.find("N") == string::npos || again.find("n") == string::npos) {
        if (flag) {
            cout << "Please select a difficulty level" <<endl;
            cout << "-------------------------------" <<endl;
            cout << "1. Normal" <<endl;
            cout << "2. Hard" <<endl;
            getline(cin,choice);
            while(choice[0] == '1' && choice[0] == '2'){
                cout << "Invalid selection, please select a difficulty level: ";
                getline(cin,choice);
            }
            if(choice[0] == '2'){
                diff = true;
            }else{
                diff = false;
            }
            cout << "Start new game or load state [New]:";
            getline(cin,game);
            //cin >> game;
            if(game.empty()){
                *chess = FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");//init_board();//FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            }else{
                //try{
                *chess = FEN(game);
                /*}catch{
                    cout << "Error parsing game state" <<endl;
                    cout << "Please enter the state again: ";
                    scanf("%s",&game);
                }*/
            }
            //*brd2 = init_board();
            print_cb(*chess);
            cout << "Please choose your color? [W]\\B: ";
            getline(cin, color);
            if (color.empty()) color = "W";
            while (color != "W" && color != "w" && color != "b" && color != "B") {
                cout << "Invalid selection, choose your color again: ";
                getline(cin, color);
            }
            flag = false;
        }
        if (color[0] == 'W' || color[0] == 'w') human = white;
        if (color[0] == 'B' || color[0] == 'b') human = black;
        bot = !human;
        if (human == white) {
            cout << "White's move" << endl;
        } else {
            if(diff){
                gettimeofday(&S, NULL);
                iter_deep(chess, ser);
            }else{
                gettimeofday(&S, NULL);
                ai = albe_search(INT_MIN, INT_MAX, MAX_DEPTH, *chess, "");
            }
            gettimeofday(&F, NULL);
            play(chess, ai);
            print_cb(*chess);
            cout << "Black's move" << endl;
        }
        cout << "Enter your move: ";
        getline(cin, mv);
        while (mv.empty()) {
            cout << "Failed to process move, enter your move again: ";
            getline(cin, mv);
        }
        while (moves(*chess, human).find(mv) == string::npos || mv.length() != 4) {
            if (mv.find("save") != string::npos) {
                string FEN = to_FEN(*chess);
                cout << "Board State: " << FEN << endl;
                cout << "Enter your move: ";
                getline(cin, mv);
            }
            if (mv.find("?") != string::npos) {
                Moves poss = moves(*chess, human);
                cout << "Possible moves (" << poss.length() / 4 << "): " << endl;
                for (int i = 0; i < poss.length(); i += 4) {
                    cout << (i / 4) + 1 << ": " << poss.substr(i, 4) << ". ";
                }
                cout << endl;
                cout << "Enter your move: ";
                getline(cin, mv);
            } else {
                cout << "Invalid move, enter your move again: ";
                getline(cin, mv);
            }
        }
        while (mv.empty()) {
            cout << "Failed to process move, enter your move again: ";
            getline(cin, mv);
        }
        if (human == white) {
            play(chess, mv);
            if(diff){
                gettimeofday(&S, NULL);
                iter_deep(chess, ser);
            }else{
                gettimeofday(&S, NULL);
                ai = albe_search(INT_MIN, INT_MAX, MAX_DEPTH, *chess, "");
            }
            gettimeofday(&F, NULL);
            play(chess, ai);
            print_cb(*chess);
        } else {
            play(chess, mv);
            print_cb(*chess);
        }
        //print(danger(*brd2,bot));
        cout << move_counter << endl;
        cout << "Move found at depth " << MAX_DEPTH << " in " << exec_time(S, F) << " seconds" << endl;
        cout << "Do you wish to make further moves? [Y]\\n: ";
        getline(cin, again);
        if (again.empty()) again = "Y";
        if (again.find("N") != string::npos || again.find("n") != string::npos) {
            cout << "Do you wish to start another game? [Y]\\n: ";
            getline(cin, again);
            if (again.empty()) {
                again = "Y";
                flag = true;
            } else {
                cout << "You have resigned!" << endl;
                string f = to_FEN(*chess);
                cout << "Board State: " << f << endl;
                //print_cb(FEN(f));
                break;
            }
        }
    }

    return 0;
}