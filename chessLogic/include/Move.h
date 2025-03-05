#ifndef MOVE_H
#define MOVE_H

const int MAX_MOVES = 218;

enum MoveType {
    NONE = -1,
    NORMAL = 0,
    EN_PASSANT = 1,
    CASTLING_KING_SIDE = 2,
    CASTLING_QUEEN_SIDE = 3,
    PROMOTION_KNIGHT = 4,
    PROMOTION_BISHOP = 5,
    PROMOTION_ROOK = 6,
    PROMOTION_QUEEN = 7
};

class Move {
public:
    int from_index;
    int to_index;
    MoveType move_type;

    Move() : from_index(-1), to_index(-1), move_type(MoveType::NONE) {}
    Move(int from, int to, const MoveType& type) : from_index(from), to_index(to), move_type(type) {}

    bool operator==(const Move& other) const {
        return from_index == other.from_index && 
               to_index == other.to_index &&
               move_type == other.move_type;
    }
};

#endif