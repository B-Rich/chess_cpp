#include "typedefs.h"
#ifndef CHESS_NEGAMAX_H
#define CHESS_NEGAMAX_H
Node ab_search(int alpha, int beta, int depth, Chessboard board, Moves move);
Moves* sortedMoves(Chessboard brd, Moves all_moves);
Moves* slice(Moves all_moves);
Moves albe_search(int alpha, int beta, int depth, Chessboard board, Moves move);
#endif //CHESS_NEGAMAX_H
