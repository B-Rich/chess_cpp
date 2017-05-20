#ifndef CHESS_BOARD_CONSTANTS_H
#define CHESS_BOARD_CONSTANTS_H
typedef long long int Long;
typedef unsigned long long int ULong;
const Long rook_left = 0x101010101010101; //Column A
const Long rook_right = 0x8080808080808080; //Column H
const Long knight_right = 0x303030303030303;
const Long knight_left = 0xC0C0C0C0C0C0C0C0;
const Long promotion_black = 0xFF; //Row 1
const Long enpassant_black = 0xFF00000000; //Row 4
const Long enpassant_white = 0xFF000000; //Row 5
const Long promotion_white = 0xFF00000000000000; //Row 8
const Long middle = 0x1818000000;
const Long middle_plus = 0x3C3C3C3C0000;
const Long king_castle = 0xF0F0F0F0F0F0F0F0; //Castling
const Long queen_castle = 0xF0F0F0F0F0F0F0F; //Castling
const Long king_region = 0x70507;
const Long knight_region = 0xA1100110A;
const ULong file[8] = {0x101010101010101, 0x202020202020202, 0x404040404040404, 0x808080808080808,
                      0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080};
const ULong rank_[8]{0xFF, 0xFF00, 0xFF0000, 0xFF000000,
                   0xFF00000000, 0xFF0000000000, 0xFF000000000000, 0xFF00000000000000};
const ULong diagonal_l2r[15] = {0x1, 0x102, 0x10204, 0x1020408, 0x102040810, 0x10204081020, 0x1020408102040,
                                0x102040810204080, 0x204081020408000, 0x408102040800000, 0x810204080000000,
                                0x1020408000000000,
                                0x2040800000000000, 0x4080000000000000, 0x8000000000000000};
const ULong diagonal_r2l[15] = {0x80, 0x8040, 0x804020, 0x80402010, 0x8040201008, 0x804020100804,
                                0x80402010080402,
                                0x8040201008040201, 0x4020100804020100, 0x2010080402010000, 0x1008040201000000,
                                0x804020100000000, 0x402010000000000,
                                0x201000000000000, 0x100000000000000};
extern Long not_to_capture;
extern Long units;
extern Long taken;
extern Long available;
extern Zobrist Z;
extern bool player;
extern bool human;
extern bool bot;
#endif //CHESS_BOARD_CONSTANTS_H
