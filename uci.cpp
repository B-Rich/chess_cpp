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
            cout << "readyok" << endl;
        //}else if(strcmp(input,"position",strlen("position"))){

        }else if(input == "quit"){
            exit(0);
        }
    }
}

