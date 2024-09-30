// Text Alien Shooter
// v0.5
// by Hylst (1996 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Change to a cross-platform lib to access terminal functionality. 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // UNIX
#include <time.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_INVADERS 50
#define PLAYER '^'
#define INVADER 'X'
#define BULLET '|'
#define EMPTY ' '
#define WALL '#'

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position invaders[MAX_INVADERS];
    int count;
    int direction; // 1 for right, -1 for left
    int speed;
} InvaderWave;

Position player, bullet;
InvaderWave invaderWave;
int score = 0, level = 1;
int gameOver = 0;
int bulletActive = 0;

void initGame() {
    player.x = WIDTH / 2;
    player.y = HEIGHT - 1;
    bulletActive = 0;
    score = 0;
    level = 1;
    gameOver = 0;
    invaderWave.count = 5;
    invaderWave.direction = 1;
    invaderWave.speed = 200000; // Initial speed for invader movement
    for (int i = 0; i < invaderWave.count; i++) {
        invaderWave.invaders[i] = (Position){i * 2, 0}; // Initialize invaders in a row
    }
}

void drawBoard() {
    system("clear");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == player.x && y == player.y) {
                printf("%c", PLAYER);
            } else if (bulletActive && x == bullet.x && y == bullet.y) {
                printf("%c", BULLET);
            } else {
                int isInvader = 0;
                for (int i = 0; i < invaderWave.count; i++) {
                    if (invaderWave.invaders[i].x == x && invaderWave.invaders[i].y == y) {
                        printf("%c", INVADER);
                        isInvader = 1;
                        break;
                    }
                }
                if (!isInvader) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    printf("Score: %d | Level: %d\n", score, level);
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

void processInput() {
    if (kbhit()) {
        char key = getchar();
        switch (key) {
            case 'q': if (player.x > 0) player.x--; break;
            case 'd': if (player.x < WIDTH - 1) player.x++; break;
            case 'z': if (!bulletActive) {
                          bullet.x = player.x;
                          bullet.y = player.y - 1;
                          bulletActive = 1;
                      }
                      break;
        }
    }
}

void moveInvaders() {
    for (int i = 0; i < invaderWave.count; i++) {
        invaderWave.invaders[i].x += invaderWave.direction;
        if (invaderWave.invaders[i].x <= 0 || invaderWave.invaders[i].x >= WIDTH - 1) {
            invaderWave.direction = -invaderWave.direction;
            for (int j = 0; j < invaderWave.count; j++) {
                invaderWave.invaders[j].y++;
                if (invaderWave.invaders[j].y >= HEIGHT - 1) {
                    gameOver = 1; // If invaders reach the bottom
                    return;
                }
            }
            break;
        }
    }
}

void moveBullet() {
    if (bulletActive) {
        bullet.y--;
        if (bullet.y < 0) {
            bulletActive = 0; // Bullet goes off screen
        } else {
            for (int i = 0; i < invaderWave.count; i++) {
                if (bullet.x == invaderWave.invaders[i].x && bullet.y == invaderWave.invaders[i].y) {
                    // Bullet hits an invader
                    for (int j = i; j < invaderWave.count - 1; j++) {
                        invaderWave.invaders[j] = invaderWave.invaders[j + 1];
                    }
                    invaderWave.count--;
                    score += 10;
                    bulletActive = 0;
                    break;
                }
            }
        }
    }
}

void nextWave() {
    level++;
    invaderWave.count += 2; // More invaders each wave
    invaderWave.speed -= 20000; // Invaders move faster each wave
    invaderWave.direction = 1;
    for (int i = 0; i < invaderWave.count; i++) {
        invaderWave.invaders[i] = (Position){i * 2 % WIDTH, 0}; // Reinitialize invaders
    }
}

void displayWelcomeMessage() {
    printf("********************************************\n");
    printf("*          WELCOME TO ALIEN SHOOTER        *\n");
    printf("********************************************\n");
    printf("Controls:\n");
    printf("  Move Left  : 'q'\n");
    printf("  Move Right : 'd'\n");
    printf("  Shoot      : 'z'\n\n");
    printf("Press any key to start the game...\n");
    getchar();
}

int main() {
    srand(time(0));
    displayWelcomeMessage();
    initGame();

    while (!gameOver) {
        drawBoard();
        processInput();
        moveBullet();

        // Move invaders at their current speed
        static int invaderMoveTimer = 0;
        if (++invaderMoveTimer > invaderWave.speed / 10000) {
            moveInvaders();
            invaderMoveTimer = 0;
        }

        // If all invaders are eliminated, go to the next wave
        if (invaderWave.count == 0) {
            nextWave();
        }

        usleep(10000); // Main game loop delay - LINUX / UNIX
        //Sleep(10); // Main game loop delay
    }

    printf("Game Over! Final Score: %d\n", score);
    return 0;
}
