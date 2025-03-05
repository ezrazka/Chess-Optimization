#ifndef PIECE_H
#define PIECE_H

class Piece {
public:
    static const int none = 0;
    static const int king = 1;
    static const int queen = 2;
    static const int rook = 3;
    static const int bishop = 4;
    static const int knight = 5;
    static const int pawn = 6;

    static const int white = 8;
    static const int black = 16;

    static int get_piece_value_from_symbol(char piece_symbol) {
        switch (piece_symbol) {
            case 'K': return king | white;
            case 'Q': return queen | white;
            case 'R': return rook | white;
            case 'B': return bishop | white;
            case 'N': return knight | white;
            case 'P': return pawn | white;
            case 'k': return king | black;
            case 'q': return queen | black;
            case 'r': return rook | black;
            case 'b': return bishop | black;
            case 'n': return knight | black;
            case 'p': return pawn | black;
            default: return none;
        }
    }
    
    static char get_piece_symbol_from_value(int piece_value) {
        switch (piece_value) {
            case king | white: return 'K';
            case queen | white: return 'Q';
            case rook | white: return 'R';
            case bishop | white: return 'B';
            case knight | white: return 'N';
            case pawn | white: return 'P';
            case king | black: return 'k';
            case queen | black: return 'q';
            case rook | black: return 'r';
            case bishop | black: return 'b';
            case knight | black: return 'n';
            case pawn | black: return 'p';
            default: return ' ';
        }
    }

    static int get_piece_color(int piece_value) {
        return piece_value & (white | black);
    }

    static int get_piece_type(int piece_value) {
        return piece_value & (white - 1);
    }

    static bool is_empty(int piece_value) {
        return piece_value == none;
    }

    static bool is_white_piece(int piece_value) {
        return piece_value & white;
    }

    static bool is_black_piece(int piece_value) {
        return piece_value & black;
    }
    
    static bool is_same_color_piece(int piece_value1, int piece_value2) {
        return (piece_value1 & piece_value2 & white) ||
               (piece_value1 & piece_value2 & black);
    }

    static bool is_opposite_color_piece(int piece_value1, int piece_value2) {
        return ((piece_value1 ^ piece_value2) & white) &&
               ((piece_value1 ^ piece_value2) & black);
    }

    static bool is_piece_type(int piece_value, int piece_type) {
        return get_piece_type(piece_value) == piece_type;
    }
};

#endif