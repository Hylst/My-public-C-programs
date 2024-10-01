// Text Alien Shooter
// v0.6
// by Hylst (1996 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Affichage jeu de caractères ascii étendu
// saisie non blocante de touche
// affichage stable
// game board 2x large
// Complete program - debug 

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <windows.h> // Pour Sleep et Beep sous Windows
#else
    #include <unistd.h>  // Pour usleep sous Unix
#endif

#define WIDTH 40
#define HEIGHT 20
#define PLAYER '^'
#define INVADER 'X'
#define BULLET '|'
#define EMPTY ' '
#define WALL '#'

// Structures
typedef struct {
    int x, y;
} Position;

typedef struct {
    Position invaders[50];
    int count;
    int direction; // 1 = droite, -1 = gauche
    int speed;
    int has_projectiles;
} InvaderWave;

// Variables globales
Position player, bullet;
InvaderWave invaderWave;
int score = 0, level = 1;
int gameOver = 0, bulletActive = 0;
char menuChoice = '1';

// Multiplateforme : Effacer l'écran
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Multiplateforme : Délai pour gérer la vitesse du jeu
void wait(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000); // Conversion de ms en µs pour usleep
    #endif
}

void playSound(int frequency, int duration) {
    #ifdef _WIN32
        Beep(frequency, duration);
    #else
        printf("\a"); // Son par défaut sur Unix (beep dans le terminal)
    #endif
}

void shoot() {
    // Son pour le tir
    playSound(1000, 100);
}

void enemyDestroyed() {
    // Son pour un ennemi détruit
    playSound(500, 200);
}

void gameOverSound() {
    // Son pour Game Over
    playSound(100, 500);
}

// Dessiner le tableau de jeu avec bordures
void drawBoard() {
    clearScreen();
    printf("╔");
    for (int i = 0; i < WIDTH; i++) printf("═");
    printf("╗\n");

    for (int y = 0; y < HEIGHT; y++) {
        printf("║");
        for (int x = 0; x < WIDTH; x++) {
            if (x == player.x && y == player.y) printf("%c", PLAYER);
            else if (bulletActive && x == bullet.x && y == bullet.y) printf("%c", BULLET);
            else {
                int isInvader = 0;
                for (int i = 0; i < invaderWave.count; i++) {
                    if (invaderWave.invaders[i].x == x && invaderWave.invaders[i].y == y) {
                        printf("%c", INVADER);
                        isInvader = 1;
                        break;
                    }
                }
                if (!isInvader) printf("%c", EMPTY);
            }
        }
        printf("║\n");
    }

    printf("╚");
    for (int i = 0; i < WIDTH; i++) printf("═");
    printf("╝\n");
    printf("Score: %d | Level: %d\n", score, level);
}

// Gérer l'entrée utilisateur
void processInput() {
    if (kbhit()) {
        char key = getchar();
        switch (key) {
            case 'q': if (player.x > 0) player.x--; break;
            case 'd': if (player.x < WIDTH - 1) player.x++; break;
            case 'z': if (!bulletActive) {
                          bullet.x = player.x;
                          bullet.y = player.y - 1;
                          bulletActive = 1;
                      }
                      break;
        }
    }
}

// ... (Fonctions de gestion du jeu comme la gestion des invaders, des collisions, etc.)

// Animation des vagues ennemies (déplacement fluide)
void animateInvaders() {
    for (int i = 0; i < invaderWave.count; i++) {
        invaderWave.invaders[i].x += invaderWave.direction;
    }

    // Changement de direction aux bords
    if (invaderWave.invaders[0].x <= 0 || invaderWave.invaders[invaderWave.count - 1].x >= WIDTH - 1) {
        invaderWave.direction = -invaderWave.direction;
        for (int i = 0; i < invaderWave.count; i++) {
            invaderWave.invaders[i].y++;  // Descendre d'une ligne
        }
    }
}

// Gestion des vagues plus complexes (différents types d'attaques)
void manageWaves() {
    if (level % 2 == 0) {
        invaderWave.has_projectiles = 1;  // Ajouter des projectiles pour les vagues paires
    } else {
        invaderWave.has_projectiles = 0;
    }

    // Ajouter d'autres types d'attaques dynamiques selon le niveau
    if (level > 3) {
        invaderWave.speed -= 20;  // Réduire la vitesse pour plus de difficulté
    }
}

// Interface utilisateur : Menu principal
void showMenu() {
    clearScreen();
    printf("******** ALIEN SHOOTER ********\n");
    printf("1. Jouer\n");
    printf("2. Instructions\n");
    printf("3. Quitter\n");
    printf("Choisissez une option : ");
    scanf(" %c", &menuChoice);

    switch (menuChoice) {
        case '1':
            clearScreen();
            break;
        case '2':
            clearScreen();
            printf("Instructions:\n");
            printf("- Utilisez les touches 'q' et 'd' pour déplacer le joueur.\n");
            printf("- Tirez avec 'z'.\n");
            printf("- Tuez tous les envahisseurs pour passer au niveau suivant.\n");
            printf("- Attention à ne pas vous faire toucher !\n");
            printf("Appuyez sur une touche pour retourner au menu.\n");
            getchar();
            getchar();
            showMenu();
            break;
        case '3':
            printf("Quitter le jeu...\n");
            exit(0);
        default:
            printf("Option invalide.\n");
            showMenu();
            break;
    }
}


int main() {
    // Initialiser le jeu et démarrer la boucle principale
    //initGame();
    showMenu();
    while (!gameOver) {
        drawBoard();
        animateInvaders();  // Animation des vagues ennemies
        manageWaves();      // Gérer la complexité des vagues
        processInput();
        //updateGame();
        wait(100);  // Délai pour ralentir le jeu
    }

    printf("Game Over! Score: %d\n", score);
    return 0;
}
