#include "typedefs.h"
#ifndef CHESS_NEGAMAX_H
#define CHESS_NEGAMAX_H
Node ab_search(int alpha, int beta, int depth, Chessboard board, Moves move);
Moves* sortedMoves(Chessboard brd, Moves all_moves);
Moves* slice(Moves all_moves);
Moves albe_search(int alpha, int beta, int depth, Chessboard board, Moves move);
Moves albe_search_sig(int alpha, int beta, int depth, int max_depth, Chessboard board, Moves move, volatile int play_sig);
//Moves iter_deep(Chessboard *board, pthread_t ser);
#endif //CHESS_NEGAMAX_H
