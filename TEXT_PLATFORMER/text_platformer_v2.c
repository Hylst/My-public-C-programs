//Text Platformer
// v0.9 
// by Hylst (1996 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Big change of game style :
/*
 ._-[ JEU D AVENTURE - BEAT THEM ALL - DE PLATEFORME EN ASCII ART ]-_. 
========================================================================

                                       ============                                     
                                     
                               =======                   ====
                                                                  B
                       =======                                  =====
                   L                       ~*~
                 ======     =======     =========
                 
                  >8-*
 _O_          ===========  
/||\         =           =     D|
 /\    ==   =             =    /\                  ^^^^^       ~*~
=========================================================================

Avec 

 _O_      
/||\  le héro en ascii art   
 /\ 

= sol, plafond et plateformes

  =          = 
 =  escaliers =
=              =

^^^^^  pics mortels au sol

>8-*  ennemi qui tire

 o
@|  ennemi qui frappe et avance lentement
/\

~*~  ennemi qui empoisonne au contact et se déplace plus vite

B Boost de tir (+1)
L Boost de vie (+1)

3 vies et 3 tirs au départ

A lot to do
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

#define WIDTH 80  // Largeur de la fenêtre d'affichage
#define HEIGHT 20 // Hauteur de la fenêtre d'affichage
#define LEVEL_LENGTH 80  // Longueur d'un niveau visible
#define MAX_LEVELS 4     // Nombre de niveaux dans le jeu

// Sprites en ASCII art
#define HERO_HEAD "_O_"
#define HERO_BODY "/||\\"
#define HERO_LEGS "/\\"
#define ENEMY_SHOOTER ">8-*"
#define ENEMY_MELEE " o \n@|\\ \n/\\"
#define ENEMY_POISON "~*~"
#define SPIKES "^^^^^"
#define GROUND "===="
#define PLATFORM "==="

// Déclaration des structures
typedef struct {
    int x, y;
} Position;

typedef struct {
    Position pos;
    int jump;
    int onGround;
    int onStairs;
    int lives;
    int projectiles;
    char *head;
    char *body;
    char *legs;
} Hero;

typedef struct {
    Position pos;
    char *sprite;
    int active;
} Enemy;

typedef struct {
    Position pos;
    char type; // 'B' = Boost tir, 'L' = Boost vie
    int collected;
} Boost;

// Variables globales
Hero hero = {{2, HEIGHT - 3}, 0, 1, 0, 3, 3, HERO_HEAD, HERO_BODY, HERO_LEGS};
Enemy enemies[5];
Boost boosts[2];
char level[HEIGHT][LEVEL_LENGTH];
int currentLevel = 1;
int gameOver = 0;
int gamePaused = 0;

// Cross-platform clear screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Fonction pour afficher le plateau de jeu (gameboard)
void drawBoard() {
    clearScreen();
    printf("Niveau: %d | Vies: %d | Projectiles: %d\n", currentLevel, hero.lives, hero.projectiles);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int levelX = x;
            if (levelX == hero.pos.x && y == hero.pos.y - 2) {
                printf("%s", hero.head);
                x += 2;
            } else if (levelX == hero.pos.x && y == hero.pos.y - 1) {
                printf("%s", hero.body);
                x += 3;
            } else if (levelX == hero.pos.x && y == hero.pos.y) {
                printf("%s", hero.legs);
                x += 1;
            } else {
                int isEnemy = 0;
                for (int i = 0; i < 5; i++) {
                    if (enemies[i].active && levelX == enemies[i].pos.x && y == enemies[i].pos.y) {
                        printf("%s", enemies[i].sprite);
                        isEnemy = 1;
                        break;
                    }
                }

                if (!isEnemy) {
                    if (y == HEIGHT - 1) {
                        printf("%s", GROUND);
                        x += 3;
                    } else if (level[y][levelX] == '=') {
                        printf("%s", PLATFORM);
                        x += 2;
                    } else {
                        printf(" ");
                    }
                }
            }
        }
        printf("\n");
    }
}

// Fonction pour gérer l'entrée utilisateur et mouvement du héros
void handleInput() {
    if (_kbhit()) {
        char key = _getch();

        if (key == 'd') {  // Droite
            hero.pos.x++;
        } else if (key == 'a') {  // Gauche
            hero.pos.x--;
        } else if (key == 'w' && hero.onStairs) {  // Monter escaliers
            hero.pos.y--;
        } else if (key == 's' && hero.onStairs) {  // Descendre escaliers
            hero.pos.y++;
        } else if (key == ' ') {  // Sauter
            if (hero.onGround) {
                hero.jump = 3;
                hero.onGround = 0;
            }
        }
    }
}

// Gestion de la gravité et des plateformes
void handlePhysics() {
    if (hero.jump > 0) {
        hero.pos.y--;
        hero.jump--;
    } else {
        if (!hero.onGround) {
            hero.pos.y++;
        }
    }

    if (level[hero.pos.y + 1][hero.pos.x] == '=' || hero.pos.y == HEIGHT - 2) {
        hero.onGround = 1;
    } else {
        hero.onGround = 0;
    }

    if (level[hero.pos.y][hero.pos.x] == '=') {
        hero.onStairs = 1;
    } else {
        hero.onStairs = 0;
    }
}

// Générer les niveaux en fonction du numéro de niveau
void generateLevel(int levelNumber) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < LEVEL_LENGTH; x++) {
            level[y][x] = ' ';
        }
    }

    hero.pos.x = 2;
    hero.pos.y = HEIGHT - 3;

    // Niveau 1 : Escalier et plateforme
    if (levelNumber == 1) {
        level[HEIGHT - 5][10] = '=';
        level[HEIGHT - 6][10] = '=';
        level[HEIGHT - 7][10] = '=';
        level[HEIGHT - 10][20] = '=';
    }
    // Niveau 2 : Un ennemi au sol sous une plateforme
    else if (levelNumber == 2) {
        enemies[0].pos.x = 30;
        enemies[0].pos.y = HEIGHT - 2;
        enemies[0].sprite = ENEMY_MELEE;
        enemies[0].active = 1;
        level[HEIGHT - 5][25] = '=';
    }
    // Niveau 3 : Escalier, un ennemi sur une plateforme, et un boost tir
    else if (levelNumber == 3) {
        level[HEIGHT - 5][15] = '=';
        level[HEIGHT - 6][15] = '=';
        level[HEIGHT - 7][15] = '=';
        enemies[1].pos.x = 40;
        enemies[1].pos.y = HEIGHT - 10;
        enemies[1].sprite = ENEMY_SHOOTER;
        enemies[1].active = 1;
        level[HEIGHT - 10][45] = '=';
        boosts[0].pos.x = 60;
        boosts[0].pos.y = HEIGHT - 10;
        boosts[0].type = 'B';
        boosts[0].collected = 0;
    }
    // Niveau 4 : Deux ennemis et un boost vie
    else if (levelNumber == 4) {
        enemies[2].pos.x = 20;
        enemies[2].pos.y = HEIGHT - 2;
        enemies[2].sprite = ENEMY_POISON;
        enemies[2].active = 1;
        enemies[3].pos.x = 50;
        enemies[3].pos.y = HEIGHT - 10;
        enemies[3].sprite = ENEMY_SHOOTER;
        enemies[3].active = 1;
        level[HEIGHT - 10][55] = '=';
        boosts[1].pos.x = 75;
        boosts[1].pos.y = HEIGHT - 10;
        boosts[1].type = 'L';
        boosts[1].collected = 0;
    }
}

// Gestion des collisions avec les ennemis et objets
void handleCollisions() {
    for (int i = 0; i < 5; i++) {
        if (enemies[i].active &&
            hero.pos.x >= enemies[i].pos.x &&
            hero.pos.x <= enemies[i].pos.x + 2 &&
            hero.pos.y == enemies[i].pos.y) {
            hero.lives--;
            enemies[i].active = 0;
        }
    }

    for (int i = 0; i < 2; i++) {
        if (!boosts[i].collected &&
            hero.pos.x == boosts[i].pos.x &&
            hero.pos.y == boosts[i].pos.y) {
            if (boosts[i].type == 'B') {
                hero.projectiles++;
            } else if (boosts[i].type == 'L') {
                hero.lives++;
            }
            boosts[i].collected = 1;
        }
    }
}

// Boucle principale du jeu
void gameLoop() {
    while (!gameOver) {
        drawBoard();
        handleInput();
        handlePhysics();
        handleCollisions();

        if (hero.lives <= 0) {
            gameOver = 1;
        }

        if (hero.pos.x >= WIDTH) {
            currentLevel++;
            if (currentLevel > MAX_LEVELS) {
                gameOver = 1;
            } else {
                generateLevel(currentLevel);
            }
        }

        Sleep(100);  // Contrôle de la vitesse de jeu
    }

    printf("Game Over!\n");
}

int main() {
    srand(time(NULL));
    generateLevel(currentLevel);
    gameLoop();
    return 0;
}
