// Hylst, your refined digital butler.
// v 1.0
// by Hylst
/*
Hylst provides you with a variety of useful and fun tools divided into four categories:

Personal Calculations
Age Calculation: Calculate your current age based on your birthdate.
Percentage Calculations: Perform various percentage calculations such as calculating percentages, discounts, and increases on values.
Recipe Adjustments: Adjust ingredient quantities for different numbers of people for three recipes: Crêpes, Savory Cake, and Chocolate Chip Cookies.
Basic Calculations: Simple arithmetic operations (addition, subtraction, multiplication, division).
Unit Conversions: Convert between common units such as volume, weight, distance, and temperature.
Days Between Dates: Calculate the number of days between two dates.

Financial Management Tools
Calculate VAT: Compute the VAT amount based on a price and a given VAT rate.
Loan Repayment Calculator: Calculate your monthly loan repayments based on the loan amount, interest rate, and duration.
Savings Planner: Plan how much you need to save monthly to reach a financial goal over a set period of time.
Investment Returns Calculator: Calculate the future value of an investment based on principal, interest rate, and time.
Currency Conversion: Convert an amount from one currency to another based on a given exchange rate.
Manage Budget: Calculate your monthly balance by inputting income and expenses.

Personal Services (Fun & Interactive)
Compliment Me: Receive a kind and encouraging compliment from Hylst.
Insult Me (for fun): Get a playful, humorous insult from Hylst.
What's for Dinner Tonight?: Get a fun suggestion for dinner (with a humorous twist).
Could You Shine My Old Shoes?: A humorous response from Hylst about shoe shining services.

Practical Tools
Generate Random Number: Generate a random number between two bounds defined by the user.
Convert Time: Convert between hours, minutes, and seconds with various options.
Calculate Area and Perimeter of Shapes: Calculate the area and perimeter for shapes such as squares, rectangles, circles, triangles, and more.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// Function Prototypes
void showMainMenu();
void personalCalculationsMenu();
void financialManagementMenu();
void personalServicesMenu();
void practicalToolsMenu();
int calculateAge();
void percentageCalculations();
void recipeCalculator();
double calculateVAT();
double calculateLoanRepayment();
double savingsPlanner();
double investmentReturnsCalculator();
void currencyConversion();
void basicCalculations();
void unitConversionMenu();
void convertTime();
void generateRandomNumber();
void calculateAreaPerimeter();
void manageBudget();
int calculateDaysBetweenDates();
void complimentMe();
void insultMe();
void whatsForDinner();
void polishShoes();
void savoryCakeRecipe();
void cookieRecipe();
void clearScreen();
void volumeConversion();
void weightConversion();
void distanceConversion();
void temperatureConversion();

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

#ifdef _WIN32
    #include <windows.h>
#endif

// Clear screen function
void clearScreen() {
#ifdef _WIN32
    system("cls");  // Windows clear screen
#else
    printf("\033[H\033[J");  // ANSI escape for Unix/Linux
#endif
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
                personalServicesMenu();
                break;
            case 4:
                practicalToolsMenu();
                break;
            case 5:
                printf("It was a pleasure serving you. Farewell!\n");
                exit(0);
                break;
            default:
                printf("Invalid choice, please select again.\n");
        }
    } while (choice != 5);

    return 0;
}

void showMainMenu() {
    printf("\n---- Main Menu ----\n");
    printf("1. Personal Calculations\n");
    printf("2. Financial Management Tools\n");
    printf("3. Personal Services\n");
    printf("4. Practical Tools\n");
    printf("5. Exit\n");
}

// Personal Calculations Menu
void personalCalculationsMenu() {
    int choice;

    do {
        printf("\n--- Personal Calculations ---\n");
        printf("1. Calculate Your Age\n");
        printf("2. Percentage Calculations\n");
        printf("3. Recipe Adjustments\n");
        printf("4. Basic Calculations (Addition, Subtraction, etc.)\n");
        printf("5. Unit Conversions\n");
        printf("6. Calculate Days Between Two Dates\n");
        printf("7. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Based on your birthdate, your current age is: %d years\n", calculateAge());
                break;
            case 2:
                percentageCalculations();
                break;
            case 3:
                recipeCalculator();
                break;
            case 4:
                basicCalculations();
                break;
            case 5:
                unitConversionMenu();
                break;
            case 6:
                printf("The total number of days between the two dates is: %d days\n", calculateDaysBetweenDates());
                break;
            case 7:
                return;
            default:
                printf("Invalid choice, please select again.\n");
        }
    } while (choice != 7);
}

// Financial Management Tools Menu
void financialManagementMenu() {
    int choice;

    do {
        printf("\n--- Financial Management Tools ---\n");
        printf("1. Calculate VAT\n");
        printf("2. Calculate Loan Repayment\n");
        printf("3. Savings Planner\n");
        printf("4. Investment Returns Calculator\n");
        printf("5. Currency Conversion\n");
        printf("6. Manage Budget (Income/Expenses)\n");
        printf("7. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Based on the entered price and VAT rate, the VAT amount is: ");
                printFloat(calculateVAT());
                break;
            case 2:
                printf("Your monthly loan repayment will be: ");
                printFloat(calculateLoanRepayment());
                break;
            case 3:
                savingsPlanner();
                break;
            case 4:
                investmentReturnsCalculator();
                break;
            case 5:
                currencyConversion();
                break;
            case 6:
                manageBudget();
                break;
            case 7:
                return;
            default:
                printf("Invalid choice, please select again.\n");
        }
    } while (choice != 7);
}

// Personal Services Menu
void personalServicesMenu() {
    int choice;

    do {
        printf("\n--- Personal Services ---\n");
        printf("1. Compliment Me\n");
        printf("2. Insult Me (for fun)\n");
        printf("3. What's for Dinner Tonight?\n");
        printf("4. Could You Shine My Old Shoes?\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                complimentMe();
                break;
            case 2:
                insultMe();
                break;
            case 3:
                whatsForDinner();
                break;
            case 4:
                polishShoes();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice, please select again.\n");
        }
    } while (choice != 5);
}

// Practical Tools Menu
void practicalToolsMenu() {
    int choice;

    do {
        printf("\n--- Practical Tools ---\n");
        printf("1. Generate a Random Number\n");
        printf("2. Convert Time (Hours, Minutes, Seconds)\n");
        printf("3. Calculate Area and Perimeter of Shapes\n");
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

// Financial Tools Implementation

double calculateVAT() {
    double price, vat_rate;
    printf("Please enter the price excluding VAT: ");
    scanf("%lf", &price);
    printf("Please enter the VAT rate (%%): ");
    scanf("%lf", &vat_rate);
    return price * (vat_rate / 100.0);
}

double calculateLoanRepayment() {
    double loan_amount, interest_rate;
    int months;
    printf("Enter the loan amount: ");
    scanf("%lf", &loan_amount);
    printf("Enter the annual interest rate (%%): ");
    scanf("%lf", &interest_rate);
    printf("Enter the number of months for the loan duration: ");
    scanf("%d", &months);

    double monthly_rate = (interest_rate / 100) / 12;
    double repayment = (loan_amount * monthly_rate) / (1 - pow(1 + monthly_rate, -months));
    return repayment;
}

double savingsPlanner() {
    double goal, months, monthly_savings;
    printf("Please enter your total savings goal: ");
    scanf("%lf", &goal);
    printf("Over how many months do you plan to save? ");
    scanf("%lf", &months);

    monthly_savings = goal / months;
    printf("You need to save %.2f per month to reach your savings goal.\n", monthly_savings);
    return monthly_savings;
}

double investmentReturnsCalculator() {
    double principal, interest_rate, years, returns;
    printf("Enter the initial investment amount: ");
    scanf("%lf", &principal);
    printf("Enter the annual interest rate (%%): ");
    scanf("%lf", &interest_rate);
    printf("Enter the number of years the investment will be held: ");
    scanf("%lf", &years);

    returns = principal * pow(1 + (interest_rate / 100), years);
    printf("Your investment will grow to: %.2f\n", returns);
    return returns;
}

void currencyConversion() {
    double amount, rate;
    printf("Enter the amount in your local currency: ");
    scanf("%lf", &amount);
    printf("Enter the exchange rate (e.g., 1.2 for EUR to USD): ");
    scanf("%lf", &rate);
    printf("The equivalent amount is: %.2f\n", amount * rate);
}

// Personal Calculations Implementation

int calculateAge() {
    int birthYear, birthMonth, birthDay;
    printf("Enter your birth year (YYYY): ");
    scanf("%d", &birthYear);
    printf("Enter your birth month (MM): ");
    scanf("%d", &birthMonth);
    printf("Enter your birth day (DD): ");
    scanf("%d", &birthDay);

    time_t now = time(NULL);
    struct tm *currentDate = localtime(&now);

    int currentYear = currentDate->tm_year + 1900;
    int currentMonth = currentDate->tm_mon + 1;
    int currentDay = currentDate->tm_mday;

    int age = currentYear - birthYear;

    if (birthMonth > currentMonth || (birthMonth == currentMonth && birthDay > currentDay)) {
        age--;
    }

    return age;
}

void percentageCalculations() {
    double value, percentage, result;
    int choice;

    printf("\n--- Percentage Calculations ---\n");
    printf("1. Calculate a percentage of a value\n");
    printf("2. Calculate a discount/reduction\n");
    printf("3. Calculate an increase on a value\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            // Calculate a percentage of a value
            printf("Enter the value: ");
            scanf("%lf", &value);
            printf("Enter the percentage to calculate: ");
            scanf("%lf", &percentage);
            result = value * (percentage / 100.0);
            printf("The result is: %.2f\n", result);
            break;
        case 2:
            // Calculate a discount/reduction
            printf("Enter the original price: ");
            scanf("%lf", &value);
            printf("Enter the discount percentage: ");
            scanf("%lf", &percentage);
            result = value * (1 - (percentage / 100.0));
            printf("The price after the discount is: %.2f\n", result);
            break;
        case 3:
            // Calculate an increase on a value
            printf("Enter the original value: ");
            scanf("%lf", &value);
            printf("Enter the percentage increase: ");
            scanf("%lf", &percentage);
            result = value * (1 + (percentage / 100.0));
            printf("The value after the increase is: %.2f\n", result);
            break;
        default:
            printf("Invalid choice.\n");
    }
}


void basicCalculations() {
    double num1, num2;
    char operator;
    printf("Enter first number: ");
    scanf("%lf", &num1);
    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &operator);
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

void manageBudget() {
    double income = 0, expenses = 0, value;
    char choice;

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

// Recipe Adjustments
void recipeCalculator() {
    int original_people, new_people;
    double flour, eggs, milk;
    int recipe_choice;

    printf("\n--- Recipe Adjustments ---\n");
    printf("1. Crêpes Recipe\n");
    printf("2. Savory Cake Recipe\n");
    printf("3. Chocolate Chip Cookie Recipe\n");
    printf("Choose a recipe to adjust: ");
    scanf("%d", &recipe_choice);

    printf("Enter the original number of people for the recipe: ");
    scanf("%d", &original_people);
    printf("Enter the new number of people: ");
    scanf("%d", &new_people);

    double scale_factor = (double)new_people / original_people;

    switch (recipe_choice) {
        case 1:
            flour = 250 * scale_factor;
            eggs = 3 * scale_factor;
            milk = 500 * scale_factor;

            printf("For %d people, you need for Crêpes:\n", new_people);
            printf("%.2f grams of flour\n", flour);
            printf("%.2f eggs\n", eggs);
            printf("%.2f milliliters of milk\n", milk);
            break;
        case 2:
            savoryCakeRecipe();
            break;
        case 3:
            cookieRecipe();
            break;
        default:
            printf("Invalid recipe choice.\n");
    }
}

void savoryCakeRecipe() {
    int original_people, new_people;
    double flour, eggs, milk, cheese, ham;

    printf("For how many people is the original recipe? ");
    scanf("%d", &original_people);
    printf("How many people would you like to make the recipe for? ");
    scanf("%d", &new_people);

    double scale_factor = (double)new_people / original_people;

    flour = 150 * scale_factor;
    eggs = 3 * scale_factor;
    milk = 100 * scale_factor;
    cheese = 100 * scale_factor;
    ham = 200 * scale_factor;

    printf("For %d people, you need for Savory Cake:\n", new_people);
    printf("%.2f grams of flour\n", flour);
    printf("%.2f eggs\n", eggs);
    printf("%.2f milliliters of milk\n", milk);
    printf("%.2f grams of grated cheese\n", cheese);
    printf("%.2f grams of ham\n", ham);
}

void cookieRecipe() {
    int original_people, new_people;
    double flour, sugar, butter, chocolate_chips;

    printf("For how many people is the original recipe? ");
    scanf("%d", &original_people);
    printf("How many people would you like to make the recipe for? ");
    scanf("%d", &new_people);

    double scale_factor = (double)new_people / original_people;

    flour = 300 * scale_factor;
    sugar = 150 * scale_factor;
    butter = 200 * scale_factor;
    chocolate_chips = 100 * scale_factor;

    printf("For %d people, you need for Chocolate Chip Cookies:\n", new_people);
    printf("%.2f grams of flour\n", flour);
    printf("%.2f grams of sugar\n", sugar);
    printf("%.2f grams of butter\n", butter);
    printf("%.2f grams of chocolate chips\n", chocolate_chips);
}

// Fun Services

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

void complimentMe() {
    printf("You are amazing, intelligent, and a joy to assist!\n");
}

void insultMe() {
    printf("Really? You couldn't even get that right? I'm not mad, just disappointed.\n");
}

void generateRandomNumber() {
    int lower, upper, random_num;
    printf("Enter lower bound: ");
    scanf("%d", &lower);
    printf("Enter upper bound: ");
    scanf("%d", &upper);

    srand(time(0));
    random_num = (rand() % (upper - lower + 1)) + lower;
    printf("Generated random number: %d\n", random_num);
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
            printf("%d hours is equivalent to %d minutes.\n", hours, hours * 60);
            break;
        case 2:
            printf("Enter time in minutes: ");
            scanf("%d", &minutes);
            printf("%d minutes is equivalent to %.2f hours.\n", minutes, minutes / 60.0);
            break;
        case 3:
            printf("Enter time in hours: ");
            scanf("%d", &hours);
            printf("%d hours is equivalent to %d seconds.\n", hours, hours * 3600);
            break;
        case 4:
            printf("Enter time in minutes: ");
            scanf("%d", &minutes);
            printf("%d minutes is equivalent to %d seconds.\n", minutes, minutes * 60);
            break;
        case 5:
            printf("Enter time in seconds: ");
            scanf("%d", &seconds);
            printf("%d seconds is equivalent to %.2f hours.\n", seconds, seconds / 3600.0);
            break;
        case 6:
            printf("Enter time in seconds: ");
            scanf("%d", &seconds);
            printf("%d seconds is equivalent to %.2f minutes.\n", seconds, seconds / 60.0);
            break;
        default:
            printf("Invalid choice.\n");
    }
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
