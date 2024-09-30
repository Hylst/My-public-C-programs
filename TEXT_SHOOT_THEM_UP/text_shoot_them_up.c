//Text Shoot Them Up
// v0.8
// by Hylst (1994 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Better shooting depth / more animations / more evolutions / levels
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define WIDTH 80  // Double the width of the game screen
#define HEIGHT 20
#define PLAYER_SYMBOL '>'
#define ENEMY_SYMBOL 'E'
#define OBSTACLE_SYMBOL '#'
#define SHOT_SYMBOL '-'
#define POWERUP_SYMBOL 'P'
#define EXPLOSION1 '#'
#define EXPLOSION2 '°'
#define EXPLOSION3 '\''  // Three-step explosion animation
#define MAX_SHOTS 5  // Max number of simultaneous shots

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

// Display the introduction with ASCII art and controls
void showIntroduction() {
    printf("===================================\n");
    printf("        WELCOME TO SHOOT 'EM UP    \n");
    printf("===================================\n\n");
    printf("        Controls:\n");
    printf("        Z: Up\n");
    printf("        S: Down\n");
    printf("        Q: Left\n");
    printf("        D: Right\n");
    printf("        Space: Shoot\n");
    printf("        Collect power-ups (P) to increase speed and fire rate!\n\n");

    printf("         Vaisseau ASCII Art :\n");
    printf("             >===>\n");
    printf("\nPress any key to start the game...\n");
    _getch();  // Wait for a key press to start the game
}

// Display the score and level at the top of the screen
void printScoreAndLevel(int score, int level) {
    printf("Score: %d\t Level: %d\n", score, level);
}

// Display the game with an ASCII border
void printGame(char gameBoard[HEIGHT][WIDTH], int score, int level) {
    clearScreen();
    printScoreAndLevel(score, level);  // Display score and level

    // Top border
    printf("+");
    for (int i = 0; i < WIDTH; i++) {
        printf("-");
    }
    printf("+\n");

    // Game board
    for (int i = 0; i < HEIGHT; i++) {
        printf("|");  // Left border
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", gameBoard[i][j]);
        }
        printf("|\n");  // Right border
    }

    // Bottom border
    printf("+");
    for (int i = 0; i < WIDTH; i++) {
        printf("-");
    }
    printf("+\n");
}

// Initialize the game board
void initBoard(char gameBoard[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            gameBoard[i][j] = ' ';
        }
    }
}

// Spawn a new enemy on the far right of the screen
void spawnEnemy(Entity *enemy) {
    enemy->x = WIDTH - 1;
    enemy->y = rand() % HEIGHT;
    enemy->is_alive = 1;
}

// Spawn a new obstacle on the far right of the screen
void spawnObstacle(Entity *obstacle) {
    obstacle->x = WIDTH - 1;
    obstacle->y = rand() % HEIGHT;
    obstacle->is_alive = 1;
}

// Spawn a power-up on the far right of the screen
void spawnPowerUp(Entity *powerUp) {
    powerUp->x = WIDTH - 1;
    powerUp->y = rand() % HEIGHT;
    powerUp->is_alive = 1;
}

// Move all enemies left across the screen
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

// Move all obstacles left across the screen
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

// Move the power-up left across the screen
void movePowerUp(Entity *powerUp) {
    if (powerUp->is_alive) {
        powerUp->x--;
        if (powerUp->x < 0) {
            powerUp->is_alive = 0;
        }
    }
}

// Move all shots across the screen
void moveShots(Entity shots[], int shotCount) {
    for (int i = 0; i < shotCount; i++) {
        if (shots[i].is_alive) {
            shots[i].x++;
            if (shots[i].x >= WIDTH) {
                shots[i].is_alive = 0;
            }
        }
    }
}

// Update the game board with the positions of all entities
void updateBoard(char gameBoard[HEIGHT][WIDTH], Entity *player, Entity enemies[], int enemyCount, Entity obstacles[], int obstacleCount, Entity shots[], int shotCount, Entity *powerUp, Entity *explosion, int explosionPhase) {
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

    for (int i = 0; i < shotCount; i++) {
        if (shots[i].is_alive) {
            gameBoard[shots[i].y][shots[i].x] = SHOT_SYMBOL;
        }
    }

    if (powerUp->is_alive) {
        gameBoard[powerUp->y][powerUp->x] = POWERUP_SYMBOL;
    }

    // Explosion animation based on phase
    if (explosion->is_alive) {
        if (explosionPhase == 1) {
            gameBoard[explosion->y][explosion->x] = EXPLOSION1;
        } else if (explosionPhase == 2) {
            gameBoard[explosion->y][explosion->x] = EXPLOSION2;
        } else if (explosionPhase == 3) {
            gameBoard[explosion->y][explosion->x] = EXPLOSION3;
        }
    }
}

// Check if two entities collide
int checkCollision(Entity *entity1, Entity *entity2) {
    return entity1->x == entity2->x && entity1->y == entity2->y;
}

int main() {
    char gameBoard[HEIGHT][WIDTH];
    Entity player = {1, HEIGHT / 2, 1};  // Player starts in the middle
    Entity enemies[20];  // Max 20 enemies
    Entity obstacles[5];  // Max 5 obstacles
    Entity shots[MAX_SHOTS];  // Max 5 shots at a time
    Entity powerUp = {0, 0, 0};  // No active power-up at the start
    Entity explosion = {0, 0, 0};  // No active explosion at the start
    PowerUpState powerUpState = {0, 0};  // No active power-up initially

    int score = 0, level = 1, enemiesKilled = 0;
    int enemiesPerWave = 1;  // Number of enemies per wave
    int enemySpeed = 3;  // Initial enemy speed
    int explosionPhase = 0;  // Explosion animation phase

    srand(time(0));

    // Initialize entities
    for (int i = 0; i < 20; i++) {
        enemies[i].is_alive = 0;
    }
    for (int i = 0; i < 5; i++) {
        obstacles[i].is_alive = 0;
    }
    for (int i = 0; i < MAX_SHOTS; i++) {
        shots[i].is_alive = 0;
    }

    showIntroduction();  // Show introduction and controls

    int enemySpawnTimer = 0;
    int obstacleSpawnTimer = 0;
    int powerUpSpawnTimer = 0;
    int explosionTimer = 0;  // Timer for explosion animation

    while (1) {
        // Handle player input
        if (_kbhit()) {
            char input = _getch();
            if (input == 'z' && player.y > 0) player.y--;  // Move up
            if (input == 's' && player.y < HEIGHT - 1) player.y++;  // Move down
            if (input == 'q' && player.x > 0) player.x--;  // Move left
            if (input == 'd' && player.x < WIDTH / 2) player.x++;  // Move right
            if (input == ' ') {  // Shoot
                for (int i = 0; i < MAX_SHOTS; i++) {
                    if (!shots[i].is_alive) {
                        shots[i].x = player.x + 1;
                        shots[i].y = player.y;
                        shots[i].is_alive = 1;
                        Beep(1000, 100);  // Sound for shot
                        break;
                    }
                }
            }
        }

        // Handle level progression
        if (enemiesKilled >= 10 * level) {
            level++;
            enemiesKilled = 0;
            enemiesPerWave += 1;  // Increase number of enemies per wave
            if (enemySpeed > 1) {
                enemySpeed--;  // Increase enemy speed
            }
            printf("Level %d !\n", level);
            Sleep(1000);
        }

        // Spawn enemies, obstacles, and power-ups
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

        // Move enemies, obstacles, shots, and power-up
        moveEnemies(enemies, 20, enemySpeed);
        moveObstacles(obstacles, 5);
        moveShots(shots, MAX_SHOTS);
        movePowerUp(&powerUp);

        // Check collisions (player/enemy, player/obstacle, shot/enemy, player/power-up)
        for (int i = 0; i < 20; i++) {
            if (enemies[i].is_alive && checkCollision(&player, &enemies[i])) {
                Beep(500, 300);  // Collision sound
                printf("Game Over! Final Score: %d\n", score);
                return 0;
            }
            for (int j = 0; j < MAX_SHOTS; j++) {
                if (enemies[i].is_alive && shots[j].is_alive && checkCollision(&shots[j], &enemies[i])) {
                    explosion.x = enemies[i].x;
                    explosion.y = enemies[i].y;
                    explosion.is_alive = 1;  // Start explosion animation
                    enemies[i].is_alive = 0;
                    shots[j].is_alive = 0;
                    score += 10;  // 10 points for each enemy killed
                    enemiesKilled++;

                    // Start explosion animation
                    explosionTimer = 1;
                    explosionPhase = 1;
                    Beep(1500, 100);  // Explosion sound
                }
            }
        }

        for (int i = 0; i < 5; i++) {
            if (obstacles[i].is_alive && checkCollision(&player, &obstacles[i])) {
                Beep(500, 300);  // Collision sound
                printf("Game Over! Final Score: %d\n", score);
                return 0;
            }
        }

        if (powerUp.is_alive && checkCollision(&player, &powerUp)) {
            Beep(1500, 200);  // Power-up sound
            powerUp.is_alive = 0;
            powerUpState.is_active = 1;
            powerUpState.timer = 200;
        }

        // Handle power-up expiration
        if (powerUpState.is_active) {
            powerUpState.timer--;
            if (powerUpState.timer <= 0) {
                powerUpState.is_active = 0;
            }
        }

        // Handle explosion animation
        if (explosion.is_alive) {
            explosionTimer++;
            if (explosionTimer == 3) {
                explosionPhase = 2;
                Beep(1200, 100);  // Explosion second phase sound
            } else if (explosionTimer == 6) {
                explosionPhase = 3;
                Beep(900, 100);  // Explosion third phase sound
            } else if (explosionTimer == 9) {
                explosion.is_alive = 0;  // End explosion
                explosionTimer = 0;
                explosionPhase = 0;
            }
        }

        // Update and display the game
        updateBoard(gameBoard, &player, enemies, 20, obstacles, 5, shots, MAX_SHOTS, &powerUp, &explosion, explosionPhase);
        printGame(gameBoard, score, level);

        Sleep(100);
    }

    return 0;
}
