#include <stdio.h>

#ifndef ROWS
#define ROWS 6
#endif

#ifndef COLS
#define COLS 7
#endif

#define CONNECT_N 4

/* Tokens */
#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'

#define HUMAN 1
#define COMPUTER 2

int isColumnFull(char[][COLS], int, int, int); //done

int isBoardFull(char[][COLS], int, int); //done

int isInBounds(int, int, int, int); //done

/* Return index of row where token will land, or -1 if column full */
int getFreeRow(char[][COLS], int, int, int); //done

/* Place token in column (0-based). Return row index or -1 if illegal */
int makeMove(char[][COLS], int, int, int, char); //done

int checkVictory(char[][COLS], int, int, int, int, char); //done

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
int humanChoose(char[][COLS], int, int); //done

/* Computer*/
int computerChoose(char[][COLS], int, int, char, char);

void runConnectFour(char[][COLS], int, int, int, int); //done

void initBoard(char[][COLS], int, int); //done

void printBoard(char[][COLS], int, int); //done

int getPlayerType(int); //done


int main() {
    char board[ROWS][COLS];
    printf("Connect Four (%d rows x %d cols)\n\n", ROWS, COLS);
    int p1Type = getPlayerType(1);
    int p2Type = getPlayerType(2);
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);
    runConnectFour(board, ROWS, COLS, p1Type, p2Type);
    return 0;
}


void printBoard(char board[][COLS], int rows, int cols) {
    printf("\n");
    for (int r = 0; r < rows; r++) {
        printf("|");
        for (int c = 0; c < cols; c++) {
            putchar(board[r][c]);
            printf("|");
        }
        printf("\n");
    }
    for (int c = 1; c <= cols; c++) {
        printf(" %d", c % 10);
    }
    printf("\n\n");
}


int getPlayerType(int playerNumber) {
    char ch;
    while (1) {
        printf("Choose type for player %d: h - human, c - computer: ", playerNumber);
        int n = scanf(" %c", &ch);
        if (n != 1) {
            printf("Input error. Try again.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        if (ch == 'h' || ch == 'H') return HUMAN;
        if (ch == 'c' || ch == 'C') return COMPUTER;

        printf("Invalid selection. Enter h or c.\n");
        while (getchar() != '\n'); // clear rest of input
    }
}


void initBoard(char board[][COLS], int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            board[r][c]=EMPTY;
        }
    }
}


void runConnectFour(char board[][COLS], int rows, int cols, int p1Type, int p2Type) {
    int currentPlayer = 1;
    char currentToken = TOKEN_P1;
    int currentPlayerType = p1Type;
    int gameOver = 0;

    while (!gameOver) {
        //print whose turn it is
        printf("Player %d (%c) turn.\n", currentPlayer, currentToken);
        // Get column choice based on player type
        int chosenColumn;
        if (currentPlayerType == HUMAN) {
            chosenColumn = humanChoose(board, rows, cols);
        }
        else {
            char opponentToken = (currentToken == TOKEN_P1) ? TOKEN_P2 : TOKEN_P1;
            chosenColumn = computerChoose(board, rows, cols, currentToken, opponentToken);
            printf("Computer chose column %d\n", chosenColumn + 1);
        }
        // Make the move
        int row = makeMove(board, rows, cols, chosenColumn, currentToken);
        
        // Print the updated board
        printBoard(board, rows, cols);
        
        // Check if current player won
        if (checkVictory(board, rows, cols, row, chosenColumn, currentToken)) {
            printf("Player %d (%c) wins!\n", currentPlayer, currentToken);
            gameOver = 1;
        }
        // Check if board is full (tie)
        else if (isBoardFull(board, rows, cols)) {
            printf("Board full and no winner. It's a tie!\n");
            gameOver = 1;
        }
        // Switch to other player
        else {
            if (currentPlayer == 1) {
                currentPlayer = 2;
                currentToken = TOKEN_P2;
                currentPlayerType = p2Type;
            }
            else {
                currentPlayer = 1;
                currentToken = TOKEN_P1;
                currentPlayerType = p1Type;
            }
        }
    }
} 


int humanChoose(char board[][COLS], int rows, int cols) {
    int column;
    
    while (1) {
        printf("Enter column (1-%d): ", cols);
        int n = scanf("%d", &column);
        if (n != 1) {
            printf("Invalid input. Enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        if (column < 1 || column > cols) {
            printf("Invalid column. Choose between 1 and %d.\n", cols);
            continue;
        }
        if (isColumnFull(board, rows, cols, column - 1)) {
            printf("Column %d is full. Choose another column.\n", column);
            continue;
        } 
        return column - 1;
    }
}


int isColumnFull(char board[][COLS], int rows, int cols, int column) {
    if (board[0][column] != EMPTY) {
        return 1;
    }
    return 0;
}


int getFreeRow(char board[][COLS], int rows, int cols, int column) {
    for (int r = rows - 1; r >= 0; r--) {
        if (board[r][column] == EMPTY) {
            return r;
        }
    }
    return -1;
}


int makeMove(char board[][COLS], int rows, int cols, int column, char token) {
    int row = getFreeRow(board, rows, cols, column);
    if (row == -1) {
        return -1;
    }
    board[row][column] = token;
    return row;
}


int isBoardFull(char board[][COLS], int rows, int cols) {
    for (int c = 0; c < cols; c++) {
        if (!isColumnFull(board, rows, cols, c)) {
            return 0;
        }
    }
    return 1;
}


int isInBounds(int row, int col, int rows, int cols) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        return 1;
    }
    return 0;
}


int checkVictory(char board[][COLS], int rows, int cols, int row, int col, char token) {
    int count;
    
    // Check horizontal
    count = 0;
    for (int c = 0; c < cols; c++) {
        if (board[row][c] == token) {
            count++;
            if (count >= CONNECT_N) {
                return 1;
            }
        }
        else {
            count = 0;
        }
    }
    
    // Check vertical
    count = 0;
    for (int r = 0; r < rows; r++) {
        if (board[r][col] == token) {
            count++;
            if (count >= CONNECT_N) {
                return 1;
            }
        }
        else {
            count = 0;
        }
    }
    
    // Check \diagonal 
    count = 0;
    // Find top left start of this diagonal
    int r = row;
    int c = col;
    while (r > 0 && c > 0) {
        r--;
        c--;
    }
    // Scan the diagonal
    while (isInBounds(r, c, rows, cols)) {
        if (board[r][c] == token) {
            count++;
            if (count >= CONNECT_N) {
                return 1;
            }
        }
        else {
            count = 0;
        }
        r++;
        c++;
    }
    
    // Check diagonal /
    count = 0;
    // Find top-right start of this diagonal
    r = row;
    c = col;
    while (r > 0 && c < cols - 1) {
        r--;
        c++;
    }
    // Scan the diagonal
    while (isInBounds(r, c, rows, cols)) {
        if (board[r][c] == token) {
            count++;
            if (count >= CONNECT_N) {
                return 1;
            }
        }
        else {
            count = 0;
        }
        r++;
        c--;
    }
    return 0;
}

int computerChoose(char board[][COLS], int rows, int cols, char computerToken, char opponentToken) {
    int centerCol = cols / 2;
    
    // Priority 1: Check for winning move
    for (int c = 0; c < cols; c++) {
        if (!isColumnFull(board, rows, cols, c)) {
            int r = getFreeRow(board, rows, cols, c);
            board[r][c] = computerToken;
            if (checkVictory(board, rows, cols, r, c, computerToken)) {
                board[r][c] = EMPTY;
                return c;
            }
            board[r][c] = EMPTY;
        }
    }
    
    // Priority 2: Block opponent's winning move
    for (int c = 0; c < cols; c++) {
        if (!isColumnFull(board, rows, cols, c)) {
            int r = getFreeRow(board, rows, cols, c);
            board[r][c] = opponentToken;
            if (checkVictory(board, rows, cols, r, c, opponentToken)) {
                board[r][c] = EMPTY;
                return c;
            }
            board[r][c] = EMPTY;
        }
    }
    
    // Priority 3: Create a sequence of three - use ordering rule
    for (int dist = 0; dist <= centerCol; dist++) {
        int c = centerCol - dist;
        if (c >= 0 && !isColumnFull(board, rows, cols, c)) {
            int r = getFreeRow(board, rows, cols, c);
            board[r][c] = computerToken;
            if (checkSequenceOfThree(board, rows, cols, r, c, computerToken)) {
                board[r][c] = EMPTY;
                return c;
            }
            board[r][c] = EMPTY;
        }
        if (dist > 0) {
            c = centerCol + dist;
            if (c < cols && !isColumnFull(board, rows, cols, c)) {
                int r = getFreeRow(board, rows, cols, c);
                board[r][c] = computerToken;
                if (checkSequenceOfThree(board, rows, cols, r, c, computerToken)) {
                    board[r][c] = EMPTY;
                    return c;
                }
                board[r][c] = EMPTY;
            }
        }
    }
    
    // Priority 4: Block opponent's sequence of three - use ordering rule
    for (int dist = 0; dist <= centerCol; dist++) {
        int c = centerCol - dist;
        if (c >= 0 && !isColumnFull(board, rows, cols, c)) {
            int r = getFreeRow(board, rows, cols, c);
            board[r][c] = opponentToken;
            if (checkSequenceOfThree(board, rows, cols, r, c, opponentToken)) {
                board[r][c] = EMPTY;
                return c;
            }
            board[r][c] = EMPTY;
        }
        if (dist > 0) {
            c = centerCol + dist;
            if (c < cols && !isColumnFull(board, rows, cols, c)) {
                int r = getFreeRow(board, rows, cols, c);
                board[r][c] = opponentToken;
                if (checkSequenceOfThree(board, rows, cols, r, c, opponentToken)) {
                    board[r][c] = EMPTY;
                    return c;
                }
                board[r][c] = EMPTY;
            }
        }
    }
    
    // Priority 5: Choose by ordering rule
    for (int dist = 0; dist <= centerCol; dist++) {
        int c = centerCol - dist;
        if (c >= 0 && !isColumnFull(board, rows, cols, c)) {
            return c;
        }
        if (dist > 0) {
            c = centerCol + dist;
            if (c < cols && !isColumnFull(board, rows, cols, c)) {
                return c;
            }
        }
    }
    
    return 0;
}

int checkSequenceOfThree(char board[][COLS], int rows, int cols, int row, int col, char token) {
    int count;
    
    // Check horizontal
    count = 0;
    for (int c = 0; c < cols; c++) {
        if (board[row][c] == token) {
            count++;
            if (count == 3) {
                return 1;
            }
        }
        else {
            count = 0;
        }
    }
    
    // Check vertical
    count = 0;
    for (int r = 0; r < rows; r++) {
        if (board[r][col] == token) {
            count++;
            if (count == 3) {
                return 1;
            }
        }
        else {
            count = 0;
        }
    }
    
    // Check \diagonal 
    count = 0;
    int r = row;
    int c = col;
    while (r > 0 && c > 0) {
        r--;
        c--;
    }
    while (isInBounds(r, c, rows, cols)) {
        if (board[r][c] == token) {
            count++;
            if (count == 3) {
                return 1;
            }
        }
        else {
            count = 0;
        }
        r++;
        c++;
    }
    
    // Check diagonal /
    count = 0;
    r = row;
    c = col;
    while (r > 0 && c < cols - 1) {
        r--;
        c++;
    }
    while (isInBounds(r, c, rows, cols)) {
        if (board[r][c] == token) {
            count++;
            if (count == 3) {
                return 1;
            }
        }
        else {
            count = 0;
        }
        r++;
        c--;
    }
    return 0;
}