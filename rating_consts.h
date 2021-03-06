#ifndef CHESS_RATING_CONSTS_H
#define CHESS_RATING_CONSTS_H
const short r_pawn[64] = {0, 0, 0, 0, 0, 0, 0, 0,
                         50, 50, 50, 50, 50, 50, 50, 50,
                         10, 10, 20, 30, 30, 20, 10, 10,
                         5, 5, 10, 25, 25, 10, 5, 5,
                         0, 0, 0, 20, 50, 0, 0, 0,
                         5, -5, -10, 0, 0, -10, -5, 5,
                         5, 10, 10, -20, -20, 10, 10, 5,
                         0, 0, 0, 0, 0, 0, 0, 0};
const short r_knight[64] = {-50, -40, -30, -30, -30, -30, -40, -50,
                           -40, -20, 0, 0, 0, 0, -20, -40,
                           -30, 0, 10, 15, 15, 10, 0, -30,
                           -30, 5, 15, 20, 20, 15, 5, -30,
                           -30, 0, 15, 20, 20, 15, 0, -30,
                           -30, 5, 10, 15, 15, 10, 5, -30,
                           -40, -20, 0, 5, 5, 0, -20, -40,
                           -50, -40, -30, -30, -30, -30, -40, -50};
const short r_bishop[64] = {-20, -10, -10, -10, -10, -10, -10, -20,
                           -10, 0, 0, 0, 0, 0, 0, -10,
                           -10, 0, 5, 10, 10, 5, 0, -10,
                           -10, 5, 5, 10, 10, 5, 5, -10,
                           -10, 0, 10, 10, 10, 10, 0, -10,
                           -10, 10, 10, 10, 10, 10, 10, -10,
                           -10, 5, 0, 0, 0, 0, 5, -10,
                           -20, -10, -10, -10, -10, -10, -10, -20};
const short r_rook[64] = {0, 0, 0, 0, 0, 0, 0, 0,
                         5, 10, 10, 10, 10, 10, 10, 5,
                         -5, 0, 0, 0, 0, 0, 0, -5,
                         -5, 0, 0, 0, 0, 0, 0, -5,
                         -5, 0, 0, 0, 0, 0, 0, -5,
                         -5, 0, 0, 0, 0, 0, 0, -5,
                         -5, 0, 0, 0, 0, 0, 0, -5,
                         0, 0, 0, 5, 5, 0, 0, 0};
const short r_queen[64] = {-20, -10, -10, -5, -5, -10, -10, -20,
                          -10, 0, 0, 0, 0, 0, 0, -10,
                          -10, 0, 5, 5, 5, 5, 0, -10,
                          -5, 0, 5, 5, 5, 5, 0, -5,
                          0, 0, 5, 5, 5, 5, 0, -5,
                          -10, 5, 5, 5, 5, 5, 0, -10,
                          -10, 0, 5, 0, 0, 0, 0, -10,
                          -20, -10, -10, -5, -5, -10, -10, -20};
const short r_king_mid[64] = {-30, -40, -40, -50, -50, -40, -40, -30,
                             -30, -40, -40, -50, -50, -40, -40, -30,
                             -30, -40, -40, -50, -50, -40, -40, -30,
                             -30, -40, -40, -50, -50, -40, -40, -30,
                             -20, -30, -30, -40, -40, -30, -30, -20,
                             -10, -20, -20, -20, -20, -20, -20, -10,
                             20, 20, 0, 0, 0, 0, 20, 20,
                             20, 30, 10, 0, 0, 10, 30, 20};
const short r_king_late[64] = {-50, -40, -30, -20, -20, -30, -40, -50,
                              -30, -20, -10, 0, 0, -10, -20, -30,
                              -30, -10, 20, 30, 30, 20, -10, -30,
                              -30, -10, 30, 40, 40, 30, -10, -30,
                              -30, -10, 30, 40, 40, 30, -10, -30,
                              -30, -10, 20, 30, 30, 20, -10, -30,
                              -30, -30, 0, 0, 0, 0, -30, -30,
                              -50, -30, -30, -30, -30, -30, -30, -50};
const short p_centi = 100;
const short n_centi= 320;
const short b_centi = 330;
const short r_centi = 500;
const short q_centi = 900;
const short k_centi = 20000;
static short p_ageing = 1;
#endif //CHESS_RATING_CONSTS_H
