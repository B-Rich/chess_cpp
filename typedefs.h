#include <iostream>
#include <string>
#include <cstring>
using namespace std;
#ifndef CHESS_TYPEDEFS_H
#define CHESS_TYPEDEFS_H
typedef long long int Board;
typedef string Moves;
typedef long long int Long;
typedef unsigned long long int ULong;
typedef struct {
    Board white_pawn;
    Board white_knight;
    Board white_bishop;
    Board white_rook;
    Board white_queen;
    Board white_king;
    /////
    Board black_pawn;
    Board black_knight;
    Board black_bishop;
    Board black_rook;
    Board black_queen;
    Board black_king;
    /////
    Board en_passant;
    /////
    bool king_castle_white;
    bool queen_castle_white;
    bool king_castle_black;
    bool queen_castle_black;
} Chessboard;
typedef struct{
    int depth;
    int alpha;
    int beta;
    Moves move;
    bool color;
    int rating;
}Node;

typedef struct{
    //Cell,Pieces,Color 64,6,2
    Long table[64][6][2];
    Long opponent_move;
    Long castle[4];
    Long en_passant[8];
}Zobrist;

enum pieces {pawn = 0, knight, bishop, rook, queen, king};
enum unit_color {black = 0, white};
#endif //CHESS_TYPEDEFS_H
