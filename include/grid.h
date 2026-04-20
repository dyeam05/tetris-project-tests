#include "tetromino.h"

#ifndef GRID_H
#define GRID_H

class Grid { 
    public:

    //Default Constructor. Creates the grid and sets all matrix values to empty '0'.
    Grid() { 
        for(int i = 0; i < 20; i++){
            for(int j = 0; j < 10; j++) {
                grid[i][j] = '0';
            }
        }
        numSquares = 0;
        pieces = nullptr;
    }

    int numSquares;
    char grid[20][10];
    Tetromino* pieces;

    void addTetromino(Tetromino* piece) {
        //std::cout << "piece orientation: " << piece->orientation << std::endl; 
        //std::cout << "piece grid coords: TopY = " << piece->tYpos << ", BottomY = " << piece->bYpos << ", LeftX = " << piece->lXpos << ", RightX = " << piece->rXpos << std::endl;
        //Builds piece from Top-Left to Bottom-Right
        int pieceX = 0;
        int pieceY = 3 - (piece->bYpos - piece->tYpos);
        for(int i = piece->tYpos; i <= piece->bYpos; i++) {
            for(int j = piece->lXpos; j <= piece->rXpos; j++) {
                grid[i][j] = piece->squares[pieceY][pieceX];
                //std::cout << "piece grid coords: (" << pieceY << ", " << pieceX << ")" << std::endl; 
                pieceX++;
            }
            pieceY++;
            pieceX = 0;
        }
    }
    void removeTetromino(Tetromino* piece) {
        int k = 3 - (piece->bYpos - piece->tYpos);
        int l = 0;
        for(int i = piece->tYpos; i <= piece->bYpos; i++) {
            for(int j = piece->lXpos; j <= piece->rXpos; j++) {
                if(grid[i][j] != '0' && piece->squares[k][l] != '0') grid[i][j] = '0';
                l++;
            }
            k++;
            l = 0;
        }

    }
    //potentially tweak logic to move pieces after rotate to avoid overlap
    void rotateTetromino(Tetromino* piece) {
        removeTetromino(piece);
        //if(leftColCheck(piece)) movePiece(piece, 'r');
        //if(rightColCheck(piece)) movePiece(piece, 'l');
        piece->rotate();
        addTetromino(piece);
        while(piece->tYpos < 0) movePiece(piece, 'd');
        while(piece->rXpos > 9) movePiece(piece, 'l');
            //printGrid();
    }

    //Moves pieces around the grid. direction char is passed as arg to determine direction of movement for piece specified in first arg. 
    void movePiece(Tetromino* piece, char direction) {
        int pieceX = 0;
        int pieceY = 3;
        switch (direction) {
            case 'l':
                if(!leftColCheck(piece)){
                    for(int i = piece->bYpos; i >= piece->tYpos; i--) {
                        for(int j = piece->lXpos; j <= piece->rXpos; j++) {
                            if(grid[i][j] != '0' && piece->squares[pieceY][pieceX] != '0') {
                                grid[i][j-1] = piece->color;
                                grid[i][j] = '0';
                            }
                            pieceX++;
                        }
                        pieceY--;
                        pieceX = 0;
                    }
                    piece->lXpos--;
                    piece->rXpos--;
                }
                break;

            case 'r':
                pieceX = piece->rXpos - piece->lXpos;
                if(!rightColCheck(piece)) {
                    for(int i = piece->bYpos; i >= piece->tYpos; i--) {
                        for(int j = piece->rXpos; j >= piece->lXpos; j--) {
                            if(grid[i][j] != '0' && piece->squares[pieceY][pieceX] != '0') {
                                grid[i][j+1] = piece->color;
                                grid[i][j] = '0';
                            }
                            pieceX--;
                        }
                        pieceY--;
                        pieceX = piece->rXpos - piece->lXpos;
                    }
                    piece->lXpos++;
                    piece->rXpos++;
                }
                break;

            case 'd':
                if(!bottomColCheck(piece)) {
                    for(int i = piece->bYpos; i >= piece->tYpos; i--) {
                        for(int j = piece->lXpos; j <= piece->rXpos; j++) {
                            if(grid[i][j] != '0' && piece->squares[pieceY][pieceX] != '0') {
                                grid[i+1][j] = piece->color;
                                grid[i][j] = '0';
                            }
                            pieceX++;
                        }
                        pieceY--;
                        pieceX = 0;
                    }
                    piece->tYpos++;
                    piece->bYpos++;
                }
            default:
                break;
        }
    }

    void addGhost(Tetromino* piece, Tetromino* ghostPiece) {
        ghostPiece->initGhost(piece);
        hardDrop(ghostPiece);
    }

    void printGrid() {
        for(int i = 0; i < 20; i++){
            for(int j = 0; j < 10; j++) {
                std::cout << grid[i][j];
            }
            std::cout << std::endl;
        }
    }

    //takes condition from movePiece down function and returns a boolean if the piece can no longer be moved down
    bool finishedFalling(Tetromino* piece) {
        if(piece->bYpos == 19) {
            return true;
        }
        if (bottomColCheck(piece)) {
                return true;
        }
            return false;
    }
    
    //checks if space underneath current piece is occupied by previous piece
    bool bottomColCheck(Tetromino* piece) {
        bool check = false;
        if(piece->bYpos == 19) return true;
        int k = 3;
        int l = 0;
        for(int i = piece->lXpos; i <= piece->rXpos; i++) {
            k = 3;
            for(int j = piece->bYpos; j >= piece->tYpos; j--) {
                if(piece->squares[k][l] != '0' && (k==3 || piece->squares[k+1][l] == '0')) {
                    //std::cout << "Piece grid spot [" << k << "] [" << l << "] contains a filled square" << std::endl;
                    if(grid[j][i] != '0' && grid[j+1][i] != '0' && grid[j+1][i] != 'G') {
                        //std::cout << "Grid space [" << j+1 << "] [" << i << "] also contains a filled square";
                        check = true;
                        break;
                    }
                }
                //else std::cout << "Piece grid spot [" << k << "] [" << l << "] does not contain a filled square" << std::endl;
                k--;
            }
            l++;
            if(check) break;
        }

        return check;
    }
    //weird crash caused by this. idk what the problem is yet
    bool topColCheck(Tetromino* piece) {
        bool check = false;
        if(piece->tYpos == 0) return true;
        for(int i = piece->lXpos; i <= piece->rXpos; i++) {
            if(grid[piece->tYpos-1][i] != '0' && grid[piece->tYpos][i] != '0') {
                check = true;
                break;
            }
        }
        return check;
    }

    bool leftColCheck(Tetromino* piece) {
        bool check = false;
        if(piece->lXpos == 0) return true;
        for(int i = piece->tYpos; i <= piece->bYpos; i++) {
            if(grid[i][piece->lXpos-1] != '0' && grid[i][piece->lXpos] != '0') {
                check = true;
                break;
            }
        }
        return check;
    }

    bool rightColCheck(Tetromino* piece) {
        bool check = false;
        if(piece->rXpos == 9) return true;
        for(int i = piece->tYpos; i <= piece->bYpos; i++) {
            if(grid[i][piece->rXpos+1] != '0' && grid[i][piece->rXpos] != '0') {
                check = true;
                break;
            }
        }
        return check;
    }


    bool checkFullRow(int row) {
        bool check = true;
        for(int i = 0; i < 10; i++) {
            if(grid[row][i] == '0') check = false;
        }
        return check;
    }

    int multiClear(Tetromino* piece) {
        int totalLines = 0;
        for(int i = piece->tYpos; i <= piece->bYpos; i++) {
            if(checkFullRow(i)) {
                totalLines++;
                lineClear(i);
                lineShift(i);
            }
        }
        return totalLines;
    }

    bool lineFilled(int line) {
        bool filled = true;
        for(int i = 0; i < 9; i++) {
            if(grid[line][i] == '0') filled = false;
        }
        return filled;
    }

    void lineClear(int line) {
        for(int i = 0; i < 10; i++) {
            grid[line][i] = '0';
        }
    }

    void lineShift(int line) {
        for(int i = line; i > 0; i--) {
            for(int j = 0; j < 10; j++) {
                grid[i][j] = grid[i-1][j];
            }
        }
    }

    void clearGrid() {
        for(int i = 0; i < 20; i++){
            for(int j = 0; j < 10; j++) {
                grid[i][j] = '0';
            }
        }
    }

    int hardDrop(Tetromino* piece) {
        int score = 0;
        while(!bottomColCheck(piece)) {
            movePiece(piece, 'd');
            score++;
        }
        return score;
    }

};

#endif