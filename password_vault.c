#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

#define MAX_ACCOUNTS 20
#define MAX_WEBSITE 60
#define MAX_USERNAME 60
#define MAX_PASSWORD 60
#define FILE_NAME "vault.txt"
#define XOR_KEY '#'

#define COLOR_RESET   "\x1b[0m"
#define COLOR_TITLE   "\x1b[1;96m"
#define COLOR_OPTION  "\x1b[1;93m"
#define COLOR_PROMPT  "\x1b[1;92m"
#define COLOR_BORDER  "\x1b[1;95m"
#define COLOR_WARN    "\x1b[1;91m"

typedef struct {
    char website[MAX_WEBSITE];
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
} Account;

typedef struct {
    Account accounts[MAX_ACCOUNTS];
    int totalAccounts;
} Vault;

void xorEncryptDecrypt(char str[]);
void trimNewline(char str[]);
void readLine(const char prompt[], char input[], int size);
int readIntInRange(const char prompt[], int min, int max);

void initializeVault(Vault *vault);
void loadVaultFromFile(Vault *vault);
void saveVaultToFile(const Vault *vault);
int findAccountIndexByWebsite(const Vault *vault, const char website[]);

void addNewAccount(Vault *vault);
void displayAllAccounts(const Vault *vault);
void searchAccountByWebsite(const Vault *vault);
void modifyAccount(Vault *vault);
void deleteAccount(Vault *vault);
void countTotalSavedAccounts(const Vault *vault);
void findAccountWithLongestPassword(const Vault *vault);
void findAccountWithShortestPassword(const Vault *vault);
void countAccountsByWebsiteSubstring(const Vault *vault);
void checkWeakPasswords(const Vault *vault);
void showMenu(void);
void enableColors(void);

void enableColors(void) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;

    if (hOut == INVALID_HANDLE_VALUE) {
        return;
    }

    if (GetConsoleMode(hOut, &mode)) {
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

void xorEncryptDecrypt(char str[]) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        str[i] = str[i] ^ XOR_KEY;
    }
}

void trimNewline(char str[]) {
    int len = (int)strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void readLine(const char prompt[], char input[], int size) {
    printf("%s", prompt);
    if (fgets(input, size, stdin) == NULL) {
        input[0] = '\0';
        return;
    }
    trimNewline(input);
}

int readIntInRange(const char prompt[], int min, int max) {
    char line[40];
    int value;
    int parsed;

    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            return min;
        }

        parsed = sscanf(line, "%d", &value);
        if (parsed == 1 && value >= min && value <= max) {
            return value;
        }

        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

void initializeVault(Vault *vault) {
    vault->totalAccounts = 0;
}

void loadVaultFromFile(Vault *vault) {
    FILE *file;
    char line[256];
    char *website;
    char *username;
    char *encryptedPassword;

    file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (vault->totalAccounts >= MAX_ACCOUNTS) {
            break;
        }

        trimNewline(line);
        website = strtok(line, "|");
        username = strtok(NULL, "|");
        encryptedPassword = strtok(NULL, "|");

        if (website != NULL && username != NULL && encryptedPassword != NULL) {
            strcpy(vault->accounts[vault->totalAccounts].website, website);
            strcpy(vault->accounts[vault->totalAccounts].username, username);
            strcpy(vault->accounts[vault->totalAccounts].password, encryptedPassword);
            vault->totalAccounts++;
        }
    }

    fclose(file);
}

void saveVaultToFile(const Vault *vault) {
    FILE *file;
    int i;

    file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        printf("Error: Could not open %s for writing.\n", FILE_NAME);
        return;
    }

    for (i = 0; i < vault->totalAccounts; i++) {
        fprintf(file, "%s|%s|%s\n",
                vault->accounts[i].website,
                vault->accounts[i].username,
                vault->accounts[i].password);
    }

    fclose(file);
}

int findAccountIndexByWebsite(const Vault *vault, const char website[]) {
    int i;
    for (i = 0; i < vault->totalAccounts; i++) {
        if (strcmp(vault->accounts[i].website, website) == 0) {
            return i;
        }
    }
    return -1;
}

void addNewAccount(Vault *vault) {
    char website[MAX_WEBSITE];
    char username[MAX_USERNAME];
    char plainPassword[MAX_PASSWORD];
    int index;

    if (vault->totalAccounts >= MAX_ACCOUNTS) {
        printf("Vault is full. Maximum %d accounts allowed.\n", MAX_ACCOUNTS);
        return;
    }

    readLine("Enter website: ", website, sizeof(website));
    if (strlen(website) == 0) {
        printf("Website cannot be empty.\n");
        return;
    }

    index = findAccountIndexByWebsite(vault, website);
    if (index != -1) {
        printf("An account for this website already exists.\n");
        return;
    }

    readLine("Enter username: ", username, sizeof(username));
    if (strlen(username) == 0) {
        printf("Username cannot be empty.\n");
        return;
    }

    readLine("Enter password: ", plainPassword, sizeof(plainPassword));
    if (strlen(plainPassword) == 0) {
        printf("Password cannot be empty.\n");
        return;
    }

    strcpy(vault->accounts[vault->totalAccounts].website, website);
    strcpy(vault->accounts[vault->totalAccounts].username, username);
    strcpy(vault->accounts[vault->totalAccounts].password, plainPassword);

    xorEncryptDecrypt(vault->accounts[vault->totalAccounts].password);
    vault->totalAccounts++;

    saveVaultToFile(vault);
    printf("Account added successfully.\n");
}

void displayAllAccounts(const Vault *vault) {
    int i;

    if (vault->totalAccounts == 0) {
        printf("No accounts available.\n");
        return;
    }

    printf("\n--- All Accounts ---\n");
    for (i = 0; i < vault->totalAccounts; i++) {
        char decryptedPassword[MAX_PASSWORD];
        char websiteLabel[100] = "Website: ";

        strcpy(decryptedPassword, vault->accounts[i].password);
        xorEncryptDecrypt(decryptedPassword);

        strcat(websiteLabel, vault->accounts[i].website);

        printf("%d. %s\n", i + 1, websiteLabel);
        printf("   Username: %s\n", vault->accounts[i].username);
        printf("   Password: %s\n", decryptedPassword);
    }
}

void searchAccountByWebsite(const Vault *vault) {
    char website[MAX_WEBSITE];
    int index;

    readLine("Enter website to search: ", website, sizeof(website));
    index = findAccountIndexByWebsite(vault, website);

    if (index == -1) {
        printf("No account found for website: %s\n", website);
    } else {
        char decryptedPassword[MAX_PASSWORD];

        strcpy(decryptedPassword, vault->accounts[index].password);
        xorEncryptDecrypt(decryptedPassword);

        printf("Account found:\n");
        printf("Website : %s\n", vault->accounts[index].website);
        printf("Username: %s\n", vault->accounts[index].username);
        printf("Password: %s\n", decryptedPassword);
    }
}

void modifyAccount(Vault *vault) {
    char website[MAX_WEBSITE];
    char newUsername[MAX_USERNAME];
    char newPassword[MAX_PASSWORD];
    int index;

    if (vault->totalAccounts == 0) {
        printf("No accounts available to modify.\n");
        return;
    }

    readLine("Enter website to modify: ", website, sizeof(website));
    index = findAccountIndexByWebsite(vault, website);

    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    readLine("Enter new username: ", newUsername, sizeof(newUsername));
    if (strlen(newUsername) == 0) {
        printf("Username cannot be empty.\n");
        return;
    }

    readLine("Enter new password: ", newPassword, sizeof(newPassword));
    if (strlen(newPassword) == 0) {
        printf("Password cannot be empty.\n");
        return;
    }

    strcpy(vault->accounts[index].username, newUsername);
    strcpy(vault->accounts[index].password, newPassword);
    xorEncryptDecrypt(vault->accounts[index].password);

    saveVaultToFile(vault);
    printf("Account updated successfully.\n");
}

void deleteAccount(Vault *vault) {
    char website[MAX_WEBSITE];
    int index;
    int i;

    if (vault->totalAccounts == 0) {
        printf("No accounts available to delete.\n");
        return;
    }

    readLine("Enter website to delete: ", website, sizeof(website));
    index = findAccountIndexByWebsite(vault, website);

    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    for (i = index; i < vault->totalAccounts - 1; i++) {
        vault->accounts[i] = vault->accounts[i + 1];
    }

    vault->totalAccounts--;
    saveVaultToFile(vault);
    printf("Account deleted successfully.\n");
}

void countTotalSavedAccounts(const Vault *vault) {
    printf("Total saved accounts: %d\n", vault->totalAccounts);
}

void findAccountWithLongestPassword(const Vault *vault) {
    int i;
    int maxIndex;

    if (vault->totalAccounts == 0) {
        printf("No accounts available.\n");
        return;
    }

    maxIndex = 0;
    for (i = 1; i < vault->totalAccounts; i++) {
        if (strlen(vault->accounts[i].password) > strlen(vault->accounts[maxIndex].password)) {
            maxIndex = i;
        }
    }

    {
        char decryptedPassword[MAX_PASSWORD];
        strcpy(decryptedPassword, vault->accounts[maxIndex].password);
        xorEncryptDecrypt(decryptedPassword);

        printf("Account with longest password:\n");
        printf("Website : %s\n", vault->accounts[maxIndex].website);
        printf("Username: %s\n", vault->accounts[maxIndex].username);
        printf("Password: %s (Length: %d)\n", decryptedPassword, (int)strlen(decryptedPassword));
    }
}

void findAccountWithShortestPassword(const Vault *vault) {
    int i;
    int minIndex;

    if (vault->totalAccounts == 0) {
        printf("No accounts available.\n");
        return;
    }

    minIndex = 0;
    for (i = 1; i < vault->totalAccounts; i++) {
        if (strlen(vault->accounts[i].password) < strlen(vault->accounts[minIndex].password)) {
            minIndex = i;
        }
    }

    {
        char decryptedPassword[MAX_PASSWORD];
        strcpy(decryptedPassword, vault->accounts[minIndex].password);
        xorEncryptDecrypt(decryptedPassword);

        printf("Account with shortest password:\n");
        printf("Website : %s\n", vault->accounts[minIndex].website);
        printf("Username: %s\n", vault->accounts[minIndex].username);
        printf("Password: %s (Length: %d)\n", decryptedPassword, (int)strlen(decryptedPassword));
    }
}

void countAccountsByWebsiteSubstring(const Vault *vault) {
    char keyword[40];
    int i;
    int count = 0;

    if (vault->totalAccounts == 0) {
        printf("No accounts available.\n");
        return;
    }

    readLine("Enter substring to search in website names: ", keyword, sizeof(keyword));
    if (strlen(keyword) == 0) {
        printf("Substring cannot be empty.\n");
        return;
    }

    for (i = 0; i < vault->totalAccounts; i++) {
        if (strstr(vault->accounts[i].website, keyword) != NULL) {
            count++;
        }
    }

    printf("Accounts containing '%s' in website name: %d\n", keyword, count);
}

void checkWeakPasswords(const Vault *vault) {
    int i;
    int weakCount = 0;

    if (vault->totalAccounts == 0) {
        printf("No accounts available.\n");
        return;
    }

    printf("\nWeak passwords (length < 6):\n");
    for (i = 0; i < vault->totalAccounts; i++) {
        if (strlen(vault->accounts[i].password) < 6) {
            char decryptedPassword[MAX_PASSWORD];

            strcpy(decryptedPassword, vault->accounts[i].password);
            xorEncryptDecrypt(decryptedPassword);

            printf("- %s | %s | %s\n",
                   vault->accounts[i].website,
                   vault->accounts[i].username,
                   decryptedPassword);
            weakCount++;
        }
    }

    if (weakCount == 0) {
        printf("No weak passwords found.\n");
    } else {
        printf("Total weak passwords: %d\n", weakCount);
    }
}

void showMenu(void) {
    printf("\n%s+--------------------------------------------------+%s\n", COLOR_BORDER, COLOR_RESET);
    printf("%s|%s             %sPASSWORD Manager MENU%s              %s|%s\n",
           COLOR_BORDER, COLOR_RESET, COLOR_TITLE, COLOR_RESET, COLOR_BORDER, COLOR_RESET);
    printf("%s+--------------------------------------------------+%s\n", COLOR_BORDER, COLOR_RESET);
    printf("%s|%s %s 1.%s Add new account                         %s|%s\n", COLOR_BORDER, COLOR_RESET, COLOR_OPTION, COLOR_RESET, COLOR_BORDER, COLOR_RESET);
    printf("%s|%s %s 2.%s Display all accounts                    %s|%s\n", COLOR_BORDER, COLOR_RESET, COLOR_OPTION, COLOR_RESET, COLOR_BORDER, COLOR_RESET);
    printf("%s|%s %s 3.%s Search account by website               %s|%s\n", COLOR_BORDER, COLOR_RESET, COLOR_OPTION, COLOR_RESET, COLOR_BORDER, COLOR_RESET);
    printf("%s|%s %s 4.%s Modify account                          %s|%s\n", COLOR_BORDER, COLOR_RESET, COLOR_OPTION, COLOR_RESET, COLOR_BORDER, COLOR_RESET);
    printf("%s|%s %s 5.%s Delete account                          %s|%s\n", COLOR_BORDER, COLOR_RESET, COLOR_OPTION, COLOR_RESET, COLOR_BORDER, COLOR_RESET);
    printf("%s|%s %s 6.%s Count total saved accounts              %s|%s\n", COLOR_BORDER, COLOR_RESET, COLOR_OPTION, COLOR_RESET, COLOR_BORDER, COLOR_RESET);
    printf("%s|%s %s 7.%s Find account with longest password      %s|%s\n", COLOR_BORDER, COLOR_RESET, COLOR_OPTION, COLOR_RESET, COLOR_BORDER, COLOR_RESET);
    printf("%s|%s %s 8.%s Find account with shortest password     %s|%s\n", COLOR_BORDER, COLOR_RESET, COLOR_OPTION, COLOR_RESET, COLOR_BORDER, COLOR_RESET);
    printf("%s|%s %s 9.%s Count websites with substring           %s|%s\n", COLOR_BORDER, COLOR_RESET, COLOR_OPTION, COLOR_RESET, COLOR_BORDER, COLOR_RESET);
    printf("%s|%s %s10.%s Check weak passwords (< 6 chars)       %s|%s\n", COLOR_BORDER, COLOR_RESET, COLOR_OPTION, COLOR_RESET, COLOR_BORDER, COLOR_RESET);
    printf("%s|%s %s 0.%s Exit                                    %s|%s\n", COLOR_BORDER, COLOR_RESET, COLOR_WARN, COLOR_RESET, COLOR_BORDER, COLOR_RESET);
    printf("%s+--------------------------------------------------+%s\n", COLOR_BORDER, COLOR_RESET);
}

int main(void) {
    Vault vault;
    int choice;

    enableColors();
    initializeVault(&vault);
    loadVaultFromFile(&vault);

    do {
        showMenu();
        choice = readIntInRange(COLOR_PROMPT "Enter your choice: " COLOR_RESET, 0, 10);

        switch (choice) {
            case 1:
                addNewAccount(&vault);
                break;
            case 2:
                displayAllAccounts(&vault);
                break;
            case 3:
                searchAccountByWebsite(&vault);
                break;
            case 4:
                modifyAccount(&vault);
                break;
            case 5:
                deleteAccount(&vault);
                break;
            case 6:
                countTotalSavedAccounts(&vault);
                break;
            case 7:
                findAccountWithLongestPassword(&vault);
                break;
            case 8:
                findAccountWithShortestPassword(&vault);
                break;
            case 9:
                countAccountsByWebsiteSubstring(&vault);
                break;
            case 10:
                checkWeakPasswords(&vault);
                break;
            case 0:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 0);

    return 0;
}
