// PAC ASCII - MINI PAC MAN LIKE GAME IN TEXT MODE
// v0.2
// by Hylst (1994 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Change to a cross-platform lib to access terminal functionality. 
// Implement Ghost moves, boost to eat, ... Everything...

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 10
#define EMPTY ' '
#define WALL '#'
#define PLAYER1 'P'
#define PLAYER2 'G'
#define FOOD '.'

typedef struct {
    int x, y;
} Position;

char board[HEIGHT][WIDTH];
Position player1, player2;
int score1 = 0, score2 = 0;
int foodCount = 0;

void initBoard() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1 || (x % 4 == 0 && y % 2 == 0)) {
                board[y][x] = WALL; // Obstacles
            } else {
                board[y][x] = FOOD;
                foodCount++;
            }
        }
    }

    // Initialize players
    player1.x = 1; player1.y = 1;
    player2.x = WIDTH - 2; player2.y = HEIGHT - 2;
    board[player1.y][player1.x] = PLAYER1;
    board[player2.y][player2.x] = PLAYER2;
}

void drawBoard() {
    system("clear");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            printf("%c", board[y][x]);
        }
        printf("\n");
    }
    printf("Score P1: %d | Score P2: %d\n", score1, score2);
}

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void movePlayer(Position *player, char direction) {
    int newX = player->x, newY = player->y;

    switch (direction) {
        case 'z': newY--; break;
        case 'q': newX--; break;
        case 's': newY++; break;
        case 'd': newX++; break;
        case 'o': newY--; break;
        case 'k': newX--; break;
        case 'l': newY++; break;
        case 'm': newX++; break;
    }

    if (board[newY][newX] != WALL) {
        if (board[newY][newX] == FOOD) {
            if (*player == player1) {
                score1++;
            } else {
                score2++;
            }
            foodCount--;
        }

        board[player->y][player->x] = EMPTY;
        player->x = newX;
        player->y = newY;
        board[player->y][player->x] = (*player == player1) ? PLAYER1 : PLAYER2;
    }
}

int main() {
    srand(time(0));
    initBoard();
    
    while (foodCount > 0) {
        drawBoard();

        if (kbhit()) {
            char key = getchar();
            movePlayer(&player1, key);
            movePlayer(&player2, key);
        }

        usleep(100000); // Delay to control speed
    }

    printf("Game Over! Final Scores - P1: %d | P2: %d\n", score1, score2);
    return 0;
}
