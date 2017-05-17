#include <iostream>
#include "typedefs.h"

void UCI(){
    string input;
    getline(cin, input);
    bool n = 1;
    while(n){
        if(input == "uci") {
            cout << "id name OberonPrimeV1" << endl;
            cout << "id author O.M.Ghozlan" << endl;
            cout << "uciok" << endl;
            //}else if(strnicmp(input,"s",strlen("s"))){
        }else if(input == "isready") {
            cout << "uciok" << endl;
        }else if(input.find("position ") == 0){
            if(input.find("startpos ") != string::npos){
                //FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            } else if (input.find("fen ") != string::npos){
                //FEN(input.substr(input.find("fen ") + 4));
            } else if (input.find("moves ") != string::npos){

            }

        }else if(input == "quit"){
            exit(0);
        }
    }
}

