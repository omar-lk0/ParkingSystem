#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TRANSACTIONS 100
#define FILENAME "/Users/omarlekhal/Desktop/ProgrammingProject/assignment_3/transactions.csv"

// ─── ANSI Color Codes ─────────────────────────────────────────────────────
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define WHITE   "\033[37m"

// ─── Data Structures ──────────────────────────────────────────────────────
typedef struct {
    char license_plate[20];
    char zone[20];
    int  duration;
    double cost;
} Transaction;

typedef struct {
    char   username[50];
    char   currency[10];
    double currency_rate; // conversion multiplier from RON
} AppSettings;

// ─── Global App Settings ──────────────────────────────────────────────────
AppSettings app_settings;

// ─── Initialize Default Settings ─────────────────────────────────────────
void init_settings(void) {
    strcpy(app_settings.username,      "Guest");
    strcpy(app_settings.currency,      "RON");
    app_settings.currency_rate = 1.0;
}

// ─── Safe Integer Input ───────────────────────────────────────────────────
// Returns 1 on success, 0 on bad input. Clears stdin on failure.
int read_int(int *out) {
    if (scanf("%d", out) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }
    return 1;
}

// ─── Clear Screen ─────────────────────────────────────────────────────────
void clear(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ─── Wait For Enter ───────────────────────────────────────────────────────
// Flushes remaining input then blocks until the user presses Enter.
void wait_for_enter(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

// ─── Zone Helpers ─────────────────────────────────────────────────────────
// Returns the ANSI color code matching the zone name.
const char *get_zone_color(const char *zone) {
    if (strcmp(zone, "Yellow") == 0) return YELLOW;
    if (strcmp(zone, "Red")    == 0) return RED;
    if (strcmp(zone, "Green")  == 0) return GREEN;
    return WHITE;
}

// Returns the hourly rate for a zone, or -1 for an unknown zone.
double get_zone_rate(const char *zone) {
    if (strcmp(zone, "Yellow") == 0) return 1.0;
    if (strcmp(zone, "Red")    == 0) return 1.5;
    if (strcmp(zone, "Green")  == 0) return 2.0;
    return -1.0;
}

// Capitalises the first letter and lower-cases the rest in-place.
// Allows the user to type "yellow", "YELLOW", etc.
void normalize_zone(char *zone) {
    if (!zone || zone[0] == '\0') return;
    if (zone[0] >= 'a' && zone[0] <= 'z') zone[0] = (char)(zone[0] - 32);
    for (int i = 1; zone[i]; i++) {
        if (zone[i] >= 'A' && zone[i] <= 'Z') zone[i] = (char)(zone[i] + 32);
    }
}

// ─── Print Cost With Current Currency ────────────────────────────────────
// Converts from RON using the stored rate and prints with CYAN colour.
void print_cost(double cost_ron) {
    double converted = cost_ron * app_settings.currency_rate;
    printf(CYAN "%.2f %s" RESET, converted, app_settings.currency);
}

// ─── Load Transactions From CSV ──────────────────────────────────────────
// Skips empty lines and any line that does not parse as exactly 4 fields
// (handles headers, trailing newlines, etc.).
// Returns the number of transactions successfully read.
int load_transactions(Transaction transactions[], int max) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) return 0;

    int  count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) != NULL && count < max) {
        // Skip blank or whitespace-only lines
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') continue;

        Transaction t;
        if (sscanf(line, "%19[^,],%19[^,],%d,%lf",
                   t.license_plate, t.zone, &t.duration, &t.cost) != 4) {
            continue; // malformed line or header row — skip silently
        }
        transactions[count++] = t;
    }

    fclose(file);

    if (count == max) {
        printf("  " YELLOW "Warning:" RESET
               " only the first %d transactions were loaded.\n", max);
    }
    return count;
}

// ─── Save All Transactions Back To CSV ───────────────────────────────────
// Overwrites the file; returns 1 on success, 0 on failure.
int save_transactions(Transaction transactions[], int count) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) return 0;

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%d,%.2f\n",
                transactions[i].license_plate,
                transactions[i].zone,
                transactions[i].duration,
                transactions[i].cost);
    }
    fclose(file);
    return 1;
}

// ─── Print Transaction Table Row ─────────────────────────────────────────
// Shared by View History and Delete so the format is identical everywhere.
void print_transaction_row(int index, const Transaction *t) {
    printf("  %-5d " CYAN "%-14s" RESET " %s%-10s" RESET " %-8d ",
           index,
           t->license_plate,
           get_zone_color(t->zone),
           t->zone,
           t->duration);
    print_cost(t->cost);
    printf("\n");
}

// ─── Main Menu Header ─────────────────────────────────────────────────────
void header(void) {
    printf(BOLD  "=====================================================\n" RESET);
    printf(BOLD  "       Online Parking Payment System\n"                 RESET);
    printf("       Welcome, " CYAN "%s" RESET "\n", app_settings.username);
    printf(BOLD  "=====================================================\n" RESET);
    printf("  " GREEN  "1" RESET " - Pay for Parking\n");
    printf("  " BLUE   "2" RESET " - View History\n");
    printf("  " RED    "3" RESET " - Delete a Transaction\n");
    printf("  " YELLOW "4" RESET " - Settings\n");
    printf("  " WHITE  "0" RESET " - Exit\n");
    printf(BOLD  "=====================================================\n" RESET);
}

// ─── Pay For Parking ──────────────────────────────────────────────────────
void pay(void) {
    int option;
    do {
        clear();
        printf(BOLD "=====================================================\n" RESET);
        printf(BOLD "                  Pay for Parking\n"                  RESET);
        printf(BOLD "=====================================================\n" RESET);

        char license_plate[20];
        char zone[20];
        int  duration;

        // --- License plate ---
        printf("  Enter License Plate "
               CYAN "(format: 2 letters + 2 digits + 3 letters, e.g., TM99XYZ)" RESET ": ");
        fflush(stdout);
        scanf("%19s", license_plate);

        if (strlen(license_plate) < 3) {
            printf("\n  " RED "Invalid license plate!" RESET
                   " Must be at least 3 characters (e.g., TM99XYZ).\n");
            printf("  Press Enter to try again...\n");
            wait_for_enter();
            continue;
        }

        // --- Zone ---
        printf("  Enter Zone "
               CYAN "(Yellow / Red / Green)" RESET ": ");
        fflush(stdout);
        scanf("%19s", zone);
        normalize_zone(zone);

        double rate = get_zone_rate(zone);
        if (rate < 0) {
            printf("\n  " RED "Invalid zone!" RESET
                   " Accepted values: "
                   YELLOW "Yellow" RESET ", "
                   RED    "Red"    RESET ", "
                   GREEN  "Green"  RESET ".\n");
            printf("  Press Enter to try again...\n");
            wait_for_enter();
            continue;
        }

        // --- Duration ---
        printf("  Enter Duration "
               CYAN "(whole number of hours ≥ 1, e.g., 2)" RESET ": ");
        fflush(stdout);
        if (!read_int(&duration) || duration <= 0) {
            printf("\n  " RED "Invalid duration!" RESET
                   " Must be a positive whole number (e.g., 1, 2, 3).\n");
            printf("  Press Enter to try again...\n");
            getchar();
            continue;
        }

        double cost = duration * rate;

        // --- Persist ---
        FILE *file = fopen(FILENAME, "a");
        if (file == NULL) {
            printf("\n  " RED "Error: Could not save transaction to file!" RESET "\n");
        } else {
            fprintf(file, "%s,%s,%d,%.2f\n",
                    license_plate, zone, duration, cost);
            fclose(file);

            printf("\n  " GREEN "Payment Successful!" RESET "\n");
            printf("  ────────────────────────────────────────────────\n");
            printf("  License Plate : " CYAN "%s"    RESET "\n", license_plate);
            printf("  Zone          : %s%s"           RESET "\n",
                   get_zone_color(zone), zone);
            printf("  Duration      : " CYAN "%d hour(s)" RESET "\n", duration);
            printf("  Rate          : " CYAN "%.2f RON/hour" RESET "\n", rate);
            printf("  Total Cost    : ");
            print_cost(cost);
            printf("\n");
            printf("  ────────────────────────────────────────────────\n");
        }

        printf("\n  " WHITE "0" RESET " - Back to Menu\n");
        printf(BOLD "=====================================================\n" RESET);
        printf("Enter option: ");
        fflush(stdout);
        if (!read_int(&option)) option = -1;

    } while (option != 0);
}

// ─── View History ─────────────────────────────────────────────────────────
void view_history(void) {
    int option;
    do {
        // Reload on every iteration so new payments appear immediately
        Transaction transactions[MAX_TRANSACTIONS];
        int count = load_transactions(transactions, MAX_TRANSACTIONS);

        clear();
        printf(BOLD "=====================================================\n" RESET);
        printf(BOLD "                   View History\n"                    RESET);
        printf(BOLD "=====================================================\n" RESET);

        if (count == 0) {
            printf("  " YELLOW "No transaction history found.\n" RESET);
        } else {
            printf("  %-5s %-14s %-10s %-8s %s\n",
                   "#", "Plate", "Zone", "Hours", "Cost");
            printf("  ────────────────────────────────────────────────\n");
            for (int i = 0; i < count; i++) {
                print_transaction_row(i + 1, &transactions[i]);
            }
        }

        printf(BOLD "=====================================================\n" RESET);
        printf("  " WHITE "0" RESET " - Back\n");
        printf(BOLD "=====================================================\n" RESET);
        printf("Enter option: ");
        fflush(stdout);
        if (!read_int(&option)) option = -1;
    } while (option != 0);
}

// ─── Delete a Transaction ────────────────────────────────────────────────
void delete_transaction(void) {
    int option;
    do {
        Transaction transactions[MAX_TRANSACTIONS];
        int count = load_transactions(transactions, MAX_TRANSACTIONS);

        clear();
        printf(BOLD "=====================================================\n" RESET);
        printf(BOLD "               Delete a Transaction\n"                RESET);
        printf(BOLD "=====================================================\n" RESET);

        if (count == 0) {
            printf("  " YELLOW "No transactions available to delete.\n" RESET);
            printf(BOLD "=====================================================\n" RESET);
            printf("  " WHITE "0" RESET " - Back\n");
            printf(BOLD "=====================================================\n" RESET);
            printf("Enter option: ");
            fflush(stdout);
            if (!read_int(&option)) option = -1;
            continue;
        }

        printf("  %-5s %-14s %-10s %-8s %s\n",
               "#", "Plate", "Zone", "Hours", "Cost");
        printf("  ────────────────────────────────────────────────\n");
        for (int i = 0; i < count; i++) {
            print_transaction_row(i + 1, &transactions[i]);
        }

        printf(BOLD "=====================================================\n" RESET);
        printf("  Enter the " RED "# of the transaction to delete" RESET
               " (1–%d)\n", count);
        printf("  " WHITE "0" RESET " - Back\n");
        printf(BOLD "=====================================================\n" RESET);
        printf("Enter option: ");
        fflush(stdout);
        if (!read_int(&option)) { option = -1; continue; }

        if (option == 0) break;

        if (option < 1 || option > count) {
            printf("\n  " RED "Invalid selection!" RESET
                   " Please enter a number between 1 and %d.\n", count);
            printf("  Press Enter to continue...\n");
            wait_for_enter();
            continue;
        }

        // Store the plate for the confirmation message
        char deleted_plate[20];
        strcpy(deleted_plate, transactions[option - 1].license_plate);

        // Shift the array left over the deleted slot
        for (int i = option - 1; i < count - 1; i++) {
            transactions[i] = transactions[i + 1];
        }
        count--;

        if (save_transactions(transactions, count)) {
            printf("\n  " GREEN "Transaction for plate " RESET
                   CYAN "%s" RESET
                   GREEN " has been deleted." RESET "\n", deleted_plate);
        } else {
            printf("\n  " RED "Error: Could not write changes to file!" RESET "\n");
        }

        printf("  Press Enter to continue...\n");
        wait_for_enter();

    } while (option != 0);
}

// ─── Change Username Sub-menu ─────────────────────────────────────────────
void change_username(void) {
    clear();
    printf(BOLD "=====================================================\n" RESET);
    printf(BOLD "                  Change Username\n"                   RESET);
    printf(BOLD "=====================================================\n" RESET);
    printf("  Current username : " CYAN "%s" RESET "\n", app_settings.username);
    printf("  Enter new username "
           CYAN "(letters/spaces, max 49 chars)" RESET ": ");
    fflush(stdout);

    // Flush leftover newline from the menu scanf, then read a full line
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    fgets(app_settings.username, sizeof(app_settings.username), stdin);
    app_settings.username[strcspn(app_settings.username, "\n")] = '\0';

    if (strlen(app_settings.username) == 0) {
        strcpy(app_settings.username, "Guest");
    }

    printf("\n  " GREEN "Username updated to: " RESET
           CYAN "%s" RESET "\n", app_settings.username);
    printf("  Press Enter to continue...\n");
    getchar();
}

// ─── Change Currency Sub-menu ─────────────────────────────────────────────
void change_currency(void) {
    int option;
    clear();
    printf(BOLD "=====================================================\n" RESET);
    printf(BOLD "                  Change Currency\n"                   RESET);
    printf(BOLD "=====================================================\n" RESET);
    printf("  " GREEN  "1" RESET " - RON  Romanian Leu   (rate: 1.0000)\n");
    printf("  " BLUE   "2" RESET " - EUR  Euro           (rate: 0.2010)\n");
    printf("  " CYAN   "3" RESET " - USD  US Dollar      (rate: 0.2180)\n");
    printf("  " WHITE  "0" RESET " - Back\n");
    printf(BOLD "=====================================================\n" RESET);
    printf("Enter option: ");
    fflush(stdout);
    if (!read_int(&option)) option = -1;

    switch (option) {
        case 1: strcpy(app_settings.currency, "RON"); app_settings.currency_rate = 1.0000; break;
        case 2: strcpy(app_settings.currency, "EUR"); app_settings.currency_rate = 0.2010; break;
        case 3: strcpy(app_settings.currency, "USD"); app_settings.currency_rate = 0.2180; break;
        case 0: return;
        default:
            printf("\n  " RED "Invalid option!" RESET "\n");
            printf("  Press Enter to continue...\n");
            wait_for_enter();
            return;
    }

    printf("\n  " GREEN "Currency updated to: " RESET
           CYAN "%s" RESET "\n", app_settings.currency);
    printf("  Press Enter to continue...\n");
    wait_for_enter();
}

// ─── Settings Menu ────────────────────────────────────────────────────────
void settings_menu(void) {
    int option;
    do {
        clear();
        printf(BOLD "=====================================================\n" RESET);
        printf(BOLD "                     Settings\n"                    RESET);
        printf(BOLD "=====================================================\n" RESET);
        printf("  Username : " CYAN "%s" RESET "\n", app_settings.username);
        printf("  Currency : " CYAN "%s" RESET
               "  (conversion rate: %.4f)\n",
               app_settings.currency, app_settings.currency_rate);
        printf(BOLD "─────────────────────────────────────────────────────\n" RESET);
        printf("  " GREEN  "1" RESET " - Change Username\n");
        printf("  " BLUE   "2" RESET " - Change Currency\n");
        printf("  " WHITE  "0" RESET " - Back\n");
        printf(BOLD "=====================================================\n" RESET);
        printf("Enter option: ");
        fflush(stdout);
        if (!read_int(&option)) { option = -1; continue; }

        switch (option) {
            case 1: change_username(); break;
            case 2: change_currency(); break;
            case 0: break;
            default:
                printf("\n  " RED "Invalid option!" RESET "\n");
                printf("  Press Enter to continue...\n");
                wait_for_enter();
                break;
        }
    } while (option != 0);
}

// ─── Menu Router ──────────────────────────────────────────────────────────
void menu(int op) {
    switch (op) {
        case 1: pay();                break;
        case 2: view_history();       break;
        case 3: delete_transaction(); break;
        case 4: settings_menu();      break;
        default:
            printf("  " RED "Invalid option!" RESET
                   " Press Enter to continue...\n");
            fflush(stdout);
            wait_for_enter();
            break;
    }
}

// ─── Main ─────────────────────────────────────────────────────────────────
int main(void) {
    init_settings();
    int option;

    do {
        clear();
        header();
        printf("Enter option: ");
        fflush(stdout);

        if (!read_int(&option)) {
            option = -1; // bad input → treat as invalid, not exit
        }

        if (option != 0) {
            menu(option);
        }
    } while (option != 0);

    printf(GREEN "Goodbye, %s!\n" RESET, app_settings.username);
    return 0;
}