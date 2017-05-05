#include "func.h"
#include "typedefs.h"
#include "board_constants.h"
#include <stdio.h>

Long not_to_capture;
Long units;
Long taken;
/*
8 ║♜ ♞ ♝ ♛ ♚ ♝ ♞ ♜
7 ║♟ ♟ ♟ ♟ ♟ ♟ ♟ ♟
6 ║… … … … … … … …
5 ║… … … … … … … …
4 ║… … … … … … … …
3 ║… … ♘ … … … … …
2 ║♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙
1 ║♖ … ♗ ♕ ♔ ♗ ♘ ♖
—╚═══════════════
——a   b   c   d   e   f   g   h*/

//functions


void print(Board board) {
    char position;
    printf("\n\n");
    for (int pos = 0; pos < 64; pos++) {
        if (((static_cast<ULong>(board) >> pos) & 1) == 1) {
            position = 'x';
        } else {
            position = '_';
        }
        if (pos % 8 == 0 && pos != 0) printf("\n");
        printf("%c ", position);
    }
    printf("\n\n");
}

Board ortho_moves(int location) {
    Long loc = static_cast<Long>(1) << location; //Binary value
    Board Hor = (taken - 2 * loc) ^reversed(reversed(taken) - 2 * reversed(loc));
    Board Ver = ((taken & col[location / 8]) - (loc * 2)) |
                reversed(reversed(taken & col[location % 8]) - (reversed(loc) * 2));
    return (Hor & row[location / 8]) | (Ver & col[location % 8]);
}

Board diag_moves(int location) {
    Long loc = static_cast<Long>(1) << location; //Binary value
    Long l2r = ((taken & diagonal_l2r[(location / 8) + (location % 8)]) - (2 * loc)) ^
               reversed(reversed(taken & diagonal_l2r[(location / 8) + (location % 8)]) - (2 * reversed(loc)));
    Long r2l = ((taken & diagonal_r2l[(location / 8) + 7 - (location % 8)]) - (2 * loc)) ^
               reversed(reversed(taken & diagonal_r2l[(location / 8) + 7 - (location % 8)]) - (2 * reversed(loc)));
    return (l2r & diagonal_l2r[(location / 8) + (location % 8)]) |
           (r2l & diagonal_r2l[(location / 8) + 7 - (location % 8)]);
}

Moves black_pawn_moves(Board black_pawns, Board white_pawns, Board en_passant) {
    Moves moves = "";
    int position;
    //right
    Long potential_moves = (black_pawns >> 7) & not_to_capture & taken & ~promotion_black & ~rook_right;
    Long capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer(position % 8 + 'a' - 1) + to_string(position / 8 + 2) +
                 stringer(position % 8 + 'a') + to_string(position / 8 + 1);
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //left
    potential_moves = (black_pawns >> 9) & not_to_capture & taken & ~promotion_black & ~rook_left;
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer(position % 8 + 'a' + 1) + to_string(position / 8 + 2) +
                 stringer(position % 8 + 'a') + to_string(position / 8 + 1);
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //advancing
    potential_moves = (black_pawns >> 8) & ~taken & ~promotion_black;
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer(position % 8 + 'a') + to_string(position / 8 + 2) +
                 stringer(position % 8 + 'a') + to_string(position / 8 + 1);
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //advancing two squares
    potential_moves = (black_pawns >> 16) & ~taken & (~taken >> 8) & enpassant_black;
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer(position % 8 + 'a') + to_string(position / 8 + 3) +
                 stringer(position % 8 + 'a') + to_string(position / 8 + 1);
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //promotion right
    potential_moves = (black_pawns >> 7) & not_to_capture & taken & promotion_black & ~rook_right;
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += to_string(position % 8 - 1) + to_string(position % 8) + "qP" + to_string(position % 8 - 1) +
                 to_string(position % 8) + "nP" + to_string(position % 8 - 1) +
                 to_string(position % 8) + "bP" + to_string(position % 8 - 1) + to_string(position % 8) + "rP";
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //promotion left
    potential_moves = (black_pawns >> 9) & not_to_capture & taken & promotion_black & ~rook_left;
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += to_string(position % 8 + 1) + to_string(position % 8) + "qP" + to_string(position % 8 + 1) +
                 to_string(position % 8) + "nP" + to_string(position % 8 + 1) +
                 to_string(position % 8) + "bP" + to_string(position % 8 + 1) + to_string(position % 8) + "rP";
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //promotion advancing
    potential_moves = (black_pawns >> 8) & ~taken & promotion_black;
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += to_string(position % 8) + to_string(position % 8) + "qP" + to_string(position % 8) +
                 to_string(position % 8) + "nP" + to_string(position % 8) + to_string(position % 8) +
                 "bP" + to_string(position % 8) + to_string(position % 8) + "rP";
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //WE = White En passant
    //right en passant
    capture = (black_pawns << 1) & white_pawns & enpassant_black & ~rook_right &
              en_passant;
    if (capture != 0) {
        position = trailing_count(capture);
        moves += to_string(position % 8 - 1) + to_string(position % 8) + "BE";
    }
    //left en passant
    capture = (black_pawns >> 1) & white_pawns & enpassant_black & ~rook_left &
              en_passant;
    if (capture != 0) {
        position = trailing_count(capture);
        moves += to_string(position % 8 + 1) + to_string(position % 8) + "BE";
    }
    return moves;
}

Moves white_pawn_moves(Board white_pawns, Board black_pawns, Board en_passant) {
    Moves moves = "";
    int position;
    //left
    Long potential_moves = (white_pawns << 7) & not_to_capture & taken & ~promotion_white & ~rook_right;
    Long capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer((position % 8 + 'a' + 1)) + to_string(position / 8 - 1) +
                 stringer((position % 8 + 'a')) + to_string(position / 8);
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //right
    potential_moves = (white_pawns << 9) & not_to_capture & taken & ~promotion_white & ~rook_left;
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer((position % 8 + 'a' - 1)) + to_string(position / 8) +
                 stringer((position % 8 + 'a')) + to_string(position / 8 + 1);
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //advancing
    potential_moves = (white_pawns << 8) & ~taken & ~promotion_white;
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer((position % 8 + 'a')) + to_string(position / 8) +
                 stringer((position % 8 + 'a')) + to_string(position / 8 + 1);
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //advancing two squares
    potential_moves = (white_pawns << 16) & ~taken & (~taken << 8) &
                      ~enpassant_white;//enpassant_black;//enpassant_white; <= original
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer((position % 8 + 'a')) + to_string(position / 8 - 1) +
                 stringer((position % 8 + 'a')) + to_string(position / 8 + 1);
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //promotion left
    potential_moves = (white_pawns << 7) & not_to_capture & taken & promotion_white & ~rook_right;
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer((position % 8 + 'a' + 1)) +
                 stringer((position % 8 + 'a')) + "QP" +
                 stringer((position % 8 + 'a' + 1)) +
                 stringer((position % 8 + 'a')) +
                 "NP" + stringer((position % 8 + 'a' + 1)) +
                 stringer((position % 8 + 'a')) + "BP" +
                 stringer((position % 8 + 'a' + 1)) +
                 stringer((position % 8 + 'a')) + "RP";
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //promotion right
    potential_moves = (white_pawns << 9) & not_to_capture & taken & promotion_white & ~rook_left;
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer((position % 8 + 'a' - 1)) +
                 stringer((position % 8 + 'a')) + "QP"
                 + stringer((position % 8 + 'a' - 1)) +
                 stringer((position % 8 + 'a')) +
                 "NP" + stringer((position % 8 + 'a' - 1)) +
                 stringer((position % 8 + 'a')) + "BP" +
                 stringer((position % 8 + 'a' - 1)) +
                 stringer((position % 8 + 'a')) + "RP";
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //promotion advancing
    potential_moves = (white_pawns << 8) & ~taken & promotion_white;
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer((position % 8 + 'a')) +
                 stringer((position % 8 + 'a')) + "QP"
                 + stringer((position % 8 + 'a')) +
                 stringer((position % 8 + 'a')) +
                 "NP" + stringer((position % 8 + 'a')) +
                 stringer((position % 8 + 'a')) + "BP" +
                 stringer((position % 8 + 'a')) +
                 stringer((position % 8 + 'a')) +
                 "RP";
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //en passant left
    capture = (white_pawns >> 1) & black_pawns & enpassant_white & ~rook_right & en_passant;
    if (capture != 0) {
        position = trailing_count(capture);
        moves += stringer((position % 8 + 'a' + 1)) + stringer((position % 8 + 'a')) + "WE";
    }
    //en passant right
    capture = (white_pawns << 1) & black_pawns & enpassant_white & ~rook_left & en_passant;
    if (capture != 0) {
        position = trailing_count(capture);
        moves += stringer((position % 8 + 'a' - 1)) + stringer((position % 8 + 'a')) + "WE";
    }
    return moves;
}

Moves knight_moves(Board knight, Long taken) {
    int knight_location, location;
    Moves moves = "";
    //Long knights=knight&~(knight-1);
    Long capture, knight_moves, knights = knight & ~(knight - 1);
    //Get all knights
    while (knights != 0) {
        knight_location = trailing_count(knights);
        if (knight_location > 0x12) {
            capture = knight_region << (knight_location - 0x12);
        } else {
            capture = knight_region >> (0x12 - knight_location);
            /*print(knight);
            print(capture);
            print(~knight_left);
            print(capture&~knight_left);
            print(not_to_capture);
            print(capture&~knight_left&~not_to_capture);*/
        }
        if (knight_location % 8 < 0x4) {
            capture &= ~knight_left & ~not_to_capture; //~knight_left & not_to_capture;
        } else {
            capture &= ~knight_right & ~not_to_capture;//~knight_right & not_to_capture;
        }
        print(capture);
        knight_moves = capture & ~(capture - 1);
        //all possible knight moves
        while (knight_moves != 0) {
            location = trailing_count(knight_moves);
            moves += stringer((knight_location % 8 + 'a')) + to_string(knight_location / 8 + 1) +
                     stringer((location % 8 + 'a')) +
                     to_string(location / 8 + 1);
            capture &= ~knight_moves;
            knight_moves = capture & ~(capture - 1);
        }
        knight &= ~knights;
        knights = knight & ~(knight - 1);
    }
    return moves;
}

Moves bishop_moves(Board bishop, Long taken) {
    int bishop_location, location;
    Moves moves = "";
    Long bishop_moves, capture, bishops = bishop & ~(bishop - 1);
    while (bishops != 0) {
        bishop_location = trailing_count(bishops);
        capture = diag_moves(bishop_location) & not_to_capture;
        bishop_moves = capture & ~(capture - 1);
        while (bishop_moves != 0) {
            location = trailing_count(bishop_moves);
            //string(1,(char)(knight_location % 8 +'a'))
            moves += stringer((bishop_location % 8 + 'a')) + to_string(bishop_location / 8 + 1) +
                     stringer((location % 8 + 'a')) + to_string(location / 8 + 1);
            capture &= ~bishop_moves;
            bishop_moves = capture & ~(capture - 1);
        }
        bishop &= ~bishops;
        bishops = bishop & ~(bishop - 1);
    }
    return moves;
}

Moves rook_moves(Board rook, Long taken) {
    int rook_location, location;
    Moves moves = "";
    Long rook_moves, capture, rooks = rook & ~(rook - 1);
    while (rooks != 0) {
        rook_location = trailing_count(rooks);
        capture = ortho_moves(rook_location) & not_to_capture;
        rook_moves = capture & ~(capture - 1);
        while (rook_moves != 0) {
            location = trailing_count(rook_moves);
            moves += stringer((rook_location % 8 + 'a')) + to_string(rook_location / 8 + 1) +
                     stringer((location % 8 + 'a')) + to_string(location / 8 + 1);
            capture &= ~rook_moves;
            rook_moves = capture & ~(capture - 1);
        }
        rook &= ~rooks;
        rooks = rook & ~(rook - 1);
    }
    return moves;
}

Moves queen_moves(Board queen, Long taken) {
    int queen_location, location;
    Moves moves = "";
    Long queen_moves, capture, queens = queen & ~(queen - 1);
    while (queens != 0) {
        queen_location = trailing_count(queens);
        capture = (diag_moves(queen_location) | ortho_moves(queen_location)) & not_to_capture;
        queen_moves = capture & ~(capture - 1);
        while (queen_moves != 0) {
            location = trailing_count(queen_moves);
            moves += stringer((queen_location % 8 + 'a')) + to_string(queen_location / 8 + 1) +
                     stringer((location % 8 + 'a')) + to_string(location / 8 + 1);
            capture &= ~queen_moves;
            queen_moves = capture & ~(capture - 1);
        }
        queen &= ~queens;
        queens = queen & ~(queen - 1);
    }
    return moves;
}

Long danger(Chessboard board, bool color) {
    Long danger, capture, unit, ortho, diag;
    int unit_location;
    taken = board.white_pawn | board.white_knight | board.white_bishop | board.white_rook | board.white_queen |
            board.white_king | board.black_pawn | board.black_knight | board.black_bishop | board.black_rook |
            board.black_queen | board.black_king;
    if (color == 0) {
        danger = (((board.black_pawn << 0x7) & ~rook_left) | ((board.black_pawn << 0x9) & ~rook_right));
        unit = board.black_knight & ~(board.black_knight - 1);
    }
    if (color == 1) {
        danger = (((static_cast<ULong>(board.white_pawn) >> 0x7) & ~rook_left) |
                  ((static_cast<ULong>(board.white_pawn) >> 0x9) & ~rook_right));
        unit = board.white_knight & ~(board.white_knight - 1);
    }
    while (unit != 0) {
        unit_location = trailing_count(unit);
        if (unit_location > 0x12) {
            capture = knight_region << (unit_location - 0x12);
        } else {
            capture = knight_region >> (0x12 - unit_location);
        }
        if (unit_location % 8 < 4) {
            capture &= ~knight_right;
        } else {
            capture &= ~knight_left;
        }
        danger |= capture;
        if (color == 0) {
            board.black_knight &= ~unit;
            unit = board.black_knight & ~(board.black_knight - 1);
        }
        if (color == 1) {
            board.white_knight &= ~unit;
            unit = board.white_knight & ~(board.white_knight - 1);
        }
    }
    if (color == 0) diag = board.black_queen | board.black_bishop;
    if (color == 1) diag = board.white_queen | board.white_bishop;
    unit = diag & ~(diag - 1);
    while (unit != 0) {
        unit_location = trailing_count(unit);
        capture = diag_moves(unit_location);
        danger |= capture;
        diag &= ~unit;
        unit = diag & ~(diag - 1);
    }
    if (color == 0) ortho = board.black_queen | board.black_rook;
    if (color == 1) ortho = board.white_queen | board.white_rook;
    unit = ortho & ~(ortho - 1);
    while (unit != 0) {
        unit_location = trailing_count(unit);
        capture = ortho_moves(unit_location);
        danger |= capture;
        ortho &= ~unit;
        unit = ortho & ~(ortho - 1);
    }
    if (color == 0) unit_location = trailing_count(board.black_king);
    if (color == 1) unit_location = trailing_count(board.white_king);
    if (unit_location > 0x9) {
        capture = king_region << (unit_location - 0x9);
    } else {
        capture = king_region >> (0x9 - unit_location);
    }
    if (unit_location % 8 < 4) {
        capture &= ~knight_right;
    } else {
        capture &= ~knight_left;
    }
    danger |= capture;
    return danger;
}

Moves king_moves(Board king, Long taken) {
    Moves moves = "";
    Long capture, king_moves;
    int position, location = trailing_count(king);
    if (location > 9) {
        capture = king_region << (location - 9);
    } else {
        capture = king_region >> (9 - location);
    }
    if (location % 8 < 4) {
        capture &= ~knight_right & taken;
    } else {
        capture &= ~knight_left & taken;
    }
    king_moves = capture & ~(capture - 1);
    while (king_moves != 0) {
        position = trailing_count(king_moves);
        moves += stringer((location % 8 + 'a')) + to_string(location / 8 + 1) +
                 stringer((position % 8 + 'a')) + to_string(position / 8 + 1);
        capture &= ~king_moves;
        king_moves = capture & ~(capture - 1);
    }
    return moves;
}

Moves castle(Chessboard board, bool color) {
    Moves moves = "";
    Long dangerous = danger(board, color);
    if (color == 0) {
        if ((dangerous & board.black_king) == 0) {
            if (board.king_castle_black && (((static_cast<Long>(1) << 0x7L) & board.black_rook) != 0)) {
                if (((taken | dangerous) & ((static_cast<Long>(1) << 5) | (static_cast<Long>(1) << 6))) == 0)
                    moves += "0406";
            }
            if (board.queen_castle_black && (((static_cast<Long>(1) << 0x0L) & board.black_rook) != 0)) {
                if (((taken | (dangerous & ~(static_cast<Long>(1) << 1))) &
                     ((static_cast<Long>(1) << 1) | (static_cast<Long>(1) << 2) | (static_cast<Long>(1) << 3))) == 0)
                    moves += "0402";
            }
        }
    }
    if (color == 1) {
        if ((dangerous & board.white_king) == 0) {
            if (board.king_castle_white && (((static_cast<Long>(1) << 0x3FL) & board.white_rook) != 0)) {
                if (((taken | dangerous) & ((static_cast<Long>(1) << 61) | (static_cast<Long>(1) << 62))) == 0)
                    moves += "7476";
            }
            if (board.queen_castle_white && (((static_cast<Long>(1) << 0x38L) & board.white_rook) != 0)) {
                if (((taken | (dangerous & ~(static_cast<Long>(1) << 57))) &
                     ((static_cast<Long>(1) << 57) | (static_cast<Long>(1) << 58) | (static_cast<Long>(1) << 59))) == 0)
                    moves += "7472";
            }
        }
    }
    return moves;
}

Moves moves(Chessboard board, bool color) {
    Moves pawn;
    if (color == 0) {
        not_to_capture = (board.white_king | board.black_bishop | board.black_knight | board.black_king |
                          board.black_pawn | board.black_queen | board.black_rook);
        units = (board.black_bishop | board.black_knight | board.black_pawn | board.black_queen | board.black_rook);
        taken = (board.white_bishop | board.white_knight | board.white_king | board.white_pawn | board.white_queen |
                 board.white_rook | board.black_bishop | board.black_knight | board.black_king | board.black_pawn |
                 board.black_queen | board.black_rook);
        pawn = black_pawn_moves(board.black_pawn, board.white_pawn, board.en_passant);
    }
    if (color == 1) {
        not_to_capture = (board.black_king | board.white_bishop | board.white_knight | board.white_king |
                          board.white_pawn | board.white_queen | board.white_rook);
        units = (board.white_bishop | board.white_knight | board.white_pawn | board.white_rook | board.white_queen);
        taken = (board.black_bishop | board.black_knight | board.black_king | board.black_pawn | board.black_queen |
                 board.black_rook | board.white_bishop | board.white_knight | board.white_king | board.white_pawn |
                 board.white_queen | board.white_rook);
        pawn = white_pawn_moves(board.white_pawn, board.black_pawn, board.en_passant);
    }
    if (color == 1)
        return pawn + rook_moves(board.white_rook, color) + bishop_moves(board.white_bishop, color) +
               knight_moves(board.white_knight, color) + queen_moves(board.white_queen, color) +
               king_moves(board.white_king, color) + castle(board, color);
    return pawn + "Rook: " + rook_moves(board.black_rook, color) + "Bishop: " +
           bishop_moves(board.black_bishop, color) +
           "Knight: " + knight_moves(board.black_knight, color) + "Queen: " + queen_moves(board.black_queen, color) +
           "King: " + king_moves(board.black_king, color) + "Castle: " + castle(board, color);
}

Chessboard init_board() {
    Chessboard board;
    board.black_pawn = 0xFF000000000000;
    board.black_knight = 0x4200000000000000;
    board.black_bishop = 0x2400000000000000;
    board.black_rook = 0x8100000000000000;
    board.black_queen = 0x800000000000000;
    board.black_king = 0x1000000000000000;
    board.white_pawn = 0xFF00;
    board.white_knight = 0x42;
    board.white_bishop = 0x24;
    board.white_rook = 0x81;
    board.white_queen = 0x8;
    board.white_king = 0x10;
    //board.en_passant = 0x;
    board.king_castle_white = true;
    board.queen_castle_white = true;
    board.king_castle_black = true;
    board.queen_castle_black = true;
    return board;
}

int evaluate(){
    return 0;
}

void init_zo(Zobrist Z){
    //Cell,Pieces,Color 64,6,2
    for(int cell = 0; cell < 64; cell++)
        for(int pieces = 0; pieces < 6; pieces++)
            for(int color = 0; color < 2; color++)
                Z.table[cell][pieces][color] = zo_rand;
    for(int c = 0; c < 8; c++){
        if(c < 4) Z.castle[c] = zo_rand;
        Z.en_passant[c] = zo_rand;
    }
    Z.opponent_move = zo_rand;
}

Long get_hash(Chessboard board, bool turn){
    Long key = 0;
    //Cell,Pieces,Color 64,6,2
    for(int cell = 0; cell < 64; cell++){
        //if
    }
}


/*Moves decision(int depth, int alpha, int beta, Moves move, bool color){
    int turn = (int)color;
    Moves possible_moves = moves();
    if(depth == 0 || possible_moves.length() == 0) return move + stringer(evaluate()*(2*turn -1));
    turn = 1 - turn;
    for(int o = 0; o  < possible_moves.length(); o+=5){
        make_moves(possible_moves.substr());
    }

}*/
