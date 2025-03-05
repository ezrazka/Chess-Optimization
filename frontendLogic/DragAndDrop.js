"use strict";

class DragAndDrop {
    constructor(boardDisplay) {
        this.boardDisplay = boardDisplay;
        this.pieceIcon = null;
        this.isPromotionState = false;
    }

    removeAllEventListeners(element) {
        const newElement = element.cloneNode(true);
        element.parentNode.replaceChild(newElement, element);
        return newElement;
    }

    resetPieceIconTransformations() {
        this.pieceIcon.style.transform = "";
        this.pieceIcon.classList.remove("dragging");
    }

    movePieceIconTowardCursor(event) {
        const board = document.getElementById("board");
        const boardRect = board.getBoundingClientRect();

        this.pieceIcon.style.transform = "";

        const pieceIconRect = this.pieceIcon.getBoundingClientRect();

        const pieceIconCenterX1 = pieceIconRect.left + pieceIconRect.width / 2;
        const pieceIconCenterY1 = pieceIconRect.top + pieceIconRect.height / 2;
        const pieceIconCenterX2 = Math.max(Math.min(event.clientX, boardRect.right), boardRect.left);
        const pieceIconCenterY2 = Math.max(Math.min(event.clientY, boardRect.bottom), boardRect.top);

        const pieceIconCenterDX = pieceIconCenterX2 - pieceIconCenterX1;
        const pieceIconCenterDY = pieceIconCenterY2 - pieceIconCenterY1;

        this.pieceIcon.style.transform = `translate(${pieceIconCenterDX}px, ${pieceIconCenterDY}px)`;
    }

    addHoveredSquareOutline(event) {
        const hoveredSquare = this.getHoveredSquare(event);
        if (hoveredSquare) {
            const hoveredSquareOutline = document.createElement("div");
            hoveredSquareOutline.classList.add("square-outline");
            this.boardDisplay.insertSquareChild(hoveredSquare, hoveredSquareOutline);
        }
    }

    removeHoveredSquareOutline() {
        document.querySelector(".square-outline")?.remove();
    }

    getHoveredSquare(event) {
        const hoveredElements = document.elementsFromPoint(event.clientX, event.clientY);
        return hoveredElements.find((element) => element.matches(".square"));
    }

    displayPromotionMenu(event) {
        const pieceIcon = this.pieceIcon;
        
        const initialSquare = pieceIcon.parentElement;
        const hoveredSquare = this.getHoveredSquare(event);

        const fromIndex = parseInt(initialSquare.dataset.index);
        const toIndex = parseInt(hoveredSquare.dataset.index);

        this.boardDisplay.insertSquareChild(hoveredSquare, pieceIcon);
        this.resetPieceIconTransformations();
        this.removeHoveredSquareOutline();

        const isWhiteToMove = fromIndex < toIndex;
        const isPromotionTop = !this.boardDisplay.isWhitePerspective ^ isWhiteToMove;

        const promotionMenu = document.createElement("div");
        const promotionMenuClass = isPromotionTop ? "promotion-menu-top" : "promotion-menu-bottom";
        promotionMenu.classList.add(promotionMenuClass);

        const promotionPieceSymbols = isWhiteToMove ? ["Q", "N", "R", "B"] : ["q", "n", "r", "b"];
        const promotionMoveTypes = [7, 4, 6, 5];

        promotionPieceSymbols.forEach((promotionPieceSymbol, index) => {
            const promotionSquare = document.createElement("div");
            promotionSquare.classList.add("promotion-square");
            const promotionPieceIcon = document.createElement("div");
            const promotionPieceIconImage = this.boardDisplay.getPieceImagePathFromSymbol(promotionPieceSymbol);
            promotionPieceIcon.classList.add("promotion-piece-icon");
            promotionPieceIcon.style.backgroundImage = `url(${promotionPieceIconImage})`;
            
            promotionSquare.appendChild(promotionPieceIcon);
            promotionMenu.appendChild(promotionSquare);

            promotionSquare.addEventListener("mousedown", () => {
                const promotionMoveType = promotionMoveTypes[index];
                document.querySelectorAll(".square-highlight").forEach((highlight) => highlight.remove());

                const squareHighlight = document.createElement("div");
                squareHighlight.classList.add("square-highlight");
                this.boardDisplay.insertSquareChild(initialSquare, squareHighlight);
                this.boardDisplay.insertSquareChild(hoveredSquare, squareHighlight.cloneNode(true));

                this.boardDisplay.chessBoard.updateBoardState(fromIndex, toIndex, promotionMoveType);
                this.boardDisplay.updateBoardData();
                console.log(this.boardDisplay.fen);

                hoveredSquare.querySelectorAll(".piece-icon").forEach((pieceIcon) => {
                   pieceIcon.remove();
                });

                const promotionPieceIcon = document.createElement("div");
                const promotionPieceIconImage = this.boardDisplay.getPieceImagePathFromSymbol(promotionPieceSymbol);
                promotionPieceIcon.classList.add("piece-icon");
                promotionPieceIcon.style.backgroundImage = `url(${promotionPieceIconImage})`;

                this.boardDisplay.insertSquareChild(hoveredSquare, promotionPieceIcon);

                const promotionCancelButton = document.querySelector(".promotion-cancel-button");
                document.querySelectorAll(".promotion-square").forEach((promotionSquare) => {
                    this.removeAllEventListeners(promotionSquare);
                });
                this.removeAllEventListeners(promotionCancelButton);
                promotionMenu.remove();
                this.isPromotionState = false;
            });
        });
        
        const promotionCancelButton = document.createElement("div");
        promotionCancelButton.classList.add("promotion-cancel-button");
        const promotionCancelSymbol = document.createElement("div");
        promotionCancelSymbol.classList.add("promotion-cancel-symbol");
        
        promotionCancelButton.appendChild(promotionCancelSymbol);
        promotionMenu.appendChild(promotionCancelButton);
        
        promotionCancelButton.addEventListener("mousedown", () => {
            this.boardDisplay.insertSquareChild(initialSquare, pieceIcon);
            
            document.querySelectorAll(".promotion-square").forEach((promotionSquare) => {
                this.removeAllEventListeners(promotionSquare);
            });
            this.removeAllEventListeners(promotionCancelButton);
            promotionMenu.remove();
            this.isPromotionState = false;
        });
        
        const board = document.getElementById("board");
        board.appendChild(promotionMenu);

        const hoveredSquareRect = hoveredSquare.getBoundingClientRect();
        const promotionMenuRect = promotionMenu.getBoundingClientRect();

        const hoveredSquareCenterX = hoveredSquareRect.left + hoveredSquareRect.width / 2;
        const promotionMenuCenterX = promotionMenuRect.left + promotionMenuRect.width / 2;

        const promotionMenuDX = hoveredSquareCenterX - promotionMenuCenterX;
        const promotionMenuDY = isPromotionTop ? 0 : hoveredSquareRect.width * 7 / 2;

        promotionMenu.style.transform = `translate(${promotionMenuDX}px, ${promotionMenuDY}px)`;
    }

    updateSquareHighlights(event) {
        const initialSquare = this.pieceIcon.parentElement;
        const hoveredSquare = this.getHoveredSquare(event);
        if (hoveredSquare == null) {
            return;
        }

        const fromIndex = parseInt(initialSquare.dataset.index);
        const toIndex = parseInt(hoveredSquare.dataset.index);

        let moveType = -1;
        for (let i = 0; i < 8; i++) {
            if (this.boardDisplay.chessBoard.isLegalMove(fromIndex, toIndex, i)) {
                moveType = i;
                break;
            }
        }

        if (moveType == -1) {
            return;
        } else if (moveType === 0) {
            hoveredSquare.querySelector(".piece-icon")?.remove();
        } else if (moveType === 1) {
            const enPassantPawnSquareIndex = fromIndex < toIndex ? toIndex - 8 : toIndex + 8;
            const enPassantPawnSquare = this.boardDisplay.getSquareElementFromIndex(enPassantPawnSquareIndex);
            enPassantPawnSquare.querySelector(".piece-icon").remove();
        } else if (moveType === 2) {
            const kingSideRookIndex = ~~(fromIndex / 8) * 8 + 7;
            const kingSideRookSquare = this.boardDisplay.getSquareElementFromIndex(kingSideRookIndex);
            const kingSideRookToSquare = this.boardDisplay.getSquareElementFromIndex(fromIndex + 1);
            const kingSideRookPieceIcon = kingSideRookSquare.querySelector(".piece-icon");

            this.boardDisplay.insertSquareChild(kingSideRookToSquare, kingSideRookPieceIcon);
        } else if (moveType === 3) {
            const queenSideRookIndex = ~~(fromIndex / 8) * 8;
            const queenSideRookSquare = this.boardDisplay.getSquareElementFromIndex(queenSideRookIndex);
            const queenSideRookToSquare = this.boardDisplay.getSquareElementFromIndex(fromIndex - 1);
            const queenSideRookPieceIcon = queenSideRookSquare.querySelector(".piece-icon");

            this.boardDisplay.insertSquareChild(queenSideRookToSquare, queenSideRookPieceIcon);
        } else {
            this.displayPromotionMenu(event);
            this.isPromotionState = true;
            return;
        }
        
        this.boardDisplay.insertSquareChild(hoveredSquare, this.pieceIcon);
        this.resetPieceIconTransformations();
        this.removeHoveredSquareOutline();
        
        document.querySelectorAll(".square-highlight").forEach((highlight) => highlight.remove());
        
        const squareHighlight = document.createElement("div");
        squareHighlight.classList.add("square-highlight");
        this.boardDisplay.insertSquareChild(initialSquare, squareHighlight);
        this.boardDisplay.insertSquareChild(hoveredSquare, squareHighlight.cloneNode(true));

        this.boardDisplay.chessBoard.updateBoardState(fromIndex, toIndex, moveType);
        this.boardDisplay.updateBoardData();
        console.log(this.boardDisplay.fen);
    }

    updateLegalSquares(event) {
        const square = this.pieceIcon.parentElement;
        const squareIndex = parseInt(square.dataset.index);

        const legalMoves = this.boardDisplay.chessBoard.getLegalMoves();
        const squareLegalMoves = legalMoves.filter((move) => move[0] == squareIndex);
        squareLegalMoves.forEach((move) => {
            const legalSquare = this.boardDisplay.getSquareElementFromIndex(move[1]);
            
            const hasSquareLegal = Boolean(legalSquare.querySelector(".square-legal"));
            if (hasSquareLegal) {
                return;
            }

            const squareLegal = document.createElement("div");
            squareLegal.classList.add("square-legal");
            this.boardDisplay.insertSquareChild(legalSquare, squareLegal);
            
            const squareHighlights = squareLegal.parentElement.querySelectorAll(".square-highlight");
            squareHighlights.forEach((squareHighlight) => {
                squareHighlight.style.display = "none";
            });
        });
    }
    
    removeLegalSquares() {
        document.querySelectorAll(".square-legal").forEach((squareLegal) => {
            const squareHighlights = squareLegal.parentElement.querySelectorAll(".square-highlight");
            squareHighlights.forEach((squareHighlight) => {
                squareHighlight.style.display = "";
            });
            
            squareLegal.remove();
        });
    }

    addDragAndDropEventListeners() {
        document.addEventListener("mousedown", (event) => {
            event.preventDefault();

            if (this.isPromotionState) {
                return;
            }

            const hoveredSquare = this.getHoveredSquare(event);
            const hoveredPieceIcon = hoveredSquare?.querySelector(".piece-icon");
            if (hoveredPieceIcon == null) {
                return;
            }

            this.pieceIcon = hoveredPieceIcon;
            this.pieceIcon.classList.add("dragging");
            this.updateLegalSquares();
            this.addHoveredSquareOutline(event);
        });
        document.addEventListener("mousemove", this.onMouseMove.bind(this));
        document.addEventListener("mouseup", this.onMouseUp.bind(this));
    }

    onMouseMove(event) {
        if (this.pieceIcon == null) {
            return;
        }
        this.movePieceIconTowardCursor(event);
        this.removeHoveredSquareOutline();
        this.addHoveredSquareOutline(event);
    }
    
    onMouseUp(event) {
        if (this.pieceIcon == null) {
            return;
        }
        this.updateSquareHighlights(event);
        this.resetPieceIconTransformations();
        this.removeHoveredSquareOutline();
        this.removeLegalSquares();
        this.pieceIcon = null;
    }
}