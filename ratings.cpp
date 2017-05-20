#include "typedefs.h"
#include "func.h"
#include "rating_consts.h"
#include "board_constants.h"
#include "move_engine.h"

void reverse(short *arr, short s, short e) {
    short temp;
    while (s < e) {
        temp = arr[s];
        arr[s] = arr[e];
        arr[e] = temp;
        s++;
        e--;
    }
}

void reverse(short *arr) {
    reverse(arr, 0, 63);
}

void pawn_ageing(Chessboard board){

}

int material_eval(Chessboard board) {
    int val = 0;
    if (player == white) {
        if (set_bits(board.white_pawn) > 0) val += (p_centi * set_bits(board.white_pawn));
        if (set_bits(board.white_knight) > 0) val += (n_centi * set_bits(board.white_knight));
        if (set_bits(board.white_bishop) >= 2) val += (b_centi * set_bits(board.white_bishop));
        if (set_bits(board.white_bishop) == 1) val += (b_centi - 30);
        if (set_bits(board.white_rook) > 0) val += (r_centi * set_bits(board.white_rook));
        if (set_bits(board.white_queen) > 0) val += (q_centi * set_bits(board.white_queen));
    } else {
        if (set_bits(board.black_pawn) > 0) val += (p_centi * set_bits(board.black_pawn));
        if (set_bits(board.black_knight) > 0) val += (n_centi * set_bits(board.black_knight));
        if (set_bits(board.black_bishop) >= 2) val += (b_centi * set_bits(board.black_bishop));
        if (set_bits(board.black_bishop) == 1) val += (b_centi - 30);
        if (set_bits(board.black_rook) > 0) val += (r_centi * set_bits(board.black_rook));
        if (set_bits(board.black_queen) > 0) val += (q_centi * set_bits(board.black_queen));
    }
    return val;
}

int mobility_eval(Chessboard board, int depth, int moves) { //fix moves
    int val = 0;
    bool king_in_danger = danger(board, player) > 0 ? 1 : 0;
    val += moves * 6;
    if (moves == 0) { //Check or stale
        if (king_in_danger) {
            val -= k_centi * depth;
        } else {
            val -= 0.75 * k_centi * depth;
        }
    }
    return val;
}

int location_eval(Chessboard board, int material_val) { //add king moves
    Board brd = player == white ? board.white_king : board.black_king;
    int king = king_moves(brd, 0).length() / 4;
    int val = 0;
    bool end_game = material_val >= q_centi + n_centi + b_centi + r_centi ? 0 : 1;
    for (int cell = 0; cell < 64; cell++) {
        if (player == white) {
            if (in_cell(board.white_pawn, cell)) val += r_pawn[64 - cell];
            if (in_cell(board.white_knight, cell)) val += r_knight[64 - cell];
            if (in_cell(board.white_bishop, cell)) val += r_bishop[64 - cell];
            if (in_cell(board.white_rook, cell)) val += r_rook[64 - cell];
            if (in_cell(board.white_queen, cell)) val += r_queen[64 - cell];
            if (in_cell(board.white_king, cell)) {
                if (end_game) {
                    val += (r_king_late[64 - cell] + king * 30);
                } else {
                    val += (r_king_mid[64 - cell] + king * 10);
                }
            }
        } else {
            if (in_cell(board.black_pawn, cell)) val += r_pawn[cell];
            if (in_cell(board.black_knight, cell)) val += r_knight[cell];
            if (in_cell(board.black_bishop, cell)) val += r_bishop[cell];
            if (in_cell(board.black_rook, cell)) val += r_rook[cell];
            if (in_cell(board.black_queen, cell)) val += r_queen[cell];
            if (in_cell(board.black_king, cell)) {
                if (end_game) {
                    val += (r_king_late[cell] + king * 30);
                } else {
                    val += (r_king_mid[cell] + king * 10);
                }
            }
        }
    }
    return val;
}

int attack_eval(Chessboard board) { //only pawns and attacked by other units
    int val = 0; //use king to simulate if an attack in performed on square
    for (int cell = 0; cell < 64; cell++) {
        if (player == white) {
            board.white_king = (static_cast<ULong>(1) << cell);
            if (in_cell(board.white_pawn, cell)) {
                if (danger(board, player))
                    val -= (p_centi - 20 * set_bits(board.white_pawn));
            }
            if (in_cell(board.white_knight, cell)) {
                if (danger(board, player))
                    val -= (n_centi * set_bits(board.white_knight));
            }
            if (in_cell(board.white_bishop, cell)) {
                if (danger(board, player))
                    val -= (b_centi * set_bits(board.white_bishop));
            }
            if (in_cell(board.white_rook, cell)) {
                if (danger(board, player))
                    val -= (r_centi * set_bits(board.white_rook));
            }
            if (in_cell(board.white_queen, cell)) {
                if (danger(board, player))
                    val -= (q_centi * set_bits(board.white_queen));
            }
        } else {
            board.black_king = (static_cast<ULong>(1) << cell);
            if (in_cell(board.black_pawn, cell)) {
                if (danger(board, player))
                    val += (p_centi - 20 * set_bits(board.black_pawn));
            }
            if (in_cell(board.black_knight, cell)) {
                if (danger(board, player))
                    val += (n_centi * set_bits(board.black_knight));
            }
            if (in_cell(board.black_bishop, cell)) {
                if (danger(board, player))
                    val += (b_centi * set_bits(board.black_bishop));
            }
            if (in_cell(board.black_rook, cell)) {
                if (danger(board, player))
                    val += (r_centi * set_bits(board.black_rook));
            }
            if (in_cell(board.black_queen, cell)) {
                if (danger(board, player))
                    val += (q_centi * set_bits(board.black_queen));
            }
        }
    }
    return val / 2;
}

int evaluate(Chessboard board, int depth, int moves) {
    /**cout << "Depth: " << endl;
    print_cb(board);**/
    int eval = 0;
    int init_mat = material_eval(board);
    eval = attack_eval(board) + init_mat + mobility_eval(board, depth, moves) +
           location_eval(board, init_mat);
    //if(player == black) return -eval;
    return eval;
}