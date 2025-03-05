"use strict";

class BoardDisplay {
    constructor(chessBoard) {
        this.chessBoard = chessBoard;

        this.updateBoardData();

        this.showNotation = true;
        this.isWhitePerspective = true;

        this.dragAndDrop = new DragAndDrop(this);
        
        this.initializeBoard();
        this.dragAndDrop.addDragAndDropEventListeners();
    }

    updateBoardData() {
        this.fen = this.chessBoard.getBoardState();
        [
            this.piecePlacement,
            this.activeColor,
            this.castlingRights,
            this.enPassantSquare,
            this.halfMoveClock,
            this.fullMoveNumber
        ] = this.fen.split(" ");

        this.boardData = this.getBoardData();
    }

    getBoardData() {
        const rankStrings = this.piecePlacement.split("/");

        const boardData = Array.from(Array(8), () => new Array(8));

        boardData.forEach((rank, rankIndex) => {
            let fileIndex = 0;
            Array.from(rankStrings[rankIndex]).forEach((char) => {
                const isDigit = char.match(/\d/);

                if (isDigit) {
                    for (let i = 0; i < parseInt(char); i++) {
                        rank[fileIndex] = " ";
                        fileIndex++;
                    }
                } else {
                    rank[fileIndex] = char;
                    fileIndex++;
                }
            });
        });

        return boardData;
    }

    getPieceImagePathFromSymbol(pieceLetter) {
        switch (pieceLetter) {
            case "K": return "../assets/images/white-king.png";
            case "Q": return "../assets/images/white-queen.png";
            case "R": return "../assets/images/white-rook.png";
            case "N": return "../assets/images/white-knight.png";
            case "B": return "../assets/images/white-bishop.png";
            case "P": return "../assets/images/white-pawn.png";
            case "k": return "../assets/images/black-king.png";
            case "q": return "../assets/images/black-queen.png";
            case "r": return "../assets/images/black-rook.png";
            case "n": return "../assets/images/black-knight.png";
            case "b": return "../assets/images/black-bishop.png";
            case "p": return "../assets/images/black-pawn.png";
        }
    }

    getSquareElementFromIndex(index) {
        return document.querySelector(`.square[data-index="${index}"]`);
    }

    reverseChildrenOfElement(parent) {
        const children = Array.from(parent.children);
        for (let i = children.length - 1; i >= 0; i--) {
            parent.appendChild(children[i]);
        }
    }

    reverseChildrenOfChildren(elements) {
        for (let i = 0; i < elements.length / 2; i++) {
            const elementFirst = elements[i];
            const elementLast = elements[elements.length - 1 - i];

            const elementFirstChildren = Array.from(elementFirst.children);
            const elementLastChildren = Array.from(elementLast.children);

            [...elementFirstChildren, ...elementLastChildren].forEach((elementChild) => {
                elementChild.remove();
            });

            elementFirstChildren.forEach((elementFirstChild) => {
                elementLast.appendChild(elementFirstChild);
            });

            elementLastChildren.forEach((elementLastChild) => {
                elementFirst.appendChild(elementLastChild);
            });
        }
    }

    insertSquareChild(square, squareChild) {
        square.appendChild(squareChild);

        const squareChildrenSelectors = [
            ".notation-letter",
            ".notation-number",
            ".square-highlight",
            ".square-outline",
            ".piece-icon"
        ];

        squareChildrenSelectors.forEach((squareChildrenSelector) => {
            const squareChildren = square.querySelectorAll(squareChildrenSelector);

            squareChildren.forEach((squareChild) => {
                square.appendChild(squareChild);
            });
        });
    }

    loadPieces() {
        const ranks = document.querySelectorAll(".rank");

        ranks.forEach((rank, rankIndex) => {
            const squares = rank.querySelectorAll(".square");

            squares.forEach((square, fileIndex) => {
                const pieceSymbol = this.boardData[rankIndex][fileIndex];

                if (pieceSymbol === " ") {
                    return;
                }

                const pieceIcon = document.createElement("div");
                const pieceIconImage = this.getPieceImagePathFromSymbol(pieceSymbol);
                pieceIcon.classList.add("piece-icon");
                pieceIcon.style.backgroundImage = `url(${pieceIconImage})`;

                this.insertSquareChild(square, pieceIcon);
            });
        });
    }

    loadNotation() {
        const notationLetterSquares = document.querySelectorAll(".rank:last-child .square");
        const notationNumberSquares = document.querySelectorAll(".square:first-child");

        notationLetterSquares.forEach((notationLetterSquare, index) => {
            const notationLetter = document.createElement("span");
            notationLetter.textContent = String.fromCharCode(this.isWhitePerspective ? 97 + index : 104 - index);
            notationLetter.classList.add("notation-letter");

            this.insertSquareChild(notationLetterSquare, notationLetter);
        });

        notationNumberSquares.forEach((notationNumberSquare, index) => {
            const notationNumber = document.createElement("span");
            notationNumber.textContent = this.isWhitePerspective ? 8 - index : index + 1;
            notationNumber.classList.add("notation-number");

            this.insertSquareChild(notationNumberSquare, notationNumber);
        });
    }

    flipBoard() {
        const ranks = document.querySelectorAll('.rank');
        this.reverseChildrenOfChildren(ranks);

        ranks.forEach((rank) => {
            this.reverseChildrenOfElement(rank);
        });

        if (this.showNotation) {
            const notationLetters = document.querySelectorAll(".notation-letter");
            const notationNumbers = document.querySelectorAll(".notation-number");

            [...notationLetters, ...notationNumbers].forEach((notationCharacter) => {
                notationCharacter.remove();
            });

            this.isWhitePerspective = !this.isWhitePerspective;
            this.loadNotation();
        }
    }

    initializeBoard() {
        this.loadPieces();

        if (this.showNotation) {
            this.loadNotation();
        }
        
        // this.flipBoard();
    }
}