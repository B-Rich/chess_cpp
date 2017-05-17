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
void print(Board board);
Chessboard init_board();
void init_zo();
Long get_hash(Chessboard board, bool turn);
Board get_board_by_name(Chessboard board, char *name);
Board* get_board(Chessboard* board, char unit);
Chessboard FEN(string FEN);
void print_cb(Chessboard board);
int move_piece(Chessboard c_board, Board *board, Moves move, char promote_to);
char unit(Chessboard board, Moves square);
void play(Chessboard *board, Moves move);
extern int move_counter;
#endif //CHESS_MOVE_ENGINE_H
