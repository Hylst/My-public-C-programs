// TEXT SNAKE 
// v0.8
// by Hylst (1994 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Border is out of game board

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

#define WIDTH 40  // Largeur de la zone de jeu (avec un cadre de 2 colonnes en plus)
#define HEIGHT 20  // Hauteur de la zone de jeu (avec un cadre de 2 lignes en plus)
#define SNAKE_BODY 'O'
#define SNAKE_HEAD '@'
#define FOOD '*'
#define EMPTY ' '
#define OBSTACLE '#'

// Structures pour la position
typedef struct {
    int x, y;
} Position;

Position snake1[100];
Position snake2[100];
int snakeLength1 = 5, snakeLength2 = 5;
Position food;
Position obstacles[100];
int numObstacles = 5;
int directionX1 = 1, directionY1 = 0;
int directionX2 = 0, directionY2 = 0;
int gameOver = 0;
int speed = 300;
int multiplayer = 0;  // 0 = Solo, 1 = VS Humain, 2 = VS AI
int paused = 0;  // 0 = non en pause, 1 = en pause
int level = 1;  // Niveau initial

// Cross-platform clear screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Cross-platform wait
void wait(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}

// Cross-platform sound for events
void playSound(int frequency, int duration) {
    #ifdef _WIN32
        Beep(frequency, duration);
    #else
        printf("\a");
    #endif
}

// Cross-platform input handling
#ifdef _WIN32
int kbhit() {
    return _kbhit();
}

char getInput() {
    return _getch();
}
#else
int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

char getInput() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

// Afficher l'introduction
void showIntroduction() {
    clearScreen();
    printf("=== SNAKE GAME ===\n\n");
    printf("Bienvenue dans le jeu de Snake !\n");
    printf("Mode Solo :\n");
    printf("  - Contrôlez un serpent avec ZQSD pour manger de la nourriture et grandir.\n");
    printf("  - Le jeu devient plus difficile au fur et à mesure avec des obstacles supplémentaires et une vitesse accrue.\n\n");
    printf("Mode Duo :\n");
    printf("  - VS Humain : Joueur 1 contrôle avec ZQSD et Joueur 2 avec les flèches.\n");
    printf("  - VS AI : Jouez contre une IA simple qui tente aussi de manger la nourriture.\n");
    printf("\nAppuyez sur une touche pour continuer...");
    getInput();
}

// Menu principal pour choisir le mode de jeu
int displayMenu() {
    int choice;
    clearScreen();
    printf("=== SNAKE GAME ===\n");
    printf("1. Jouer seul\n");
    printf("2. Jouer à deux (VS Humain)\n");
    printf("3. Jouer à deux (VS AI)\n");
    printf("4. Quitter\n");
    printf("Choisissez une option : ");
    scanf("%d", &choice);
    return choice;
}

// Demander si l'utilisateur souhaite rejouer
int askReplay() {
    char choice;
    printf("Voulez-vous rejouer ? (o/n) : ");
    scanf(" %c", &choice);
    return (choice == 'o' || choice == 'O');
}

// Initialiser le jeu
void initGame() {
    snakeLength1 = 5;
    directionX1 = 1; directionY1 = 0;
    snakeLength2 = 5;
    directionX2 = 0; directionY2 = 0;

    gameOver = 0;
    speed = 300;
    paused = 0;
    level = 1;
    numObstacles = 5;

    // Initialiser la position du serpent 1
    for (int i = 0; i < snakeLength1; i++) {
        snake1[i].x = 5 - i;
        snake1[i].y = 10;
    }
    
    // Initialiser la position du serpent 2
    for (int i = 0; i < snakeLength2; i++) {
        snake2[i].x = WIDTH - 5 + i;
        snake2[i].y = 15;
    }

    // Initialiser la nourriture et les obstacles
    food.x = rand() % (WIDTH - 2) + 1;  // Éviter les bords
    food.y = rand() % (HEIGHT - 2) + 1;

    for (int i = 0; i < numObstacles; i++) {
        obstacles[i].x = rand() % (WIDTH - 2) + 1;
        obstacles[i].y = rand() % (HEIGHT - 2) + 1;
    }
}

// Dessiner le cadre
void drawFrame() {
    for (int x = 0; x < WIDTH; x++) {
        printf("-");
    }
    printf("\n");

    for (int y = 1; y < HEIGHT - 1; y++) {
        printf("|");
        for (int x = 1; x < WIDTH - 1; x++) {
            printf("%c", EMPTY);
        }
        printf("|\n");
    }

    for (int x = 0; x < WIDTH; x++) {
        printf("-");
    }
    printf("\n");
}

// Afficher le plateau de jeu
void drawBoard() {
    clearScreen();
    drawFrame();
    for (int y = 1; y < HEIGHT - 1; y++) {
        for (int x = 1; x < WIDTH - 1; x++) {
            int isSnake1 = 0, isSnake2 = 0, isObstacle = 0;

            // Dessiner le serpent 1
            for (int i = 0; i < snakeLength1; i++) {
                if (x == snake1[i].x && y == snake1[i].y) {
                    printf(i == 0 ? "%c" : "%c", SNAKE_HEAD, SNAKE_BODY);
                    isSnake1 = 1;
                    break;
                }
            }

            // Dessiner le serpent 2
            if (!isSnake1) {
                for (int i = 0; i < snakeLength2; i++) {
                    if (x == snake2[i].x && y == snake2[i].y) {
                        printf(i == 0 ? "%c" : "%c", SNAKE_HEAD, SNAKE_BODY);
                        isSnake2 = 1;
                        break;
                    }
                }
            }

            // Dessiner la nourriture
            if (x == food.x && y == food.y) {
                printf("%c", FOOD);
            } 
            // Dessiner les obstacles
            else {
                for (int i = 0; i < numObstacles; i++) {
                    if (x == obstacles[i].x && y == obstacles[i].y) {
                        printf("%c", OBSTACLE);
                        isObstacle = 1;
                        break;
                    }
                }

                // Si pas de serpent ou de nourriture, laisser vide
                if (!isSnake1 && !isSnake2 && !isObstacle) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    printf("Score Joueur 1: %d | Score Joueur 2: %d | Niveau: %d\n", snakeLength1 - 5, snakeLength2 - 5, level);
}

// Mettre à jour la position du serpent et gérer les collisions
void updateSnake(Position snake[], int *snakeLength, int directionX, int directionY) {
    Position newHead = {snake[0].x + directionX, snake[0].y + directionY};

    // Vérification des collisions avec les murs
    if (newHead.x <= 0 || newHead.x >= WIDTH - 1 || newHead.y <= 0 || newHead.y >= HEIGHT - 1) {
        gameOver = 1;
        playSound(200, 500);
        return;
    }

    // Vérification des collisions avec les obstacles
    for (int i = 0; i < numObstacles; i++) {
        if (newHead.x == obstacles[i].x && newHead.y == obstacles[i].y) {
            gameOver = 1;
            playSound(200, 500);
            return;
        }
    }

    // Vérification si le serpent mange la nourriture
    if (newHead.x == food.x && newHead.y == food.y) {
        (*snakeLength)++;
        food.x = rand() % (WIDTH - 2) + 1;
        food.y = rand() % (HEIGHT - 2) + 1;
        playSound(1000, 100);
        speed -= 10;
        level++;
        numObstacles++;  // Ajouter un obstacle à chaque niveau
        obstacles[numObstacles - 1].x = rand() % (WIDTH - 2) + 1;
        obstacles[numObstacles - 1].y = rand() % (HEIGHT - 2) + 1;
    }

    // Mise à jour du corps du serpent
    for (int i = (*snakeLength) - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = newHead;
}

// Contrôler l'IA pour le joueur 2
void controlAI(Position snake[], int *directionX, int *directionY) {
    if (food.x > snake[0].x) {
        *directionX = 1;
        *directionY = 0;
    } else if (food.x < snake[0].x) {
        *directionX = -1;
        *directionY = 0;
    } else if (food.y > snake[0].y) {
        *directionX = 0;
        *directionY = 1;
    } else {
        *directionX = 0;
        *directionY = -1;
    }
}

// Gérer les entrées utilisateur
void processInput() {
    if (kbhit()) {
        char key = getInput();
        switch (key) {
            // Joueur 1 (ZQSD)
            case 'z': if (directionY1 == 0) { directionX1 = 0; directionY1 = -1; } break;
            case 's': if (directionY1 == 0) { directionX1 = 0; directionY1 = 1; } break;
            case 'q': if (directionX1 == 0) { directionX1 = -1; directionY1 = 0; } break;
            case 'd': if (directionX1 == 0) { directionX1 = 1; directionY1 = 0; } break;
            // Joueur 2 (flèches)
            case 72: if (directionY2 == 0) { directionX2 = 0; directionY2 = -1; } break;  // Flèche haut
            case 80: if (directionY2 == 0) { directionX2 = 0; directionY2 = 1; } break;   // Flèche bas
            case 75: if (directionX2 == 0) { directionX2 = -1; directionY2 = 0; } break;  // Flèche gauche
            case 77: if (directionX2 == 0) { directionX2 = 1; directionY2 = 0; } break;   // Flèche droite
            // Pause
            case 'p': paused = !paused; break;
        }
    }
}

// Boucle principale du jeu
void gameLoop() {
    while (!gameOver) {
        if (!paused) {
            drawBoard();
            processInput();
            updateSnake(snake1, &snakeLength1, directionX1, directionY1);
            if (multiplayer == 1) {  // VS Humain
                updateSnake(snake2, &snakeLength2, directionX2, directionY2);
            } else if (multiplayer == 2) {  // VS AI
                controlAI(snake2, &directionX2, &directionY2);
                updateSnake(snake2, &snakeLength2, directionX2, directionY2);
            }
            wait(speed);
        } else {
            printf("PAUSE - Appuyez sur 'P' pour reprendre.\n");
            processInput();
        }
    }
    printf("Game Over! Scores : Joueur 1: %d, Joueur 2: %d\n", snakeLength1 - 5, snakeLength2 - 5);
}

// Fonction principale
int main() {
    showIntroduction();

    while (1) {
        int choice = displayMenu();
        if (choice == 4) {
            break;  // Quitter le jeu
        }

        multiplayer = (choice == 2) ? 1 : (choice == 3) ? 2 : 0;
        initGame();
        gameLoop();

        if (!askReplay()) {
            break;
        }
    }

    return 0;
}
