"use strict";

// const FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
// const FEN = "r1b1kb1r/p4ppp/nq2p3/1p1pP3/1PpP1PP1/P1N2n2/2P1NB1P/R2QKB1R w KQkq - 3 14";
// const FEN = "r1b1k2r/pppn3p/1b3n2/5p2/1PB4P/3Kq3/P1P3P1/RN1Q2NR w kq - 0 14";

document.addEventListener("DOMContentLoaded", () => {
    Module.onRuntimeInitialized = () => {
        const chessBoard = new ChessBoard();
        
        const fromSquare = 12; // Example square index
        const toSquare = 28; // Example square index
        const moveType = 0; // Example move type

        // Test updateBoardState
        // chessBoard.updateBoardState(fromSquare, toSquare, moveType);
        // const boardState = chessBoard.getBoardState();
        // console.log("Board State:", boardState);

        // Test islegalMove
        const legalMove = chessBoard.isLegalMove(fromSquare, toSquare, moveType);
        console.log("Is Legal Move:", legalMove);

        // Test getlegalMoves
        const legalMovesPtr = chessBoard.getLegalMoves();
        const legalMoves = chessBoard.getLegalMoves();
        console.log("Legal Moves:", legalMoves);

        // Test getNextBestMove
        const nextBestMove = chessBoard.getNextBestMove();
        console.log("Next Best Move:", nextBestMove);

        const boardDisplay = new BoardDisplay(chessBoard);
    };
});