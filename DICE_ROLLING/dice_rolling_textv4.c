// This C program simulates a dice-rolling game in text mode (console). 
// It allows users to choose the type of dice (4, 6, 8, 12, 20, or 100-sided) and the number of dice to roll. 
// The program then generates random roll results, calculates the total, and provides options to view past rolls and apply modifiers.
// By Hylst (2011) - Updated / commented / Gitted (2024)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h> // For isdigit() function
#include <string.h> // For strlen() function

#define MAX_DICE 20  // Maximum number of dice per roll
#define MAX_ROLLS 10 // Maximum number of rolls to store

int is_valid_number(const char *str) {
    // Check if the string is a valid number (integer)
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) {
            return 0; // Not a valid number
        }
    }
    return 1;
}

void display_ascii_art() {
    printf("********************************************\n");
    printf("*   Welcome to Dice Roller Simulator!      *\n");
    printf("********************************************\n");
}

int main() {
    srand(time(NULL)); // Initialize the random number generator

    int scores[MAX_ROLLS][MAX_DICE] = {{0}}; // Array to store multiple rolls
    int totals[MAX_ROLLS] = {0}; // Store the total of each roll
    int rollCounts[MAX_ROLLS] = {0}; // Store the number of dice rolled each time
    int scoreIndex = 0; // Index to keep track of the current roll

    display_ascii_art();

    while (1) {
        int diceType = 0, numDice = 0, modifier = 0;
        char choice[10];
        char input[10]; // Buffer for user input

        // Ask for the type of dice and the number of dice, with 'q' to quit
        printf("\nWhat kind of dice do you want to roll? (4, 6, 8, 12, 20, 100) or q to quit: ");
        fgets(input, 10, stdin); // Read user input as a string
        input[strcspn(input, "\n")] = 0; // Remove trailing newline character

        // Check if user entered 'q' to quit
        if (input[0] == 'q' && strlen(input) == 1) {
            break; // Exit the loop
        }

        // Check if the input is a valid number
        if (!is_valid_number(input)) {
            printf("Invalid input. Please enter a valid dice type or 'q' to quit.\n");
            continue;
        }

        // Convert input to integer
        diceType = atoi(input);

        // Check for valid dice types
        if (!(diceType == 4 || diceType == 6 || diceType == 8 || diceType == 12 || diceType == 20 || diceType == 100)) {
            printf("Invalid dice type. Please choose from 4, 6, 8, 12, 20, or 100.\n");
            continue;
        }

        printf("How many dice do you want to roll (up to 20)? ");
        fgets(input, 10, stdin); // Read user input as a string
        input[strcspn(input, "\n")] = 0; // Remove trailing newline character

        // Check if the input is a valid number
        if (!is_valid_number(input)) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        // Convert input to integer
        numDice = atoi(input);

        // Ensure numDice does not exceed MAX_DICE
        if (numDice > MAX_DICE) {
            numDice = MAX_DICE;
            printf("Number of dice limited to %d\n", MAX_DICE);
        }

        // Roll the dice
        int results[MAX_DICE];
        int total = 0;
        printf("\nRolling %d dice of type d%d:\n", numDice, diceType);
        for (int i = 0; i < numDice; i++) {
            results[i] = (rand() % diceType) + 1;
            total += results[i];
            printf("Die %d: %d\n", i + 1, results[i]); 
        }
        printf("-------------------------\n");
        printf("Total: %d\n", total);
        printf("-------------------------\n");

        // Store the roll
        if (scoreIndex < MAX_ROLLS) {
            for (int i = 0; i < numDice; i++) {
                scores[scoreIndex][i] = results[i];
            }
            totals[scoreIndex] = total;
            rollCounts[scoreIndex] = numDice;
            scoreIndex++;
            printf("Roll stored in history.\n");
        } else {
            printf("Score history is full. Cannot store more scores.\n");
        }

        // Combined prompt for score history, modifier, restart, or quit
        printf("\nWhat would you like to do next?\n");
        printf("Press 's' to see score history, apply modifier (enter a number),\n");
        printf("restart (r), or quit (q): ");
        fgets(choice, 10, stdin);
        choice[strcspn(choice, "\n")] = 0; // Remove trailing newline character

        if (choice[0] == 's') {
            printf("****************************************\n");
            printf("*           Score History              *\n");
            printf("****************************************\n");
            for (int i = 0; i < scoreIndex; i++) {
                printf("Roll %d (%d dice): ", i + 1, rollCounts[i]);
                for (int j = 0; j < rollCounts[i]; j++) {
                    printf("%d ", scores[i][j]);
                }
                printf(" | Total: %d\n", totals[i]);
            }
            printf("****************************************\n");
        } else if (choice[0] == 'r') {
            continue; // Restart the program
        } else if (choice[0] == 'q') {
            break; // Quit the program
        } else if (is_valid_number(choice) || (choice[0] == '-' && is_valid_number(&choice[1]))) { // Check for modifier
            modifier = atoi(choice);
            // Apply the modifier to results and total
            printf("Modified results:\n");
            for (int i = 0; i < numDice; i++) {
                results[i] += modifier;
                printf("Die %d: %d\n", i + 1, results[i]);
            }
            total += modifier * numDice;
            printf("New total: %d\n", total);
        } else {
            printf("Invalid input.\n");
        }
    }

    printf("Thank you for using Dice Roller Simulator!\n");
    return 0;
}
