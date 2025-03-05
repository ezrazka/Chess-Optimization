#include "include/BoardState.h"
#include <emscripten.h>

BoardState board;

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void init_board() {
        board = BoardState();
    }

    EMSCRIPTEN_KEEPALIVE
    const char* get_board_state() {
        return board.get_board_state();
    }

    EMSCRIPTEN_KEEPALIVE
    void update_board_state(int from_index, int to_index, int move_type) {
        board.update_board_state(Move(from_index, to_index, static_cast<MoveType>(move_type)));
    }

    EMSCRIPTEN_KEEPALIVE
    bool is_legal_move(int from_index, int to_index, int move_type) {
        return board.is_legal_move(Move(from_index, to_index, static_cast<MoveType>(move_type)));
    }
    
    EMSCRIPTEN_KEEPALIVE
    const int** get_legal_moves() {
        return board.get_legal_moves();
    }

    EMSCRIPTEN_KEEPALIVE
    const int* get_next_best_move() {
        return board.get_next_best_move();
    }
}