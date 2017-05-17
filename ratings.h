#include "typedefs.h"

#ifndef CHESS_RATINGS_H
#define CHESS_RATINGS_H
void reverse(short* arr, short s, short e);
void reverse(short *arr);
int material_eval(Chessboard board);
int mobility_eval(Chessboard board, int depth, int moves);
int location_eval(Chessboard board, int material_val);
int attack_eval(Chessboard board);
int evaluate(Chessboard board, int depth, int moves);
//Ending
//Both sides have no queens or
//Every side which has a queen has additionally no other pieces or one minor piece maximum. Minor being knights and bishops
#endif //CHESS_RATINGS_H
