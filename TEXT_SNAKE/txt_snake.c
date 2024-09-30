// TEXT SNAKE 
// v0.3
// by Hylst (1994 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Change to a cross-platform lib to access terminal functionality. 
// 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_LENGTH 100

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position body[MAX_LENGTH];
    int length;
    char direction;
} Snake;

typedef struct {
    Position pos[MAX_LENGTH];
    int count;
} Obstacles;

Snake player1, player2;
Obstacles obstacles;
Position food;
int gameOver = 0;
int playAgainstAI = 0;
int difficulty = 1;

void initGame() {
    player1.length = 3;
    player1.body[0] = (Position) {WIDTH / 4, HEIGHT / 2};
    player1.direction = 'd';

    player2.length = 3;
    player2.body[0] = (Position) {3 * WIDTH / 4, HEIGHT / 2};
    player2.direction = 'k';

    obstacles.count = 5;
    for (int i = 0; i < obstacles.count; i++) {
        obstacles.pos[i] = (Position) {rand() % WIDTH, rand() % HEIGHT};
    }

    food.x = rand() % WIDTH;
    food.y = rand() % HEIGHT;
}

void drawBoard() {
    system("clear");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int isSnake1 = 0, isSnake2 = 0, isObstacle = 0, isFood = 0;

            for (int i = 0; i < player1.length; i++) {
                if (player1.body[i].x == x && player1.body[i].y == y) {
                    isSnake1 = 1;
                }
            }

            for (int i = 0; i < player2.length; i++) {
                if (player2.body[i].x == x && player2.body[i].y == y) {
                    isSnake2 = 1;
                }
            }

            for (int i = 0; i < obstacles.count; i++) {
                if (obstacles.pos[i].x == x && obstacles.pos[i].y == y) {
                    isObstacle = 1;
                }
            }

            if (food.x == x && food.y == y) {
                isFood = 1;
            }

            if (isSnake1) printf("1");
            else if (isSnake2) printf("2");
            else if (isObstacle) printf("X");
            else if (isFood) printf("*");
            else printf(" ");
        }
        printf("\n");
    }
}

void moveSnake(Snake *snake) {
    Position newHead = snake->body[0];
    
    switch (snake->direction) {
        case 'z': newHead.y--; break;
        case 'q': newHead.x--; break;
        case 's': newHead.y++; break;
        case 'd': newHead.x++; break;
        case 'o': newHead.y--; break;
        case 'k': newHead.x--; break;
        case 'l': newHead.y++; break;
        case 'm': newHead.x++; break;
    }

    if (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT) {
        gameOver = 1;
        return;
    }

    for (int i = 0; i < snake->length; i++) {
        if (newHead.x == snake->body[i].x && newHead.y == snake->body[i].y) {
            gameOver = 1;
            return;
        }
    }

    for (int i = 0; i < obstacles.count; i++) {
        if (newHead.x == obstacles.pos[i].x && newHead.y == obstacles.pos[i].y) {
            gameOver = 1;
            return;
        }
    }

    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    snake->body[0] = newHead;

    if (newHead.x == food.x && newHead.y == food.y) {
        snake->length++;
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
    }
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
            case 'z': case 'q': case 's': case 'd': player1.direction = key; break;
            case 'o': case 'k': case 'l': case 'm': player2.direction = key; break;
        }
    }
}

void moveAI(Snake *aiSnake, Snake *target) {
    if (difficulty == 1) {
        if (rand() % 2) {
            aiSnake->direction = aiSnake->body[0].y < target->body[0].y ? 's' : 'z';
        } else {
            aiSnake->direction = aiSnake->body[0].x < target->body[0].x ? 'd' : 'q';
        }
    } else if (difficulty == 2) {
        if (rand() % 3) {
            aiSnake->direction = aiSnake->body[0].y < target->body[0].y ? 's' : 'z';
        } else {
            aiSnake->direction = aiSnake->body[0].x < target->body[0].x ? 'd' : 'q';
        }
    } else {
        aiSnake->direction = (aiSnake->body[0].y < target->body[0].y) ? 's' :
                             (aiSnake->body[0].y > target->body[0].y) ? 'z' :
                             (aiSnake->body[0].x < target->body[0].x) ? 'd' : 'q';
    }
}

void displayWelcomeMessage() {
    printf("Bienvenue dans le jeu de Snake !\n");
    printf("1. Jouer contre un autre joueur\n");
    printf("2. Jouer contre l'ordinateur\n");
    printf("Choisissez une option (1 ou 2) : ");
    int choice;
    scanf("%d", &choice);

    if (choice == 2) {
        playAgainstAI = 1;
        printf("Choisissez un niveau de difficulté (1 = facile, 2 = moyen, 3 = difficile) : ");
        scanf("%d", &difficulty);
    }
}

int main() {
    srand(time(0));
    displayWelcomeMessage();
    initGame();

    while (!gameOver) {
        drawBoard();
        processInput();
        moveSnake(&player1);

        if (playAgainstAI) {
            moveAI(&player2, &player1);
        } else {
            moveSnake(&player2);
        }

        moveSnake(&player2);
        usleep(200000);  // Delay to control the game speed
    }

    printf("Game Over!\n");
    return 0;
}
