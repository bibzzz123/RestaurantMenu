// David, Jay Noel R.//
// Oliveros, Ronald Clarence H.//
//12-04-2024//
//Final Project of CC102-T (BSIT-1A)//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ITEMS 12
#define MAX_ORDERS 20
#define TAKEOUT_FEE 10.00
#define DISCOUNT_RATE 0.20
#define MAX_PAYMENT_METHODS 3

// Global arrays for menu items and prices (Same as original code)
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
char customerName[50] = "";
char customerAddress[100] = "";
char paymentReference[50] = ""; // New field for payment reference

int orderCount = 0;

char paymentMethods[MAX_PAYMENT_METHODS][20] = {
    "Cash",
    "E-money",
    "Card"
};

// Function prototypes (added new function prototypes)
void clearScreen();
void displayMenu();
void getOrderDetails(int index, int itemNumber);
void addOrder();
void removeOrder();
void showOrders();
void processCheckout();
void printReceipt(double subtotal, double takeoutFees, double discount, double total, int paymentMethod, double amountPaid, double changeDue);
int getDiscountCard();
int getPaymentMethod();
double getCashPayment(double total);
void clearAllOrders();
char* getPaymentReference(int paymentMethod);

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
    if (index < 0 || index >= MAX_ORDERS) {
        printf("Error: Invalid order index!\n");
        return;
    }

    if (itemNumber < 1 || itemNumber > MAX_ITEMS) {
        printf("Error: Invalid item number!\n");
        return;
    }

    int itemIndex = itemNumber - 1;
    char dineOption;
    char input[10];  
    
    // Dine option validation
    while (1) {
        printf("Dine In or Take Out? (D for Dine In, T for Take Out): ");
        if (scanf(" %c", &dineOption) != 1) {
            printf("Error: Invalid input!\n");
            while (getchar() != '\n');  
            continue;
        }
        
        dineOption = toupper(dineOption);
        if (dineOption == 'D' || dineOption == 'T') {
            break;
        }
        printf("Error: Please enter D or T only!\n");
    }
    
    dineOptions[index][0] = dineOption;
    dineOptions[index][1] = '\0';
    
    // Quantity validation
    while (1) {
        printf("Quantity: ");
        if (scanf("%d", &quantities[index]) != 1) {
            printf("Error: Please enter a valid number!\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (quantities[index] <= 0) {
            printf("Error: Quantity must be greater than 0!\n");
            continue;
        }
        
        if (quantities[index] > 100) {  
            printf("Error: Maximum quantity limit is 100!\n");
            continue;
        }
        break;
    }

    // Initialize with default values
    strcpy(extraOrders[index], "N");
    strcpy(types[index], "R");  // Default to Regular

    // Item-specific options
    if (itemIndex >= 0 && itemIndex <= 4) {  // Noodle dishes
        while (1) {
            printf("Special or Regular? (S for Special, R for Regular): ");
            scanf(" %s", input);
            if (strlen(input) == 1 && (toupper(input[0]) == 'S' || toupper(input[0]) == 'R')) {
                strcpy(types[index], input);
                break;
            }
            printf("Error: Please enter S or R only!\n");
        }
        
        while (1) {
            printf("Extra Toppings? (Y for Yes, N for No): ");
            scanf(" %s", input);
            if (strlen(input) == 1 && (toupper(input[0]) == 'Y' || toupper(input[0]) == 'N')) {
                strcpy(extraOrders[index], input);
                break;
            }
            printf("Error: Please enter Y or N only!\n");
        }
    }
    else if (itemIndex >= 5 && itemIndex <= 8) {  // Silog meals
        while (1) {
            printf("Extra Rice? Add Php 15.00 (Y for Yes, N for No): ");
            scanf(" %s", input);
            if (strlen(input) == 1 && (toupper(input[0]) == 'Y' || toupper(input[0]) == 'N')) {
                strcpy(extraOrders[index], input);
                strcpy(types[index], input);
                break;
            }
            printf("Error: Please enter Y or N only!\n");
        }
    }
    else if (itemIndex == 11) {  // Softdrinks
        while (1) {
            printf("Regular or Large? (R for Regular, L for Large): ");
            scanf(" %s", input);
            if (strlen(input) == 1 && (toupper(input[0]) == 'R' || toupper(input[0]) == 'L')) {
                strcpy(extraOrders[index], input);
                strcpy(types[index], input);
                break;
            }
            printf("Error: Please enter R or L only!\n");
        }
    }

    while (getchar() != '\n'); 
}

void getCustomerInfo() {
    // Clear previous input buffer
    while (getchar() != '\n');

    // Get Customer Name
    while (1) {
        printf("Enter your full name: ");
        if (fgets(customerName, sizeof(customerName), stdin) == NULL) {
            printf("Error: Invalid input!\n");
            continue;
        }

        // Remove newline character if present
        customerName[strcspn(customerName, "\n")] = 0;

        // Basic validation of name 
        int spaceCount = 0;
        for (int i = 0; customerName[i] != '\0'; i++) {
            if (customerName[i] == ' ') {
                spaceCount++;
            }
        }

        if (strlen(customerName) < 3 || spaceCount < 1) {
            printf("Error: Please enter your full name (first and last name).\n");
            continue;
        }

        break;
    }

    // Get Customer Address
    while (1) {
        printf("Enter your address: ");
        if (fgets(customerAddress, sizeof(customerAddress), stdin) == NULL) {
            printf("Error: Invalid input!\n");
            continue;
        }

        // Remove newline character if present
        customerAddress[strcspn(customerAddress, "\n")] = 0;

        if (strlen(customerAddress) < 3) {
            printf("Error: Please enter a valid address.\n");
            continue;
        }

        break;
    }
}

void clearAllOrders() {
    orderCount = 0;  
    
    for(int i = 0; i < MAX_ORDERS; i++) {
        orders[i] = 0;
        quantities[i] = 0;
        strcpy(types[i], "");
        strcpy(extraOrders[i], "");
        strcpy(dineOptions[i], "");
    }
    strcpy(phoneNumber, "");
    strcpy(cardType, "");
    
    // Clear customer information
    strcpy(customerName, "");
    strcpy(customerAddress, "");
}


void addOrder() {
    if (orderCount >= MAX_ORDERS) {
        printf("Error: Maximum order limit (%d) reached!\n", MAX_ORDERS);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    char input[10];
    int itemNumber;

    while (1) {
        displayMenu();
        
        printf("\nEnter the item number to order (S to show orders): ");
        if (scanf("%s", input) != 1) {
            printf("Error: Invalid input!\n");
            while (getchar() != '\n');
            continue;
        }

        if (strlen(input) == 1) {
            if (toupper(input[0]) == 'S') {
                clearScreen();
                showOrders();
                return;
            }
        }

        // Convert input to number
        itemNumber = atoi(input);
        if (itemNumber < 1 || itemNumber > MAX_ITEMS) {
            printf("Error: Please enter a number between 1 and %d, or S/R!\n", MAX_ITEMS);
            continue;
        }

        break;
    }

    orders[orderCount] = itemNumber - 1;
    getOrderDetails(orderCount, itemNumber);
    orderCount++;

    clearScreen();
    showOrders();
}

void removeOrder() {
    if (orderCount == 0) {
        printf("\nError: No orders to remove!\n");
        printf("\nPress Enter to continue...");
        getchar();
        getchar();
        clearScreen();
        addOrder();
        return;
    }

    int orderNumber, removeQuantity;
    
    // Remove order prompt
    printf("\n----- Remove Order -----\n");
    printf("Enter the order number to remove (0 to cancel): ");

    while (1) {
        if (scanf("%d", &orderNumber) != 1) {
            printf("Error: Please enter a valid number!\n");
            printf("Enter the order number to remove (0 to cancel): ");
            while (getchar() != '\n');
            continue;
        }

        if (orderNumber == 0) {
            clearScreen();
            addOrder();
            return;
        }

        if (orderNumber < 1 || orderNumber > orderCount) {
            printf("Error: Please enter a number between 1 and %d!\n", orderCount);
            printf("Enter the order number to remove (0 to cancel): ");
            continue;
        }

        // quantity to remove
        printf("Enter quantity to remove (current quantity: %d): ", quantities[orderNumber - 1]);
        while (1) {
            if (scanf("%d", &removeQuantity) != 1) {
                printf("Error: Please enter a valid number!\n");
                printf("Enter quantity to remove (current quantity: %d): ", quantities[orderNumber - 1]);
                while (getchar() != '\n');
                continue;
            }

            if (removeQuantity <= 0) {
                printf("Error: Quantity to remove must be greater than 0!\n");
                printf("Enter quantity to remove (current quantity: %d): ", quantities[orderNumber - 1]);
                continue;
            }

            if (removeQuantity > quantities[orderNumber - 1]) {
                printf("Error: Cannot remove more than current quantity (%d)!\n", 
                       quantities[orderNumber - 1]);
                printf("Enter quantity to remove (current quantity: %d): ", quantities[orderNumber - 1]);
                continue;
            }

            break;
        }

        break;
    }

    // Adjust quantity or remove entire order
    if (removeQuantity == quantities[orderNumber - 1]) {
        // Remove entire order by shifting remaining orders
        for (int i = orderNumber - 1; i < orderCount - 1; i++) {
            orders[i] = orders[i + 1];
            quantities[i] = quantities[i + 1];
            strcpy(types[i], types[i + 1]);
            strcpy(extraOrders[i], extraOrders[i + 1]);
            strcpy(dineOptions[i], dineOptions[i + 1]);
        }
        orderCount--;
        printf("\nOrder number %d completely removed!\n", orderNumber);

    } else {
        // Reduce quantity of the specific order
        quantities[orderNumber - 1] -= removeQuantity;
        printf("\nRemoved %d quantity from order number %d!\n", removeQuantity, orderNumber);
    }
    
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
    clearScreen();
    addOrder();
}

void showOrders() {
    if (orderCount == 0) {
        printf("\nNothing to show no orders yet!\n");
        printf("\nPress Enter to continue...");
        getchar();
        getchar();
        clearScreen();
        addOrder();
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
        if (orders[i] == 11) { 
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
    char choice[5];
    
    while (1) {
        printf("Press C to checkout, B to go back, or R to remove an order: ");
        scanf(" %s", choice);  

        if (toupper(choice[0]) == 'C') {  
            clearScreen();
            processCheckout();
            
            char continueChoice[2];
            printf("\nDo you want to place another order? (Y/N): ");
            scanf(" %s", continueChoice);
            
            if(toupper(continueChoice[0]) == 'Y') {
                clearAllOrders(); 
                clearScreen();
                addOrder(); 
            }
            else {
                printf("\nThank you for ordering! Please come again!\n");
                exit(0);  
            }
            return;
        }
        else if (toupper(choice[0]) == 'B') {
            clearScreen();
            addOrder();
            return;
        }
        else if (toupper(choice[0]) == 'R') {
            // Remove order functionality
            int orderNumber, removeQuantity;
            
            printf("\nEnter the order number to remove (0 to cancel): ");
            while (1) {
                if (scanf("%d", &orderNumber) != 1) {
                    printf("Error: Please enter a valid number!\n");
                    printf("Enter the order number to remove (0 to cancel): ");
                    while (getchar() != '\n');
                    continue;
                }

                if (orderNumber == 0) {
                    clearScreen();
                    showOrders();
                    return;
                }

                if (orderNumber < 1 || orderNumber > orderCount) {
                    printf("Error: Please enter a number between 1 and %d!\n", orderCount);
                    printf("Enter the order number to remove (0 to cancel): ");
                    continue;
                }

                // Get quantity to remove
                printf("Enter quantity to remove (current quantity: %d): ", quantities[orderNumber - 1]);
                while (1) {
                    if (scanf("%d", &removeQuantity) != 1) {
                        printf("Error: Please enter a valid number!\n");
                        printf("Enter quantity to remove (current quantity: %d): ", quantities[orderNumber - 1]);
                        while (getchar() != '\n');
                        continue;
                    }

                    if (removeQuantity <= 0) {
                        printf("Error: Quantity to remove must be greater than 0!\n");
                        printf("Enter quantity to remove (current quantity: %d): ", quantities[orderNumber - 1]);
                        continue;
                    }

                    if (removeQuantity > quantities[orderNumber - 1]) {
                        printf("Error: Cannot remove more than current quantity (%d)!\n", 
                               quantities[orderNumber - 1]);
                        printf("Enter quantity to remove (current quantity: %d): ", quantities[orderNumber - 1]);
                        continue;
                    }

                    break;
                }

                // Adjust quantity or remove entire order
                if (removeQuantity == quantities[orderNumber - 1]) {
                    // Remove entire order by shifting remaining orders
                    for (int i = orderNumber - 1; i < orderCount - 1; i++) {
                        orders[i] = orders[i + 1];
                        quantities[i] = quantities[i + 1];
                        strcpy(types[i], types[i + 1]);
                        strcpy(extraOrders[i], extraOrders[i + 1]);
                        strcpy(dineOptions[i], dineOptions[i + 1]);
                    }
                    orderCount--;
                    printf("\nOrder number %d completely removed!\n", orderNumber);
                } else {
                    // Reduce quantity of the specific order
                    quantities[orderNumber - 1] -= removeQuantity;
                    printf("\nRemoved %d quantity from order number %d!\n", removeQuantity, orderNumber);
                }
                
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                clearScreen();
                showOrders();
                return;
            }
        }
        else {
            printf("Invalid choice! Please press C to checkout, B to go back, or R to remove an order.\n");
        }
    }
}

void processCheckout() {
    double subtotal = 0.0;
    double takeoutFees = 0.0;

    // Existing subtotal and takeout fees calculation (unchanged)
    for (int i = 0; i < orderCount; i++) {
        int itemIndex = orders[i];
        char upperType = toupper(types[i][0]);
        double itemPrice;
        
        // Special handling for softdrinks
        if (itemIndex == 11) { 
            itemPrice = (toupper(extraOrders[i][0]) == 'L') ? specialPrices[itemIndex] : regularPrices[itemIndex];
        } else {
            itemPrice = (upperType == 'S') ? specialPrices[itemIndex] : regularPrices[itemIndex];
        }
        
        double lineTotal = itemPrice * quantities[i];

        // Add extra toppings or rice charge if applicable
        if (toupper(extraOrders[i][0]) == 'Y') {
            if (itemIndex >= 5 && itemIndex <= 8) {  // *silog meals
                lineTotal += regularPrices[9] * quantities[i];  // Extra rice
            } else if (itemIndex <= 4) {  // Noodle dishes
                lineTotal += regularPrices[10] * quantities[i];  // Extra toppings
            }
        }

        subtotal += lineTotal;

        // Count takeout fees per item and quantity
        if (strcmp(dineOptions[i], "T") == 0) {
            takeoutFees += (TAKEOUT_FEE * quantities[i]);
        }
    }
 
    getCustomerInfo();

    int discountCard = getDiscountCard();
    
    // NEW MODIFICATION: Calculate discount on subtotal + takeout fees
    double discountBase = subtotal + takeoutFees;
    double discount = discountCard ? (discountBase * DISCOUNT_RATE) : 0.0;

    int paymentMethod = getPaymentMethod();
    
    // Compute total amount
    double total = discountBase - discount;
    
    // Get payment reference for E-money and Card
    strcpy(paymentReference, getPaymentReference(paymentMethod));
    
    // Handle cash payment with change computation
    double amountPaid = 0.0;
    double changeDue = 0.0;
    
    if (paymentMethod == 0) { // Cash payment
        amountPaid = getCashPayment(total);
        changeDue = amountPaid - total;
    }
    
    printReceipt(subtotal, takeoutFees, discount, total, paymentMethod, amountPaid, changeDue);
}

void printReceipt(double subtotal, double takeoutFees, double discount, double total, int paymentMethod, double amountPaid, double changeDue) {
    clearScreen();
    printf("\n========================== RECEIPT =============================\n");
    printf("Date: %s\n", __DATE__);
    
    // Customer Information
    printf("Customer Name:               %s\n", customerName);
    printf("Address:                     %s\n", customerAddress);
    
    printf("----------------------------------------------------------------\n");
    printf("%-4s %-20s %-8s %-10s %s\n", "Qty", "Item", "Type", "Option", "Amount");
    printf("----------------------------------------------------------------\n");

    // Existing order details printing (unchanged)
    for (int i = 0; i < orderCount; i++) {
        int itemIndex = orders[i];
        char upperType = toupper(types[i][0]);
        double itemPrice;
        
        // Special handling for softdrinks
        if (itemIndex == 11) { 
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

    printf("================================================================\n");
    printf("Subtotal:                      Php %.2f\n", subtotal);
    
    if (takeoutFees > 0) {
        printf("Take-out Fees:                 Php %.2f\n", takeoutFees);
    }
    
    if (discount > 0) {
        printf("Discount (20%%):                Php %.2f\n", discount);
    }
    
    printf("-----------------------------------------------------------------\n");
    printf("Total Amount:                  Php %.2f\n", total);
    printf("Payment Method:                %s\n", paymentMethods[paymentMethod]);
    
    // Add payment method specific details
    if (paymentMethod == 1) { // E-money
        printf("Gcash/Maya Number:             %s\n", phoneNumber);
        printf("Payment Reference:             %s\n", paymentReference);
    } 
    else if (paymentMethod == 2) { // Card
        printf("Card Type:                     %s\n", cardType);
        printf("Payment Reference:             %s\n", paymentReference);
    }
    else if (paymentMethod == 0) { // Cash
        if (amountPaid > 0) {
            printf("Amount Paid:                   Php %.2f\n", amountPaid);
            printf("Change Due:                    Php %.2f\n", changeDue);
        }
    }
    
    printf("=============================================\n");
    printf("          Thank you for your order!\n");
    printf("        Please come again. God Bless!\n");
    printf("=============================================\n");
}


int getDiscountCard() {
    char input[3];
    printf("\nDo you have a discount card? (Y for Yes, N for No): ");
    scanf("%s", input);
    return (strcmp(input, "Y") == 0 || strcmp(input, "y") == 0);
}

int getPaymentMethod() {
    int choice;
    char input[20];
    
    printf("\nSelect Payment Method:\n");
    for (int i = 0; i < MAX_PAYMENT_METHODS; i++) {
        printf("%d. %s\n", i + 1, paymentMethods[i]);
    }

    while (1) {
        printf("Enter your choice (1-%d): ", MAX_PAYMENT_METHODS);
        if (scanf("%d", &choice) != 1) {
            printf("Error: Please enter a valid number!\n");
            while (getchar() != '\n');
            continue;
        }

        if (choice < 1 || choice > MAX_PAYMENT_METHODS) {
            printf("Error: Please enter a number between 1 and %d!\n", MAX_PAYMENT_METHODS);
            continue;
        }
        break;
    }

    while (getchar() != '\n');  

    // Handle E-money payment
    if (choice == 2) {
        while (1) {
            printf("Enter 11-digit Gcash/Maya number: ");
            if (scanf("%s", input) != 1) {
                printf("Error: Invalid input!\n");
                continue;
            }

            if (strlen(input) != 11) {
                printf("Error: Phone number must be exactly 11 digits!\n");
                continue;
            }

            if (strspn(input, "0123456789") != 11) {
                printf("Error: Phone number must contain only digits!\n");
                continue;
            }

            strcpy(phoneNumber, input);
            break;
        }
    }
    // Handle Card payment
    else if (choice == 3) {
        while (1) {
            printf("Select card type (C for Credit, D for Debit): ");
            scanf(" %c", &input[0]);
            input[0] = toupper(input[0]);
            
            if (input[0] == 'C') {
                strcpy(cardType, "Credit"); 
                break;
            }
            else if (input[0] == 'D') {
                strcpy(cardType, "Debit");
                break;
            }
            
            printf("Error: Please enter C or D only!\n");
        }
    }

    return choice - 1;
}
double getCashPayment(double total) {
    double amountPaid;
    
    clearScreen();
    
    while (1) {
        
        printf("Total amount due: Php %.2f\n", total);
        printf("Enter amount paid: Php ");
        
        if (scanf("%lf", &amountPaid) != 1) {
            printf("Error: Please enter a valid number!\n");
            while (getchar() != '\n');
            continue;
        }

        if (amountPaid < total) {
            printf("Error: Insufficient payment. Amount must be at least Php %.2f\n", total);
            continue;
        }

        return amountPaid;
    }
}

char* getPaymentReference(int paymentMethod) {
    static char reference[50];
    
    // Only prompt for reference for E-money and Card payments
    if (paymentMethod == 1 || paymentMethod == 2) { // E-money or Card
        //printf("Enter payment reference number: ");
        if (fgets(reference, sizeof(reference), stdin) == NULL) {
            reference[0] = '\0';
            return reference;
        }
        
        // Remove newline character
        reference[strcspn(reference, "\n")] = 0;
        
        // If input is empty or too short, keep prompting
        while (strlen(reference) < 3) {
            printf("Enter payment reference number: ");
            if (fgets(reference, sizeof(reference), stdin) == NULL) {
                reference[0] = '\0';
                return reference;
            }
            reference[strcspn(reference, "\n")] = 0;
        }
        
        return reference;
    }
    
    return ""; // Return empty string for cash payments
}

int main() {
    clearScreen();
    addOrder(); 
    printf("\nThank you for ordering!\n");
    return 0;
}