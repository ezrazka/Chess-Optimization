"use strict";

const MAXIMUM_MOVES_OF_BOARD = 218;

class ChessBoard {
    constructor() {
        this.initBoard = (...args) => {
            return Module.cwrap("init_board", null, [])(...args);
        }
        this.updateBoardState = (...args) => {
            return Module.cwrap("update_board_state", null, ["number", "number", "number"])(...args);
        }
        this.getBoardState = (...args) => {
            const getBoardStatePtr = Module.cwrap("get_board_state", "number", []);
            return UTF8ToString(getBoardStatePtr(...args));
        }
        this.isLegalMove = (...args) => {
            return Module.cwrap("is_legal_move", "boolean", ["number", "number", "number"])(...args);
        }
        this.getLegalMoves = (...args) => {
            const getLegalMovesPtr = Module.cwrap("get_legal_moves", "number", []);
            const legalMovesPtr = getLegalMovesPtr(...args);

            const legalMovesArray = new Array(...(new Int32Array(Module.HEAP32.buffer, legalMovesPtr, MAXIMUM_MOVES_OF_BOARD * 3)));

            const legalMoves2DArray = new Array();
            for (let i = 0; i < MAXIMUM_MOVES_OF_BOARD; i++) {
                const moveArray = legalMovesArray.slice(i * 3, i * 3 + 3);
                if (moveArray[2] === -1) {
                    break;
                }
                legalMoves2DArray.push(moveArray);
            }
        
            return legalMoves2DArray;
        }
        this.getNextBestMove = (...args) => {
            const getNextBestMovePtr = Module.cwrap("get_next_best_move", "number", []);
            return new Array(...(new Int32Array(Module.HEAP32.buffer, getNextBestMovePtr(...args), 3)));
        }

        this.initBoard();
    }
}