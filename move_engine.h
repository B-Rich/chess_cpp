#include "typedefs.h"
#ifndef CHESS_MOVE_ENGINE_H
#define CHESS_MOVE_ENGINE_H
Board ortho_moves(int location);
Board diag_moves(int location);
Moves white_pawn_moves(Board white_pawns, Board black_pawns, Board en_passant);
Moves black_pawn_moves(Board black_pawns, Board white_pawns, Board en_passant);
Moves knight_moves(Board knight, Long taken);
Moves bishop_moves(Board bishop, Long taken);
Moves rook_moves(Board rook, Long taken);
Moves queen_moves(Board queen, Long taken);
Long danger(Chessboard board, bool color);
Moves king_moves(Board king, Long taken);
Moves castle(Chessboard board, bool color);
Moves moves(Chessboard board, bool color);
void print(Board);
Chessboard init_board();
void init_zo();
Long get_hash(Chessboard board, bool turn);
Board get_board_by_name(Chessboard board, char *name);
Chessboard FEN(string FEN);
#define MAX_DEPTH 8;
#endif //CHESS_MOVE_ENGINE_H
