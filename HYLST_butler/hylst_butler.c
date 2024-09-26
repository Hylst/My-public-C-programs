// Hylst, your refined digital butler.
// v 0.3
// by Hylst
// TO DO :
// functions to implement
// user input to secure
// ...
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// Function Prototypes
void showMainMenu();
void personalCalculationsMenu();
void financialManagementMenu();
void practicalToolsMenu();
int calculateAge();
double calculateVAT();
double calculateLoanRepayment();
void basicCalculations();
void unitConversionMenu();
void volumeConversion();
void weightConversion();
void distanceConversion();
void temperatureConversion();
double netSalary();
void generateRandomNumber();
void convertTime();
void calculateAreaPerimeter();
void manageBudget();
int calculateDaysBetweenDates();
void complimentMe();
void insultMe();
void clearScreen();

// Utility to format and display floating-point numbers with precision, removing unnecessary zeros
void printFloat(double value) {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%.10f", value);
    char *end = buffer + strlen(buffer) - 1;
    while (*end == '0' && *(end - 1) != '.') {
        *end-- = '\0';
    }
    printf("%s\n", buffer);
}

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

void calculateAreaPerimeter() {
    int choice;
    double side, length, width, radius, base, height, a, b, c, d;

    printf("\nShape Calculation Options:\n");
    printf("1. Square (Area and Perimeter)\n");
    printf("2. Rectangle (Area and Perimeter)\n");
    printf("3. Circle (Area and Circumference)\n");
    printf("4. Triangle (Area and Perimeter)\n");
    printf("5. Trapezoid (Area and Perimeter)\n");
    printf("6. Rhombus (Area and Perimeter)\n");
    printf("7. Parallelogram (Area and Perimeter)\n");
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
        case 4:
            printf("Enter the base of the triangle: ");
            scanf("%lf", &base);
            printf("Enter the height of the triangle: ");
            scanf("%lf", &height);
            printf("Enter the lengths of the three sides (a b c): ");
            scanf("%lf %lf %lf", &a, &b, &c);
            printf("Area: %.2f\n", 0.5 * base * height);
            printf("Perimeter: %.2f\n", a + b + c);
            break;
        case 5:
            printf("Enter the lengths of the parallel sides (a and b): ");
            scanf("%lf %lf", &a, &b);
            printf("Enter the height of the trapezoid: ");
            scanf("%lf", &height);
            printf("Enter the lengths of the other two sides (c and d): ");
            scanf("%lf %lf", &c, &d);
            printf("Area: %.2f\n", 0.5 * (a + b) * height);
            printf("Perimeter: %.2f\n", a + b + c + d);
            break;
        case 6:
            printf("Enter the length of a side of the rhombus: ");
            scanf("%lf", &side);
            printf("Enter the lengths of the diagonals: ");
            scanf("%lf %lf", &a, &b);
            printf("Area: %.2f\n", 0.5 * a * b);
            printf("Perimeter: %.2f\n", 4 * side);
            break;
        case 7:
            printf("Enter the base of the parallelogram: ");
            scanf("%lf", &base);
            printf("Enter the height of the parallelogram: ");
            scanf("%lf", &height);
            printf("Enter the length of the side: ");
            scanf("%lf", &side);
            printf("Area: %.2f\n", base * height);
            printf("Perimeter: %.2f\n", 2 * (base + side));
            break;
        default:
            printf("Invalid choice.\n");
    }
}

void convertTime() {
    int choice, hours, minutes, seconds;
    printf("\nTime Conversion Options:\n");
    printf("1. Hours to Minutes\n");
    printf("2. Minutes to Hours\n");
    printf("3. Hours to Seconds\n");
    printf("4. Minutes to Seconds\n");
    printf("5. Seconds to Hours\n");
    printf("6. Seconds to Minutes\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter time in hours: ");
            scanf("%d", &hours);
            printf("%d hours is equivalent to ");
            printFloat(hours * 60.0);
            printf(" minutes.\n");
            break;
        case 2:
            printf("Enter time in minutes: ");
            scanf("%d", &minutes);
            printf("%d minutes is equivalent to ");
            printFloat(minutes / 60.0);
            printf(" hours.\n");
            break;
        case 3:
            printf("Enter time in hours: ");
            scanf("%d", &hours);
            printf("%d hours is equivalent to ");
            printFloat(hours * 3600.0);
            printf(" seconds.\n");
            break;
        case 4:
            printf("Enter time in minutes: ");
            scanf("%d", &minutes);
            printf("%d minutes is equivalent to ");
            printFloat(minutes * 60.0);
            printf(" seconds.\n");
            break;
        case 5:
            printf("Enter time in seconds: ");
            scanf("%d", &seconds);
            printf("%d seconds is equivalent to ");
            printFloat(seconds / 3600.0);
            printf(" hours.\n");
            break;
        case 6:
            printf("Enter time in seconds: ");
            scanf("%d", &seconds);
            printf("%d seconds is equivalent to ");
            printFloat(seconds / 60.0);
            printf(" minutes.\n");
            break;
        default:
            printf("Invalid choice.\n");
    }
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
        printf("6. Calculate Days Between Two Dates\n");
        printf("7. Compliment Me\n");
        printf("8. Insult Me\n");
        printf("9. Qu'avons-nous pour le dîner ce soir ?\n");
        printf("10. Pourriez-vous cirer mes vieilles chaussures ?\n");
        printf("11. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Your age is: %d years\n", calculateAge());
                break;
            case 2:
                printf("VAT amount: ");
                printFloat(calculateVAT());
                break;
            case 3:
                printf("Monthly repayment: ");
                printFloat(calculateLoanRepayment());
                break;
            case 4:
                unitConversionMenu();
                break;
            case 5:
                basicCalculations();
                break;
            case 6:
                printf("Days between the two dates: %d days\n", calculateDaysBetweenDates());
                break;
            case 7:
                complimentMe();
                break;
            case 8:
                insultMe();
                break;
            case 9:
                whatsForDinner();
                break;
            case 10:
                polishShoes();
                break;
            case 11:
                return;
            default:
                printf("Invalid choice, please select again.\n");
        }
    } while (choice != 11);
}


// Financial Management Menu
void financialManagementMenu() {
    int choice;

    do {
        printf("\n--- Financial Management ---\n");
        printf("1. Calculate Net Salary\n");
        printf("2. Manage Budget (Income/Expenses)\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Net salary: ");
                printFloat(netSalary());
                break;
            case 2:
                manageBudget();
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
            printf("Result: ");
            printFloat(num1 + num2);
            break;
        case '-':
            printf("Result: ");
            printFloat(num1 - num2);
            break;
        case '*':
            printf("Result: ");
            printFloat(num1 * num2);
            break;
        case '/':
            if (num2 != 0) {
                printf("Result: ");
                printFloat(num1 / num2);
            } else {
                printf("Cannot divide by zero.\n");
            }
            break;
        default:
            printf("Invalid operator.\n");
    }
}

// Function to display unit conversion menu
void unitConversionMenu() {
    int choice;

    do {
        printf("\n--- Unit Conversion ---\n");
        printf("1. Volume Conversion\n");
        printf("2. Weight Conversion\n");
        printf("3. Distance Conversion\n");
        printf("4. Temperature Conversion\n");
        printf("5. Back to Personal Calculations Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                volumeConversion();
                break;
            case 2:
                weightConversion();
                break;
            case 3:
                distanceConversion();
                break;
            case 4:
                temperatureConversion();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice, please select again.\n");
        }
    } while (choice != 5);
}

// Volume conversion function (liters to milliliters and vice versa)
void volumeConversion() {
    double value;
    int choice;
    printf("\nVolume Conversion Options:\n");
    printf("1. Liters to Milliliters\n");
    printf("2. Milliliters to Liters\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter value in liters: ");
            scanf("%lf", &value);
            printf("Converted value: ");
            printFloat(value * 1000);
            break;
        case 2:
            printf("Enter value in milliliters: ");
            scanf("%lf", &value);
            printf("Converted value: ");
            printFloat(value / 1000);
            break;
        default:
            printf("Invalid choice.\n");
    }
}

// Weight conversion function (kilograms to grams and vice versa)
void weightConversion() {
    double value;
    int choice;
    printf("\nWeight Conversion Options:\n");
    printf("1. Kilograms to Grams\n");
    printf("2. Grams to Kilograms\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter value in kilograms: ");
            scanf("%lf", &value);
            printf("Converted value: ");
            printFloat(value * 1000);
            break;
        case 2:
            printf("Enter value in grams: ");
            scanf("%lf", &value);
            printf("Converted value: ");
            printFloat(value / 1000);
            break;
        default:
            printf("Invalid choice.\n");
    }
}

// Distance conversion function (kilometers to meters and vice versa)
void distanceConversion() {
    double value;
    int choice;
    printf("\nDistance Conversion Options:\n");
    printf("1. Kilometers to Meters\n");
    printf("2. Meters to Kilometers\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter value in kilometers: ");
            scanf("%lf", &value);
            printf("Converted value: ");
            printFloat(value * 1000);
            break;
        case 2:
            printf("Enter value in meters: ");
            scanf("%lf", &value);
            printf("Converted value: ");
            printFloat(value / 1000);
            break;
        default:
            printf("Invalid choice.\n");
    }
}

// Temperature conversion function (Celsius to Fahrenheit and vice versa)
void temperatureConversion() {
    double value;
    int choice;
    printf("\nTemperature Conversion Options:\n");
    printf("1. Celsius to Fahrenheit\n");
    printf("2. Fahrenheit to Celsius\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter temperature in Celsius: ");
            scanf("%lf", &value);
            printf("Converted temperature: ");
            printFloat((value * 9/5) + 32);
            break;
        case 2:
            printf("Enter temperature in Fahrenheit: ");
            scanf("%lf", &value);
            printf("Converted temperature: ");
            printFloat((value - 32) * 5/9);
            break;
        default:
            printf("Invalid choice.\n");
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

// Function to manage monthly budget (enter expenses and income one by one)
void manageBudget() {
    double income = 0, expenses = 0, value;
    char choice;

    // Enter expenses
    printf("Enter your expenses one by one (press 'x' to stop):\n");
    while (1) {
        printf("Enter expense: ");
        if (scanf("%lf", &value) == 1) {
            expenses += value;
        } else {
            scanf("%s", &choice);
            if (choice == 'x' || choice == 'X') {
                break;
            } else {
                printf("Invalid input. Please try again.\n");
            }
        }
    }

    // Enter income
    printf("Enter your income one by one (press 'x' to stop):\n");
    while (1) {
        printf("Enter income: ");
        if (scanf("%lf", &value) == 1) {
            income += value;
        } else {
            scanf("%s", &choice);
            if (choice == 'x' || choice == 'X') {
                break;
            } else {
                printf("Invalid input. Please try again.\n");
            }
        }
    }

    printf("Your balance for the month is: ");
    printFloat(income - expenses);
}

// Function to generate a random number between two bounds set by the user
void generateRandomNumber() {
    int lower_bound, upper_bound, random_num;
    printf("Enter the lower bound: ");
    scanf("%d", &lower_bound);
    printf("Enter the upper bound: ");
    scanf("%d", &upper_bound);

    if (lower_bound > upper_bound) {
        printf("Invalid bounds. Lower bound should be less than or equal to upper bound.\n");
        return;
    }

    srand(time(0));
    random_num = (rand() % (upper_bound - lower_bound + 1)) + lower_bound;
    printf("Random number between %d and %d: %d\n", lower_bound, upper_bound, random_num);
}

// Function to calculate the number of days between two dates
int calculateDaysBetweenDates() {
    int day1, month1, year1, day2, month2, year2;
    printf("Enter the first date (DD MM YYYY): ");
    scanf("%d %d %d", &day1, &month1, &year1);
    printf("Enter the second date (DD MM YYYY): ");
    scanf("%d %d %d", &day2, &month2, &year2);

    struct tm date1 = {0}, date2 = {0};
    date1.tm_mday = day1;
    date1.tm_mon = month1 - 1;
    date1.tm_year = year1 - 1900;

    date2.tm_mday = day2;
    date2.tm_mon = month2 - 1;
    date2.tm_year = year2 - 1900;

    time_t time1 = mktime(&date1);
    time_t time2 = mktime(&date2);

    double seconds = difftime(time2, time1);
    int days = seconds / (60 * 60 * 24);
    return abs(days);
}

// Function to compliment the user
void complimentMe() {
    printf("You are amazing, intelligent, and a joy to assist!\n");
}

// Function to humorously insult the user
void insultMe() {
    printf("Really? You couldn't even get that right? I'm not mad, just disappointed.\n");
}

void whatsForDinner() {
    const char *dishes[] = {
        "Tonight, we have a gourmet meal of instant noodles, paired with tap water.",
        "How about some spaghetti with a side of existential dread?",
        "I recommend a five-star dish of toast, served with an air of sophistication."
    };
    srand(time(0));
    printf("%s\n", dishes[rand() % 3]);
}

void polishShoes() {
    printf("Ah, your shoes deserve only the finest polish! Unfortunately, my polishing arms are out of commission. But I assure you, they're shining... in spirit!\n");
}

void clearScreen() {
    printf("\033[H\033[J"); // Clear screen (ANSI escape code)
}
