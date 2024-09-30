// Text Pong
// v0.4
// by Hylst (1994 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Change to a cross-platform lib to access terminal functionality.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define WIDTH 40
#define HEIGHT 20
#define PADDLE_SIZE 3

int ballX, ballY, ballDirX, ballDirY;
int player1Y, player2Y;
int score1 = 0, score2 = 0;
int playAgainstAI = 0;
int difficulty = 1;

void initGame() {
    ballX = WIDTH / 2;
    ballY = HEIGHT / 2;
    ballDirX = -1;
    ballDirY = -1;
    player1Y = HEIGHT / 2 - PADDLE_SIZE / 2;
    player2Y = HEIGHT / 2 - PADDLE_SIZE / 2;
}

void drawBoard() {
    system("clear");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 && y >= player1Y && y < player1Y + PADDLE_SIZE) {
                printf("|"); // Player 1 paddle
            } else if (x == WIDTH - 1 && y >= player2Y && y < player2Y + PADDLE_SIZE) {
                printf("|"); // Player 2 or AI paddle
            } else if (x == ballX && y == ballY) {
                printf("O"); // Ball
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("Player 1: %d | Player 2: %d\n", score1, score2);
}

void moveBall() {
    ballX += ballDirX;
    ballY += ballDirY;

    if (ballY == 0 || ballY == HEIGHT - 1) {
        ballDirY = -ballDirY; // Bounce off top/bottom walls
    }

    if (ballX == 1 && ballY >= player1Y && ballY < player1Y + PADDLE_SIZE) {
        ballDirX = -ballDirX; // Player 1 paddle bounce
    } else if (ballX == WIDTH - 2 && ballY >= player2Y && ballY < player2Y + PADDLE_SIZE) {
        ballDirX = -ballDirX; // Player 2 or AI paddle bounce
    } else if (ballX == 0) {
        score2++;
        initGame(); // Reset after scoring
    } else if (ballX == WIDTH - 1) {
        score1++;
        initGame(); // Reset after scoring
    }
}

int kbhit(void) {
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
        if (key == 'a' && player1Y > 0) {
            player1Y--;
        }
        if (key == 'q' && player1Y < HEIGHT - PADDLE_SIZE) {
            player1Y++;
        }

        if (!playAgainstAI) {
            if (key == 'p' && player2Y > 0) {
                player2Y--;
            }
            if (key == 'm' && player2Y < HEIGHT - PADDLE_SIZE) {
                player2Y++;
            }
        }
    }
}

void moveAI() {
    // Simple AI based on difficulty
    if (difficulty == 1) {
        // Easy: AI moves slowly
        if (ballY > player2Y + PADDLE_SIZE / 2 && player2Y < HEIGHT - PADDLE_SIZE) {
            player2Y++;
        } else if (ballY < player2Y + PADDLE_SIZE / 2 && player2Y > 0) {
            player2Y--;
        }
    } else if (difficulty == 2) {
        // Medium: AI moves faster
        if (ballY > player2Y && player2Y < HEIGHT - PADDLE_SIZE) {
            player2Y++;
        } else if (ballY < player2Y && player2Y > 0) {
            player2Y--;
        }
    } else {
        // Hard: AI always matches the ball's Y position
        player2Y = ballY - PADDLE_SIZE / 2;
    }
}

void displayWelcomeMessage() {
    printf("Bienvenue dans le jeu de Pong!\n");
    printf("1. Jouer contre un autre joueur\n");
    printf("2. Jouer contre l'ordinateur\n");
    printf("Choisissez une option (1 ou 2): ");
    int choice;
    scanf("%d", &choice);

    if (choice == 2) {
        playAgainstAI = 1;
        printf("Choisissez un niveau de difficulté (1 = facile, 2 = moyen, 3 = difficile): ");
        scanf("%d", &difficulty);
    }
}

int main() {
    displayWelcomeMessage();
    initGame();

    while (1) {
        drawBoard();
        processInput();
        if (playAgainstAI) {
            moveAI();
        }
        moveBall();
        usleep(100000); // Delay to control speed
    }

    return 0;
}
