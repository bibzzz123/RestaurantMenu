#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ITEMS 12
#define MAX_ORDERS 10
#define TAKEOUT_FEE 10.00
#define DISCOUNT_RATE 0.20
#define MAX_PAYMENT_METHODS 3

// Global arrays for menu items and prices
char menuItems[MAX_ITEMS][30] = {
    "Lomi",
    "Chami",
    "Pancit Canton",
    "Pancit Bihon",
    "Pancit Tustado",
    "Tapsilog",
    "Hotsilog",
    "Tocilog",
    "Chixsilog",
    "Extra Rice",
    "Extra Toppings",
    "Softdrink"
};

double regularPrices[MAX_ITEMS] = {
    65.00, // Lomi
    50.00, // Chami
    50.00, // Pancit Canton
    50.00, // Pancit Bihon
    50.00, // Pancit Tustado
    60.00, // Tapsilog
    60.00, // Hotsilog
    60.00, // Tocilog
    60.00, // Chixsilog
    15.00, // Extra Rice
    20.00, // Extra Toppings
    15.00  // Softdrink
};

double specialPrices[MAX_ITEMS] = {
    85.00, // Lomi
    70.00, // Chami
    70.00, // Pancit Canton
    70.00, // Pancit Bihon
    70.00, // Pancit Tustado
    0.00,  // Tapsilog
    0.00,  // Hotsilog
    0.00,  // Tocilog
    0.00,  // Chixsilog
    0.00,  // Extra Rice
    0.00,  // Extra Toppings
    30.00   // Softdrink
};

int orders[MAX_ORDERS];
int quantities[MAX_ORDERS];
char types[MAX_ORDERS][10];
char extraOrders[MAX_ORDERS][5];
char dineOptions[MAX_ORDERS][2];
char phoneNumber[12] = "";  
char cardType[10] = "";     

int orderCount = 0;

char paymentMethods[MAX_PAYMENT_METHODS][20] = {
    "Cash",
    "E-money",
    "Card"
};

// Function prototypes
void clearScreen();
void displayMenu();
void getOrderDetails(int index, int itemNumber);
void addOrder();
void removeOrder();
void showOrders();
void processCheckout();
void printReceipt(double subtotal, double takeoutFees, double discount, double total, int paymentMethod);
int getDiscountCard();
int getPaymentMethod();

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayMenu() {
    printf("Welcome to the ABC123 Lomian atbp!\n");
    printf("\n----------------- Menu ------------------------------\n");
    printf("%-3s %-20s %-15s %-15s\n", "No.", "Item", "Regular Price", "Special Price");
    printf("-----------------------------------------------------\n");
    
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (specialPrices[i] > 0.00) {
            printf("%-3d %-20s Php %.2f        Php %.2f\n", 
                i + 1, menuItems[i], regularPrices[i], specialPrices[i]);
        } else {
            printf("%-3d %-20s Php %.2f\n", 
                i + 1, menuItems[i], regularPrices[i]);
        }
    }
    printf("-----------------------------------------------------\n");
    printf("Note: Take-out orders have an additional Php %.2f charge per item\n", TAKEOUT_FEE);
}

void getOrderDetails(int index, int itemNumber) {
    int itemIndex = itemNumber - 1;
    char dineOption;
    
    do {
        printf("Dine In or Take Out? (D for Dine In, T for Take Out): ");
        scanf(" %c", &dineOption);
        dineOption = toupper(dineOption);
        
        if (dineOption != 'D' && dineOption != 'T') {
            printf("Invalid option. Please enter D or T.\n");
        }
    } while (dineOption != 'D' && dineOption != 'T');
    
    dineOptions[index][0] = dineOption;
    dineOptions[index][1] = '\0';
    
    do {
        printf("Quantity: ");
        scanf("%d", &quantities[index]);
        if (quantities[index] <= 0) {
            printf("Invalid quantity! Please enter a positive number.\n");
        }
    } while (quantities[index] <= 0);

    strcpy(extraOrders[index], "N");

    switch(itemIndex) {
        case 0: // Lomi
        case 1: // Chami
        case 2: // Pancit Canton
        case 3: // Pancit Bihon
        case 4: // Pancit Tustado
            printf("Special or Regular? (S for Special, R for Regular): ");
            scanf("%s", types[index]);

            printf("Extra Toppings? (Y for Yes, N for No): ");
            char extraTopping[3];
            scanf("%s", extraTopping);
            strcpy(extraOrders[index], extraTopping);
            break;

        case 5: // Tapsilog
        case 6: // Hotsilog
        case 7: // Tocilog
        case 8: // Chixsilog
            printf("Extra Rice? Add Php 15.00 (Y for Yes, N for No): ");
            scanf("%s", extraOrders[index]);
            strcpy(types[index], "R");
            break;

        case 9: // Extra Rice
        case 10: // Extra Toppings
            strcpy(types[index], "R");
            break;

        case 11: // Softdrinks
            printf("Regular or Large? (R for Regular, L for Large): ");
            scanf("%s", extraOrders[index]);
            strcpy(types[index], "R");
            break;

        default:
            printf("Invalid item selection.\n");
            break;
    }
}
void addOrder() {
    int itemNumber;
    char anotherOrder[2]; 
    char input[3];  

    do {
        displayMenu();
        
        itemNumber = -1;

        while (itemNumber < 1 || itemNumber > MAX_ITEMS) {
            printf("\nEnter the item number to order (S to show orders, R to remove an order): ");
            scanf("%s", input);

            if (strcmp(input, "S") == 0 || strcmp(input, "s") == 0) {
                clearScreen();
                showOrders();
                printf("\nPress Enter to go back...");
                getchar();
                getchar();
                clearScreen();
                addOrder();
                return;
            } else if (strcmp(input, "R") == 0 || strcmp(input, "r") == 0) {
                clearScreen();
                removeOrder();
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                clearScreen();
                addOrder();
                return;
            }

            itemNumber = atoi(input);

            if (itemNumber < 1 || itemNumber > MAX_ITEMS) {
                printf("Invalid item number. Please enter a number between 1 and %d.\n", MAX_ITEMS);
            }
        }

        if (orderCount >= MAX_ORDERS) {
            printf("Order limit reached! You can only place up to %d orders.\n", MAX_ORDERS);
            break;
        }

        orders[orderCount] = itemNumber - 1;  
        getOrderDetails(orderCount, itemNumber); 
        orderCount++;

        clearScreen();
        do {
            showOrders();
            printf("\nPress Y to order again or Press N to checkout? (Y or N): ");
            scanf("%s", anotherOrder);

            if (strcmp(anotherOrder, "Y") == 0 || strcmp(anotherOrder, "y") == 0) {
                break;
            } 
            else if (strcmp(anotherOrder, "N") == 0 || strcmp(anotherOrder, "n") == 0) {
                showOrders();
                break;
            } else {
                printf("Invalid input! Please enter 'Y' or 'N'.\n");
            }
        } while (1);

        clearScreen();
        
    } while (strcmp(anotherOrder, "Y") == 0 || strcmp(anotherOrder, "y") == 0);
}

void removeOrder() {
    int orderNumber;
    
    if (orderCount == 0) {
        printf("\nNo orders to remove!\n");
        return;
    }
    
    showOrders();
    
    printf("\nEnter the order number to remove: ");
    scanf("%d", &orderNumber);
    
    if (orderNumber < 1 || orderNumber > orderCount) {
        printf("Invalid order number!\n");
        return;
    }

    for (int i = orderNumber - 1; i < orderCount - 1; i++) {
        orders[i] = orders[i + 1];
        quantities[i] = quantities[i + 1];
        strcpy(types[i], types[i + 1]);
        strcpy(extraOrders[i], extraOrders[i + 1]);
        strcpy(dineOptions[i], dineOptions[i + 1]);
    }

    orderCount--;
    printf("Order number %d removed successfully!\n", orderNumber);
}

void showOrders() {
    if (orderCount == 0) {
        printf("No orders placed yet.\n");
        return;
    }

    printf("\n----------- Current Orders -----------\n");
    printf("%-3s %-20s %-10s %-10s %-10s %-10s\n", "No.", "Item", "Quantity", "Type", "Dine In/Take Out", "Price");
    printf("-------------------------------------------------------------------------\n");
    
    double total = 0.0;
    for (int i = 0; i < orderCount; i++) {
        char upperType = toupper(types[i][0]);
        double itemPrice;
        
        // Special handling for softdrinks
        if (orders[i] == 11) { // Softdrink index
            itemPrice = (toupper(extraOrders[i][0]) == 'L') ? specialPrices[orders[i]] : regularPrices[orders[i]];
        } else {
            itemPrice = (upperType == 'S') ? specialPrices[orders[i]] : regularPrices[orders[i]];
        }
        
        double lineTotal = itemPrice * quantities[i];
        
        // Add extra charge based on item type
        if (toupper(extraOrders[i][0]) == 'Y') {
            // For *silog meals, add extra rice charge
            if (orders[i] >= 5 && orders[i] <= 8) {  // Indices for *silog meals
                lineTotal += regularPrices[9] * quantities[i];  // Extra rice price (15.00)
            } 
            // For noodle dishes, add extra toppings charge
            else if (orders[i] <= 4) {  // Indices for noodle dishes
                lineTotal += regularPrices[10] * quantities[i];  // Extra toppings price (20.00)
            }
        }

        // Print the main order line
        printf("%-3d %-20s %-10d %-10s %-10s Php %.2f\n", 
            i + 1, menuItems[orders[i]], quantities[i], types[i], dineOptions[i], lineTotal);
        
        // Print extra item line if applicable
        if (toupper(extraOrders[i][0]) == 'Y') {
            if (orders[i] >= 5 && orders[i] <= 8) {
                printf("    + Extra Rice (Php %.2f)\n", regularPrices[9] * quantities[i]);
            } else if (orders[i] <= 4) {
                printf("    + Extra Toppings (Php %.2f)\n", regularPrices[10] * quantities[i]);
            }
        }
        
        total += lineTotal;
    }
    printf("-------------------------------------------------------------------------\n");
    printf("Total: Php %.2f\n", total);
}
void processCheckout() {
    double subtotal = 0.0;
    double takeoutFees = 0.0;
    double discount = 0.0;

    for (int i = 0; i < orderCount; i++) {
        int itemIndex = orders[i];
        char upperType = toupper(types[i][0]);
        double itemPrice;
        
        // Special handling for softdrinks
        if (itemIndex == 11) { // Softdrink index
            itemPrice = (toupper(extraOrders[i][0]) == 'L') ? specialPrices[itemIndex] : regularPrices[itemIndex];
        } else {
            itemPrice = (upperType == 'S') ? specialPrices[itemIndex] : regularPrices[itemIndex];
        }
        
        double lineTotal = itemPrice * quantities[i];

        // Add extra toppings charge if applicable
        if (toupper(extraOrders[i][0]) == 'Y') {
            lineTotal += regularPrices[10] * quantities[i];
        }

        subtotal += lineTotal;

        if (strcmp(dineOptions[i], "T") == 0) {
            takeoutFees += TAKEOUT_FEE * quantities[i];
        }
    }

    int discountCard = getDiscountCard();
    if (discountCard) {
        discount = subtotal * DISCOUNT_RATE;
    }

    int paymentMethod = getPaymentMethod();
    
    printReceipt(subtotal, takeoutFees, discount, subtotal + takeoutFees - discount, paymentMethod);
}
void printReceipt(double subtotal, double takeoutFees, double discount, double total, int paymentMethod) {
    clearScreen();
    printf("\n========================== RECEIPT =============================\n");
    printf("Date: %s\n", __DATE__);
    printf("----------------------------------------------------------------\n");
    printf("%-4s %-20s %-8s %-10s %s\n", "Qty", "Item", "Type", "Option", "Amount");
    printf("----------------------------------------------------------------\n");

    for (int i = 0; i < orderCount; i++) {
        int itemIndex = orders[i];
        char upperType = toupper(types[i][0]);
        double itemPrice;
        
        // Special handling for softdrinks
        if (itemIndex == 11) { // Softdrink index
            itemPrice = (toupper(extraOrders[i][0]) == 'L') ? specialPrices[itemIndex] : regularPrices[itemIndex];
        } else {
            itemPrice = (upperType == 'S') ? specialPrices[itemIndex] : regularPrices[itemIndex];
        }
        
        double lineTotal = itemPrice * quantities[i];
        
        // Add extra charge based on item type
        if (toupper(extraOrders[i][0]) == 'Y') {
            if (itemIndex >= 5 && itemIndex <= 8) {  // *silog meals
                lineTotal += regularPrices[9] * quantities[i];  // Extra rice
            } else if (itemIndex <= 4) {  // Noodle dishes
                lineTotal += regularPrices[10] * quantities[i];  // Extra toppings
            }
        }

        printf("%-4d %-20s %-8s %-10s Php %.2f\n",
            quantities[i],
            menuItems[itemIndex],
            types[i],
            dineOptions[i],
            lineTotal
        );

        // Show extra item line
        if (toupper(extraOrders[i][0]) == 'Y') {
            if (itemIndex >= 5 && itemIndex <= 8) {
                printf("     + Extra Rice (Php %.2f)\n", regularPrices[9] * quantities[i]);
            } else if (itemIndex <= 4) {
                printf("     + Extra Toppings (Php %.2f)\n", regularPrices[10] * quantities[i]);
            }
        }
    }

    printf("=============================================\n");
    printf("Subtotal:                      Php %.2f\n", subtotal);
    
    if (takeoutFees > 0) {
        printf("Take-out Fees:                 Php %.2f\n", takeoutFees);
    }
    
    if (discount > 0) {
        printf("Discount (20%%):                Php %.2f\n", discount);
    }
    
    printf("---------------------------------------------\n");
    printf("Total Amount:                  Php %.2f\n", total);
    printf("Payment Method:                %s\n", paymentMethods[paymentMethod]);
    
    if (paymentMethod == 1) {
        printf("Gcash/Maya Number:               %s\n", phoneNumber);
    } else if (paymentMethod == 2) {
        printf("Card Type:                    %s\n", cardType);
    }
    
    printf("=============================================\n");
    printf("          Thank you for your order!\n");
    printf("        Please come again. God Bless!\n");
    printf("=============================================\n");
}
int getDiscountCard() {
    char input[3];
    printf("Do you have a discount card? (Y for Yes, N for No): ");
    scanf("%s", input);
    return (strcmp(input, "Y") == 0 || strcmp(input, "y") == 0);
}

int getPaymentMethod() {
    int choice;
    
    printf("\nSelect Payment Method:\n");
    for (int i = 0; i < MAX_PAYMENT_METHODS; i++) {
        printf("%d. %s\n", i + 1, paymentMethods[i]);
    }

    do {
        printf("Enter your choice (1-%d): ", MAX_PAYMENT_METHODS);
        scanf("%d", &choice);
        if (choice < 1 || choice > MAX_PAYMENT_METHODS) {
            printf("Invalid choice! Please enter a number between 1 and %d.\n", MAX_PAYMENT_METHODS);
        }
    } while (choice < 1 || choice > MAX_PAYMENT_METHODS);

    // Clear input buffer
    while (getchar() != '\n');

    // Handle E-money payment
    if (choice == 2) {  // E-money option
        do {
            printf("Enter 11-digit Gcash/Maya number: ");
            scanf("%s", phoneNumber);
            if (strlen(phoneNumber) != 11 || strspn(phoneNumber, "0123456789") != 11) {
                printf("Invalid phone number! Please enter exactly 11 digits.\n");
                phoneNumber[0] = '\0';  // Clear the invalid input
            }
        } while (strlen(phoneNumber) != 11);
    }
    // Handle Card payment
    else if (choice == 3) {  // Card option
        char cardChoice;
        do {
            printf("Select card type (C for Credit, D for Debit): ");
            scanf(" %c", &cardChoice);
            cardChoice = toupper(cardChoice);
            
            if (cardChoice == 'C') {
                strcpy(cardType, "Credit");
            } else if (cardChoice == 'D') {
                strcpy(cardType, "Debit");
            } else {
                printf("Invalid choice! Please enter C or D.\n");
            }
        } while (cardChoice != 'C' && cardChoice != 'D');
    }

    return choice - 1;
}

int main() {
    char continueOrdering;

    printf("Welcome to the ABC123 Lomian atbp!\n");

    do {
        clearScreen();
        addOrder();
        processCheckout();
        printf("\nDo you want to place another order? (Y for Yes, N for No): ");
        scanf(" %c", &continueOrdering);
    } while (continueOrdering == 'Y' || continueOrdering == 'y');

    printf("Thank you for ordering!\n");
    return 0;
}
