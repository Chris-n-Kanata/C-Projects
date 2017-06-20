
//
//
// Date: June 20, 2017
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//
// Some maximum settings.
//

#define MAX_PRODUCTS        200
#define MAX_ACCOUNTS        50
#define PRODUCT_NAME_SIZE   50
#define ACCOUNT_NAME_SIZE   50

#define MAX_BUFFER_SIZE     1024
#define MAX_STRING_SIZE     100

#define ADMIN_ACCOUNT       0
#define VENDOR_ACCOUNT      1
#define BUYER_ACCOUNT       3
#define ADMIN_ACCOUNT_ID    0

#define MAX_PASSWORD_ATTEMPTS   3

//
// Comment
//
typedef struct product {
    int id;
    int active;
    float price;
    int inventory;
    int account_id;
    char name[PRODUCT_NAME_SIZE];
} ProductType;

//
// Comment
//
int total_products = 0;
ProductType Product_Storage[MAX_PRODUCTS];

//
// Comment
//
typedef struct account {
    int id;
    int active;
    int password;
    int type;
    char name[ACCOUNT_NAME_SIZE];
} AccountType;

//
// Comment
//
int total_accounts = 0;
AccountType Account_Storage[MAX_ACCOUNTS];

//
// Comment
//
int current_product_id = -1;
int current_account_id = -1;

//
// Comment:
//
ProductType buyer_cart_items[MAX_PRODUCTS];

//
// Main Start Here
//
main() {

    // Says who is in the system.
    int my_account_id = -1;
    int my_account_type = -1;

    // Load our products.
    total_products = readProducts();
    if (total_products < 0) {
        exit(-1);
    }

    // Load our accounts.
    total_accounts = readAccounts();
    if (total_accounts < 0) {
        exit(-1);
    }

    // Get our account type, login and run the store.
    if (run_store(&my_account_type,&my_account_id) == 0) {
        exit(-1);
    }

    // Save our products.
    save_products();

    // Save our accounts.
    save_accounts();

    printf("\n\nThank-you for shopping with us...\n");
}

//
// Comment
//
int readProducts() {

    int done = 0;
    int loop = 0;
    FILE *fp = NULL;
    char buffer[MAX_BUFFER_SIZE];

    // Open our food file for processing.
    fp = fopen("products.txt","r");

    if (fp == NULL) {

        printf("Error - Could not find the products file - products.txt\n");
        return -1;

    } else {

        do {

            // Load in our file line.
            if (fgets(buffer,MAX_BUFFER_SIZE,fp) == NULL) {
                break;
            }

            // Trim the newline.
            buffer[strcspn(buffer, "\n")] = 0;

            // Break it up into parts.
            done = sscanf(buffer,"%i,%i,%[^,],%f,%i\n",&Product_Storage[loop].id,&Product_Storage[loop].inventory,&Product_Storage[loop].name,&Product_Storage[loop].price,&Product_Storage[loop].account_id);
            if (done != 5) {
                break;
            }

            // Set our product as active.
            Product_Storage[loop].active = 1;

            // Do we have the highest id.
            if (Product_Storage[loop].id > current_product_id) {
                current_product_id = Product_Storage[loop].id;
            }

            // Next product
            loop++;

            // Make sure we don't read too much.
            if (loop == MAX_PRODUCTS) {
                break;
            }

        } while (1);

        fclose(fp);
        return(loop);
    }
}

//
// Comment
//
int readAccounts() {

    int done = 0;
    int loop = 0;
    FILE *fp = NULL;
    char buffer[MAX_BUFFER_SIZE];

    // Open our account file for processing.
    fp = fopen("accounts.txt","r");

    if (fp == NULL) {

        printf("Error - Could not find the accounts file - accounts.txt\n");
        return -1;

    } else {

        do {

            // Load in our file line.
            if (fgets(buffer,MAX_BUFFER_SIZE,fp) == NULL) {
                break;
            }

            // Trim the newline.
            buffer[strcspn(buffer, "\n")] = 0;

            // Load in our items.
            done = sscanf(buffer,"%i,%[^,],%i,%i",&Account_Storage[loop].id,&Account_Storage[loop].name,&Account_Storage[loop].password,&Account_Storage[loop].type);
            if (done < 4) {
                break;
            }

            // Set our account as active.
            Account_Storage[loop].active = 1;

            // Do we have the highest id.
            if (Account_Storage[loop].id > current_account_id) {
                current_account_id = Account_Storage[loop].id;
            }

            // Next product
            loop++;

            // Make sure we don't read too much.
            if (loop == MAX_ACCOUNTS) {
                break;
            }

        } while (1);

        fclose(fp);
        return(loop);
    }
}

//
// Comment
//
void save_products() {

    int loop = 0;
    FILE *fp = NULL;

    // Products
    fp = fopen("products.txt","w+");
    if (fp == NULL) {
        printf("Error - Unable to find the products file - products.txt\n");
    } else {

        do {

            // Only save what is active.
            if (Product_Storage[loop].active == 1) {
                fprintf(fp,"%i,%i,%s,%f,%i\n",Product_Storage[loop].id,Product_Storage[loop].inventory,Product_Storage[loop].name,Product_Storage[loop].price,Product_Storage[loop].account_id);
            }
            loop++;

        } while (loop < total_products);

        fclose(fp);
    }
}

//
// Comment
//
void save_accounts() {

    int loop = 0;
    FILE *fp = NULL;

    // Accounts
    fp = fopen("accounts.txt","w+");
    if (fp == NULL) {
        printf("Error - Unable to find the accounts file - accounts.txt\n");
    } else {

        do {

            // Only save what is active.
            if (Account_Storage[loop].active == 1) {
                fprintf(fp,"%i,%s,%i,%i\n",Account_Storage[loop].id,Account_Storage[loop].name,Account_Storage[loop].password,Account_Storage[loop].type);
            }
            loop++;

        } while (loop < total_accounts);

        fclose(fp);
    }
}

//
// Helper Functions
//
void clearScreen() {
    system("cls");
}

int readInteger() {

    char buffer[MAX_STRING_SIZE];
    fgets(buffer,MAX_STRING_SIZE,stdin);
    return atoi(buffer);
}

float readFloat() {

    float temp = 0.0;
    float temp2 = 0.0;

    char buffer[MAX_STRING_SIZE];
    fgets(buffer,MAX_STRING_SIZE,stdin);
    buffer[strcspn(buffer, "\n")] = 0;

    temp = atof(buffer);
    temp2 = round(temp * 100.0)/100.0;
    return temp2;
}

char* readString(char *buffer, int buffer_size) {

    fgets(buffer,buffer_size,stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

//
// Comment
//
void view_products(account_type,account_id) {

    int loop = 0;

    clearScreen();

    // Change our title a bit.
    if (account_type == BUYER_ACCOUNT) {
        printf("\n\nOur Market Product Listing\n\n");
    }
    else {
        printf("\n\nYour Product List\n\n");
    }

    do {

        // Only show products that are active and are mine if I'm a vendor.
        if (Product_Storage[loop].active == 1) {

            // Buyers and admins can look at all items and vendors see only their own items.
            if (account_type == BUYER_ACCOUNT || (account_type == VENDOR_ACCOUNT && account_id == Product_Storage[loop].account_id)) {
                printf("Product ID: %i   Name: %s   Inventory: %i   Cost: $%f\n",Product_Storage[loop].id,
                   Product_Storage[loop].name,Product_Storage[loop].inventory,Product_Storage[loop].price);
            }
        }
        loop++;

    } while(loop < total_products);

    printf("\n\n");
}

//
// Comment
//
void add_product(account_id){

    char new_name[PRODUCT_NAME_SIZE];
    int new_quantity = 0;
    float new_price = 0.0;

    clearScreen();

    printf("\n\nEnter new item name: ");
    readString(new_name,PRODUCT_NAME_SIZE);

    do {
        printf("\nEnter new item price: ");
        new_price = readFloat();

    } while (new_price < 0);

    do {
        printf("\nEnter new item quantity: ");
        new_quantity = readInteger();

    } while (new_quantity < 0);  // 0 quantity is fine.

    // Add the new item to our list.
    Product_Storage[total_products].active = 1;
    Product_Storage[total_products].account_id = account_id;
    Product_Storage[total_products].id = current_product_id + 1;
    Product_Storage[total_products].inventory = new_quantity;
    Product_Storage[total_products].price = new_price;
    sprintf(Product_Storage[total_products].name,"%s",new_name);

    // Update number of products and ids.
    total_products++;
    current_product_id += 1;
}

void edit_product(account_id) {

    int fix;
    int grab;
    int loop = 0;

    char new_name[PRODUCT_NAME_SIZE];
    int new_quantity = 0;
    float new_price = 0.0;

    printf("\n\nWhich product ID do you want to edit ==>");
    fix = readInteger();

    // Find the item in our list.
    for (loop = 0 ; loop < MAX_PRODUCTS ; loop++) {
        if (Product_Storage[loop].id == fix) {
            break;
        }
    }

    // Did we find it.
    if (loop == MAX_PRODUCTS) {
        return;
    }

    // Make sure we can edit this product.
    if (Product_Storage[loop].account_id != account_id) {
        printf("You do not have permission to edit this product\n");
        return;
    }

    printf("\nEnter item name: ");
    readString(new_name,PRODUCT_NAME_SIZE);

    do {
        printf("\nEnter item price: ");
        new_price = readFloat();

    } while (new_price < 0);

    do {
        printf("\nEnter item quantity: ");
        new_quantity = readInteger();

    } while (new_quantity < 0);  // 0 quantity is fine.

    printf("\nAre you sure?  1=Yes   0=No  ==>");
    grab = readInteger();

    if (grab == 0) {
        return;
    }

    // Update the item.
    Product_Storage[loop].inventory = new_quantity;
    Product_Storage[loop].price = new_price;
    sprintf(Product_Storage[loop].name,"%s",new_name);
}

//
// Comment
//
void cut_product(account_id) {

    int cut = 0;
    int loop = 0;
    int grab = 0;

    printf("\n\nWhich product ID do you want to remove ==>");
    cut = readInteger();

    // Find the item in our list.
    for (loop = 0 ; loop < MAX_PRODUCTS ; loop++) {
        if (Product_Storage[loop].id == cut) {
            break;
        }
    }

    // Did we find it.
    if (loop == MAX_PRODUCTS) {
        return;
    }

    // Make sure we can remove this product.
    if (Product_Storage[loop].account_id != account_id) {
        printf("You do not have permission to remove this product\n");
        return;
    }

    printf("\nAre you sure?  1=Yes   0=No  ==>");
    grab = readInteger();

    if (grab == 0) {
        return;
    }

    // Turn this product off.
    Product_Storage[loop].active = 0;
}

//
// Comment
//
void clear_buyer_cart() {

    // Clear our cart.
    for (int loop = 0 ; loop < MAX_PRODUCTS ; loop++) {

        buyer_cart_items[loop].active = 0;
        buyer_cart_items[loop].id = -1;
        buyer_cart_items[loop].inventory = 0;
        buyer_cart_items[loop].price = 0;
    }
}

//
// Comment
//
int add_cart_item(item,quantity) {

    int loop = 0;
    int loop2 = 0;
    int current_items = 0;

    // Don't add things that aren't there.
    if (quantity <= 0) {
        return 0;
    }

    // First look for items in cart already.
    for (loop = 0 ; loop < MAX_PRODUCTS ; loop++) {
        if (buyer_cart_items[loop].active == 1 && buyer_cart_items[loop].id == item) {

            // Set our items already in the cart.
            current_items = buyer_cart_items[loop].inventory;
            break;
        }
    }

    // Not there - find a new spot in the cart.
    if (loop == MAX_PRODUCTS) {
        for (loop = 0 ; loop < MAX_PRODUCTS ; loop++) {
            if (buyer_cart_items[loop].active == 0) {
                break;
            }
        }
    }

    // Did we find a spot.
    if (loop == MAX_PRODUCTS) {
        return 1;
    }

    // Lookup product details and load the cart.
    for (loop2 = 0 ; loop2 < MAX_PRODUCTS ; loop2++) {
        if (Product_Storage[loop2].id == item) {

            // Is there enough to sell - include the inventory and what's already in the cart.
            if ((current_items + quantity) > Product_Storage[loop2].inventory) {
                return 2;
            }

            // Load our cart details adding items.
            buyer_cart_items[loop].active = 1;
            buyer_cart_items[loop].id = item;
            buyer_cart_items[loop].inventory += quantity;
            buyer_cart_items[loop].price = Product_Storage[loop2].price;
            sprintf(buyer_cart_items[loop].name,Product_Storage[loop2].name);

            break;
        }
    }

    // Did we find the product.
    if (loop2 == MAX_PRODUCTS) {
        return 1;
    }

    return 0;
}

//
// Comment
//
int commit_the_cart() {

    int loop = 0;
    int loop2 = 0;

    // First look for items in cart.
    for (loop = 0 ; loop < MAX_PRODUCTS ; loop++) {
        if (buyer_cart_items[loop].active == 1) {

            // Lookup product details and commit the cart.
            for (loop2 = 0 ; loop2 < MAX_PRODUCTS ; loop2++) {
                if (Product_Storage[loop2].id == buyer_cart_items[loop].id) {

                    // Sell our products.
                    Product_Storage[loop2].inventory -= buyer_cart_items[loop].inventory;
                }
            }
        }
    }

    // Clear the cart now.
    clear_buyer_cart();
}

//
// Comment
//
int time_for_change(float owe) {

    float change;
    float give;
    int grab;

    printf("\n\nPlease pay C$ %f ==>",owe);

    // Until the amount is correct.
    do {

        // Get the payment.
        give = readInteger();

        // Did they not give enough.
        if(owe > give) {
            printf("haha... no, try again  ==>");
        }

    } while(owe > give);

    printf("\n\nThank you for shopping Jared's Market - Your change:\n\n");

    change = give - owe;

    while(change>=100){
        change=change-100;
        printf("[$100]");
    }
    while (change>=20){
        change=change-20;
        printf("[$20]");
    }
    while (change>=10){
        change=change-10;
        printf("[$10]");
    }
    while (change>=5){
        change=change-5;
        printf("[$5+]");
    }
    while (change>=2){
        change=change-2;
        printf("($2)");
    }
    while (change>=1){
        change=change-1;
        printf("($1)");
    }
    while(change>=.25){
        change=change-.25;
        printf("(.25)");
    }
    while(change>=.1){
        change=change-.1;
        printf("(.10)");
    }
    while(change>=.05){
        change=change-.05;
        printf("(.05)");
    }

    printf("\n\nEnter any key to return to the main menu...");
    grab = readInteger();

    return 0;
}

//
// Comment
//
void buy_products() {

    int cart_return = 0;
    int item_number = 0;
    int quantity = 0;

    int loop = 0;
    int loop2 = 0;
    int grab = 0;

    float money=0;

    // Clear buyer cart first.
    clear_buyer_cart();

    // Offer buy options.
    printf("Ready to buy items?   Yes=1   No=0   ==>");
    grab = readInteger();

    if (grab == 1) {

        // Get all they wish to buy.
        do {

            printf("\n\nEnter the product ID number: ");
            item_number = readInteger();

            do {
                printf("\nEnter the quantity: ");
                quantity = readInteger();
            } while (quantity < 0);

            // Add these items to our cart if we can.
            cart_return = add_cart_item(item_number,quantity);

            if (cart_return == 1) {

                printf("\n\nSorry we couldn't find that item - try adding your item again...");
                continue;
            }

            // Not enough items to sell.
            if (cart_return == 2) {

                printf("\n\nSorry we don't have enough of that item - try making another selection...");
                continue;
            }

            printf("\nAnything more?   Yes=1   No=0   ==>");
            grab = readInteger();

        } while (grab == 1);

        printf("\n\nYour current purchases\n\n");

        // Print our cart next.
        do {

            // Print out and summarize our cart items.
            if (buyer_cart_items[loop].active == 1) {

                loop2++;

                printf("%d) Product ID: %i Name: %s Quantity: %i Cost: $%f\n",loop2,buyer_cart_items[loop].id,buyer_cart_items[loop].name,buyer_cart_items[loop].inventory,buyer_cart_items[loop].price);

                // Keep track of the cost.
                money = money + (buyer_cart_items[loop].price * buyer_cart_items[loop].inventory);
            }

            // Next item.
            loop++;

        } while(loop < MAX_PRODUCTS);

        // In case they select nothing.
        if (loop2 > 0) {

            printf("\nProceed to purchase?   Yes=1   No=0   ==>");
            grab = readInteger();

            if (grab == 1) {

                //Round our cost.
                money = round(money * 100.0)/100.0;

                // Process the payment.
                if (time_for_change(money) == 0) {
                    commit_the_cart();
                }
            }
            else {
                clear_buyer_cart();
            }
        }
    }
}

//
// Comment
//
int run_store(int *account_type,int *account_id) {

    int grab = -1;

    // Until the user quits.
    do {

        // Until we get a good answer.
        while (grab < 0 || grab > 4) {

            // Clear the screen to start
            system("cls");

            printf("\n\nWelcome to Jared's Market\n\nSelect Account Type: (1) - Buyer  (2) - Vendor  (3) - Admin  (4) - Quit\n\n==>");
            grab = readInteger();
        }

        // Handle buyer
        if (grab == 1) {

            *account_type = BUYER_ACCOUNT;
            *account_id = -1;

            view_products(*account_type,*account_id);
            buy_products();

            grab = -1;
        }

        // Handle the vendors.
        else if (grab == 2) {
            vendor_options(account_type,account_id);
            grab = -1;
        }

        // Handle the admin.
        else if (grab == 3) {
            admin_options(account_type,account_id);
            grab = -1;
        }

    } while (grab != 4);
}

//
// Comment
//
void vendor_options(int *account_type,int *account_id){

    int loop = 0;
    int grab = 0;
    int grab2 = 0;
    int my_id = 0;
    int new_password = 0;

    // Login as a vendor
    my_id = login_vendor();

    // Did we get in.
    if (my_id > 0) {

        // Set our values.
        *account_type = VENDOR_ACCOUNT;
        *account_id = my_id;

        do {

            clearScreen();

            printf("\n\nVendor Options\n\n0 = Quit\n1 = Add Item\n2 = Remove Item\n3 = Edit Item\n4 = View Products\n5 = Change your password\n\n==");
            grab = readInteger();

            // Add product
            if (grab == 1) {

                view_products(*account_type,*account_id);
                add_product(*account_id);
            }

            // Remove product
            else if (grab == 2) {

                view_products(*account_type,*account_id);
                cut_product(*account_id);
            }

            // Edit product
            else if (grab == 3) {

                view_products(*account_type,*account_id);
                edit_product(*account_id);

            }

            // View product
            else if (grab == 4) {

                view_products(*account_type,*account_id);

                printf("Enter a key to continue...");
                grab2 = readInteger();
            }

            else if (grab == 5) {

                printf("\n\nNew password: ");
                new_password = readInteger();

                // Find our own account.
                for (loop = 0 ; loop < MAX_ACCOUNTS ; loop++) {
                    if (Account_Storage[loop].id == my_id) {
                        break;
                    }
                }

                // Did we find it.
                if (loop < MAX_ACCOUNTS) {

                    // Set the new password.
                    Account_Storage[loop].password = new_password;
                }
            }

        } while (grab != 0);
    }
    else {
        printf("Could not login to the selected vendor account\n");
    }
}

//
// Comment
//
void admin_options(int *account_type,int *account_id) {

    int grab = 0;
    int grab2 = 0;
    int loop = 0;

    clearScreen();

    printf("\n\nEnter the Admin Account Password: ");
    grab = readInteger();

    // Get the admin account - only one.
    for (loop = 0 ; loop < MAX_ACCOUNTS ; loop++) {
        if (Account_Storage[loop].type == ADMIN_ACCOUNT) {
            break;
        }
    }

    // Did we find it.
    if (loop == MAX_ACCOUNTS) {
        printf("Could not find the admin account\n");
        return;
    }

    // Is this the proper password.
    if (grab != Account_Storage[loop].password) {
        printf("\nIncorrect password for admin - enter a key to continue...\n");
        grab = readInteger();
        return;
    }

    // Set our values.
    *account_type = ADMIN_ACCOUNT;
    *account_id = ADMIN_ACCOUNT_ID;

    do {

        clearScreen();

        printf("\n\nAdmin Account Options\n\n0 = Quit\n1 = Force Save\n2 = Force Read\n3 = Create New Account\n4 = Edit Existing Account\n5 = Delete Account\n6 = View Accounts\n7 = Edit Admin Password\n\n==");
        grab = readInteger();

        if (grab == 1) {

            save_products();
            save_accounts();
        }

        else if (grab == 2) {

            current_product_id = -1;
            current_account_id = -1;

            total_products = 0;
            total_accounts = 0;

            // Reload our products.
            total_products = readProducts();
            if (total_products < 0) {
                return;
            }

            // Reload our accounts.
            total_accounts = readAccounts();
            if (total_accounts < 0) {
                return;
            }
        }

        else if (grab == 3) {
            addAccount();
        }

        else if (grab == 4) {
            view_accounts();
            editAccount();
        }

        else if (grab == 5) {
            view_accounts();
            deleteAccount();
        }

        else if (grab == 6) {

            view_accounts();

            printf("Enter a key to continue...");
            grab2 = readInteger();
        }

        else if (grab == 7) {

            printf("\n\nEnter new admin account password\n\n== ");
            Account_Storage[loop].password = readInteger();

            printf("\nPassword changed for admin - enter a key to continue...\n");
            grab2 = readInteger();
        }

        else if (grab == 0) {
            return;
        }

   } while (1);
}

//
// Comment
//
int login_vendor() {

    int loop = 0;
    int loop2 = 0;
    int loop3 = 0;

    int grab = 3;
    int pass = 0;

    clearScreen();

    printf("\n\nVendor Listing\n\n");

    // Show a list of vendor accounts.
    do {

        // Only show the vendor accounts
        if (Account_Storage[loop].type == VENDOR_ACCOUNT) {
            printf("Account ID: %i Name: %s\n",Account_Storage[loop].id,Account_Storage[loop].name);
        }
        loop++;

    } while (loop < total_accounts);

    printf("\nSelect a vendor account ID ==>");
    grab = readInteger();

    // Find the account in our list
    for (loop2 = 0 ; loop2 < MAX_ACCOUNTS ; loop2++) {
        if (Account_Storage[loop2].id == grab) {
            break;
        }
    }

    // Did we find the account.
    if (loop2 == MAX_ACCOUNTS) {
        return -1;
    }

   do {

        printf("\nEnter the password for %s: ",Account_Storage[loop2].name);
        pass = readInteger();

        // Did the proper password get entered.
        if (pass == Account_Storage[loop2].password){
            return Account_Storage[loop2].id;
        }
        else {
            loop3++;
            printf("\nIncorrect password, %d attempts remaining...enter any key to continue\n",MAX_PASSWORD_ATTEMPTS-loop3);
            pass = readInteger();
        }

   } while (loop3 < MAX_PASSWORD_ATTEMPTS);

   // Did they failed too much
   if (loop3 == MAX_PASSWORD_ATTEMPTS) {
        printf("\nAccount suspended...enter any key to continue\n");
        pass = readInteger();
        return -1;
   }

   return Account_Storage[loop2].id;
}

//
// Comment
//
void view_accounts() {

    int loop = 0;

    clearScreen();

    printf("\nVendor Accounts\n\n");

    do {

        // Only show accounts that are active.
        if (Account_Storage[loop].active == 1) {
            printf("Account ID: %i Name: %s\n",Account_Storage[loop].id,Account_Storage[loop].name);
        }
        loop++;

    } while(loop < total_accounts);

    printf("\n");
}

void addAccount(){

    int loop = 0;
    int new_account_password = 0;
    char new_account_name[ACCOUNT_NAME_SIZE];

    clearScreen();

    // Do we have too many accounts?
    if (total_accounts == MAX_ACCOUNTS) {
        printf("Sorry - our vendor list is full, try next year...\n");
        return;
    }

    printf("\n\nEnter the new vendor name: ");
    readString(new_account_name,ACCOUNT_NAME_SIZE);

    printf("\nEnter the new vendor password: ");
    new_account_password = readInteger();

    // Add the new vendor.
    Account_Storage[total_accounts].type = VENDOR_ACCOUNT;
    Account_Storage[total_accounts].id = current_account_id + 1;
    Account_Storage[total_accounts].active = 1;
    Account_Storage[total_accounts].password = new_account_password;
    sprintf(Account_Storage[total_accounts].name,"%s",new_account_name);

    total_accounts = total_accounts + 1;
    current_account_id = current_account_id + 1;
}

void editAccount(){

    int grab = 0;
    int loop = 0;
    int new_account_password = 0;
    char new_account_name[ACCOUNT_NAME_SIZE];

    printf("\nEnter the Vendor ID to edit : ");
    grab = readInteger();

    if (grab == ADMIN_ACCOUNT_ID) {
        printf("\nThe Admin account cannot be edited...enter a key to continue");
        grab = readInteger();
        return;
    }

    // Find the vendor
    for (loop = 0 ; loop < MAX_ACCOUNTS ; loop++) {
        if (Account_Storage[loop].id == grab) {
            break;
        }
    }

    // Did we find it
    if (loop == MAX_ACCOUNTS) {
        return;
    }

    printf("\nEnter the vendor name: ");
    readString(new_account_name,ACCOUNT_NAME_SIZE);

    printf("\nEnter the vendor password: ");
    new_account_password = readInteger();

    printf("\nAre you sure?   1=Yes  0=No  ==>");
    grab = readInteger();

    if (grab != 1) {
        return;
    }

    // Update the vendor.
    Account_Storage[loop].password = new_account_password;
    sprintf(Account_Storage[loop].name,"%s",new_account_name);
}

void deleteAccount(){

    int grab = 0;
    int loop = 0;

    printf("\nEnter the Vendor ID to delete : ");
    grab = readInteger();

    if (grab == ADMIN_ACCOUNT_ID) {
        printf("\nThe Admin account cannot be deleted...enter a key to continue");
        grab = readInteger();
        return;
    }

    // Find the vendor
    for (loop = 0 ; loop < MAX_ACCOUNTS ; loop++) {
        if (Account_Storage[loop].id == grab) {
            break;
        }
    }

    // Did we find it
    if (loop == MAX_ACCOUNTS) {
        return;
    }

    printf("\nAre you sure   1=Yes   0=No   ==>");
    grab = readInteger();

    if (grab != 1) {
        return;
    }

    Account_Storage[loop].active = 0;

}


