// Checkers (Draughts)
// v0.9e 
// by Hylst (1996 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Mising multi moves - Implementing : WIP
// ...

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

#define SIZE 8
#define MAX_DEPTH_EASY 3
#define MAX_DEPTH_MEDIUM 5
#define MAX_DEPTH_HARD 7
#define BOARD_SIZE 8
#define EMPTY 0
#define PLAYER1 1
#define PLAYER2 2
#define KING1 3
#define KING2 4

typedef struct {
    char board[SIZE][SIZE];
    bool is_ai;
    int ai_level;
} Game;

// Function prototypes
void init_board(Game *game);
void print_board(Game *game);
void display_intro();
void choose_game_mode(Game *game);
bool is_valid_move(Game *game, int startY, int startX, int endY, int endX, char player, bool *can_capture);
void make_move(Game *game, int startY, int startX, int endY, int endX, bool can_capture);
int evaluate(Game *game);
int minimax(Game *game, int depth, bool is_maximizing, int alpha, int beta);
void ai_move(Game *game);
bool has_any_moves(Game *game, char player);
bool is_game_over(Game *game, char *winner);
bool has_captures(Game *game, int x, int y, char player);
bool can_move(Game *game, int x, int y, char player);
void player_move(Game *game, char player);
void promote_if_possible(Game *game, int x, int y);
void copy_board(char dest[SIZE][SIZE], char src[SIZE][SIZE]);
int is_valid_jump(int board[BOARD_SIZE][BOARD_SIZE], int x1, int y1, int x2, int y2);
void perform_jump(int board[BOARD_SIZE][BOARD_SIZE], int x1, int y1, int x2, int y2);
int can_jump_again(int board[BOARD_SIZE][BOARD_SIZE], int x, int y);

// Initialize the game board
void init_board(Game *game) {
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if ((x + y) % 2 != 0) { // Only dark squares are playable
                if (y < 3) {
                    game->board[y][x] = 'X'; // Player 1's pieces
                } else if (y > 4) {
                    game->board[y][x] = 'O'; // Player 2's pieces
                } else {
                    game->board[y][x] = '.'; // Empty playable squares
                }
            } else {
                game->board[y][x] = ' '; // Non-playable squares
            }
        }
    }
}

// Display the game board
void print_board(Game *game) {
    printf("   ");
    for (int x = 0; x < SIZE; x++) {
        printf(" %d  ", x);
    }
    printf("\n");

    printf("  +");
    for (int x = 0; x < SIZE; x++) {
        printf("---+");
    }
    printf("\n");

    for (int y = 0; y < SIZE; y++) {
        printf("%d |", y);
        for (int x = 0; x < SIZE; x++) {
            printf(" %c |", game->board[y][x]);
        }
        printf("\n");

        printf("  +");
        for (int x = 0; x < SIZE; x++) {
            printf("---+");
        }
        printf("\n");
    }
}

// Display introduction and rules
void display_intro() {
    printf("***********************************\n");
    printf("*           WELCOME TO            *\n");
    printf("*         ASCII CHECKERS GAME      *\n");
    printf("***********************************\n\n");
    printf("Rules:\n");
    printf("1. Player 1 (X) moves first, followed by Player 2 (O).\n");
    printf("2. Move diagonally on dark squares.\n");
    printf("3. Capture opponent's pieces by jumping over them.\n");
    printf("4. If you can capture, you must capture.\n");
    printf("5. Multiple captures are allowed in a single turn.\n");
    printf("6. Reach the opposite end to promote your piece to a King (K for Player 1, Q for Player 2).\n");
    printf("7. Kings can move and capture both forwards and backwards.\n\n");
}

// Choose game mode and AI difficulty
void choose_game_mode(Game *game) {
    int choice;
    printf("Choose Game Mode:\n");
    printf("1. Player vs Player\n");
    printf("2. Player vs AI\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 2) {
        game->is_ai = true;
        printf("\nChoose AI Difficulty Level:\n");
        printf("1. Easy\n");
        printf("2. Medium\n");
        printf("3. Hard\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            game->ai_level = MAX_DEPTH_EASY;
        } else if (choice == 2) {
            game->ai_level = MAX_DEPTH_MEDIUM;
        } else {
            game->ai_level = MAX_DEPTH_HARD;
        }
    } else {
        game->is_ai = false;
    }
}

// Check if a move is valid
bool is_valid_move(Game *game, int startY, int startX, int endY, int endX, char player, bool *can_capture) {
    // Check boundaries
    if (endX < 0 || endX >= SIZE || endY < 0 || endY >= SIZE)
        return false;

    // Destination must be empty
    if (game->board[endY][endX] != '.')
        return false;

    // Determine the piece and its capabilities
    char piece = game->board[startY][startX];
    bool is_king = (piece == 'K' || piece == 'Q');

    int dx = endX - startX;
    int dy = endY - startY;

    // Reset capture flag
    *can_capture = false;

    // Determine movement direction based on player
    int forward = (player == 'X') ? 1 : -1;

    // Normal move
    if (abs(dx) == 1 && abs(dy) == 1) {
        if (!is_king && dy != forward) return false;
        return true;
    }

    // Capture move
    if (abs(dx) == 2 && abs(dy) == 2) {
        if (!is_king && dy / 2 != forward) return false;
        int midX = (startX + endX) / 2;
        int midY = (startY + endY) / 2;
        char mid_piece = game->board[midY][midX];
        if ((player == 'X' && (mid_piece == 'O' || mid_piece == 'Q')) ||
            (player == 'O' && (mid_piece == 'X' || mid_piece == 'K'))) {
            *can_capture = true;
            return true;
        }
    }

    return false;
}

// Make a move and handle captures
void make_move(Game *game, int startY, int startX, int endY, int endX, bool can_capture) {
    char piece = game->board[startY][startX];
    game->board[startY][startX] = '.';  // Remove piece from start

    // Handle capture
    if (can_capture) {
        int midX = (startX + endX) / 2;
        int midY = (startY + endY) / 2;
        game->board[midY][midX] = '.';  // Remove captured piece
    }

    // Place piece at new location
    game->board[endY][endX] = piece;

    // Promote to King if necessary
    promote_if_possible(game, endY, endX);
}

// Function to check if the jump is valid
int is_valid_jump(int board[BOARD_SIZE][BOARD_SIZE], int x1, int y1, int x2, int y2) {
    if (x2 < 0 || x2 >= BOARD_SIZE || y2 < 0 || y2 >= BOARD_SIZE) {
        return 0; // Destination out of bounds
    }
    
    if (board[x2][y2] != EMPTY) {
        return 0; // Destination not empty
    }
    
    int middle_x = (x1 + x2) / 2;
    int middle_y = (y1 + y2) / 2;
    
    // Check if there is an opponent's piece in the middle
    if (board[middle_x][middle_y] == EMPTY || board[middle_x][middle_y] == board[x1][y1]) {
        return 0; // No opponent's piece to capture
    }
    
    return 1; // Valid jump
}

// Function to perform a jump and capture the opponent's piece
void perform_jump(int board[BOARD_SIZE][BOARD_SIZE], int x1, int y1, int x2, int y2) {
    int middle_x = (x1 + x2) / 2;
    int middle_y = (y1 + y2) / 2;
    
    board[middle_x][middle_y] = EMPTY; // Remove the captured piece
    board[x2][y2] = board[x1][y1];     // Move the piece to the new position
    board[x1][y1] = EMPTY;             // Empty the old position
    
    // Check if another jump is possible
    if (can_jump_again(board, x2, y2)) {
        printf("Another jump is possible! You must continue jumping.\n");
    }
}

// Function to check if another jump is possible
int can_jump_again(int board[BOARD_SIZE][BOARD_SIZE], int x, int y) {
    int directions[4][2] = {{2, 2}, {2, -2}, {-2, 2}, {-2, -2}};
    
    for (int i = 0; i < 4; i++) {
        int new_x = x + directions[i][0];
        int new_y = y + directions[i][1];
        if (is_valid_jump(board, x, y, new_x, new_y)) {
            return 1; // Can jump again
        }
    }
    return 0; // No more jumps possible
}

void multiple_jumps(int board[BOARD_SIZE][BOARD_SIZE], int x, int y) {
    while (can_jump_again(board, x, y)) {
        int new_x, new_y;
        printf("Enter the coordinates for the next jump: ");
        scanf("%d %d", &new_x, &new_y);
        
        if (is_valid_jump(board, x, y, new_x, new_y)) {
            perform_jump(board, x, y, new_x, new_y);
            x = new_x; // Update to new position
            y = new_y;
        } else {
            printf("Invalid jump. Please enter valid coordinates.\n");
        }
    }
}

// Execute multiple jumps consecutively
void handle_multiple_jumps(int board[BOARD_SIZE][BOARD_SIZE], int *x, int *y) {
    while (can_jump_again(board, *x, *y)) {
        int new_x, new_y;
        // Here, you would likely have logic to choose the next jump in your game loop
        // For now, let's assume it's automatically selected or player-provided
        printf("Enter coordinates for next jump (x y): ");
        scanf("%d %d", &new_x, &new_y);

        if (is_valid_jump(board, *x, *y, new_x, new_y)) {
            perform_jump(board, *x, *y, new_x, new_y);
            *x = new_x; // Update position after jump
            *y = new_y;
        } else {
            printf("Invalid jump. Please enter valid coordinates.\n");
        }
    }
}

// Function for moving pieces, integrated with multiple jump handling
void move_piece(int board[BOARD_SIZE][BOARD_SIZE], int x1, int y1, int x2, int y2) {
    if (is_valid_jump(board, x1, y1, x2, y2)) {
        perform_jump(board, x1, y1, x2, y2);
        handle_multiple_jumps(board, &x2, &y2); // Check for and handle multiple jumps
    } else {
        printf("Invalid move or jump.\n");
    }
}

// Promote a piece to King if it reaches the opposite end
void promote_if_possible(Game *game, int x, int y) {
    char piece = game->board[y][x];
    if (piece == 'X' && y == SIZE - 1) {
        game->board[y][x] = 'K';
        printf("Player X's piece at (%d, %d) has been promoted to King (K)!\n", y, x);
    }
    if (piece == 'O' && y == 0) {
        game->board[y][x] = 'Q';
        printf("Player O's piece at (%d, %d) has been promoted to King (Q)!\n", y, x);
    }
}

// Evaluation function for AI
int evaluate(Game *game) {
    int score = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            switch (game->board[i][j]) {
                case 'X':
                    score += 10;
                    break;
                case 'O':
                    score -= 10;
                    break;
                case 'K':
                    score += 15;
                    break;
                case 'Q':
                    score -= 15;
                    break;
                default:
                    break;
            }
        }
    }
    return score;
}

// Copy board state
void copy_board(char dest[SIZE][SIZE], char src[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

// Minimax with Alpha-Beta pruning
int minimax(Game *game, int depth, bool is_maximizing, int alpha, int beta) {
    char winner;
    if (depth == 0 || is_game_over(game, &winner)) {
        return evaluate(game);
    }

    int best_value = is_maximizing ? INT_MIN : INT_MAX;
    char current_player = is_maximizing ? 'X' : 'O';

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (game->board[y][x] == current_player || game->board[y][x] == (current_player == 'X' ? 'K' : 'Q')) {
                for (int dy = -2; dy <= 2; dy++) {
                    for (int dx = -2; dx <= 2; dx++) {
                        int newY = y + dy;
                        int newX = x + dx;
                        bool can_capture = false;
                        if (is_valid_move(game, y, x, newY, newX, current_player, &can_capture)) {
                            char backup_board[SIZE][SIZE];
                            copy_board(backup_board, game->board);

                            make_move(game, y, x, newY, newX, can_capture);
                            int eval = minimax(game, depth - 1, !is_maximizing, alpha, beta);

                            copy_board(game->board, backup_board);  // Undo move

                            if (is_maximizing) {
                                best_value = eval > best_value ? eval : best_value;
                                alpha = eval > alpha ? eval : alpha;
                            } else {
                                best_value = eval < best_value ? eval : best_value;
                                beta = eval < beta ? eval : beta;
                            }

                            if (beta <= alpha) {
                                return best_value;
                            }
                        }
                    }
                }
            }
        }
    }

    return best_value;
}

// AI move
void ai_move(Game *game) {
    int best_value = INT_MIN;
    int best_startX, best_startY, best_endX, best_endY;
    bool best_can_capture = false;

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (game->board[y][x] == 'X' || game->board[y][x] == 'K') {
                for (int dy = -2; dy <= 2; dy++) {
                    for (int dx = -2; dx <= 2; dx++) {
                        int newY = y + dy;
                        int newX = x + dx;
                        bool can_capture = false;

                        if (is_valid_move(game, y, x, newY, newX, 'X', &can_capture)) {
                            char backup_board[SIZE][SIZE];
                            copy_board(backup_board, game->board);

                            make_move(game, y, x, newY, newX, can_capture);
                            int eval = minimax(game, game->ai_level, false, INT_MIN, INT_MAX);

                            copy_board(game->board, backup_board);  // Undo move

                            if (eval > best_value) {
                                best_value = eval;
                                best_startX = x;
                                best_startY = y;
                                best_endX = newX;
                                best_endY = newY;
                                best_can_capture = can_capture;
                            }
                        }
                    }
                }
            }
        }
    }

    printf("\nAI makes move from (%d, %d) to (%d, %d)\n", best_startY, best_startX, best_endY, best_endX);
    make_move(game, best_startY, best_startX, best_endY, best_endX, best_can_capture);
}

// Check if the game is over
bool is_game_over(Game *game, char *winner) {
    bool has_x = false;
    bool has_o = false;

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (game->board[y][x] == 'X' || game->board[y][x] == 'K') {
                has_x = true;
            }
            if (game->board[y][x] == 'O' || game->board[y][x] == 'Q') {
                has_o = true;
            }
        }
    }

    if (!has_x) {
        *winner = 'O';
        return true;
    }
    if (!has_o) {
        *winner = 'X';
        return true;
    }

    return false;
}

// Check if player has any possible moves
bool has_any_moves(Game *game, char player) {
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (game->board[y][x] == player || game->board[y][x] == (player == 'X' ? 'K' : 'Q')) {
                if (can_move(game, x, y, player)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Check if a piece can move or capture
bool can_move(Game *game, int x, int y, char player) {
    for (int dy = -2; dy <= 2; dy++) {
        for (int dx = -2; dx <= 2; dx++) {
            int newX = x + dx;
            int newY = y + dy;
            bool can_capture = false;
            if (is_valid_move(game, y, x, newY, newX, player, &can_capture)) {
                return true;
            }
        }
    }
    return false;
}

// Handle player move input
void player_move(Game *game, char player) {
    int startX, startY, endX, endY;
    bool valid_move = false;
    bool can_capture = false;

    while (!valid_move) {
        printf("Player %c, enter your move (startY startX endY endX): ", player);
        scanf("%d %d %d %d", &startY, &startX, &endY, &endX);

        if (is_valid_move(game, startY, startX, endY, endX, player, &can_capture)) {
            valid_move = true;
        } else {
            printf("Invalid move. Please try again.\n");
        }
    }

    make_move(game, startY, startX, endY, endX, can_capture);
}

// Main game loop
int main() {
    Game game;
    char winner;
    init_board(&game);
    display_intro();
    choose_game_mode(&game);

    bool game_over = false;
    while (!game_over) {
        print_board(&game);

        if (has_any_moves(&game, 'X')) {
            if (game.is_ai) {
                ai_move(&game);
            } else {
                player_move(&game, 'X');
            }
        } else {
            printf("Player X has no moves left.\n");
            winner = 'O';
            game_over = true;
            break;
        }

        if (is_game_over(&game, &winner)) {
            game_over = true;
            break;
        }

        print_board(&game);

        if (has_any_moves(&game, 'O')) {
            player_move(&game, 'O');
        } else {
            printf("Player O has no moves left.\n");
            winner = 'X';
            game_over = true;
            break;
        }

        if (is_game_over(&game, &winner)) {
            game_over = true;
        }
    }

    print_board(&game);
    printf("Game Over! The winner is Player %c\n", winner);

    return 0;
}
