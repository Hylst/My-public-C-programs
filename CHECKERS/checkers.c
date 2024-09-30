// Checkers (Draughts)
// v0.9a 
// by Hylst (1996 - 2024 (cleaning code / good practice / comments) )
// To Do :
// BUG with Chess AI : missing ai branch
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
bool is_valid_move(Game *game, int startX, int startY, int endX, int endY, char player, bool *can_capture);
void make_move(Game *game, int startX, int startY, int endX, int endY, bool can_capture);
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

// Initialize the game board
void init_board(Game *game) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((i + j) % 2 != 0) { // Only dark squares are playable
                if (i < 3) {
                    game->board[i][j] = 'X';  // Player 1's pieces
                } else if (i > 4) {
                    game->board[i][j] = 'O';  // Player 2's pieces
                } else {
                    game->board[i][j] = '.';  // Empty playable squares
                }
            } else {
                game->board[i][j] = ' ';  // Non-playable squares
            }
        }
    }
}

// Display the game board
void print_board(Game *game) {
    printf("    ");
    for (int j = 0; j < SIZE; j++) {
        printf(" %d ", j);
    }
    printf("\n   +");
    for (int j = 0; j < SIZE; j++) {
        printf("---+");
    }
    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        printf(" %d |", i);
        for (int j = 0; j < SIZE; j++) {
            printf(" %c |", game->board[i][j]);
        }
        printf("\n   +");
        for (int j = 0; j < SIZE; j++) {
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
bool is_valid_move(Game *game, int startX, int startY, int endX, int endY, char player, bool *can_capture) {
    // Check boundaries
    if (endX < 0 || endX >= SIZE || endY < 0 || endY >= SIZE)
        return false;

    // Destination must be empty
    if (game->board[endX][endY] != '.')
        return false;

    // Determine the piece and its capabilities
    char piece = game->board[startX][startY];
    bool is_king = (piece == 'K' || piece == 'Q');

    int dx = endX - startX;
    int dy = endY - startY;

    // Reset capture flag
    *can_capture = false;

    // Determine movement direction based on player
    if (player == 'X' || is_king) {
        // Normal move
        if (dx == 1 && abs(dy) == 1)
            return true;

        // Capture move
        if (dx == 2 && abs(dy) == 2) {
            int midX = startX + dx / 2;
            int midY = startY + dy / 2;
            char mid_piece = game->board[midX][midY];
            if (mid_piece == 'O' || mid_piece == 'Q') {
                *can_capture = true;
                return true;
            }
        }
    }

    if (player == 'O' || is_king) {
        // Normal move
        if (dx == -1 && abs(dy) == 1)
            return true;

        // Capture move
        if (dx == -2 && abs(dy) == 2) {
            int midX = startX + dx / 2;
            int midY = startY + dy / 2;
            char mid_piece = game->board[midX][midY];
            if (mid_piece == 'X' || mid_piece == 'K') {
                *can_capture = true;
                return true;
            }
        }
    }

    return false;
}

// Make a move and handle captures
void make_move(Game *game, int startX, int startY, int endX, int endY, bool can_capture) {
    char piece = game->board[startX][startY];
    game->board[startX][startY] = '.';  // Remove piece from start

    // Handle capture
    if (can_capture) {
        int midX = (startX + endX) / 2;
        int midY = (startY + endY) / 2;
        game->board[midX][midY] = '.';  // Remove captured piece
    }

    // Place piece at new location
    game->board[endX][endY] = piece;

    // Promote to King if necessary
    promote_if_possible(game, endX, endY);
}

// Promote a piece to King if it reaches the opposite end
void promote_if_possible(Game *game, int x, int y) {
    char piece = game->board[x][y];
    if (piece == 'X' && x == SIZE - 1) {
        game->board[x][y] = 'K';
        printf("Player X's piece at (%d, %d) has been promoted to King (K)!\n", x, y);
    }
    if (piece == 'O' && x == 0) {
        game->board[x][y] = 'Q';
        printf("Player O's piece at (%d, %d) has been promoted to King (Q)!\n", x, y);
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
    // Check for terminal state
    char winner;
    if (is_game_over(game, &winner)) {
        if (winner == 'X') return INT_MAX;
        if (winner == 'O') return INT_MIN;
    }

    if (depth == 0) {
        return evaluate(game);
    }

    if (is_maximizing) {
        int max_eval = INT_MIN;
        // Traverse all possible moves for player 'X'
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (game->board[i][j] == 'X' || game->board[i][j] == 'K') {
                    for (int dx = -2; dx <= 2; dx += 2) {
                        for (int dy = -2; dy <= 2; dy += 2) {
                            int newX = i + dx;
                            int newY = j + dy;
                            bool can_capture;
                            if (is_valid_move(game, i, j, newX, newY, 'X', &can_capture)) {
                                // Make a temporary copy of the board
                                Game temp_game;
                                copy_board(temp_game.board, game->board);

                                // Apply the move
                                make_move(&temp_game, i, j, newX, newY, can_capture);

                                // Recursive call
                                int eval = minimax(&temp_game, depth - 1, false, alpha, beta);
                                if (eval > max_eval)
                                    max_eval = eval;
                                if (eval > alpha)
                                    alpha = eval;
                                if (beta <= alpha)
                                    break;
                            }
                        }
                    }
                }
            }
        }
        return max_eval;
    } else {
        int min_eval = INT_MAX;
        // Traverse all possible moves for player 'O'
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (game->board[i][j] == 'O' || game->board[i][j] == 'Q') {
                    for (int dx = -2; dx <= 2; dx += 2) {
                        for (int dy = -2; dy <= 2; dy += 2) {
                            int newX = i + dx;
                            int newY = j + dy;
                            bool can_capture;
                            if (is_valid_move(game, i, j, newX, newY, 'O', &can_capture)) {
                                // Make a temporary copy of the board
                                Game temp_game;
                                copy_board(temp_game.board, game->board);

                                // Apply the move
                                make_move(&temp_game, i, j, newX, newY, can_capture);

                                // Recursive call
                                int eval = minimax(&temp_game, depth - 1, true, alpha, beta);
                                if (eval < min_eval)
                                    min_eval = eval;
                                if (eval < beta)
                                    beta = eval;
                                if (beta <= alpha)
                                    break;
                            }
                        }
                    }
                }
            }
        }
        return min_eval;
    }
}

// AI move selection using Minimax
void ai_move(Game *game) {
    int best_score = INT_MIN;
    int best_startX = -1, best_startY = -1, best_endX = -1, best_endY = -1;
    bool best_can_capture = false;

    // Traverse all possible moves for AI (Player 'O')
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (game->board[i][j] == 'O' || game->board[i][j] == 'Q') {
                for (int dx = -2; dx <= 2; dx += 2) {
                    for (int dy = -2; dy <= 2; dy += 2) {
                        int newX = i + dx;
                        int newY = j + dy;
                        bool can_capture;
                        if (is_valid_move(game, i, j, newX, newY, 'O', &can_capture)) {
                            // Make a temporary copy of the board
                            Game temp_game;
                            copy_board(temp_game.board, game->board);

                            // Apply the move
                            make_move(&temp_game, i, j, newX, newY, can_capture);

                            // Evaluate the move using Minimax
                            int move_score = minimax(&temp_game, game->ai_level, true, INT_MIN, INT_MAX);

                            // Update best move if necessary
                            if (move_score < best_score || best_score == INT_MIN) { // Minimizing for 'O'
                                best_score = move_score;
                                best_startX = i;
                                best_startY = j;
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

    if (best_startX != -1) {
        printf("AI moves from (%d, %d) to (%d, %d)\n", best_startX, best_startY, best_endX, best_endY);
        make_move(game, best_startX, best_startY, best_endX, best_endY, best_can_capture);

        // Handle multiple captures
        char piece = game->board[best_endX][best_endY];
        bool continue_capture = false;
        do {
            continue_capture = false;
            for (int dx = -2; dx <= 2; dx += 2) {
                for (int dy = -2; dy <= 2; dy += 2) {
                    int nextX = best_endX + dx;
                    int nextY = best_endY + dy;
                    bool can_capture_next;
                    if (is_valid_move(game, best_endX, best_endY, nextX, nextY, (piece == 'O' || piece == 'Q') ? 'O' : 'X', &can_capture_next)) {
                        if (can_capture_next) {
                            printf("AI makes a multiple capture from (%d, %d) to (%d, %d)\n", best_endX, best_endY, nextX, nextY);
                            make_move(game, best_endX, best_endY, nextX, nextY, can_capture_next);
                            best_endX = nextX;
                            best_endY = nextY;
                            continue_capture = true;
                            break;
                        }
                    }
                }
                if (continue_capture)
                    break;
            }
        } while (continue_capture);
    } else {
        printf("AI has no valid moves!\n");
    }
}

// Check if a player has any valid moves
bool has_any_moves(Game *game, char player) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (game->board[i][j] == player || (player == 'X' && game->board[i][j] == 'K') || (player == 'O' && game->board[i][j] == 'Q')) {
                for (int dx = -2; dx <= 2; dx += 2) {
                    for (int dy = -2; dy <= 2; dy += 2) {
                        int newX = i + dx;
                        int newY = j + dy;
                        bool can_capture;
                        if (player == 'X' || game->board[i][j] == 'K') {
                            if (is_valid_move(game, i, j, newX, newY, 'X', &can_capture))
                                return true;
                        }
                        if (player == 'O' || game->board[i][j] == 'Q') {
                            if (is_valid_move(game, i, j, newX, newY, 'O', &can_capture))
                                return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

// Check if the game is over and set the winner
bool is_game_over(Game *game, char *winner) {
    bool x_exists = false, o_exists = false;
    bool x_can_move = false, o_can_move = false;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (game->board[i][j] == 'X' || game->board[i][j] == 'K')
                x_exists = true;
            if (game->board[i][j] == 'O' || game->board[i][j] == 'Q')
                o_exists = true;
        }
    }

    if (!x_exists) {
        *winner = 'O';
        return true;
    }
    if (!o_exists) {
        *winner = 'X';
        return true;
    }

    // Check if players can move
    x_can_move = has_any_moves(game, 'X');
    o_can_move = has_any_moves(game, 'O');

    if (!x_can_move) {
        *winner = 'O';
        return true;
    }
    if (!o_can_move) {
        *winner = 'X';
        return true;
    }

    return false;
}

// Check if a piece at (x, y) has any captures
bool has_captures(Game *game, int x, int y, char player) {
    char piece = game->board[x][y];
    bool can_capture = false;

    for (int dx = -2; dx <= 2; dx += 2) {
        for (int dy = -2; dy <= 2; dy += 2) {
            int newX = x + dx;
            int newY = y + dy;
            if (is_valid_move(game, x, y, newX, newY, player, &can_capture)) {
                if (can_capture)
                    return true;
            }
        }
    }
    return false;
}

// Check if a piece at (x, y) can move to any position
bool can_move(Game *game, int x, int y, char player) {
    char piece = game->board[x][y];
    bool can_capture = false;

    for (int dx = -2; dx <= 2; dx += 2) {
        for (int dy = -2; dy <= 2; dy += 2) {
            int newX = x + dx;
            int newY = y + dy;
            if (is_valid_move(game, x, y, newX, newY, player, &can_capture)) {
                return true;
            }
        }
    }
    return false;
}

// Handle player move input and execution
void player_move(Game *game, char player) {
    int startX, startY, endX, endY;
    bool must_capture = false;

    // Check if any capture is possible
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((player == 'X' && (game->board[i][j] == 'X' || game->board[i][j] == 'K')) ||
                (player == 'O' && (game->board[i][j] == 'O' || game->board[i][j] == 'Q'))) {
                if (has_captures(game, i, j, player)) {
                    must_capture = true;
                    break;
                }
            }
        }
        if (must_capture)
            break;
    }

    while (true) {
        printf("Player %c, enter your move (startX startY endX endY): ", player);
        scanf("%d %d %d %d", &startX, &startY, &endX, &endY);

        // Validate coordinates
        if (startX < 0 || startX >= SIZE || startY < 0 || startY >= SIZE ||
            endX < 0 || endX >= SIZE || endY < 0 || endY >= SIZE) {
            printf("Invalid coordinates. Please try again.\n");
            continue;
        }

        // Check if the start piece belongs to the player
        char piece = game->board[startX][startY];
        if ((player == 'X' && !(piece == 'X' || piece == 'K')) ||
            (player == 'O' && !(piece == 'O' || piece == 'Q'))) {
            printf("You can only move your own pieces. Please try again.\n");
            continue;
        }

        bool can_capture_move = false;
        if (is_valid_move(game, startX, startY, endX, endY, player, &can_capture_move)) {
            // If captures are mandatory, ensure the move is a capture
            if (must_capture && !can_capture_move) {
                printf("You must capture an opponent's piece. Please try again.\n");
                continue;
            }

            // Make the move
            make_move(game, startX, startY, endX, endY, can_capture_move);

            // Handle multiple captures
            if (can_capture_move) {
                char moved_piece = game->board[endX][endY];
                bool additional_capture = false;
                do {
                    additional_capture = false;
                    for (int dx = -2; dx <= 2; dx += 2) {
                        for (int dy = -2; dy <= 2; dy += 2) {
                            int newX = endX + dx;
                            int newY = endY + dy;
                            bool can_capture_next;
                            if (is_valid_move(game, endX, endY, newX, newY, player, &can_capture_next)) {
                                if (can_capture_next) {
                                    printf("You have another capture! Move again.\n");
                                    printf("Enter your next move (startX startY endX endY): ");
                                    scanf("%d %d %d %d", &startX, &startY, &endX, &endY);
                                    
                                    // Validate that the player continues with the same piece
                                    if (startX != endX - dx || startY != endY - dy) {
                                        printf("You must continue capturing with the same piece. Move cancelled.\n");
                                        break;
                                    }

                                    make_move(game, startX, startY, endX, endY, can_capture_next);
                                    additional_capture = true;
                                    break;
                                }
                            }
                        }
                        if (additional_capture)
                            break;
                    }
                } while (additional_capture);
            }

            break;  // Move completed successfully
        } else {
            printf("Invalid move. Please try again.\n");
        }
    }
}

// Main game loop
int main() {
    Game game;
    char winner = '\0';

    init_board(&game);
    display_intro();
    choose_game_mode(&game);
    print_board(&game);

    char current_player = 'X';  // Player 1 starts

    while (!is_game_over(&game, &winner)) {
        if (game.is_ai && current_player == 'O') {
            printf("AI is making a move...\n");
            ai_move(&game);
        } else {
            player_move(&game, current_player);
        }

        print_board(&game);

        // Switch player
        current_player = (current_player == 'X') ? 'O' : 'X';
    }

    printf("Game Over!\n");
    if (winner == 'X') {
        printf("Player X wins!\n");
    } else if (winner == 'O') {
        if (game.is_ai)
            printf("AI (Player O) wins!\n");
        else
            printf("Player O wins!\n");
    } else {
        printf("It's a draw!\n");
    }

    return 0;
}
