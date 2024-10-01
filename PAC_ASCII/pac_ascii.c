// PAC ASCII - MINI PAC MAN LIKE GAME IN TEXT MODE
// v0.3
// by Hylst (1994 - 2024 (cleaning code / good practice / comments) )
// To Do :
// add readme.md
// Complete code
// Finish implementing of Ghost moves, boost to eat, ... Everything...
// Add introduction / welcome screen explaining game & controls
// ...


#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <windows.h> // Pour Sleep et Beep sous Windows
#else
    #include <unistd.h>  // Pour usleep sous Unix
#endif

#define WIDTH 20
#define HEIGHT 10
#define PACMAN '@'
#define GHOST 'G'
#define WALL '#'
#define PELLET '.'

// Structure pour position
typedef struct {
    int x, y;
} Position;

// Fonction multiplateforme pour effacer l'écran
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Fonction multiplateforme pour gérer les délais
void wait(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000); // Conversion en microsecondes pour Unix
    #endif
}

// Variables globales pour Pac-Man et les fantômes
Position pacman = {1, 1};
Position ghosts[2] = {{10, 5}, {15, 7}};
char board[HEIGHT][WIDTH] = {
    "####################",
    "#........#.........#",
    "#.####.#.###.#.####.#",
    "#.#...........#....#",
    "#.#.#####.#.#####.#",
    "#.#.....#.#...#...#",
    "#.#####.#.###.#.###",
    "#.................#",
    "####################"
};

// Fonction pour dessiner le tableau
void drawBoard() {
    clearScreen();
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == pacman.x && y == pacman.y) {
                printf("%c", PACMAN);  // Pac-Man
            } else {
                int isGhost = 0;
                for (int g = 0; g < 2; g++) {
                    if (ghosts[g].x == x && ghosts[g].y == y) {
                        printf("%c", GHOST);  // Fantômes
                        isGhost = 1;
                        break;
                    }
                }
                if (!isGhost) {
                    printf("%c", board[y][x]);  // Le reste du plateau (murs, pastilles)
                }
            }
        }
        printf("\n");
    }
}

// Fonction pour jouer un son multiplateforme
void playSound(int frequency, int duration) {
    #ifdef _WIN32
        Beep(frequency, duration); // Son sous Windows
    #else
        printf("\a");  // Beep sous Unix
    #endif
}

// Exemple d'utilisation
void checkCollisions() {
    // Collision avec un fantôme
    for (int g = 0; g < 2; g++) {
        if (pacman.x == ghosts[g].x && pacman.y == ghosts[g].y) {
            playSound(200, 300);  // Son grave pour la collision avec un fantôme
            printf("Pac-Man a été attrapé !\n");
            gameOver = 1;
        }
    }
    // Collecte d'une pastille
    if (board[pacman.y][pacman.x] == PELLET) {
        playSound(1000, 100);  // Son aigu pour la collecte d'une pastille
        board[pacman.y][pacman.x] = ' ';  // La pastille disparaît
    }
}

void checkLevelCompletion() {
    int pelletsRemaining = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (board[y][x] == PELLET) {
                pelletsRemaining++;
            }
        }
    }
    if (pelletsRemaining == 0) {
        printf("Niveau terminé !\n");
        level++;
        loadNextLevel();  // Charger le prochain niveau
    }
}

void loadNextLevel() {
    // Charger une nouvelle carte pour le niveau suivant
    char newBoard[HEIGHT][WIDTH] = {
        "####################",
        "#......#....#......#",
        "#.####.#.##.#.####.#",
        "#.#............#...#",
        "#.#.######.#####.#.#",
        "#.#.......#......#.#",
        "#.######.#.#####.#.#",
        "#........#........#",
        "####################"
    };
    // Copier la nouvelle carte dans le tableau global
    memcpy(board, newBoard, sizeof(newBoard));
    // Réinitialiser Pac-Man et les fantômes
    pacman.x = 1; pacman.y = 1;
    ghosts[0].x = 10; ghosts[0].y = 5;
    ghosts[1].x = 15; ghosts[1].y = 7;
}

void moveGhosts() {
    for (int g = 0; g < ghostCount; g++) {
        if (ghosts[g].x < pacman.x) {
            ghosts[g].x++;
        } else if (ghosts[g].x > pacman.x) {
            ghosts[g].x--;
        }

        if (ghosts[g].y < pacman.y) {
            ghosts[g].y++;
        } else if (ghosts[g].y > pacman.y) {
            ghosts[g].y--;
        }

        // Gestion des collisions fantôme-mur
        if (board[ghosts[g].y][ghosts[g].x] == WALL) {
            // Si le fantôme frappe un mur, il change de direction
            ghosts[g].x = (rand() % WIDTH);
            ghosts[g].y = (rand() % HEIGHT);
        }
    }
}

void generateRandomLevel() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int randomTile = rand() % 5;
            if (randomTile == 0) {
                board[y][x] = WALL;
            } else {
                board[y][x] = PELLET;
            }
        }
    }
    // Créer une zone vide autour de la position initiale de Pac-Man
    pacman.x = 1;
    pacman.y = 1;
    board[pacman.y][pacman.x] = EMPTY;
}

void moveSmartGhost(Ghost *ghost) {
    // Déplacement vers Pac-Man mais éviter les collisions avec d'autres fantômes
    if (abs(ghost->x - pacman.x) > abs(ghost->y - pacman.y)) {
        ghost->x += (ghost->x < pacman.x) ? 1 : -1;
    } else {
        ghost->y += (ghost->y < pacman.y) ? 1 : -1;
    }
    // Si le fantôme frappe un autre fantôme, changer de direction
    for (int g = 0; g < ghostCount; g++) {
        if (&ghosts[g] != ghost && ghosts[g].x == ghost->x && ghosts[g].y == ghost->y) {
            ghost->x = (rand() % WIDTH);
            ghost->y = (rand() % HEIGHT);
        }
    }
}

void updateHighScores(int score) {
    FILE *file = fopen("highscores.txt", "a");
    fprintf(file, "Score: %d\n", score);
    fclose(file);
}

void displayHighScores() {
    FILE *file = fopen("highscores.txt", "r");
    char line[100];
    printf("Meilleurs Scores:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

int main() {
    initGame();
    displayHighScores();  // Affiche les scores avant de commencer

    while (!gameOver) {
        drawBoard();
        processInput();
        moveGhosts();  // Déplacer les fantômes de manière intelligente
        checkCollisions();
        checkLevelCompletion();
        wait(200);
    }

    updateHighScores(score);  // Mise à jour du tableau de scores après la fin du jeu
    printf("Game Over! Score: %d\n", score);
    return 0;
}
