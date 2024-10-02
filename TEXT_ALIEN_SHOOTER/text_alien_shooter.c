// Text Alien Shooter
// v0.8
// by Hylst (1996 - 2024 (cleaning code / good code practice / comments / cross platform ) )

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
    #include <conio.h>   // For _kbhit() and _getch() on Windows
    #include <windows.h> // For Sleep(), Beep(), and SetConsoleOutputCP on Windows
#else
    #include <unistd.h>  // For usleep() on Unix
    #include <termios.h> // For non-blocking input on Unix
    #include <fcntl.h>
#endif

#define WIDTH 40
#define HEIGHT 25  // Reduced height to 25
#define PLAYER_CHAR 'U'
#define ALIEN_CHAR 'X'
#define BULLET_CHAR '|'
#define ALIEN_BULLET_CHAR '*'
#define EMPTY ' '
#define BORDER_HORIZONTAL '-'
#define BORDER_VERTICAL '|'
#define BORDER_CORNER '+'
#define MAX_BULLETS 5

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position position;
    int active;
} Bullet;

Position player = {WIDTH / 2, HEIGHT - 2};  // Player starts at the bottom row
Position aliens[50];
Bullet bullets[MAX_BULLETS];  // Allow up to 5 bullets simultaneously
Bullet alienBullets[50];
int alienBulletCount = 0;
int alienCount = 5;
int score = 0;
int gameOver = 0;
int paused = 0;
int level = 1;
int alienDirection = 1;
int alienMoveCooldown = 0;
int alienShootCooldown = 0;
int difficultyDelay = 200;
int oscillationRange = 1;  // Aliens oscillate horizontally by 1 case at level 1

// Cross-platform screen clear
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Cross-platform delay
void wait(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}

// Cross-platform sound
void playSound(int frequency, int duration) {
    #ifdef _WIN32
        Beep(frequency, duration);
    #else
        printf("\a");  // Unix beep
    #endif
}

// Unix-specific non-blocking input
#ifndef _WIN32
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

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif

// Initialize aliens
void initAliens() {
    for (int i = 0; i < alienCount; i++) {
        aliens[i].x = i * (WIDTH / alienCount);  // Spread aliens horizontally
        aliens[i].y = 1;  // Start from the second-highest row (y=1)
    }
}

// Initialize bullets
void initBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
}

// Initialize alien bullets
void initAlienBullets() {
    for (int i = 0; i < alienCount; i++) {
        alienBullets[i].active = 0;
    }
}

// Display the game buffer
void render(char buffer[HEIGHT][WIDTH]) {
    clearScreen();  // Clear only at the start of rendering

    // Top border
    printf("%c", BORDER_CORNER);
    for (int i = 0; i < WIDTH; i++) printf("%c", BORDER_HORIZONTAL);
    printf("%c\n", BORDER_CORNER);

    // Render game content with side borders
    for (int y = 0; y < HEIGHT; y++) {
        printf("%c", BORDER_VERTICAL);  // Left border
        for (int x = 0; x < WIDTH; x++) {
            printf("%c", buffer[y][x]);
        }
        printf("%c\n", BORDER_VERTICAL);  // Right border
    }

    // Bottom border
    printf("%c", BORDER_CORNER);
    for (int i = 0; i < WIDTH; i++) printf("%c", BORDER_HORIZONTAL);
    printf("%c\n", BORDER_CORNER);

    printf(".-=(Score: %d )=-.  ", score);
    printf(".-=(Level: %d )=-.", level);
}

// Display the introduction menu
void displayMenu() {
    clearScreen();
    printf("########################################\n");
    printf("#           WELCOME TO ALIEN           #\n");
    printf("#              SHOOTER!                #\n");
    printf("########################################\n");
    printf("# Controls:                            #\n");
    printf("#  'q' - Move left                     #\n");
    printf("#  'd' - Move right                    #\n");
    printf("#  'z' - Shoot (up to 5 bullets)       #\n");
    printf("#  'p' - Pause                         #\n");
    printf("#  'ESC' - Quit the game               #\n");
    printf("########################################\n");
    printf("Press ENTER to start the game...\n");
    getchar();  // Wait for any key press to start
}

// Move player based on input
void processInput() {
    if (_kbhit()) {
        char key = 
        #ifdef _WIN32
            _getch(); // No ENTER required
        #else
            getchar();
        #endif

        if (key == 'q' && player.x > 0) player.x--;     // Move left
        if (key == 'd' && player.x < WIDTH - 1) player.x++;  // Move right
        if (key == 'z') {  // Fire bullet
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!bullets[i].active) {
                    bullets[i].position.x = player.x;
                    bullets[i].position.y = player.y - 1;
                    bullets[i].active = 1;
                    playSound(1000, 100);
                    break;
                }
            }
        }
        if (key == 'p') paused = !paused;  // Toggle pause
        if (key == 27) gameOver = 1;  // Quit the game
    }
}

// Move aliens in a progressive difficulty pattern
void moveAliens() {
    if (alienMoveCooldown > 0 || paused) {
        alienMoveCooldown--;
        return;
    }

    // Horizontal oscillation for level 1
    for (int i = 0; i < alienCount; i++) {
        aliens[i].x += alienDirection;
        if (aliens[i].x <= 0 || aliens[i].x >= WIDTH - 1) {
            alienDirection = -alienDirection;  // Change direction when hitting screen edges
            break;
        }
    }

    alienMoveCooldown = difficultyDelay / 10;
}

// Move player bullets and handle collision
void moveBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].position.y--;
            if (bullets[i].position.y < 0) {
                bullets[i].active = 0;
            }
            for (int j = 0; j < alienCount; j++) {
                if (bullets[i].position.x == aliens[j].x && bullets[i].position.y == aliens[j].y) {
                    score++;
                    aliens[j].x = rand() % WIDTH;  // Respawn alien at random x position
                    aliens[j].y = 1;  // Reset y position
                    bullets[i].active = 0;
                    playSound(1500, 200);  // Hit sound
                }
            }
        }
    }
}

// Move alien bullets and handle collision
void moveAlienBullets() {
    for (int i = 0; i < alienCount; i++) {
        if (alienBullets[i].active) {
            alienBullets[i].position.y++;
            if (alienBullets[i].position.y >= HEIGHT) {
                alienBullets[i].active = 0;
            }
            if (alienBullets[i].position.x == player.x && alienBullets[i].position.y == player.y) {
                gameOver = 1;  // Player is hit
                playSound(200, 500);
            }
        }
    }
}

// Alien random shooting logic
void alienShoot() {
    if (alienShootCooldown > 0 || paused) {
        alienShootCooldown--;
        return;
    }
    for (int i = 0; i < alienCount; i++) {
        if (rand() % 100 < level * 5) {  // Increase shooting frequency with level
            alienBullets[i].position.x = aliens[i].x;
            alienBullets[i].position.y = aliens[i].y + 1;
            alienBullets[i].active = 1;
        }
    }
    alienShootCooldown = difficultyDelay / 5;  // Shooting cooldown depends on difficulty
}

// Update the difficulty and alien count based on the current level
void updateDifficulty() {
    if (score >= level * 5) {  // Increase difficulty every 5 points
        level++;
        alienCount += (level >= 5) ? 2 : 0;  // Add 2 aliens at level 5
        difficultyDelay = (difficultyDelay > 50) ? difficultyDelay - 20 : 50;  // Speed up aliens
        initAliens();  // Reinitialize aliens with more count
        initAlienBullets();  // Reset alien bullets
    }
}

// Main game loop
int main() {
    srand(time(NULL));  // Initialize random seed for alien shooting

    displayMenu();  // Show the introduction menu

    char buffer[HEIGHT][WIDTH];  // Buffer for double-buffering screen

    initAliens();
    initBullets();
    initAlienBullets();

    while (!gameOver) {
        if (paused) {
            wait(10000);  // Wait while paused
            paused = FALSE;
            continue;
        }

        // Clear the buffer
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                buffer[y][x] = EMPTY;
            }
        }

        // Place the player in the buffer
        buffer[player.y][player.x] = PLAYER_CHAR;

        // Place the bullets in the buffer if active
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                buffer[bullets[i].position.y][bullets[i].position.x] = BULLET_CHAR;
            }
        }

        // Place the aliens in the buffer
        for (int i = 0; i < alienCount; i++) {
            buffer[aliens[i].y][aliens[i].x] = ALIEN_CHAR;
        }

        // Place alien bullets in the buffer if active
        for (int i = 0; i < alienCount; i++) {
            if (alienBullets[i].active) {
                buffer[alienBullets[i].position.y][alienBullets[i].position.x] = ALIEN_BULLET_CHAR;
            }
        }

        // Render the buffer to the screen
        render(buffer);

        // Handle game mechanics
        processInput();  // Player input
        moveBullets();  // Move player bullets
        moveAliens();  // Move aliens based on level logic
        alienShoot();  // Aliens shoot based on level logic
        moveAlienBullets();  // Move alien bullets
        updateDifficulty();  // Adjust difficulty as score increases

        wait(100);  // Control game speed
    }

    // Game over message
    clearScreen();
    printf("Game Over! Final Score: %d\n", score);
    return 0;
}
