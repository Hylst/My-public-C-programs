//Text Platformer
// v0.4f 
// by Hylst (1996 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Change to a cross-platform lib like ncurses to access terminal functionality. 

#include <stdio.h>
#include <stdlib.h>

// Change folowing libs for a cross-platform lib : ncurses to access terminal functionality. 
#include <unistd.h>
#include <termios.h>

#include <fcntl.h>

#define WIDTH 50
#define HEIGHT 15
#define PLAYER 'P'
#define ENEMY 'E'
#define BULLET '-'
#define OBSTACLE '#'
#define PLATFORM '='
#define EMPTY ' '

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position pos;
    int isActive;
    int range;
} Bullet;

typedef struct {
    Position player;
    Bullet bullet;
    int jumping;
    int velocityY;
    int groundLevel;
    int health;
} GameState;

Position enemies[5];
int enemyCount = 3;
char board[HEIGHT][WIDTH];
GameState game;
int gameOver = 0;
int score = 0;
int level = 1;

void initGame() {
    game.player.x = 1;
    game.player.y = HEIGHT - 2;
    game.jumping = 0;
    game.velocityY = 0;
    game.groundLevel = HEIGHT - 2;
    game.health = 3;
    game.bullet.isActive = 0;

    // Place enemies
    for (int i = 0; i < enemyCount; i++) {
        enemies[i].x = 10 + i * 10;
        enemies[i].y = HEIGHT - 2;
    }

    // Place obstacles and platforms
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == HEIGHT - 1 || (x % 12 == 0 && y == HEIGHT - 4)) {
                board[y][x] = PLATFORM; // Platforms
            } else {
                board[y][x] = EMPTY;
            }
        }
    }
}

void drawBoard() {
    system("clear");

    // Clear the board
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == HEIGHT - 1 || (x % 12 == 0 && y == HEIGHT - 4)) {
                board[y][x] = PLATFORM; // Platforms
            } else {
                board[y][x] = EMPTY;
            }
        }
    }

    // Place player
    board[game.player.y][game.player.x] = PLAYER;

    // Place enemies
    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i].x >= 0 && enemies[i].x < WIDTH) {
            board[enemies[i].y][enemies[i].x] = ENEMY;
        }
    }

    // Place bullet
    if (game.bullet.isActive) {
        board[game.bullet.pos.y][game.bullet.pos.x] = BULLET;
    }

    // Draw board
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            printf("%c", board[y][x]);
        }
        printf("\n");
    }
    printf("Score: %d | Health: %d | Level: %d\n", score, game.health, level);
}

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
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

void processInput() {
    if (kbhit()) {
        char key = getchar();
        switch (key) {
            case 'q':
                if (game.player.x > 0) game.player.x--; // Move left
                break;
            case 'd':
                if (game.player.x < WIDTH - 1) game.player.x++; // Move right
                break;
            case 'z':
                if (!game.jumping && game.player.y == game.groundLevel) {
                    game.jumping = 1;
                    game.velocityY = 3; // Initial jump velocity
                }
                break;
            case 's':
                // Shoot bullet if not active
                if (!game.bullet.isActive) {
                    game.bullet.pos.x = game.player.x + 1;
                    game.bullet.pos.y = game.player.y;
                    game.bullet.isActive = 1;
                    game.bullet.range = 5; // Bullet range
                }
                break;
        }
    }
}

void updatePlayerJump() {
    if (game.jumping) {
        if (game.velocityY > 0) {
            game.player.y--; // Jump up
            game.velocityY--; // Decrease upward velocity
        } else {
            game.jumping = 0; // Reached max height, start falling
        }
    } else {
        if (game.player.y < game.groundLevel && board[game.player.y + 1][game.player.x] == EMPTY) {
            game.player.y++; // Fall down with gravity
        }
    }
}

void updateBullet() {
    if (game.bullet.isActive) {
        game.bullet.pos.x++;
        game.bullet.range--; // Decrease bullet range
        if (game.bullet.pos.x >= WIDTH || game.bullet.range <= 0) {
            game.bullet.isActive = 0; // Bullet went off screen or out of range
        } else {
            // Check collision with enemies
            for (int i = 0; i < enemyCount; i++) {
                if (game.bullet.pos.x == enemies[i].x && game.bullet.pos.y == enemies[i].y) {
                    // Enemy hit
                    enemies[i].x = -1; // Remove enemy
                    score += 100; // Increase score
                    game.bullet.isActive = 0; // Bullet disappears
                }
            }
        }
    }
}

void updateEnemies() {
    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i].x > 0) {
            enemies[i].x--; // Move enemies towards player
        }
        if (enemies[i].x == game.player.x && enemies[i].y == game.player.y) {
            game.health--; // Player hit by enemy
            if (game.health <= 0) {
                gameOver = 1; // Game over if health reaches 0
            }
        }
    }
}

void nextLevel() {
    level++;
    enemyCount += 2; // Add more enemies
    for (int i = 0; i < enemyCount; i++) {
        enemies[i].x = 10 + i * 5; // Reinitialize enemies at the next level
        enemies[i].y = HEIGHT - 2;
    }
}

void gameLoop() {
    while (!gameOver) {
        drawBoard();
        processInput();
        updatePlayerJump();
        updateBullet();
        updateEnemies();
        usleep(100000); // Delay for game speed

        // If all enemies are gone, go to the next level
        int remainingEnemies = 0;
        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i].x >= 0) remainingEnemies++;
        }
        if (remainingEnemies == 0) {
            nextLevel();
        }
    }

    printf("Game Over! Final Score: %d\n", score);
}

int main() {
    initGame();
    gameLoop();
    return 0;
}
