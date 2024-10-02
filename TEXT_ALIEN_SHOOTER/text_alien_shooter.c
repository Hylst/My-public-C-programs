// Text Alien Shooter
// v1.0
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
#define HEIGHT 23  // Internal height set to 23 for a total height of 25 including borders
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
    int baseX, baseY;  // Base positions for "floating" movement
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
int aliensRemaining = 5;  // Number of aliens remaining in the current wave
int score = 0;
int gameOver = 0;
int paused = 0;
int level = 1;
int alienDirection = 1;
int alienMoveCooldown = 0;
int alienShootCooldown = 0;
int difficultyDelay = 200;
int alienDescentSpeed = 500;  // Speed at which aliens descend with levels
int oscillationRange = 1;  // Aliens oscillate around their base position

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
        printf("\a");  // Unix beep for sound
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

// Initialize aliens for the current wave based on the level
void initAliens() {
    aliensRemaining = alienCount;  // Set the number of aliens for the current wave
    for (int i = 0; i < alienCount; i++) {
        aliens[i].x = i * (WIDTH / alienCount);  // Spread aliens horizontally
        aliens[i].y = 1;  // Start from the second-highest row (y=1)
        aliens[i].baseX = aliens[i].x;  // Save the base position for oscillation
        aliens[i].baseY = aliens[i].y;
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

// Display ASCII art with level and score
void displayScoreAndLevel() {
    printf("Level: %d   Score: %d\n", level, score);
}

// Display a message for a new wave with animation and sound
void displayNewWaveMessage() {
    clearScreen();
    playSound(500, 300);  // Sound when new wave starts
    printf("\n*** NEW WAVE %d ***\n", level);
    printf("Get ready for the next challenge!\n");
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        wait(500);
    }
    printf("\n");
    wait(500);  // Brief pause before resuming the game
}

// Display the game buffer
void render(char buffer[HEIGHT][WIDTH]) {
    clearScreen();  // Clear only at the start of rendering
    displayScoreAndLevel();  // Show level and score

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
    printf("Press ENTER to begin...\n");
    
    // Wait for the user to press Enter
    while (getchar() != '\n');
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
        if (key == 'p') paused = !paused;  // Toggle pause; pressing p again resumes the game
        if (key == 27) gameOver = 1;  // ESC key to quit
    }
}

// Move aliens in a floating pattern with descent based on level
void moveAliens() {
    if (alienMoveCooldown > 0 || paused) {
        alienMoveCooldown--;
        return;
    }

    // Aliens float around their base position and descend progressively with level
    for (int i = 0; i < alienCount; i++) {
        // Float horizontally around the base X position
        if (rand() % 2 == 0) {
            aliens[i].x = aliens[i].baseX + (rand() % (oscillationRange * 2)) - oscillationRange;
            if (aliens[i].x < 0) aliens[i].x = 0;  // Ensure aliens don't leave the screen
            if (aliens[i].x >= WIDTH) aliens[i].x = WIDTH - 1;
        }
        
        // Descend progressively
        if (rand() % alienDescentSpeed == 0) {
            aliens[i].y++;  // Slowly move the aliens down
            if (aliens[i].y >= HEIGHT - 1) gameOver = 1;  // If an alien reaches the bottom, game over
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
                    aliens[j].x = -1;  // Mark alien as "dead" by moving it off-screen
                    aliensRemaining--;  // Reduce alien count in current wave
                    bullets[i].active = 0;
                    playSound(1500, 200);  // Hit sound
                }
            }
        }
    }
}

// Check if all aliens are dead, move to next level
void checkAliensDead() {
    if (aliensRemaining == 0) {
        level++;  // Go to the next level
        alienCount = (level <= 3) ? 5 : 7;  // 5 aliens for levels 1-3, 7 for levels 4-6, etc.
        difficultyDelay -= 20;  // Make the game faster
        if (difficultyDelay < 50) difficultyDelay = 50;  // Cap at a minimum delay
        alienDescentSpeed -= 50;  // Make descent faster
        if (alienDescentSpeed < 100) alienDescentSpeed = 100;  // Cap descent speed
        displayNewWaveMessage();  // Display the new wave message
        initAliens();  // Reinitialize aliens for the new wave
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
        if (rand() % 100 < level * 5 && aliens[i].x >= 0) {  // Only allow active aliens to shoot
            alienBullets[i].position.x = aliens[i].x;
            alienBullets[i].position.y = aliens[i].y + 1;
            alienBullets[i].active = 1;
        }
    }
    alienShootCooldown = difficultyDelay / 5;  // Shooting cooldown depends on difficulty
}

// Display game over message and ask if user wants to restart
void gameOverScreen() {
    clearScreen();
    printf("\nOh no, the aliens got you!\n\n");
    printf("Your final score: %d\n", score);
    printf("\nDo you want to play again? (y/n): ");
    
    char choice;
    choice = getchar();
    if (choice == 'y' || choice == 'Y') {
        score = 0;
        level = 1;
        alienCount = 5;
        difficultyDelay = 200;
        alienDescentSpeed = 500;
        gameOver = 0;
        initAliens();
        initBullets();
        initAlienBullets();
    } else {
        printf("\nThanks for playing! Goodbye!\n");
        exit(0);  // Exit the game
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
            wait(100);  // Wait while paused
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
            if (aliens[i].x >= 0) {  // Only render active aliens
                buffer[aliens[i].y][aliens[i].x] = ALIEN_CHAR;
            }
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
        moveAliens();  // Move aliens in a floating pattern with descent
        alienShoot();  // Aliens shoot based on level logic
        moveAlienBullets();  // Move alien bullets
        checkAliensDead();  // Check if all aliens are dead, and move to next level if necessary

        wait(100);  // Control game speed
    }

    // Display game over screen and ask to restart
    gameOverScreen();

    return 0;
}
