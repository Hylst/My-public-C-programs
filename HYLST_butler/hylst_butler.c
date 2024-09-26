// Hylst, your refined digital butler.
// v 0.2
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
                printf("Your age is: %d years\n", calculateAge());
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

// Function to calculate age based on birth date
int calculateAge() {
    int birthYear, birthMonth, birthDay;
    printf("Enter your birth year (YYYY): ");
    scanf("%d", &birthYear);
    printf("Enter your birth month (MM): ");
    scanf("%d", &birthMonth);
    printf("Enter your birth day (DD): ");
    scanf("%d", &birthDay);

    // Get current date
    time_t now = time(NULL);
    struct tm *currentDate = localtime(&now);

    int currentYear = currentDate->tm_year + 1900;
    int currentMonth = currentDate->tm_mon + 1;
    int currentDay = currentDate->tm_mday;

    // Calculate age
    int age = currentYear - birthYear;

    // Adjust if birthdate hasn't occurred yet this year
    if (birthMonth > currentMonth || (birthMonth == currentMonth && birthDay > currentDay)) {
        age--;
    }

    return age;
}

// Function to calculate VAT
double calculateVAT() {
    double price, vat_rate;
    printf("Enter the price (excl. VAT): ");
    scanf("%lf", &price);
    printf("Enter the VAT rate (%%): ");
    scanf("%lf", &vat_rate);
    return price * (vat_rate / 100.0);
}

// Function to calculate loan repayment
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

// Basic calculations (+, -, *, /)
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

// Function to convert units (example: kilometers to meters)
double convertUnits() {
    double value;
    int choice;
    printf("\nUnit Conversion Options:\n");
    printf("1. Kilometers to Meters\n");
    printf("2. Meters to Kilometers\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter value in kilometers: ");
            scanf("%lf", &value);
            return value * 1000;
        case 2:
            printf("Enter value in meters: ");
            scanf("%lf", &value);
            return value / 1000;
        default:
            printf("Invalid choice.\n");
            return 0;
    }
}

// Function to calculate net salary based on gross salary and social security rate
double netSalary() {
    double gross, social_security;
    printf("Enter gross salary: ");
    scanf("%lf", &gross);
    printf("Enter social security rate (%%): ");
    scanf("%lf", &social_security);
    return gross * (1 - social_security / 100);
}

// Function to generate random numbers
void generateRandomNumber() {
    srand(time(0));
    int random_num = rand();
    printf("Random number: %d\n", random_num);
}

// Function to convert time (hours to minutes and vice-versa)
void convertTime() {
    int choice, hours, minutes;
    printf("\nTime Conversion Options:\n");
    printf("1. Hours to Minutes\n");
    printf("2. Minutes to Hours\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter time in hours: ");
            scanf("%d", &hours);
            printf("%d hours is equivalent to %d minutes.\n", hours, hours * 60);
            break;
        case 2:
            printf("Enter time in minutes: ");
            scanf("%d", &minutes);
            printf("%d minutes is equivalent to %.2f hours.\n", minutes, minutes / 60.0);
            break;
        default:
            printf("Invalid choice.\n");
    }
}

// Function to calculate area and perimeter of basic shapes
void calculateAreaPerimeter() {
    int choice;
    double side, length, width, radius;

    printf("\nShape Calculation Options:\n");
    printf("1. Square (Area and Perimeter)\n");
    printf("2. Rectangle (Area and Perimeter)\n");
    printf("3. Circle (Area and Circumference)\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter the side length of the square: ");
            scanf("%lf", &side);
            printf("Area: %.2f\n", side * side);
            printf("Perimeter: %.2f\n", 4 * side);
            break;
        case 2:
            printf("Enter the length of the rectangle: ");
            scanf("%lf", &length);
            printf("Enter the width of the rectangle: ");
            scanf("%lf", &width);
            printf("Area: %.2f\n", length * width);
            printf("Perimeter: %.2f\n", 2 * (length + width));
            break;
        case 3:
            printf("Enter the radius of the circle: ");
            scanf("%lf", &radius);
            printf("Area: %.2f\n", M_PI * radius * radius);
            printf("Circumference: %.2f\n", 2 * M_PI * radius);
            break;
        default:
            printf("Invalid choice.\n");
    }
}

void clearScreen() {
    printf("\033[H\033[J"); // Clear screen (ANSI escape code)
}
