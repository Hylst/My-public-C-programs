// Moto Game: Because who needs a social life when you can ride a virtual motorcycle?
// Not functionnal : no moto moves, all vehicules static, big ascii static area on top.
// Hylst 

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#define SLEEP(ms) Sleep(ms)
#else
#include <unistd.h>
#define SLEEP(ms) usleep(ms * 1000)
#endif
#include <time.h>

// Constants: because who doesn't love a good constant?
#define WIDTH 80
#define HEIGHT 30
#define SPEED 100000

// Structure to hold the moto's position and fuel level
typedef struct {
    int x;
    int y;
    int fuel;
    int score;
} Moto;

typedef enum {
    CAR,
    TRUCK,
    FUEL,
    OBSTACLE
} EntityType;

typedef struct {
    int x;
    int y;
    EntityType type;
} Entity;

// Function to clear the screen, because we don't want any clutter
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Function to print the moto, because we want to see our beautiful bike
void print_moto(int x, int y) {
    printf("\033[%d;%dH", y, x);
    printf(" /_/\\ ");
    printf("\033[%d;%dH", y+1, x);
    printf("(o.o )");
    printf("\033[%d;%dH", y+2, x);
    printf(" > ^ <");
}

// Function to print a car, because cars are the bane of our existence
void print_car(int x, int y) {
    printf("\033[%d;%dH", y, x);
    printf(" ____");
    printf("\033[%d;%dH", y+1, x);
    printf("/|__\\\\");
    printf("\033[%d;%dH", y+2, x);
    printf("O-O-O");
}

// Function to print a truck, because trucks are even worse than cars
void print_truck(int x, int y) {
    printf("\033[%d;%dH", y, x);
    printf(" _____");
    printf("\033[%d;%dH", y+1, x);
    printf("|     |__");
    printf("\033[%d;%dH", y+2, x);
    printf("|_______||");
    printf("\033[%d;%dH", y+3, x);
    printf(" O     O ");
}

// Function to print fuel, because we need to keep our moto running
void print_fuel(int x, int y) {
    printf("\033[%d;%dH", y, x);
    printf(" ___ ");
    printf("\033[%d;%dH", y+1, x);
    printf("|_F_|");
    printf("\033[%d;%dH", y+2, x);
    printf(" | | ");
}

// Function to print an obstacle, because obstacles are just trying to ruin our fun
void print_obstacle(int x, int y) {
    printf("\033[%d;%dH", y, x);
    printf(" /\\ ");
    printf("\033[%d;%dH", y+1, x);
    printf("/**\\");
    printf("\033[%d;%dH", y+2, x);
    printf("\\__/");
}

// Function to print the road, because we need something to ride on
void print_road(char road[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", road[i][j]);
        }
        printf("\n");
    }
}

// Function to initialize the road, because we need a clean slate
void init_road(char road[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0 || j == WIDTH - 1) {
                road[i][j] = '|';
            } else if (j == WIDTH / 3 || j == 2 * WIDTH / 3) {
                road[i][j] = '.';
            } else {
                road[i][j] = ' ';
            }
        }
    }
}

void update_road(char road[HEIGHT][WIDTH], Moto *moto, Entity entities[], int entity_count) {
    init_road(road);
    
    // Imprimez les entités sur la route
    for (int i = 0; i < entity_count; i++) {
        switch (entities[i].type) {
            case CAR:
                road[entities[i].y][entities[i].x] = 'C';
                break;
            case TRUCK:
                road[entities[i].y][entities[i].x] = 'T';
                break;
            case FUEL:
                road[entities[i].y][entities[i].x] = 'F';
                break;
            case OBSTACLE:
                road[entities[i].y][entities[i].x] = 'O';
                break;
        }
    }
    
    // Imprimez la moto sur la route
    road[moto->y][moto->x] = 'M';
    print_moto(moto->x, moto->y);
}

// Function to move the obstacles and vehicles, because they're always on the move
void move_entities(Entity entities[], int entity_count) {
    for (int i = 0; i < entity_count; i++) {
        entities[i].y++;
        if (entities[i].y >= HEIGHT) {
            entities[i].y = 0;
            entities[i].x = rand() % (WIDTH - 10) + 5;
            
            int r = rand() % 100;
            if (r < 60) {
                entities[i].type = OBSTACLE;
            } else if (r < 80) {
                entities[i].type = CAR;
            } else if (r < 95) {
                entities[i].type = TRUCK;
            } else {
                entities[i].type = FUEL;
            }
        }
    }
}

// Function to check for collisions, because we don't want to crash
int check_collision(Moto *moto, Entity entities[], int entity_count) {
    for (int i = 0; i < entity_count; i++) {
        if (abs(entities[i].x - moto->x) < 3 && abs(entities[i].y - moto->y) < 3) {
            if (entities[i].type == FUEL) {
                moto->fuel += 20;
                if (moto->fuel > 100) moto->fuel = 100;
                moto->score += 10;
                entities[i].y = 0;
                entities[i].x = rand() % (WIDTH - 10) + 5;
            } else {
                return 1; // Collision detected
            }
        }
    }
    return 0; // No collision
}

// Function to play a sound, because we want to hear the excitement
void play_sound() {
#ifdef _WIN32
    Beep(2500, 100);
#else
    printf("\a");
#endif
}

// The main function, where all the magic happens
int main() {
    srand(time(NULL));

    // Print the title, because we need to know what we're playing
    printf("  _______  _______  _______  _______  _______  _______  _______ \n");
    printf(" |       ||       ||       ||       ||       ||       ||       |\n");
    printf(" |  _____||_     _||   _   ||   _   ||   _   ||   _   ||   _   |\n");
    printf(" | |_____   |   |  |  | |  |  | |  |  | |  |  |  | |  |  | |  |\n");
    printf(" |_____  |  |   |  |  |_|  |  |_|  |  |_|  |  |  |_|  |  |_|  |\n");
    printf("  _____| |  |   |  |       |       |       |  |       |       |\n");
    printf(" |_______|  |___|  |_______|_______|_______|  |_______|_______|\n");
    printf("  _______  _______  _______  _______  _______  _______  _______ \n");
    printf(" |       ||       ||       ||       ||       ||       ||       |\n");
    printf(" |  _____||_     _||   _   ||   _   ||   _   ||   _   ||   _   |\n");
    printf(" | |_____   |   |  |  | |  |  | |  |  | |  |  |  | |  |  | |  |\n");
    printf(" |_____  |  |   |  |  |_|  |  |_|  |  |_|  |  |  |_|  |  |_|  |\n");
    printf("  _____| |  |   |  |       |       |       |  |       |       |\n");
    printf(" |_______|  |___|  |_______|_______|_______|  |_______|_______|\n");
    printf("\n");
    printf("  Welcome to Moto Game! \n");
    printf("  Use the A (left), D (right), W (up), S (down) keys to move. \n");
    printf("  Avoid obstacles and vehicles, collect fuel. \n");
    printf("  Press any key to start...\n");
    getchar();

    // Initialize the moto
    Moto moto = {WIDTH / 2, HEIGHT - 5, 100, 0};
    char road[HEIGHT][WIDTH];
    
    // Initialize the obstacles and vehicles
    Entity entities[20];
    for (int i = 0; i < 20; i++) {
        entities[i].x = rand() % (WIDTH - 10) + 5;
        entities[i].y = rand() % HEIGHT;
        int r = rand() % 100;
        if (r < 60) {
            entities[i].type = OBSTACLE;
        } else if (r < 80) {
            entities[i].type = CAR;
        } else if (r < 95) {
            entities[i].type = TRUCK;
        } else {
            entities[i].type = FUEL;
        }
    }

    // The main game loop, where the action happens
    while (1) {
        clear_screen();
        update_road(road, &moto, entities, 20);
        print_road(road);
        
        printf("Fuel: %d%% | Score: %d\n", moto.fuel, moto.score);

        move_entities(entities, 20);

        if (check_collision(&moto, entities, 20)) {
            play_sound();
            printf("Game Over! Final Score: %d\n", moto.score);
            return 0;
        }

        moto.fuel--;
        if (moto.fuel <= 0) {
            play_sound();
            printf("Out of fuel! Game Over! Final Score: %d\n", moto.score);
            return 0;
        }

        moto.score++;

        SLEEP(SPEED);

        // Get the player's input
        char input;
        if (scanf("%c", &input) == 1) {
            switch (input) {
                case 'a':
                    if (moto.x > 5) moto.x--;
                    break;
                case 'd':
                    if (moto.x < WIDTH - 5) moto.x++;
                    break;
                case 'w':
                    if (moto.y > 0) moto.y--;
                    break;
                case 's':
                    if (moto.y < HEIGHT - 3) moto.y++;
                    break;
            }
        }
    }

    return 0;
}