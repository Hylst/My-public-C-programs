//Text Shoot Them Up
// v1.0
// by Hylst (1994 - 2024 (cleaning code / good practice / comments) )
// To Do :

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define CLEAR_SCREEN() system("cls")  // Clear screen for Windows
    #define SLEEP(ms) Sleep(ms)

#else
    #include <unistd.h>  // For sleep in Unix-like systems
    #include <termios.h>
    #define CLEAR_SCREEN() printf("\033[H\033[J")  // Clear screen for Unix-like systems
    #define SLEEP(ms) usleep(ms * 1000)  // sleep for Unix-like systems in milliseconds

    // Cross-platform input function to simulate _kbhit and getch
    int _kbhit() {
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
        if(ch != EOF) {
            ungetc(ch, stdin);
            return 1;
        }
        return 0;
    }

    int _getch() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif


#define WIDTH 80
#define HEIGHT 20
#define PLAYER_SYMBOL '>'
#define WEAK_ENEMY_SYMBOL 'E'
#define STRONG_ENEMY_SYMBOL 'S'
#define SHOOTING_ENEMY_SYMBOL 'T'
#define PLAYER_SHOT_SYMBOL '-'
#define ENEMY_SHOT_SYMBOL '|'
#define POWERUP_SYMBOL 'P'
#define MOUNTAIN_LEFT '/'
#define MOUNTAIN_RIGHT '\\'
#define MOUNTAIN_FLAT '_'
#define MAX_SHOTS 5
#define MAX_ENEMY_SHOTS 5
#define PLAYER_MAX_HEALTH 3
#define ENEMY_HEALTH_WEAK 1
#define ENEMY_HEALTH_STRONG 3

typedef struct {
    int x, y;
    int is_alive;
    int health;  // Health for player and enemies
} Entity;

typedef struct {
    int is_active;
    int timer;
} PowerUpState;

void clearScreen() {
    CLEAR_SCREEN();
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
    printf("        Collect power-ups (P) to increase speed and fire rate!\n");
    printf("        Avoid enemy shots!\n\n");

    printf("         Vaisseau ASCII Art :\n");
    printf("             >===>\n");
    printf("\nPress any key to start the game...\n");
    _getch();  // Wait for a key press to start the game
}

// Display the score, level, multiplier, and player's health
void printScoreAndLevel(int score, int level, int multiplier, int player_health) {
    printf("Score: %d\t Level: %d\t Multiplier: x%d\t Health: %d\n", score, level, multiplier, player_health);
}

// Display the game with an ASCII border and mountain terrain at the bottom
void printGame(char gameBoard[HEIGHT][WIDTH], int score, int level, int multiplier, int player_health) {
    clearScreen();
    printScoreAndLevel(score, level, multiplier, player_health);  // Display score, level, multiplier, and health

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

    // Bottom border and mountain terrain
    printf("+");
    for (int i = 0; i < WIDTH; i++) {
        if (i % 6 == 0)
            printf("%c", MOUNTAIN_LEFT);
        else if (i % 6 == 3)
            printf("%c", MOUNTAIN_RIGHT);
        else
            printf("%c", MOUNTAIN_FLAT);
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

// Spawn a new weak enemy
void spawnWeakEnemy(Entity *enemy) {
    enemy->x = WIDTH - 1;
    enemy->y = rand() % (HEIGHT - 3);
    enemy->is_alive = 1;
    enemy->health = ENEMY_HEALTH_WEAK;
}

// Spawn a new strong enemy
void spawnStrongEnemy(Entity *enemy) {
    enemy->x = WIDTH - 1;
    enemy->y = rand() % (HEIGHT - 3);
    enemy->is_alive = 1;
    enemy->health = ENEMY_HEALTH_STRONG;
}

// Spawn a shooting enemy that can fire shots
void spawnShootingEnemy(Entity *enemy) {
    enemy->x = WIDTH - 1;
    enemy->y = rand() % (HEIGHT - 3);
    enemy->is_alive = 1;
    enemy->health = ENEMY_HEALTH_STRONG;
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
    powerUp->y = rand() % (HEIGHT - 3);  // Power-ups also spawn above the mountains
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

// Move all enemy shots across the screen
void moveEnemyShots(Entity enemy_shots[], int shotCount) {
    for (int i = 0; i < shotCount; i++) {
        if (enemy_shots[i].is_alive) {
            enemy_shots[i].x--;
            if (enemy_shots[i].x < 0) {
                enemy_shots[i].is_alive = 0;
            }
        }
    }
}

// Update the game board with the positions of all entities
void updateBoard(char gameBoard[HEIGHT][WIDTH], Entity *player, Entity enemies[], int enemyCount, Entity obstacles[], int obstacleCount, Entity shots[], int shotCount, Entity enemy_shots[], int enemyShotCount, Entity *powerUp) {
    initBoard(gameBoard);
    gameBoard[player->y][player->x] = PLAYER_SYMBOL;

    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i].is_alive) {
            char enemy_symbol = (enemies[i].health == ENEMY_HEALTH_STRONG) ? STRONG_ENEMY_SYMBOL : WEAK_ENEMY_SYMBOL;
            gameBoard[enemies[i].y][enemies[i].x] = enemy_symbol;
        }
    }

    for (int i = 0; i < obstacleCount; i++) {
        if (obstacles[i].is_alive) {
            gameBoard[obstacles[i].y][obstacles[i].x] = '#';
        }
    }

    for (int i = 0; i < shotCount; i++) {
        if (shots[i].is_alive) {
            gameBoard[shots[i].y][shots[i].x] = PLAYER_SHOT_SYMBOL;
        }
    }

    for (int i = 0; i < enemyShotCount; i++) {
        if (enemy_shots[i].is_alive) {
            gameBoard[enemy_shots[i].y][enemy_shots[i].x] = ENEMY_SHOT_SYMBOL;
        }
    }

    if (powerUp->is_alive) {
        gameBoard[powerUp->y][powerUp->x] = POWERUP_SYMBOL;
    }
}

// Check if two entities collide
int checkCollision(Entity *entity1, Entity *entity2) {
    return entity1->x == entity2->x && entity1->y == entity2->y;
}

int main() {
    char gameBoard[HEIGHT][WIDTH];
    Entity player = {1, HEIGHT / 2, 1, PLAYER_MAX_HEALTH};  // Player starts in the middle with full health
    Entity enemies[20];  // Max 20 enemies
    Entity obstacles[5];  // Max 5 obstacles
    Entity shots[MAX_SHOTS];  // Max 5 shots at a time
    Entity enemy_shots[MAX_ENEMY_SHOTS];  // Max 5 enemy shots at a time
    Entity powerUp = {0, 0, 0};  // No active power-up at the start
    PowerUpState powerUpState = {0, 0};  // No active power-up initially

    int score = 0, level = 1, enemiesKilled = 0;
    int enemiesPerWave = 1;  // Number of enemies per wave
    int enemySpeed = 3;  // Initial enemy speed
    int multiplier = 1;  // Scoring multiplier

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
    for (int i = 0; i < MAX_ENEMY_SHOTS; i++) {
        enemy_shots[i].is_alive = 0;
    }

    showIntroduction();  // Show introduction and controls

    int enemySpawnTimer = 0;
    int obstacleSpawnTimer = 0;
    int powerUpSpawnTimer = 0;
    int enemyShootTimer = 0;

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
                        break;
                    }
                }
            }
        }

        // Move enemies, obstacles, shots, and power-up
        moveEnemies(enemies, 20, enemySpeed);
        moveObstacles(obstacles, 5);
        moveShots(shots, MAX_SHOTS);
        movePowerUp(&powerUp);
        moveEnemyShots(enemy_shots, MAX_ENEMY_SHOTS);

        // Spawn enemies, obstacles, and power-ups
        if (enemySpawnTimer >= 10) {
            for (int i = 0; i < enemiesPerWave; i++) {
                for (int j = 0; j < 20; j++) {
                    if (!enemies[j].is_alive) {
                        if (rand() % 2 == 0)
                            spawnWeakEnemy(&enemies[j]);
                        else if (rand() % 4 == 0)
                            spawnShootingEnemy(&enemies[j]);  // 25% chance for a shooting enemy
                        else
                            spawnStrongEnemy(&enemies[j]);
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

        // Enemy shooting behavior
        if (enemyShootTimer >= 30) {
            for (int i = 0; i < 20; i++) {
                if (enemies[i].is_alive && enemies[i].health == ENEMY_HEALTH_STRONG) {  // Only shooting enemies
                    for (int j = 0; j < MAX_ENEMY_SHOTS; j++) {
                        if (!enemy_shots[j].is_alive) {
                            enemy_shots[j].x = enemies[i].x - 1;
                            enemy_shots[j].y = enemies[i].y;
                            enemy_shots[j].is_alive = 1;
                            break;
                        }
                    }
                }
            }
            enemyShootTimer = 0;
        } else {
            enemyShootTimer++;
        }

        // Check collisions (player/enemy, player/obstacle, player/enemy shot, shot/enemy, player/power-up)
        for (int i = 0; i < 20; i++) {
            if (enemies[i].is_alive && checkCollision(&player, &enemies[i])) {
                player.health--;
                enemies[i].is_alive = 0;
                if (player.health <= 0) {
                    printf("Game Over! Final Score: %d\n", score);
                    return 0;
                }
            }
            for (int j = 0; j < MAX_SHOTS; j++) {
                if (enemies[i].is_alive && shots[j].is_alive && checkCollision(&shots[j], &enemies[i])) {
                    shots[j].is_alive = 0;
                    enemies[i].health--;
                    if (enemies[i].health <= 0) {
                        enemies[i].is_alive = 0;
                        score += 10 * multiplier;  // Scoring multiplier
                        enemiesKilled++;
                    }
                }
            }
        }

        for (int i = 0; i < MAX_ENEMY_SHOTS; i++) {
            if (enemy_shots[i].is_alive && checkCollision(&player, &enemy_shots[i])) {
                player.health--;
                enemy_shots[i].is_alive = 0;
                if (player.health <= 0) {
                    printf("Game Over! Final Score: %d\n", score);
                    return 0;
                }
            }
        }

        for (int i = 0; i < 5; i++) {
            if (obstacles[i].is_alive && checkCollision(&player, &obstacles[i])) {
                player.health--;
                obstacles[i].is_alive = 0;
                if (player.health <= 0) {
                    printf("Game Over! Final Score: %d\n", score);
                    return 0;
                }
            }
        }

        if (powerUp.is_alive && checkCollision(&player, &powerUp)) {
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

        // Update and display the game
        updateBoard(gameBoard, &player, enemies, 20, obstacles, 5, shots, MAX_SHOTS, enemy_shots, MAX_ENEMY_SHOTS, &powerUp);
        printGame(gameBoard, score, level, multiplier, player.health);

        SLEEP(100);
    }

    return 0;
}
