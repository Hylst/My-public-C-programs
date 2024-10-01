// TEXT SNAKE 
// v0.7
// by Hylst (1994 - 2024 (cleaning code / good practice / comments) )

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

#define WIDTH 80  // Double la largeur
#define HEIGHT 25
#define SNAKE_BODY 'O'
#define SNAKE_HEAD '@'
#define FOOD 'X'
#define EMPTY ' '
#define OBSTACLE '#'

typedef struct {
    int x, y;
} Position;

Position snake1[100];
Position snake2[100];
int snakeLength1 = 5, snakeLength2 = 5;
Position food;
Position obstacles[5];
int directionX1 = 1, directionY1 = 0;  // Joueur 1 : Initialement vers la droite
int directionX2 = 0, directionY2 = 0;  // Joueur 2 : Initialement immobile
int gameOver = 0;
int speed = 200;
int multiplayer = 0;  // 0 = Solo, 1 = VS Humain, 2 = VS AI

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
    printf("Fonctionnalités :\n");
    printf("- Solo ou mode multijoueur (1 vs 1 humain ou IA)\n");
    printf("- Déplacement fluide et croissance du serpent en mangeant des aliments\n");
    printf("- Sons pour différents événements (manger, collisions, etc.)\n");
    printf("\nContrôles :\n");
    printf("Joueur 1 :\n");
    printf("  Z : Haut\n  S : Bas\n  Q : Gauche\n  D : Droite\n");
    printf("Joueur 2 :\n");
    printf("  Flèche Haut : Haut\n  Flèche Bas : Bas\n  Flèche Gauche : Gauche\n  Flèche Droite : Droite\n");
    printf("\nAppuyez sur une touche pour continuer...");
    getInput();
}

// Menu principal pour choisir solo ou multijoueur
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
    directionX2 = 0; directionY2 = 0;  // Joueur 2 inactif jusqu'à l'entrée

    gameOver = 0;
    speed = 200;

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
    food.x = rand() % WIDTH;
    food.y = rand() % HEIGHT;

    for (int i = 0; i < 5; i++) {
        obstacles[i].x = rand() % WIDTH;
        obstacles[i].y = rand() % HEIGHT;
    }
}

// Afficher le plateau de jeu
void drawBoard() {
    clearScreen();
    printf("Joueur 1 : ZQSD | Joueur 2 : Flèches (si activé)\n");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
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
                for (int i = 0; i < 5; i++) {
                    if (x == obstacles[i].x && y == obstacles[i].y) {
                        printf("%c", OBSTACLE);
                        isObstacle = 1;
                        break;
                    }
                }

                // Si pas de serpent ou de nourriture, dessiner un espace vide
                if (!isSnake1 && !isSnake2 && !isObstacle) {
                    printf("%c", EMPTY);
                }
            }
        }
        printf("\n");
    }
    printf("Score Joueur 1: %d | Score Joueur 2: %d\n", snakeLength1 - 5, snakeLength2 - 5);
}

// Mettre à jour la position du serpent et gérer les collisions
void updateSnake(Position snake[], int *snakeLength, int directionX, int directionY) {
    Position newHead = {snake[0].x + directionX, snake[0].y + directionY};

    // Vérification des collisions avec les murs
    if (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT) {
        gameOver = 1;
        playSound(200, 500);
        return;
    }

    // Vérification des collisions avec les obstacles
    for (int i = 0; i < 5; i++) {
        if (newHead.x == obstacles[i].x && newHead.y == obstacles[i].y) {
            gameOver = 1;
            playSound(200, 500);
            return;
        }
    }

    // Vérification si le serpent mange la nourriture
    if (newHead.x == food.x && newHead.y == food.y) {
        (*snakeLength)++;
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
        playSound(1000, 100);
        speed -= 10;
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
        }
    }
}

// Boucle principale du jeu
void gameLoop() {
    while (!gameOver) {
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
