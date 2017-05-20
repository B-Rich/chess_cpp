#include "typedefs.h"
#include "move_engine.h"
#include "ratings.h"
#include "board_constants.h"
#include <stdexcept>
//get_board(&mimic,sliced[move].substr(2,2))
#define no_available_moves (possible_moves.length() == 0)
#define leaf_node (depth == 0)
#define piece(i) (unit(mimic,sliced[m].substr(i,2)))
#define promo_unit (sliced[m][3])
#define board(i) (get_board(&mimic, piece(i)))
#define cur_move (sliced[m])
#define best_move (best.substr(0,4))

int partition(int rating[], Moves move[], int start, int end) {
    int pivot = rating[end];
    int i = (start - 1);
    for (int j = start; j <= end - 1; j++) {
        /**if(player == human){
            if (rating[j] <= pivot) {
                swap(move[++i], move[j]);
            }
        }
        if(player == bot){
            if (rating[j] > pivot) {
                swap(move[++i], move[j]);
            }
        }**/
        if (rating[j] > pivot) {
            swap(move[++i], move[j]);
        }
    }
    swap(move[i + 1], move[end]);
    return (i + 1);
}

void movesort(int rating[], Moves move[], int start, int end) {
    if (start < end) {
        int pivot = partition(rating, move, start, end);
        movesort(rating, move, start, pivot - 1);
        movesort(rating, move, pivot + 1, end);
    }
}

Moves *slice(Moves all_moves) {
    Moves *separate = new Moves[all_moves.length() / 4];
    for (int m = 0; m < all_moves.length(); m += 4) {
        separate[m / 4] = (Moves) all_moves.substr(m, 4);
    }
    return separate;
}

Moves *sortedMoves(Chessboard brd, Moves all_moves) {
    int move_count = all_moves.length() / 4;
    int *rating = new int[move_count];
    Chessboard mimic = brd;
    Board *src = NULL;
    Board *dest = NULL;
    Moves *sliced = slice(all_moves);
    //print_cb(brd);
    for (int m = 0; m < move_count; m++) {
        src = board(0);
        /**cout << "Unit @ src:" << piece(0) << endl;
        cout << "Unit @ dest:" << piece(2) << endl;
        cout << "Promotion unit is:" << promo_unit << endl;**/
        dest = board(2);
        move_piece(mimic, src, cur_move, promo_unit);
        //print(*src);
        if (dest != NULL) move_piece(mimic, dest, cur_move, promo_unit);
        rating[m] = evaluate(mimic, -1, 0);
        mimic = brd;
    }
    /*cout << "Pre-sorting " << endl;
    for (int i = 0; i < move_count; i++) {
        cout << (i + 1) << ": " << sliced[i] << " rating: " << rating[i] <<endl;
    }*/
    movesort(rating, sliced, 0, move_count - 1);
    /*cout << "After sorting " << endl;
    for (int i = 0; i < move_count; i++) {
        cout << (i + 1) << ": " << sliced[i] << " rating: " << rating[i] <<endl;
    }*/
    /*for (int move = 0; move < move_count; move++) {
        int highest_rating = INT_MIN, best = 0;
        for (int i_best = 0; i_best < move_count; i_best++) {
            if (rating[i_best] > highest_rating) {
                highest_rating = rating[i_best];
                best = i_best;
            }
        }
        rating[best] = INT_MIN;
    }*/
    delete (rating);
    Moves *sorted = sliced;
    return sorted;
}

int zero_search(int alpha, int beta, int depth, Chessboard board, Moves move) {
    int rating = INT_MIN;
    Chessboard mimic = board;
    Moves possible_moves = moves(mimic, player);
    Board *src = NULL;
    Board *dest = NULL;
    if (depth == MAX_DEPTH)
        return evaluate(board, depth, possible_moves.length() / 4);

}

/**typedef struct{
    int depth;
    int alpha;
    int beta;
    Moves move;
    bool color;
}Node;**/
Node ab_search(int alpha, int beta, int depth, Chessboard board, Moves move) {
    Node best, data;
    Moves possible_moves = moves(board, player);
    Chessboard mimic = board;
    Board *src = NULL;
    Board *dest = NULL;
    if (leaf_node || no_available_moves) {
        best.alpha = alpha;
        best.beta = beta;
        best.color = player;
        best.depth = MAX_DEPTH;
        best.move = move;
        best.rating = evaluate(board, depth, possible_moves.length() / 4);
        return best;
    }
    Moves *sliced = sortedMoves(board, possible_moves);
    for (int m = 0; m < possible_moves.length() / 4; m++) {
        src = board(0);
        dest = board(2);
        move_piece(mimic, src, cur_move, promo_unit);
        if (dest != NULL) move_piece(mimic, dest, cur_move, promo_unit);
        player = !player;
        best = ab_search(alpha, beta, depth - 1, mimic, cur_move);
        mimic = board;
        if (player == human) {
            if (best.rating > alpha) {
                alpha = best.rating;
                if (depth == MAX_DEPTH)
                    best.move = cur_move;
            }
        } else {
            if (best.rating <= beta) {
                beta = best.rating;
                if (depth == MAX_DEPTH)
                    best.move = cur_move;
            }
        }
        if (beta <= alpha) {
            return best;
        }
    }
    return best;
}

Moves albe_search(int alpha, int beta, int depth, Chessboard board, Moves move) {
    Chessboard mimic = board;
    Moves possible_moves = moves(mimic, player);
    Board *src = NULL;
    Board *dest = NULL;
    if (leaf_node || no_available_moves) {
        return move + "EVAL" + to_string(evaluate(board, depth, possible_moves.length() / 4));
        //return max_score;
    }
    Moves *sliced = sortedMoves(mimic, possible_moves);
    for (int m = 0; m < possible_moves.length() / 4; m++) {
        src = board(0);
        dest = board(2);
        //cout << "Source (" << src << ") : " << *src << " Destination (" << dest << ")" << endl;
        if (src == NULL) {
            cout << "NULL REACHED" << endl;
            cout << "Piece: " << piece(0) << endl;
            return to_string(piece(0));
        }
        move_piece(mimic, src, cur_move, promo_unit);
        if (dest != NULL) move_piece(mimic, dest, cur_move, promo_unit);
        player = !player;
        Moves best = albe_search(alpha, beta, depth - 1, mimic, cur_move);
        int rating = stoi(best.substr(best.find("EVAL") + 4));
        player = !player;
        mimic = board;
        if (player == white) {
            if (rating > alpha) {
                alpha = rating;
                if (depth == MAX_DEPTH) move = best_move;
            }
        } else {
            if (rating <= beta) {
                beta = rating;
                if (depth == MAX_DEPTH) move = best_move;
            }
        }
        if (beta <= alpha) {
            return move;
        }
    }
    return move;
}


Moves albe_search_sig(int alpha, int beta, int depth, int max_depth, Chessboard board, Moves move, volatile int play_sig) {
    try {
        Chessboard mimic = board;
        Moves possible_moves = moves(mimic, player);
        Board *src = NULL;
        Board *dest = NULL;
        if (leaf_node || no_available_moves || play_sig) {
            return move + "EVAL" + to_string(evaluate(board, depth, possible_moves.length() / 4));
            //return max_score;
        }
        Moves *sliced = sortedMoves(mimic, possible_moves);
        for (int m = 0; m < possible_moves.length() / 4; m++) {
            src = board(0);
            dest = board(2);
            //cout << "Source (" << src << ") : " << *src << " Destination (" << dest << ")" << endl;
            if (src == NULL) {
                cout << "NULL REACHED" << endl;
                cout << "Piece: " << piece(0) << endl;
                return to_string(piece(0));
            }
            move_piece(mimic, src, cur_move, promo_unit);
            if (dest != NULL) move_piece(mimic, dest, cur_move, promo_unit);
            player = !player;
            Moves best = albe_search_sig(alpha, beta, depth - 1, max_depth, mimic, cur_move, play_sig);
            if (play_sig) {
                throw logic_error("Terminating thread");
            }
            int rating = stoi(best.substr(best.find("EVAL") + 4));
            player = !player;
            mimic = board;
            if (player == white) {
                if (rating > alpha) {
                    alpha = rating;
                    if (depth == max_depth) move = best_move;
                }
            } else {
                if (rating <= beta) {
                    beta = rating;
                    if (depth == max_depth) move = best_move;
                }
            }
            if (beta <= alpha) {
                return move;
            }
        }
        return move;

    } catch (logic_error const& err) {
        return move;
    }
}
