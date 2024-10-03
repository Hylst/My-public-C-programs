//Text Platformer
// v0.8 
// by Hylst (1996 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Display management not finished
// A lot more to do
// Maybe more platfor / side scrolling beat them all like ...

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #include <mmsystem.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

#define WIDTH 80
#define HEIGHT 25
#define LEVEL_LENGTH 800
#define PLAYER_IDLE "O-|"
#define PLAYER_RUNNING_LEFT "O<<"
#define PLAYER_RUNNING_RIGHT "O>>"
#define PLAYER_SHOOT "O-|>"
#define PLAYER_DEAD "X_X"
#define ENEMY "><>"
#define BOSS_IDLE "BOSS"
#define BOSS_ATTACK "B-|-"
#define OBSTACLE "###"
#define EMPTY ' '
#define BULLET '*'
#define BOSS_BULLET '@'
#define GROUND "===="
#define PLATFORM "==="

typedef struct {
    int x, y;
} Position;

typedef struct {
    int x, y;
    int hp;
    int isBoss;
    int speed;
    int attackCooldown;
    int bossBulletX, bossBulletY;
    int bossBulletActive;
    char *sprite;
} EnemyState;

typedef struct {
    int x, y;
    int jumping;
    int jumpHeight;
    int jumpPeak;
    int falling;
    char *sprite;
} PlayerState;

PlayerState player = {2, HEIGHT - 2, 0, 0, 5, 0, PLAYER_IDLE};
Position bullet = {-1, -1};
int playerLives = 3;
int playerMaxLives = 5;
int score = 0;
int gameOver = 0;
int currentLevel = 0;
int bulletActive = 0;
int levelScrollOffset = 0;
int gamePaused = 0;
int checkpointReached = 0;

// Enemies and Obstacles
EnemyState enemies[10];  
int enemyCount = 0;
char level[HEIGHT][LEVEL_LENGTH];

void displayIntro();
void displayGameOver(int win);
void processInput();
int isObstacle(int x, int y);

// Function to check if a position contains an obstacle
int isObstacle(int x, int y) {
    if (level[y][x] == OBSTACLE[0]) {
        return 1;
    }
    return 0;
}

// Platform-specific sound generation
void playSound(const char* sound) {
    #ifdef _WIN32
        if (strcmp(sound, "jump") == 0) Beep(1000, 100);
        else if (strcmp(sound, "shoot") == 0) Beep(1500, 100);
        else if (strcmp(sound, "run") == 0) Beep(500, 50);
        else if (strcmp(sound, "death") == 0) Beep(300, 500);
        else if (strcmp(sound, "boss_shoot") == 0) Beep(2000, 100);
        else if (strcmp(sound, "checkpoint") == 0) Beep(800, 200);
    #else
        printf("\a");
    #endif
}

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

// Cross-platform input detection for UNIX
int _kbhit() {
    #ifdef _WIN32
      //  return _kbhit();  // Fonction native Windows
    #else
        struct termios oldt, newt;
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt);         // Récupère l'état actuel
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);       // Mode sans écho
        tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Applique les modifications
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // Non-bloquant

        ch = getchar();

        // Restaurer l'état initial
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if (ch != EOF) {
            ungetc(ch, stdin);
            return 1;  // Une touche a été pressée
        }

        return 0;  // Pas de touche pressée
    #endif
}

// Wait for a specific keypress
char waitForKeypress() {
    #ifdef _WIN32
        return _getch();  // Utilise _getch() sous Windows
    #else
        return getchar(); // Utilise getchar() sous UNIX
    #endif
    }

// Generate a random level with enemies, bosses, and obstacles
void generateLevel(int levelNum) {
    srand(time(NULL) + levelNum);
    enemyCount = (levelNum % 5 == 0) ? 1 : 5 + levelNum;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < LEVEL_LENGTH; x++) {
            if (y == HEIGHT - 1) {
                level[y][x] = GROUND[0];
            } else if (rand() % 100 < 10 && y != 0 && y != HEIGHT - 1) {
                level[y][x] = PLATFORM[0];
            } else if (rand() % 100 < 5) {
                level[y][x] = OBSTACLE[0];
            } else {
                level[y][x] = EMPTY;
            }
        }
    }

    // Place enemies or boss
    for (int i = 0; i < enemyCount; i++) {
        enemies[i].x = rand() % (LEVEL_LENGTH - 20) + 20;
        enemies[i].y = HEIGHT - 2;
        enemies[i].hp = (levelNum % 5 == 0) ? 10 + levelNum : 2 + levelNum / 2;
        enemies[i].isBoss = (levelNum % 5 == 0) ? 1 : 0;
        enemies[i].speed = 1 + (levelNum / 2);
        enemies[i].attackCooldown = 100 - (levelNum * 5);
        enemies[i].bossBulletX = -1;
        enemies[i].bossBulletY = -1;
        enemies[i].bossBulletActive = 0;
        enemies[i].sprite = BOSS_IDLE;
    }
}

// Display level transition narrative
void displayLevelNarrative(int levelNum) {
    clearScreen();
    if (levelNum % 5 == 0) {
        printf("LEVEL %d - A mighty boss stands in your way!\n", levelNum);
    } else {
        printf("LEVEL %d - You continue your quest to save your love...\n", levelNum);
    }
    printf("Press any key to continue...\n");
    waitForKeypress();
}

// Draw the current segment of the game board with a border
void drawBoard() {
    if (gamePaused) {
        return;
    }
    
    clearScreen();

    printf("##################################################\n");

    for (int y = 0; y < HEIGHT; y++) {
        printf("#");
        for (int x = 0; x < WIDTH; x++) {
            int levelX = x + levelScrollOffset;
            if (levelX == player.x && y == player.y) {
                printf("%s", player.sprite);
            } else if (bulletActive && levelX == bullet.x && y == bullet.y) {
                printf("%c", BULLET);
            } else {
                int isEnemy = 0;
                for (int i = 0; i < enemyCount; i++) {
                    if (levelX == enemies[i].x && y == enemies[i].y) {
                        printf("%s", enemies[i].sprite);
                        isEnemy = 1;
                        break;
                    }
                }
                if (!isEnemy) {
                    if (y == HEIGHT - 1) {
                        printf("%s", GROUND);
                    } else {
                        printf("%c", level[y][levelX]);
                    }
                }
            }
        }
        printf("#\n");
    }

    printf("##################################################\n");
    printf("Lives: %d  Score: %d  Level: %d  Checkpoint: %s\n", playerLives, score, currentLevel, checkpointReached ? "YES" : "NO");
}

void processInput() {
    if (_kbhit()) {
        char key = _getch();  
        int nextX = player.x, nextY = player.y;
        
        if (key == 'q' && player.x > 0) {
            nextX--;
            player.sprite = PLAYER_RUNNING_LEFT;
            playSound("run");
        }
        if (key == 'd' && player.x < LEVEL_LENGTH - 1) {
            nextX++;
            player.sprite = PLAYER_RUNNING_RIGHT;
            playSound("run");
        }
        if (key == 'z' && !player.jumping && !player.falling) {
            player.jumping = 1;
            player.jumpHeight = 0;
            player.sprite = PLAYER_IDLE;
            playSound("jump");
        }
        if (key == 's') {
            nextY++;
        }
        if (key == 'p') {
            gamePaused = !gamePaused;
        }
        if (key == 27) {  // ESC key
            gameOver = 1;
        }
        if (key == ' ' && !bulletActive) {
            bullet.x = player.x;
            bullet.y = player.y - 1;
            bulletActive = 1;
            player.sprite = PLAYER_SHOOT;
            playSound("shoot");
        }

        // Check if the next position is blocked by an obstacle
        if (!isObstacle(nextX, player.y)) {
            player.x = nextX;
            if (player.x < levelScrollOffset) {
                levelScrollOffset--;
            }
            if (player.x >= levelScrollOffset + WIDTH - 1) {
                levelScrollOffset++;
            }
        }
    }

    wait(50);  
}

// Handle gravity and jumping logic
void handleGravity() {
    if (player.jumping) {
        player.jumpHeight++;
        player.y--;
        if (player.jumpHeight >= player.jumpPeak) {
            player.jumping = 0;
            player.falling = 1;
        }
    } else if (player.falling) {
        player.y++;
        if (player.y >= HEIGHT - 2) {
            player.y = HEIGHT - 2;
            player.falling = 0;
            player.sprite = PLAYER_IDLE;
        }
    }
}



// Boss attack logic, considering difficulty (speed and attack frequency)
void bossAttackLogic(EnemyState *boss) {
    if (!boss->isBoss || boss->hp <= 0) return;

    boss->attackCooldown--;
    if (boss->attackCooldown <= 0 && !boss->bossBulletActive) {
        boss->bossBulletX = boss->x;
        boss->bossBulletY = boss->y;
        boss->bossBulletActive = 1;
        boss->attackCooldown = 100 - (currentLevel * 5);
        boss->sprite = BOSS_ATTACK;
        playSound("boss_shoot");
    }

    // Move boss's bullet towards player
    if (boss->bossBulletActive) {
        boss->bossBulletY += boss->speed;
        if (boss->bossBulletY >= HEIGHT || (boss->bossBulletY == player.y && boss->bossBulletX == player.x)) {
            if (boss->bossBulletY == player.y && boss->bossBulletX == player.x) {
                playerLives--;
                if (playerLives <= 0) {
                    player.sprite = PLAYER_DEAD;
                    playSound("death");
                    gameOver = 1;
                    return;
                }
            }
            boss->bossBulletActive = 0;
            boss->sprite = BOSS_IDLE;
        }
    }
}

void displayIntro() {
    clearScreen();
    printf("****************************************************\n");
    printf("*                PLATFORMER GAME                   *\n");
    printf("*           Quest for your love's rescue!          *\n");
    printf("*   Controls:                                      *\n");
    printf("*   z: Jump                                        *\n");
    printf("*   q: Left                                        *\n");
    printf("*   d: Right                                       *\n");
    printf("*   s: Down                                        *\n");
    printf("*   p: Pause                                       *\n");
    printf("*   ESC: Quit                                      *\n");
    printf("****************************************************\n");
    printf("Press ENTER to start!\n");
    waitForKeypress();
}

void displayGameOver(int win) {
    clearScreen();
    if (win) {
        printf("****************************************************\n");
        printf("*                    YOU WIN!                      *\n");
        printf("*              Your score: %d                      *\n", score);
        printf("****************************************************\n");
    } else {
        printf("****************************************************\n");
        printf("*                  GAME OVER!                      *\n");
        printf("*              Your score: %d                      *\n", score);
        printf("****************************************************\n");
    }
    printf("Press any key to exit...\n");
    waitForKeypress();
}

// Move bullet and check collision
void moveBullet() {
    if (bulletActive) {
        bullet.y--;
        if (bullet.y < 0) {
            bulletActive = 0;
        } else {
            for (int i = 0; i < enemyCount; i++) {
                if (bullet.x == enemies[i].x && bullet.y == enemies[i].y) {
                    enemies[i].hp--;
                    if (enemies[i].hp <= 0) {
                        score += (enemies[i].isBoss) ? 1000 : 100;
                        enemies[i].x = -1;
                    }
                    bulletActive = 0;
                }
            }
        }
    }
}

// Check if the level is complete (all enemies defeated)
int isLevelComplete() {
    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i].x != -1) {
            return 0;
        }
    }
    return 1;
}

// Checkpoint logic: Save position after reaching halfway point
void checkCheckpoint() {
    if (player.x > LEVEL_LENGTH / 2 && !checkpointReached) {
        checkpointReached = 1;
        printf("Checkpoint reached! You gained 1 extra life!\n");
        playSound("checkpoint");
        if (playerLives < playerMaxLives) {
            playerLives++;
        }
        wait(1000);
    }
}

// Main game loop
void gameLoop() {
    generateLevel(currentLevel);
    displayLevelNarrative(currentLevel);
    
    while (!gameOver) {
        if (!gamePaused) {
            drawBoard();
            processInput();
            handleGravity();
            moveBullet();
            checkCheckpoint();
            
            for (int i = 0; i < enemyCount; i++) {
                if (enemies[i].isBoss) {
                    bossAttackLogic(&enemies[i]);
                }
            }
        }

        if (isLevelComplete()) {
            currentLevel++;
            if (currentLevel > 10) {
                gameOver = 1;
                displayGameOver(1);
                return;
            }
            generateLevel(currentLevel);
            displayLevelNarrative(currentLevel);
        }

        wait(100);
    }

    displayGameOver(0);
}

// Main function
int main() {
    displayIntro();
    gameLoop();
    return 0;
}
