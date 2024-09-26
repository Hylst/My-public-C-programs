// Hylst, your refined digital butler.
// v 0.1
// by Hylst
// TO DO :
// functions to implement
// user input to secure
// ...

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Function Prototypes
void showMainMenu();
void personalCalculationsMenu();
void financialManagementMenu();
void practicalToolsMenu();
int calculateAge();
double calculateVAT();
double calculateLoanRepayment();
void basicCalculations();
double convertUnits();
double netSalary();
void generateRandomNumber();
void convertTime();
void calculateAreaPerimeter();
void clearScreen();

int main() {
    int choice;

    // Welcome message
    printf("Greetings, I am Hylst, your refined digital butler.\n");
    printf("How may I assist you today?\n");

    do {
        showMainMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                personalCalculationsMenu();
                break;
            case 2:
                financialManagementMenu();
                break;
            case 3:
                practicalToolsMenu();
                break;
            case 4:
                printf("It was a pleasure serving you. Farewell!\n");
                exit(0);
                break;
            default:
                printf("Invalid choice, please select again.\n");
        }
    } while (choice != 4);

    return 0;
}

void showMainMenu() {
    printf("\n---- Main Menu ----\n");
    printf("1. Personal Calculations\n");
    printf("2. Financial Management\n");
    printf("3. Practical Tools\n");
    printf("4. Exit\n");
}

// Personal Calculations Menu
void personalCalculationsMenu() {
    int choice;

    do {
        printf("\n--- Personal Calculations ---\n");
        printf("1. Calculate Age\n");
        printf("2. Calculate VAT\n");
        printf("3. Calculate Loan Repayment\n");
        printf("4. Unit Conversion\n");
        printf("5. Basic Calculations\n");
        printf("6. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                calculateAge();
                break;
            case 2:
                printf("VAT amount: %.2f\n", calculateVAT());
                break;
            case 3:
                printf("Monthly repayment: %.2f\n", calculateLoanRepayment());
                break;
            case 4:
                printf("Converted unit: %.2f\n", convertUnits());
                break;
            case 5:
                basicCalculations();
                break;
            case 6:
                return;
            default:
                printf("Invalid choice, please select again.\n");
        }
    } while (choice != 6);
}

// Financial Management Menu
void financialManagementMenu() {
    int choice;

    do {
        printf("\n--- Financial Management ---\n");
        printf("1. Calculate Net Salary\n");
        printf("2. Simple Budget\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Net salary: %.2f\n", netSalary());
                break;
            case 2:
                printf("Feature not implemented yet.\n"); // Placeholder
                break;
            case 3:
                return;
            default:
                printf("Invalid choice, please select again.\n");
        }
    } while (choice != 3);
}

// Practical Tools Menu
void practicalToolsMenu() {
    int choice;

    do {
        printf("\n--- Practical Tools ---\n");
        printf("1. Generate Random Number\n");
        printf("2. Convert Time\n");
        printf("3. Calculate Area and Perimeter\n");
        printf("4. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                generateRandomNumber();
                break;
            case 2:
                convertTime();
                break;
            case 3:
                calculateAreaPerimeter();
                break;
            case 4:
                return;
            default:
                printf("Invalid choice, please select again.\n");
        }
    } while (choice != 4);
}

// Functions for each feature (To be implemented)
int calculateAge() {
    printf("Feature not implemented yet.\n");
    return 0; // Placeholder
}

double calculateVAT() {
    double price, vat_rate;
    printf("Enter the price (excl. VAT): ");
    scanf("%lf", &price);
    printf("Enter the VAT rate (%%): ");
    scanf("%lf", &vat_rate);
    return price * (vat_rate / 100.0);
}

double calculateLoanRepayment() {
    double capital, interest_rate;
    int months;
    printf("Enter loan amount: ");
    scanf("%lf", &capital);
    printf("Enter interest rate (annual in %%): ");
    scanf("%lf", &interest_rate);
    printf("Enter duration in months: ");
    scanf("%d", &months);

    double monthly_rate = (interest_rate / 100) / 12;
    double repayment = (capital * monthly_rate) / (1 - pow(1 + monthly_rate, -months));
    return repayment;
}

void basicCalculations() {
    double num1, num2;
    char operator;
    printf("Enter first number: ");
    scanf("%lf", &num1);
    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &operator);  // space before %c to consume newline
    printf("Enter second number: ");
    scanf("%lf", &num2);

    switch (operator) {
        case '+':
            printf("Result: %.2f\n", num1 + num2);
            break;
        case '-':
            printf("Result: %.2f\n", num1 - num2);
            break;
        case '*':
            printf("Result: %.2f\n", num1 * num2);
            break;
        case '/':
            if (num2 != 0)
                printf("Result: %.2f\n", num1 / num2);
            else
                printf("Cannot divide by zero.\n");
            break;
        default:
            printf("Invalid operator.\n");
    }
}

double convertUnits() {
    printf("Feature not implemented yet.\n");
    return 0; // Placeholder
}

double netSalary() {
    double gross, social_security;
    printf("Enter gross salary: ");
    scanf("%lf", &gross);
    printf("Enter social security rate (%%): ");
    scanf("%lf", &social_security);
    return gross * (1 - social_security / 100);
}

void generateRandomNumber() {
    srand(time(0));
    int random_num = rand();
    printf("Random number: %d\n", random_num);
}

void convertTime() {
    printf("Feature not implemented yet.\n");
}

void calculateAreaPerimeter() {
    printf("Feature not implemented yet.\n");
}

void clearScreen() {
    printf("\033[H\033[J"); // Clear screen (ANSI escape code)
}
