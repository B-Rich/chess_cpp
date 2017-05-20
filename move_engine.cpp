#include "func.h"
#include "typedefs.h"
#include "board_constants.h"

#define num_c(i) ((int)(move[i] - 'a'))
#define num_r(i) ((int)((move[i] - '1')*8))
#define legal(move) (all_moves.find(move) != std::string::npos)
Long not_to_capture;
Long units;
Long taken;
Zobrist Z;
bool player;
bool human;
bool bot;
int move_counter;
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
    Long loc = static_cast<ULong>(1) << location; //Binary value
    Board Hor = (taken - 2 * loc) ^reversed(reversed(taken) - (2 * reversed(loc)));
    Board Ver = ((taken & file[location % 8]) - (loc * 2)) ^
                reversed(reversed(taken & file[location % 8]) - (reversed(loc) * 2));
    return (Hor & rank_[location / 8]) | (Ver & file[location % 8]);
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
    Long potential_moves = (black_pawns >> 7) & not_to_capture & taken & ~promotion_black & ~rook_left;
    Long capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer(position % 8 + 'a' - 1) + to_string(position / 8 + 2) +
                 stringer(position % 8 + 'a') + to_string(position / 8 + 1);
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //left
    potential_moves = (black_pawns >> 9) & not_to_capture & taken & ~promotion_black & ~rook_right;
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
        moves += stringer((position % 8 + 'a' + 1)) + to_string(position / 8) +
                 stringer((position % 8 + 'a')) + to_string(position / 8 + 1);
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
                      enpassant_white;//enpassant_black;//enpassant_white; <= original
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
                 stringer((position % 8)) + "QP" +
                 stringer((position % 8 + 'a' + 1)) +
                 stringer((position % 8)) +
                 "NP" + stringer((position % 8 + 'a' + 1)) +
                 stringer((position % 8)) + "BP" +
                 stringer((position % 8 + 'a' + 1)) +
                 stringer((position % 8)) + "RP";
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //promotion right
    potential_moves = (white_pawns << 9) & not_to_capture & taken & promotion_white & ~rook_left;
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer((position % 8 + 'a' - 1)) +
                 stringer((position % 8)) + "QP"
                 + stringer((position % 8 + 'a' - 1)) +
                 stringer((position % 8)) +
                 "NP" + stringer((position % 8 + 'a' - 1)) +
                 stringer((position % 8)) + "BP" +
                 stringer((position % 8 + 'a' - 1)) +
                 stringer((position % 8)) + "RP";
        potential_moves &= ~capture;
        capture = potential_moves & ~(potential_moves - 1);
    }
    //promotion advancing
    potential_moves = (white_pawns << 8) & ~taken & promotion_white;
    capture = potential_moves & ~(potential_moves - 1);
    while (capture != 0) {
        position = trailing_count(capture);
        moves += stringer((position % 8 + 'a')) +
                 stringer((position % 8)) + "QP"
                 + stringer((position % 8 + 'a')) +
                 stringer((position % 8)) +
                 "NP" + stringer((position % 8 + 'a')) +
                 stringer((position % 8)) + "BP" +
                 stringer((position % 8 + 'a')) +
                 stringer((position % 8)) +
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
            print(capture&~knight_left&not_to_capture);*/
        }
        if (knight_location % 8 < 0x4) {
            capture &= ~knight_left & not_to_capture;//~knight_left & not_to_capture; //~knight_left & not_to_capture;
        } else {
            capture &= ~knight_right & not_to_capture;//~knight_right & not_to_capture;
        }
        //print(capture);
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

Moves rook_moves(Board rook, Long takenn) {
    int rook_location, location;
    Moves moves = "";
    Long rook_moves, capture, rooks = rook & ~(rook - 1);
    while (rooks != 0) {
        rook_location = trailing_count(rooks);
        capture = ortho_moves(rook_location) & not_to_capture;//not_to_capture;
        rook_moves = capture & ~(capture - 1);
        /*print(rook);
        print((rank_[0] | file[0]) & taken);
        print(ortho_moves(rook_location));
        print(capture);
        print(not_to_capture);*/
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
    if (location % 8 >= 4) {
        capture &= ~knight_right & not_to_capture;//capture &= ~knight_right & taken;
    } else {
        capture &= ~knight_left & not_to_capture;//capture &= ~knight_left & taken;
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
                    moves += "d1f1";
            }
            if (board.queen_castle_black && (((static_cast<Long>(1) << 0x0L) & board.black_rook) != 0)) {
                if (((taken | (dangerous & ~(static_cast<Long>(1) << 1))) &
                     ((static_cast<Long>(1) << 1) | (static_cast<Long>(1) << 2) | (static_cast<Long>(1) << 3))) == 0)
                    moves += "d1b1";
            }
        }
    }
    if (color == 1) {
        if ((dangerous & board.white_king) == 0) {
            if (board.king_castle_white && (((static_cast<Long>(1) << 0x3FL) & board.white_rook) != 0)) {
                if (((taken | dangerous) & ((static_cast<Long>(1) << 61) | (static_cast<Long>(1) << 62))) == 0)
                    moves += "d8f8";
            }
            if (board.queen_castle_white && (((static_cast<Long>(1) << 0x38L) & board.white_rook) != 0)) {
                if (((taken | (dangerous & ~(static_cast<Long>(1) << 57))) &
                     ((static_cast<Long>(1) << 57) | (static_cast<Long>(1) << 58) | (static_cast<Long>(1) << 59))) == 0)
                    moves += "d8b8";
            }
        }
    }
    return moves;
}

Moves moves(Chessboard board, bool color) {
    Moves pawn;
    if (color == 0) {
        not_to_capture = ~(board.white_king | board.black_bishop | board.black_knight | board.black_king |
                          board.black_pawn | board.black_queen | board.black_rook);
        units = (board.black_bishop | board.black_knight | board.black_pawn | board.black_queen | board.black_rook);
        taken = (board.white_bishop | board.white_knight | board.white_king | board.white_pawn | board.white_queen |
                 board.white_rook | board.black_bishop | board.black_knight | board.black_king | board.black_pawn |
                 board.black_queen | board.black_rook);
        pawn = black_pawn_moves(board.black_pawn, board.white_pawn, board.en_passant);
    }
    if (color == 1) {
        not_to_capture = ~(board.black_king | board.white_bishop | board.white_knight | board.white_king |
                          board.white_pawn | board.white_queen | board.white_rook);
        units = (board.white_bishop | board.white_knight | board.white_pawn | board.white_rook | board.white_queen);
        taken = (board.black_bishop | board.black_knight | board.black_king | board.black_pawn | board.black_queen |
                 board.black_rook | board.white_bishop | board.white_knight | board.white_king | board.white_pawn |
                 board.white_queen | board.white_rook);
        pawn = white_pawn_moves(board.white_pawn, board.black_pawn, board.en_passant);
    }
    if (color == 1)
        return pawn + rook_moves(board.white_rook, color) +
               bishop_moves(board.white_bishop, color) +
               knight_moves(board.white_knight, color) +
               queen_moves(board.white_queen, color) +
               king_moves(board.white_king, color) + castle(board, color);
    return pawn + rook_moves(board.black_rook, color) +
           bishop_moves(board.black_bishop, color) +
           knight_moves(board.black_knight, color) +
           queen_moves(board.black_queen, color) +
           king_moves(board.black_king, color) + castle(board, color);
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
    board.en_passant = 0x0;
    board.king_castle_white = true;
    board.queen_castle_white = true;
    board.king_castle_black = true;
    board.queen_castle_black = true;
    return board;
}

Chessboard init_null(){
    Chessboard board;
    board.black_pawn = 0x0;
    board.black_knight = 0x0;
    board.black_bishop = 0x0;
    board.black_rook = 0x0;
    board.black_queen = 0x0;
    board.black_king = 0x0;
    board.white_pawn = 0x0;
    board.white_knight = 0x0;
    board.white_bishop = 0x0;
    board.white_rook = 0x0;
    board.white_queen = 0x0;
    board.white_king = 0x0;
    board.en_passant = 0x0;
    board.king_castle_white = false;
    board.queen_castle_white = false;
    board.king_castle_black = false;
    board.queen_castle_black = false;
    return board;
}

int move_piece(Chessboard c_board, Board *board, Moves move, char promote_to){//Moves source, Moves destination) {
    //Moves move = source+destination;
    //cout << "All moves: " << all_moves <<endl;
    int src, dst;
    if(isdigit(move[3])){
        src = num_c(0) + num_r(1); //en
        dst = num_c(2) + num_r(3);
        if (((static_cast<ULong>(*board) >> src) & 1) == 1) { //Exists
            *board &= ~(static_cast<ULong>(1) << src); //remove
            *board |= (static_cast<ULong>(1) << dst); //move
        } else {
            *board &= ~(static_cast<ULong>(1) << dst);
        }
    }else if(move[3] == 'E'){
        if(move[2] == 'B'){
            src  = trailing_count(file[move[0]-'a']&rank_[4]);
            dst  = trailing_count(file[move[1]-'a']&rank_[5]);
            *board&=~(file[move[1]-'a']&rank_[4]);
        }else{
            src  = trailing_count(file[move[0]-'a']&rank_[3]);
            dst  = trailing_count(file[move[1]-'a']&rank_[2]);
            *board&=~(file[move[1]-'a']&rank_[3]);
        }if (((static_cast<ULong>(*board) >> src) & 1) == 1) {
            *board &= ~(static_cast<ULong>(1) << src);
            *board |= (static_cast<ULong>(1) << dst);
        }
    }else if(move[3] == 'P') {
        if (islower(move[2])) {
            src = trailing_count(file[move[0] - 'a'] & rank_[6]);
            dst = trailing_count(file[move[1] - 'a'] & rank_[7]);
        } else {
            src = trailing_count(file[move[0] - 'a'] & rank_[1]);
            dst = trailing_count(file[move[1] - 'a'] & rank_[0]);
        }
        if (move[2] == promote_to) {
            *board &= ~(static_cast<ULong>(1) << src);
            *board |= (static_cast<ULong>(1) << dst);
        } else {
            *board &= ~(static_cast<ULong>(1) << dst);
        }
    }
    return 1;
}

Board get_board_by_name(Chessboard board, char *name) {
    //p n b r q k
    (char) tolower(name[1]);
    if ((char) tolower(name[0]) == 'w') {
        switch ((char) tolower(name[1])) {
            case 'p':
                return board.white_pawn;
            case 'n':
                return board.white_knight;
            case 'b':
                return board.white_bishop;
            case 'r':
                return board.white_knight;
            case 'q':
                return board.white_queen;
            case 'k':
                return board.white_king;
            default:
                break;
        }
    }
    if ((char) tolower(name[0]) == 'b') {
        switch ((char) tolower(name[1])) {
            case 'p':
                return board.black_pawn;
            case 'n':
                return board.black_knight;
            case 'b':
                return board.black_bishop;
            case 'r':
                return board.black_knight;
            case 'q':
                return board.black_queen;
            case 'k':
                return board.black_king;
            default:
                break;
        }
    }
}

Board* get_board(Chessboard* board, char unit) {
    //p n b r q k
    switch (unit) {
        case 'P':
            return &board->white_pawn;
        case 'N':
            return &board->white_knight;
        case 'B':
            return &board->white_bishop;
        case 'R':
            return &board->white_rook;
        case 'Q':
            return &board->white_queen;
        case 'K':
            return &board->white_king;
        case 'p':
            return &board->black_pawn;
        case 'n':
            return &board->black_knight;
        case 'b':
            return &board->black_bishop;
        case 'r':
            return &board->black_rook;
        case 'q':
            return &board->black_queen;
        case 'k':
            return &board->black_king;
        default:
            return NULL;
            break;
    }
}


void init_zo() {
    //Cell,Pieces,Color 64,6,2
    for (int cell = 0; cell < 64; cell++)
        for (int pieces = 0; pieces < 6; pieces++)
            for (int color = 0; color < 2; color++)
                Z.table[cell][pieces][color] = zo_rand;
    for (int c = 0; c < 8; c++) {
        if (c < 4) Z.castle[c] = zo_rand;
        Z.en_passant[c] = zo_rand;
    }
    Z.opponent_move = zo_rand;
}

Long get_hash(Chessboard board, bool turn) {
    Long key = 0;
    //Cell,Pieces,Color 64,6,2
    //pawn , knight, bishop, rook, queen, king
    for (int cell = 0; cell < 64; cell++) {
        if (in_cell(board.white_pawn, cell)) {
            key ^= Z.table[cell][pawn][white];
        } else if (in_cell(board.white_knight, cell)) {
            key ^= Z.table[cell][knight][white];
        } else if (in_cell(board.white_bishop, cell)) {
            key ^= Z.table[cell][bishop][white];
        } else if (in_cell(board.white_rook, cell)) {
            key ^= Z.table[cell][rook][white];
        } else if (in_cell(board.white_queen, cell)) {
            key ^= Z.table[cell][queen][white];
        } else if (in_cell(board.white_king, cell)) {
            key ^= Z.table[cell][king][white];
        } else if (in_cell(board.black_pawn, cell)) {
            key ^= Z.table[cell][pawn][black];
        } else if (in_cell(board.black_knight, cell)) {
            key ^= Z.table[cell][knight][black];
        } else if (in_cell(board.black_bishop, cell)) {
            key ^= Z.table[cell][bishop][black];
        } else if (in_cell(board.black_rook, cell)) {
            key ^= Z.table[cell][rook][black];
        } else if (in_cell(board.black_queen, cell)) {
            key ^= Z.table[cell][queen][black];
        } else if (in_cell(board.black_king, cell)) {
            key ^= Z.table[cell][king][black];
        }
    }
    for (int c = 0; c < 8; c++) {
        if (board.en_passant == rank_[c]) key ^= Z.en_passant[c];
    }
    if (board.king_castle_white) key ^= Z.castle[0];
    if (board.queen_castle_white) key ^= Z.castle[1];
    if (board.king_castle_black) key ^= Z.castle[2];
    if (board.queen_castle_black) key ^= Z.castle[3];
    if (turn != human) key ^= Z.opponent_move;//if (turn == black) key ^= Z.opponent_move;
    return key;
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
Chessboard FEN(string FEN) {
    Chessboard board = init_null();
    int i = 0, location = 0;
    while (FEN[i] != ' ') {
        switch (FEN[i++]) {
            case 'P':
                add(board.white_pawn, location);
                break;
            case 'N':
                add(board.white_knight, location);
                break;
            case 'B':
                add(board.white_bishop, location);
                break;
            case 'R':
                add(board.white_rook, location);
                break;
            case 'Q':
                add(board.white_queen, location);
                break;
            case 'K':
                add(board.white_king, location);
                break;
            case 'p':
                add(board.black_pawn, location);
                break;
            case 'n':
                add(board.black_knight, location);
                break;
            case 'b':
                add(board.black_bishop, location);
                break;
            case 'r':
                add(board.black_rook, location);
                break;
            case 'q':
                add(board.black_queen, location);
                break;
            case 'k':
                add(board.black_king, location);
                break;
            case '/':
                break;
            case '1':
                location++;
                break;
            case '2':
                location+=2;
                break;
            case '3':
                location+=3;
                break;
            case '4':
                location+=4;
                break;
            case '5':
                location+=5;
                break;
            case '6':
                location+=6;
                break;
            case '7':
                location+=7;
                break;
            case '8':
                location+=8;
                break;
            default:
                break;
        }
    }
    player = (FEN[++i] == 'w');
    i += 2;
    while (FEN[i] != ' ') {
        switch (FEN[i++]) {
            case '-':
                break;
            case 'K':
                board.king_castle_white = true;
                break;
            case 'Q':
                board.queen_castle_white = true;
                break;
            case 'k':
                board.king_castle_black = true;
                break;
            case 'q':
                board.queen_castle_black = true;
                break;
            default:
                break;
        }
    }
    if (FEN[++i] != '-') board.en_passant = rank_[(int) (FEN[i++] - 'a')];
    return board;
}

void print_cb(Chessboard board) {
    char position;
    printf("\n\n");
    //printf("  ");
    for (int cell = 0; cell < 64; cell++) {
        if (cell % 8 == 0 && cell != 0) printf("\n");
        if (in_cell(board.white_pawn, cell)) {
            cout << "P ";
        } else if (in_cell(board.white_knight, cell)) {
            cout << "N ";
        } else if (in_cell(board.white_bishop, cell)) {
            cout << "B ";
        } else if (in_cell(board.white_rook, cell)) {
            cout << "R ";
        } else if (in_cell(board.white_queen, cell)) {
            cout << "Q ";
        } else if (in_cell(board.white_king, cell)) {
            cout << "K ";
        } else if (in_cell(board.black_pawn, cell)) {
            cout << "p ";
        } else if (in_cell(board.black_knight, cell)) {
            cout << "n ";
        } else if (in_cell(board.black_bishop, cell)) {
            cout << "b ";
        } else if (in_cell(board.black_rook, cell)) {
            cout << "r ";
        } else if (in_cell(board.black_queen, cell)) {
            cout << "q ";
        } else if (in_cell(board.black_king, cell)) {
            cout << "k ";
        } else {
            cout << "_ ";
        }
    }
    printf("\n\n");
}

char unit(Chessboard board, Moves square){
    int location = (square[0] - 'a') + ((square[1] - '1')*8);
    if (in_cell(board.white_pawn, location)) {
        return 'P';
    } else if (in_cell(board.white_knight, location)) {
        return 'N';
    } else if (in_cell(board.white_bishop, location)) {
        return 'B';
    } else if (in_cell(board.white_rook, location)) {
        return 'R';
    } else if (in_cell(board.white_queen, location)) {
        return 'Q';
    } else if (in_cell(board.white_king, location)) {
        return 'K';
    } else if (in_cell(board.black_pawn, location)) {
        return 'p';
    } else if (in_cell(board.black_knight, location)) {
        return 'n';
    } else if (in_cell(board.black_bishop, location)) {
        return 'b';
    } else if (in_cell(board.black_rook, location)) {
        return 'r';
    } else if (in_cell(board.black_queen, location)) {
        return 'q';
    } else if (in_cell(board.black_king, location)) {
        return 'k';
    } else {
        return 'X';
    }
}

int play(Chessboard *board, Moves move){
    Moves all_moves = moves(*board,player);
    Board *src = get_board(board,unit(*board,move.substr(0,2)));//board(0);
    Board *dest = get_board(board,unit(*board,move.substr(2,2)));
    if(legal(move)) {
            move_piece(*board, src, move, move[3]);
            if (dest != NULL) move_piece(*board, dest, move, move[3]);
            player = !player;
        move_counter++;
        return 1;
    }
    return 0;
}

