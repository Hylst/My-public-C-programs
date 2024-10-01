//Text Platformer
// v0.6 
// by Hylst (1996 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Display management not finished
// A lot more 

#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // Pour Sleep et Beep sur Windows

#define WIDTH 50
#define HEIGHT 15
#define PLAYER 'P'
#define ENEMY '@'
#define BULLET '-'
#define PLATFORM '■'
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

// Fonction pour jouer un son (disponible uniquement sous Windows)
void playSound(int frequency, int duration) {
    Beep(frequency, duration);
}

void intro() {
    system("cls"); // Clear screen for Windows

    printf("********************************************\n");
    printf("*        WELCOME TO THE PLATFORMER GAME     *\n");
    printf("********************************************\n");
    printf("Controls:\n");
    printf("  - Move Left  : 'q'\n");
    printf("  - Move Right : 'd'\n");
    printf("  - Jump       : 'z'\n");
    printf("  - Shoot      : 's'\n\n");

    printf("Press any key to start...\n");
    getch(); // Wait for user to press a key
}

void initGame() {
    game.player.x = 1;
    game.player.y = HEIGHT - 2;
    game.jumping = 0;
    game.velocityY = 0;
    game.groundLevel = HEIGHT - 2;
    game.health = 3;
    game.bullet.isActive = 0;

    // Placer les ennemis
    for (int i = 0; i < enemyCount; i++) {
        enemies[i].x = 10 + i * 10;
        enemies[i].y = HEIGHT - 2;
    }

    // Placer les plateformes
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == HEIGHT - 1 || (x % 12 == 0 && y == HEIGHT - 4)) {
                board[y][x] = PLATFORM;
            } else {
                board[y][x] = EMPTY;
            }
        }
    }
}

void drawBoard() {
    system("cls"); // Utiliser "cls" pour Windows

    // Bordure supérieure
    printf("╔");
    for (int x = 0; x < WIDTH; x++) printf("═");
    printf("╗\n");

    // Afficher le plateau
    for (int y = 0; y < HEIGHT; y++) {
        printf("║"); // Bordure gauche
        for (int x = 0; x < WIDTH; x++) {
            printf("%c", board[y][x]);
        }
        printf("║\n"); // Bordure droite
    }

    // Bordure inférieure
    printf("╚");
    for (int x = 0; x < WIDTH; x++) printf("═");
    printf("╝\n");

    printf("Score: %d | Health: %d | Level: %d\n", score, game.health, level);
}

int kbhit() {
    return _kbhit(); // Remplacer pour Windows
}

void processInput() {
    if (kbhit()) {
        char key = getch(); // Utiliser getch() pour Windows
        switch (key) {
            case 'q':
                if (game.player.x > 0) game.player.x--; // Déplacement gauche
                break;
            case 'd':
                if (game.player.x < WIDTH - 1) game.player.x++; // Déplacement droit
                break;
            case 'z':
                if (!game.jumping && game.player.y == game.groundLevel) {
                    game.jumping = 1;
                    game.velocityY = 3; // Vitesse de saut
                    playSound(750, 100); // Son pour le saut
                }
                break;
            case 's':
                // Tirer une balle si inactif
                if (!game.bullet.isActive) {
                    game.bullet.pos.x = game.player.x + 1;
                    game.bullet.pos.y = game.player.y;
                    game.bullet.isActive = 1;
                    game.bullet.range = 5; // Portée des balles
                    playSound(1000, 50); // Son pour le tir
                }
                break;
        }
    }
}

void updatePlayerJump() {
    if (game.jumping) {
        if (game.velocityY > 0) {
            game.player.y--; // Monter
            game.velocityY--; // Réduire la vitesse verticale
        } else {
            game.jumping = 0; // Atteint la hauteur max, commencer à descendre
        }
    } else {
        if (game.player.y < game.groundLevel && board[game.player.y + 1][game.player.x] == EMPTY) {
            game.player.y++; // Descente par gravité
        }
    }
}

void updateBullet() {
    if (game.bullet.isActive) {
        game.bullet.pos.x++;
        game.bullet.range--; // Réduire la portée de la balle
        if (game.bullet.pos.x >= WIDTH || game.bullet.range <= 0) {
            game.bullet.isActive = 0; // La balle sort de l'écran ou perd sa portée
        } else {
            // Vérifier la collision avec les ennemis
            for (int i = 0; i < enemyCount; i++) {
                if (game.bullet.pos.x == enemies[i].x && game.bullet.pos.y == enemies[i].y) {
                    enemies[i].x = -1; // Supprimer l'ennemi
                    score += 100; // Augmenter le score
                    game.bullet.isActive = 0; // La balle disparaît
                    playSound(400, 100); // Son pour la destruction d'un ennemi
                }
            }
        }
    }
}

void updateEnemies() {
    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i].x > 0) {
            enemies[i].x--; // Les ennemis avancent vers le joueur
        }
        if (enemies[i].x == game.player.x && enemies[i].y == game.player.y) {
            game.health--; // Le joueur est touché par un ennemi
            playSound(200, 200); // Son pour un coup reçu
            if (game.health <= 0) {
                gameOver = 1; // Game over si la santé atteint 0
            }
        }
    }
}

void nextLevel() {
    level++;
    enemyCount += 2; // Ajouter plus d'ennemis à chaque niveau
    for (int i = 0; i < enemyCount; i++) {
        enemies[i].x = 10 + i * 5; // Réinitialiser les ennemis pour le niveau suivant
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
        Sleep(100); // Utiliser Sleep pour Windows

        // Passer au niveau suivant si tous les ennemis sont éliminés
        int remainingEnemies = 0;
        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i].x >= 0) remainingEnemies++;
        }
        if (remainingEnemies == 0) {
            nextLevel();
            playSound(1200, 200); // Son pour le passage de niveau
        }
    }

    printf("Game Over! Final Score: %d\n", score);
    playSound(100, 600); // Son de fin de partie
}

int main() {
    intro(); // Affichage de l'intro avec instructions
    initGame();
    gameLoop();
    return 0;
}
