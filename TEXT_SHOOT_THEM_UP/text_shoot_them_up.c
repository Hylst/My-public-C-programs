//Text Shoot Them Up
// v0.6e 
// by Hylst (1994 - 2024 (cleaning code / good practice / comments) )
// To Do :
// More than 1 shot at a time 

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define WIDTH 40
#define HEIGHT 20
#define PLAYER_SYMBOL '>'
#define ENEMY_SYMBOL 'E'
#define OBSTACLE_SYMBOL '#'
#define SHOT_SYMBOL '-'
#define POWERUP_SYMBOL 'P'
#define EXPLOSION_SYMBOL '*'

typedef struct {
    int x, y;
    int is_alive;
} Entity;

typedef struct {
    int is_active;
    int timer;
} PowerUpState;

void clearScreen() {
    system("cls");
}

void printScoreAndLevel(int score, int level) {
    printf("Score: %d\t Niveau: %d\n", score, level);
}

void printGame(char gameBoard[HEIGHT][WIDTH], int score, int level) {
    clearScreen();
    printScoreAndLevel(score, level);  // Affichage du score et du niveau
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", gameBoard[i][j]);
        }
        printf("\n");
    }
}

void initBoard(char gameBoard[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            gameBoard[i][j] = ' ';
        }
    }
}

void spawnEnemy(Entity *enemy) {
    enemy->x = WIDTH - 1;
    enemy->y = rand() % HEIGHT;
    enemy->is_alive = 1;
}

void spawnObstacle(Entity *obstacle) {
    obstacle->x = WIDTH - 1;
    obstacle->y = rand() % HEIGHT;
    obstacle->is_alive = 1;
}

void spawnPowerUp(Entity *powerUp) {
    powerUp->x = WIDTH - 1;
    powerUp->y = rand() % HEIGHT;
    powerUp->is_alive = 1;
}

void moveEnemies(Entity enemies[], int count, int speed) {
    for (int i = 0; i < count; i++) {
        if (enemies[i].is_alive) {
            if (rand() % speed == 0) {
                enemies[i].x--;
            }
            if (enemies[i].x < 0) {
                enemies[i].is_alive = 0;
            }
        }
    }
}

void moveObstacles(Entity obstacles[], int count) {
    for (int i = 0; i < count; i++) {
        if (obstacles[i].is_alive) {
            obstacles[i].x--;
            if (obstacles[i].x < 0) {
                obstacles[i].is_alive = 0;
            }
        }
    }
}

void movePowerUp(Entity *powerUp) {
    if (powerUp->is_alive) {
        powerUp->x--;
        if (powerUp->x < 0) {
            powerUp->is_alive = 0;
        }
    }
}

void updateBoard(char gameBoard[HEIGHT][WIDTH], Entity *player, Entity enemies[], int enemyCount, Entity obstacles[], int obstacleCount, Entity *shot, Entity *powerUp, Entity *explosion) {
    initBoard(gameBoard);
    gameBoard[player->y][player->x] = PLAYER_SYMBOL;

    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i].is_alive) {
            gameBoard[enemies[i].y][enemies[i].x] = ENEMY_SYMBOL;
        }
    }

    for (int i = 0; i < obstacleCount; i++) {
        if (obstacles[i].is_alive) {
            gameBoard[obstacles[i].y][obstacles[i].x] = OBSTACLE_SYMBOL;
        }
    }

    if (shot->is_alive) {
        gameBoard[shot->y][shot->x] = SHOT_SYMBOL;
    }

    if (powerUp->is_alive) {
        gameBoard[powerUp->y][powerUp->x] = POWERUP_SYMBOL;
    }

    if (explosion->is_alive) {
        gameBoard[explosion->y][explosion->x] = EXPLOSION_SYMBOL;
    }
}

void moveShot(Entity *shot) {
    if (shot->is_alive) {
        shot->x++;
        if (shot->x >= WIDTH) {
            shot->is_alive = 0;
        }
    }
}

int checkCollision(Entity *entity1, Entity *entity2) {
    return entity1->x == entity2->x && entity1->y == entity2->y;
}

int main() {
    char gameBoard[HEIGHT][WIDTH];
    Entity player = {1, HEIGHT / 2, 1};  // Le joueur commence au centre gauche
    Entity enemies[20];  // Augmentation du nombre d'ennemis potentiels
    Entity obstacles[5];
    Entity shot = {0, 0, 0};  // Aucun tir actif au début
    Entity powerUp = {0, 0, 0};  // Aucun power-up au début
    Entity explosion = {0, 0, 0};  // Animation d'explosion inactive au début
    PowerUpState powerUpState = {0, 0};  // État des power-ups

    int score = 0, level = 1, enemiesKilled = 0;
    int enemiesPerWave = 1;  // Ennemis par vague
    int enemySpeed = 3;  // Vitesse initiale des ennemis
    int shotSpeed = 1;  // Vitesse initiale du tir
    int playerSpeed = 1;  // Vitesse initiale du joueur

    srand(time(0));

    for (int i = 0; i < 20; i++) {
        enemies[i].is_alive = 0;
    }
    for (int i = 0; i < 5; i++) {
        obstacles[i].is_alive = 0;
    }

    int enemySpawnTimer = 0;
    int obstacleSpawnTimer = 0;
    int powerUpSpawnTimer = 0;

    while (1) {
        // Gestion des entrées clavier
        if (_kbhit()) {
            char input = _getch();
            if (input == 'z' && player.y > 0) player.y--;  // Haut
            if (input == 's' && player.y < HEIGHT - 1) player.y++;  // Bas
            if (input == 'q' && player.x > 0) player.x--;  // Gauche
            if (input == 'd' && player.x < WIDTH / 2) player.x++;  // Droite
            if (input == ' ' && !shot.is_alive) {  // Tirer
                shot.x = player.x + 1;
                shot.y = player.y;
                shot.is_alive = 1;
                Beep(1000, 100);  // Son du tir
            }
        }

        // Gestion des niveaux
        if (enemiesKilled >= 10 * level) {  // Passer au niveau suivant après 10 * niveau ennemis
            level++;
            enemiesKilled = 0;
            enemiesPerWave += 1;  // Plus d'ennemis à chaque vague
            if (enemySpeed > 1) {
                enemySpeed--;  // Les ennemis deviennent plus rapides
            }
            printf("Niveau %d !\n", level);
            Sleep(1000);
        }

        // Apparition des ennemis, obstacles et power-ups
        if (enemySpawnTimer >= 10) {
            for (int i = 0; i < enemiesPerWave; i++) {
                for (int j = 0; j < 20; j++) {
                    if (!enemies[j].is_alive) {
                        spawnEnemy(&enemies[j]);
                        break;
                    }
                }
            }
            enemySpawnTimer = 0;
        } else {
            enemySpawnTimer++;
        }

        if (obstacleSpawnTimer >= 20) {
            for (int i = 0; i < 5; i++) {
                if (!obstacles[i].is_alive) {
                    spawnObstacle(&obstacles[i]);
                    break;
                }
            }
            obstacleSpawnTimer = 0;
        } else {
            obstacleSpawnTimer++;
        }

        if (powerUpSpawnTimer >= 50 && !powerUp.is_alive) {
            spawnPowerUp(&powerUp);
            powerUpSpawnTimer = 0;
        } else {
            powerUpSpawnTimer++;
        }

        // Déplacer les ennemis, obstacles, power-ups et tir
        moveEnemies(enemies, 20, enemySpeed);
        moveObstacles(obstacles, 5);
        moveShot(&shot);
        movePowerUp(&powerUp);

        // Vérifier les collisions (joueur/ennemi, joueur/obstacle, tir/ennemi, joueur/power-up)
        for (int i = 0; i < 20; i++) {
            if (enemies[i].is_alive && checkCollision(&player, &enemies[i])) {
                Beep(500, 300);  // Son de collision
                printf("Game Over! Score final: %d\n", score);
                return 0;
            }
            if (enemies[i].is_alive && shot.is_alive && checkCollision(&shot, &enemies[i])) {
                explosion.x = enemies[i].x;
                explosion.y = enemies[i].y;
                explosion.is_alive = 1;  // Activer l'explosion
                enemies[i].is_alive = 0;
                shot.is_alive = 0;
                score += 10;  // Ajouter 10 points pour chaque ennemi tué
                enemiesKilled++;
            }
        }

        for (int i = 0; i < 5; i++) {
            if (obstacles[i].is_alive && checkCollision(&player, &obstacles[i])) {
                Beep(500, 300);  // Son de collision
                printf("Game Over! Score final: %d\n", score);
                return 0;
            }
        }

        if (powerUp.is_alive && checkCollision(&player, &powerUp)) {
            Beep(1500, 200);  // Son du power-up
            powerUp.is_alive = 0;
            powerUpState.is_active = 1;  // Activer le power-up
            powerUpState.timer = 200;  // Durer 20 cycles (environ 10 secondes)
            shotSpeed = 2;  // Augmentation de la cadence de tir
            playerSpeed = 2;  // Augmentation de la vitesse du joueur
        }

        // Gérer l'expiration des power-ups
        if (powerUpState.is_active) {
            powerUpState.timer--;
            if (powerUpState.timer <= 0) {
                powerUpState.is_active = 0;  // Désactiver le power-up
                shotSpeed = 1;  // Remettre la cadence de tir à la normale
                playerSpeed = 1;  // Remettre la vitesse du joueur à la normale
            }
        }

        // Désactiver l'explosion après quelques cycles
        if (explosion.is_alive) {
            explosion.is_alive = 0;
        }

        // Mettre à jour le tableau de jeu et l'afficher
        updateBoard(gameBoard, &player, enemies, 20, obstacles, 5, &shot, &powerUp, &explosion);
        printGame(gameBoard, score, level);

        // Pause pour fluidité du jeu
        Sleep(100);
    }

    return 0;
}
