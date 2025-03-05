#ifndef BOARD_STATE_H
#define BOARD_STATE_H

#include "Move.h"
#include "Piece.h"
#include <cctype>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

const std::string DEFAULT_BOARD_STATE = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
// const std::string DEFAULT_BOARD_STATE = "rnb1kbnr/pp1Pqp1p/2p3p1/8/8/4P3/PPP2PPP/RNBQKBNR w KQkq - 0 6";

class BoardState {
public:
    BoardState() {
        set_board_state(DEFAULT_BOARD_STATE);
    }

    const char* get_board_state() {
        std::string fen = "";
        {

            int empty_count = 0;
            int i = 0;
            while (i < 64) {
                int fen_index = (7 - i / 8) * 8 + i % 8;
                if (i != 0 && i % 8 == 0) {
                    if (empty_count > 0) {
                        fen += std::to_string(empty_count);
                        empty_count = 0;
                    }
                    fen += '/';
                }
                if (Piece::is_empty(board[fen_index])) {
                    empty_count++;
                } else {
                    if (empty_count > 0) {
                        fen += std::to_string(empty_count);
                        empty_count = 0;
                    }
                    fen += Piece::get_piece_symbol_from_value(board[fen_index]);
                }
                i++;
            }
            if (empty_count > 0) {
                fen += std::to_string(empty_count);
            }
        }
        {
            fen += ' ';
            fen += white_to_move ? 'w' : 'b';
        }
        {
            fen += ' ';
            if (can_castle_kingside_white) fen += 'K';
            if (can_castle_queenside_white) fen += 'Q';
            if (can_castle_kingside_black) fen += 'k';
            if (can_castle_kingside_black) fen += 'q';
            if (fen.back() == ' ') fen += '-';
        }
        {
            fen += ' ';
            std::string en_passant_square_notation = "";
            en_passant_square_notation += 'a' + en_passant_index % 8;
            en_passant_square_notation += std::to_string(en_passant_index / 8 + 1);
            fen += en_passant_index == -1 ? "-" : en_passant_square_notation;
        }
        {
            fen += ' ';
            fen += std::to_string(half_move_clock);
        }
        {
            fen += ' ';
            fen += std::to_string(full_move_number);
        }
        char* fen_buffer = new char[fen.size() + 1];
        std::strcpy(fen_buffer, fen.c_str());
        return fen_buffer;
    }

    void update_board_state_normal(const Move& move) {
        en_passant_index = -1;
        if (Piece::is_piece_type(board[move.from_index], Piece::pawn) && abs(move.from_index - move.to_index) == 16) {
            en_passant_index = (move.from_index + move.to_index) / 2;
        }

        if (!Piece::is_piece_type(board[move.from_index], Piece::pawn) && Piece::is_empty(board[move.to_index])) {
            half_move_clock++;
        } else {
            half_move_clock = 0;
        }

        if (Piece::is_black_piece(board[move.from_index])) {
            full_move_number++;
        }

        int kingside_rook_index_white = 7;
        int queenside_rook_index_white = 0;
        int kingside_rook_index_black = 63;
        int queenside_rook_index_black = 56;
        if (
            (can_castle_kingside_white || can_castle_queenside_white) &&
            board[move.from_index] == (Piece::king | Piece::white)
        ) {
            can_castle_kingside_white = false;
            can_castle_queenside_white = false;
        }
        if (can_castle_kingside_white && move.from_index == kingside_rook_index_white) {
            can_castle_kingside_white = false;
        }
        if (can_castle_queenside_white && move.from_index == queenside_rook_index_white) {
            can_castle_queenside_white = false;
        }
        if (
            (can_castle_kingside_black || can_castle_queenside_black) &&
            board[move.from_index] == (Piece::king | Piece::black)
        ) {
            can_castle_kingside_black = false;
            can_castle_queenside_black = false;
        }
        if (can_castle_kingside_black && move.from_index == kingside_rook_index_black) {
            can_castle_kingside_black = false;
        }
        if (can_castle_queenside_black && move.from_index == queenside_rook_index_black) {
            can_castle_queenside_black = false;
        }

        board[move.to_index] = board[move.from_index];
        board[move.from_index] = Piece::none;

        white_to_move = !white_to_move;
    }

    void update_board_state_en_passant(const Move& move) {
        en_passant_index = -1;

        half_move_clock = 0;

        if (Piece::is_black_piece(board[move.from_index])) {
            full_move_number++;
        }

        board[move.to_index] = board[move.from_index];
        board[move.from_index] = Piece::none;
        if (move.from_index < move.to_index) {
            board[move.to_index - 8] = Piece::none;
        } else {
            board[move.to_index + 8] = Piece::none;
        }

        white_to_move = !white_to_move;
    }

    void update_board_state_castling_king_side(const Move& move) {
        if (Piece::is_white_piece(board[move.from_index])) {
            can_castle_kingside_white = false;
            en_passant_index = -1;
            half_move_clock = 0;

            int kingside_rook_index = 7;
            board[move.from_index + 1] = board[kingside_rook_index];
            board[move.to_index] = board[move.from_index];
            board[move.from_index] = Piece::none;
            board[kingside_rook_index] = Piece::none;

            white_to_move = !white_to_move;
        } else {
            can_castle_kingside_white = false;
            en_passant_index = -1;
            half_move_clock = 0;
            full_move_number++;

            int kingside_rook_index = 63;
            board[move.from_index + 1] = board[kingside_rook_index];
            board[move.to_index] = board[move.from_index];
            board[move.from_index] = Piece::none;
            board[kingside_rook_index] = Piece::none;

            white_to_move = !white_to_move;
        }
    }

    void update_board_state_castling_queen_side(const Move& move) {
        if (Piece::is_white_piece(board[move.from_index])) {
            can_castle_kingside_white = false;
            en_passant_index = -1;
            half_move_clock = 0;

            int queenside_rook_index = 0;
            board[move.from_index - 1] = board[queenside_rook_index];
            board[move.to_index] = board[move.from_index];
            board[move.from_index] = Piece::none;
            board[queenside_rook_index] = Piece::none;

            white_to_move = !white_to_move;
        } else {
            can_castle_kingside_white = false;
            en_passant_index = -1;
            half_move_clock = 0;
            full_move_number++;

            int queenside_rook_index = 56;
            board[move.from_index - 1] = board[queenside_rook_index];
            board[move.to_index] = board[move.from_index];
            board[move.from_index] = Piece::none;
            board[queenside_rook_index] = Piece::none;

            white_to_move = !white_to_move;
        }
    }

    void update_board_state_promotion_knight(const Move& move) {
        en_passant_index = -1;

        half_move_clock = 0;

        if (Piece::is_black_piece(board[move.from_index])) {
            full_move_number++;
        }

        int piece_color = Piece::get_piece_color(board[move.from_index]);
        board[move.to_index] = Piece::knight | piece_color;
        board[move.from_index] = Piece::none;

        white_to_move = !white_to_move;
    }

    void update_board_state_promotion_bishop(const Move& move) {
        en_passant_index = -1;

        half_move_clock = 0;

        if (Piece::is_black_piece(board[move.from_index])) {
            full_move_number++;
        }

        int piece_color = Piece::get_piece_color(board[move.from_index]);
        board[move.to_index] = Piece::bishop | piece_color;
        board[move.from_index] = Piece::none;

        white_to_move = !white_to_move;
    }

    void update_board_state_promotion_rook(const Move& move) {
        en_passant_index = -1;

        half_move_clock = 0;

        if (Piece::is_black_piece(board[move.from_index])) {
            full_move_number++;
        }

        int piece_color = Piece::get_piece_color(board[move.from_index]);
        board[move.to_index] = Piece::rook | piece_color;
        board[move.from_index] = Piece::none;

        white_to_move = !white_to_move;
    }

    void update_board_state_promotion_queen(const Move& move) {
        en_passant_index = -1;

        half_move_clock = 0;

        if (Piece::is_black_piece(board[move.from_index])) {
            full_move_number++;
        }

        int piece_color = Piece::get_piece_color(board[move.from_index]);
        board[move.to_index] = Piece::queen | piece_color;
        board[move.from_index] = Piece::none;

        white_to_move = !white_to_move;
    }
    
    void update_board_state(const Move& move) {
        if (move.move_type == MoveType::NORMAL) {
            update_board_state_normal(move);
        } else if (move.move_type == MoveType::EN_PASSANT) {
            update_board_state_en_passant(move);
        } else if (move.move_type == MoveType::CASTLING_KING_SIDE) {
            update_board_state_castling_king_side(move);
        } else if (move.move_type == MoveType::CASTLING_QUEEN_SIDE) {
            update_board_state_castling_queen_side(move);
        } else if (move.move_type == MoveType::PROMOTION_KNIGHT) {
            update_board_state_promotion_knight(move);
        } else if (move.move_type == MoveType::PROMOTION_BISHOP) {
            update_board_state_promotion_bishop(move);
        } else if (move.move_type == MoveType::PROMOTION_ROOK) {
            update_board_state_promotion_rook(move);
        } else if (move.move_type == MoveType::PROMOTION_QUEEN) {
            update_board_state_promotion_queen(move);
        }
        update_legal_moves();
    }

    bool is_legal_move(const Move& move) {
        for (int i = 0; i < legal_moves_count; i++) {
            if (legal_moves[i] == move) {
                return true;
            }
        }
        return false;
    }

    const int** get_legal_moves() {
        static int legal_moves_array[MAX_MOVES][3];
        for (int i = 0; i < MAX_MOVES; i++) {
            legal_moves_array[i][0] = legal_moves[i].from_index;
            legal_moves_array[i][1] = legal_moves[i].to_index;
            legal_moves_array[i][2] = static_cast<int>(legal_moves[i].move_type);
        }
        return reinterpret_cast<const int**>(legal_moves_array);
    }

    const int* get_next_best_move() {
        static int best_move_array[3] = {best_move.from_index, best_move.to_index, best_move.move_type};
        return best_move_array;
    }

private:
    int board[64];
    bool white_to_move;
    bool can_castle_kingside_white;
    bool can_castle_queenside_white;
    bool can_castle_kingside_black;
    bool can_castle_queenside_black;
    int en_passant_index;
    int half_move_clock;
    int full_move_number;

    Move best_move;
    Move legal_moves[MAX_MOVES];
    int legal_moves_count;

    void set_board_state(const std::string& fen) {
        std::vector<std::string> fen_parts;
        {
            int i = 0, j = 0;
            while (i < fen.length()) {
                if (fen[i] == ' ') {
                    fen_parts.push_back(fen.substr(j, i - j));
                    j = i + 1;
                }
                i++;
            }
            fen_parts.push_back(fen.substr(j, i - j));
        }
        {
            int i = 0, index = 56;
            while (i < fen_parts[0].length()) {
                if (fen_parts[0][i] == '/') {
                    i++;
                    continue;
                }
                if (isdigit(fen_parts[0][i])) {
                    for (int j = 0; j < fen_parts[0][i] - '0'; j++) {
                        board[index++] = Piece::none;
                    }
                } else {
                    board[index++] = Piece::get_piece_value_from_symbol(fen_parts[0][i]);
                }
                if (index % 8 == 0) {
                    index -= 16;
                }
                i++;
            }
        }
        {
            white_to_move = fen_parts[1] == "w";
            can_castle_kingside_white = fen_parts[2].find('K') != std::string::npos;
            can_castle_queenside_white = fen_parts[2].find('Q') != std::string::npos;
            can_castle_kingside_black = fen_parts[2].find('k') != std::string::npos;
            can_castle_queenside_black = fen_parts[2].find('q') != std::string::npos;
            en_passant_index = fen_parts[3] == "-" ? -1 : (fen_parts[3][0] - 'a') + (fen_parts[3][1] - '1') * 8;
            half_move_clock = std::stoi(fen_parts[4]);
            full_move_number = std::stoi(fen_parts[5]);
        }
        {
            best_move = Move();
            for (int i = 0; i < MAX_MOVES; i++) {
                legal_moves[i] = Move();
            }
            legal_moves_count = 0;
        }
        {
            update_legal_moves();
        }
    }

    void reset_legal_moves() {
        for (int i = 0; i < legal_moves_count; i++) {
            legal_moves[i].from_index = -1;
            legal_moves[i].to_index = -1;
            legal_moves[i].move_type = MoveType::NONE;
        }
        legal_moves_count = 0;
    }

    void add_move(const Move& move) {
        legal_moves[legal_moves_count++] = move;
    }

    void update_legal_move_pawn(int index) {
        if (Piece::is_white_piece(board[index])) {
            int rank = index / 8, file = index % 8;
            int starting_rank_index = 1;
            if (rank == starting_rank_index && Piece::is_empty(board[index + 8]) && Piece::is_empty(board[index + 16])) {
                add_move(Move(index, index + 16, MoveType::NORMAL));
            }
            if (rank < 6 && Piece::is_empty(board[index + 8])) {
                add_move(Move(index, index + 8, MoveType::NORMAL));
            }
            if (rank < 6 && file != 0 && Piece::is_black_piece(board[index + 7])) {
                add_move(Move(index, index + 7, MoveType::NORMAL));
            }
            if (rank < 6 && file != 7 && Piece::is_black_piece(board[index + 9])) {
                add_move(Move(index, index + 9, MoveType::NORMAL));
            }
            if (file != 0 && index + 7 == en_passant_index) {
                add_move(Move(index, index + 7, MoveType::EN_PASSANT)); 
            }
            if (file != 7 && index + 9 == en_passant_index) {
                add_move(Move(index, index + 9, MoveType::EN_PASSANT)); 
            }
            if (rank == 6 && Piece::is_empty(board[index + 8])) {
                add_move(Move(index, index + 8, MoveType::PROMOTION_KNIGHT));
                add_move(Move(index, index + 8, MoveType::PROMOTION_BISHOP));
                add_move(Move(index, index + 8, MoveType::PROMOTION_ROOK));
                add_move(Move(index, index + 8, MoveType::PROMOTION_QUEEN));
            }
            if (rank == 6 && file != 0 && Piece::is_black_piece(board[index + 7])) {
                add_move(Move(index, index + 7, MoveType::PROMOTION_KNIGHT));
                add_move(Move(index, index + 7, MoveType::PROMOTION_BISHOP));
                add_move(Move(index, index + 7, MoveType::PROMOTION_ROOK));
                add_move(Move(index, index + 7, MoveType::PROMOTION_QUEEN));
            }
            if (rank == 6 && file != 7 && Piece::is_black_piece(board[index + 9])) {
                add_move(Move(index, index + 9, MoveType::PROMOTION_KNIGHT));
                add_move(Move(index, index + 9, MoveType::PROMOTION_BISHOP));
                add_move(Move(index, index + 9, MoveType::PROMOTION_ROOK));
                add_move(Move(index, index + 9, MoveType::PROMOTION_QUEEN));
            }
        } else {
            int rank = index / 8, file = index / 8;
            int starting_rank_index = 6;
            if (rank == starting_rank_index && Piece::is_empty(board[index - 8]) && Piece::is_empty(board[index - 16])) {
                add_move(Move(index, index - 16, MoveType::NORMAL));
            }
            if (rank > 1 && Piece::is_empty(board[index - 8])) {
                add_move(Move(index, index - 8, MoveType::NORMAL));
            }
            if (rank > 1 && file != 7 && Piece::is_white_piece(board[index - 7])) {
                add_move(Move(index, index - 7, MoveType::NORMAL));
            }
            if (rank > 1 && file != 0 && Piece::is_white_piece(board[index - 9])) {
                add_move(Move(index, index - 9, MoveType::NORMAL));
            }
            if (file != 7 && index - 7 == en_passant_index) {
                add_move(Move(index, index - 7, MoveType::EN_PASSANT)); 
            }
            if (file != 0 && index - 9 == en_passant_index) {
                add_move(Move(index, index - 9, MoveType::EN_PASSANT)); 
            }
            if (rank == 1 && Piece::is_empty(board[index - 8])) {
                add_move(Move(index, index - 8, MoveType::PROMOTION_KNIGHT));
                add_move(Move(index, index - 8, MoveType::PROMOTION_BISHOP));
                add_move(Move(index, index - 8, MoveType::PROMOTION_ROOK));
                add_move(Move(index, index - 8, MoveType::PROMOTION_QUEEN));
            }
            if (rank == 1 && file != 7 && Piece::is_white_piece(board[index - 7])) {
                add_move(Move(index, index - 7, MoveType::PROMOTION_KNIGHT));
                add_move(Move(index, index - 7, MoveType::PROMOTION_BISHOP));
                add_move(Move(index, index - 7, MoveType::PROMOTION_ROOK));
                add_move(Move(index, index - 7, MoveType::PROMOTION_QUEEN));
            }
            if (rank == 1 && file != 0 && Piece::is_white_piece(board[index - 9])) {
                add_move(Move(index, index - 9, MoveType::PROMOTION_KNIGHT));
                add_move(Move(index, index - 9, MoveType::PROMOTION_BISHOP));
                add_move(Move(index, index - 9, MoveType::PROMOTION_ROOK));
                add_move(Move(index, index - 9, MoveType::PROMOTION_QUEEN));
            }
        }
    }

    void update_legal_move_knight(int index) {
        int offset_rank[8] = {-1, -1, 1, 1, -2, -2, 2, 2};
        int offset_file[8] = {-2, 2, -2, 2, -1, 1, -1, 1};

        int rank = index / 8, file = index % 8;
        for (int k = 0; k < 8; k++) {
            int new_rank = rank + offset_rank[k], new_file = file + offset_file[k];
            bool in_board = 0 <= new_rank && new_rank < 8 && 0 <= new_file && new_file < 8;
            if (in_board) {
                int new_index = new_rank * 8 + new_file;
                if (Piece::is_same_color_piece(board[index], board[new_index])) {
                    continue;
                }
                add_move(Move(index, new_index, MoveType::NORMAL));
            }
        }
    }

    void update_legal_move_bishop(int index) {
        int rank = index / 8, file = index % 8;
        for (int i = rank + 1, j = file + 1; i < 8 && j < 8; i++, j++) {
            int new_index = i * 8 + j;
            if (Piece::is_same_color_piece(board[index], board[new_index])) {
                break;
            }
            add_move(Move(index, new_index, MoveType::NORMAL));
            if (Piece::is_opposite_color_piece(board[index], board[new_index])) {
                break;
            }
        }
        for (int i = rank - 1, j = file + 1; i >= 0 && j < 8; i--, j++) {
            int new_index = i * 8 + j;
            if (Piece::is_same_color_piece(board[index], board[new_index])) {
                break;
            }
            add_move(Move(index, new_index, MoveType::NORMAL));
            if (Piece::is_opposite_color_piece(board[index], board[new_index])) {
                break;
            }
        }
        for (int i = rank + 1, j = file - 1; i < 8 && j >= 0; i++, j--) {
            int new_index = i * 8 + j;
            if (Piece::is_same_color_piece(board[index], board[new_index])) {
                break;
            }
            add_move(Move(index, new_index, MoveType::NORMAL));
            if (Piece::is_opposite_color_piece(board[index], board[new_index])) {
                break;
            }
        }
        for (int i = rank - 1, j = file - 1; i >= 0 && j >= 0; i--, j--) {
            int new_index = i * 8 + j;
            if (Piece::is_same_color_piece(board[index], board[new_index])) {
                break;
            }
            add_move(Move(index, new_index, MoveType::NORMAL));
            if (Piece::is_opposite_color_piece(board[index], board[new_index])) {
                break;
            }
        }
    }

    void update_legal_move_rook(int index) {
        int rank = index / 8, file = index % 8;
        for (int i = rank + 1; i < 8; i++) {
            int new_index = i * 8 + file;
            if (Piece::is_same_color_piece(board[index], board[new_index])) {
                break;
            }
            add_move(Move(index, new_index, MoveType::NORMAL));
            if (Piece::is_opposite_color_piece(board[index], board[new_index])) {
                break;
            }
        }
        for (int i = rank - 1; i >= 0; i--) {
            int new_index = i * 8 + file;
            if (Piece::is_same_color_piece(board[index], board[new_index])) {
                break;
            }
            add_move(Move(index, new_index, MoveType::NORMAL));
            if (Piece::is_opposite_color_piece(board[index], board[new_index])) {
                break;
            }
        }
        for (int i = file + 1; i < 8; i++) {
            int new_index = rank * 8 + i;
            if (Piece::is_same_color_piece(board[index], board[new_index])) {
                break;
            }
            add_move(Move(index, new_index, MoveType::NORMAL));
            if (Piece::is_opposite_color_piece(board[index], board[new_index])) {
                break;
            }
        }
        for (int i = file - 1; i >= 0; i--) {
            int new_index = rank * 8 + i;
            if (Piece::is_same_color_piece(board[index], board[new_index])) {
                break;
            }
            add_move(Move(index, new_index, MoveType::NORMAL));
            if (Piece::is_opposite_color_piece(board[index], board[new_index])) {
                break;
            }
        }
    }

    void update_legal_move_queen(int index) {
        update_legal_move_bishop(index);
        update_legal_move_rook(index);
    }

    void update_legal_move_king(int index) {
        int offset_rank[8] = {0, 0, -1, 1, -1, 1, -1, 1};
        int offset_file[8] = {-1, 1, 0, 0, -1, 1, 1, -1};

        int rank = index / 8, file = index % 8;
        for (int i = 0; i < 8; i++) {
            int new_rank = rank + offset_rank[i], new_file = file + offset_file[i];
            bool in_board = 0 <= new_rank && new_rank < 8 && 0 <= new_file && new_file < 8;
            if (in_board) {
                int new_index = new_rank * 8 + new_file;
                if (Piece::is_same_color_piece(board[index], board[new_index])) {
                    continue;
                }
                add_move(Move(index, new_index, MoveType::NORMAL));
            }
        }

        if (Piece::is_white_piece(board[index])) {
            if (can_castle_kingside_white) {
                int kingside_rook_index = 7;
                bool is_castling_blocked = false;
                for (int new_index = index + 1; new_index < kingside_rook_index; new_index++) {
                    if (!Piece::is_empty(board[new_index])) {
                        is_castling_blocked = true;
                        break;
                    }
                }
                if (!is_castling_blocked) {
                    add_move(Move(index, index + 2, MoveType::CASTLING_KING_SIDE));
                }
            }
            if (can_castle_queenside_white) {
                int queenside_rook_index = 0;
                bool is_castling_blocked = false;
                for (int new_index = index - 1; new_index > queenside_rook_index; new_index--) {
                    if (!Piece::is_empty(board[new_index])) {
                        is_castling_blocked = true;
                        break;
                    }
                }
                if (!is_castling_blocked) {
                    add_move(Move(index, index - 2, MoveType::CASTLING_QUEEN_SIDE));
                }
            }
        } else {
            if (can_castle_kingside_black) {
                int kingside_rook_index = 63;
                bool is_castling_blocked = false;
                for (int new_index = index + 1; new_index < kingside_rook_index; new_index++) {
                    if (!Piece::is_empty(board[new_index])) {
                        is_castling_blocked = true;
                        break;
                    }
                }
                if (!is_castling_blocked) {
                    add_move(Move(index, index + 2, MoveType::CASTLING_KING_SIDE));
                }
            }
            if (can_castle_queenside_black) {
                int queenside_rook_index = 56;
                bool is_castling_blocked = false;
                for (int new_index = index - 1; new_index > queenside_rook_index; new_index--) {
                    if (!Piece::is_empty(board[new_index])) {
                        is_castling_blocked = true;
                        break;
                    }
                }
                if (!is_castling_blocked) {
                    add_move(Move(index, index - 2, MoveType::CASTLING_QUEEN_SIDE));
                }
            }
        }
    }

    void update_legal_moves() {
        reset_legal_moves();
        for (int i = 0; i < 64; i++) {
            if (
                Piece::is_white_piece(board[i]) && !white_to_move ||
                Piece::is_black_piece(board[i]) && white_to_move
            ) {
                continue;
            }
            if (Piece::is_piece_type(board[i], Piece::pawn)) {
                update_legal_move_pawn(i);
            } if (Piece::is_piece_type(board[i], Piece::knight)) {
                update_legal_move_knight(i);
            } else if (Piece::is_piece_type(board[i], Piece::bishop)) {
                update_legal_move_bishop(i);
            } else if (Piece::is_piece_type(board[i], Piece::rook)) {
                update_legal_move_rook(i);
            } else if (Piece::is_piece_type(board[i], Piece::queen)) {
                update_legal_move_queen(i);
            } else if (Piece::is_piece_type(board[i], Piece::king)) {
                update_legal_move_king(i);
            }
        }
    }
};

#endif
// optimizations:
/*
- store indices that have pieces
- checkk optimizaions
*/