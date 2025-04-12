#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

// Function Prototypes
void addResident();
void viewResidents();
void searchResident();
void deleteResident();
void menu();
char *strcasestr_custom(const char *haystack, const char *needle);

// Main Function
int main() {
    menu();
    return 0;
}

// Menu Function
void menu() {
    int choice;
    do {
        system(CLEAR_SCREEN);
        printf("\n==========================================\n");
        printf("   BARANGAY SALAWAGAN MANAGEMENT SYSTEM\n");
        printf("==========================================\n");
        printf("1. Add Resident\n");
        printf("2. View Residents\n");
        printf("3. Search Resident\n");
        printf("4. Delete Resident\n");
        printf("5. Exit\n");
        printf("==========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addResident(); break;
            case 2: viewResidents(); break;
            case 3: searchResident(); break;
            case 4: deleteResident(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 5);
}

// Function to Add Resident
void addResident() {
    FILE *file = fopen("residents.txt", "a");
    if (file == NULL) {
        printf("Error opening residents file!\n");
        return;
    }

    char name[50], address[100];
    int age;

    printf("Enter Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter Age: ");
    scanf("%d", &age);
    while (getchar() != '\n');

    printf("Enter Address: ");
    fgets(address, sizeof(address), stdin);
    address[strcspn(address, "\n")] = 0;

    fprintf(file, "%s, %d, %s\n", name, age, address);
    fclose(file);

    printf("Resident added successfully!\n");
}

// Function to View Residents
void viewResidents() {
    FILE *file = fopen("residents.txt", "r");
    if (file == NULL) {
        printf("No residents found!\n");
        return;
    }

    char name[50], address[100], line[200];
    int age;

    printf("\n==========================================\n");
    printf("   #   NAME                 AGE   ADDRESS\n");
    printf("==========================================\n");

    int index = 1;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^,], %d, %99[^\n]", name, &age, address);
        printf("%-3d %-20s %3d   %-30s\n", index++, name, age, address);
    }

    fclose(file);
    system("pause");
}

// Function to Search Resident with Case-Insensitive & Partial Matching
void searchResident() {
    FILE *file = fopen("residents.txt", "r");
    if (file == NULL) {
        printf("No residents found!\n");
        return;
    }

    char searchTerm[50], name[50], address[100], line[200];
    int age, searchAge, isAgeSearch = 0, found = 0;

    printf("Enter Name or Age to Search: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;

    if (sscanf(searchTerm, "%d", &searchAge) == 1) {
        isAgeSearch = 1;
    }

    printf("\n==========================================\n");
    printf("   #   NAME                 AGE   ADDRESS\n");
    printf("==========================================\n");

    int index = 1;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^,], %d, %99[^\n]", name, &age, address);

        if ((isAgeSearch && age == searchAge) || (!isAgeSearch && strcasestr_custom(name, searchTerm) != NULL)) {
            printf("%-3d %-20s %3d   %-30s\n", index++, name, age, address);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching records found.\n");
    }

    fclose(file);
    system("pause");
}

// Function to Delete Resident
void deleteResident() {
    FILE *file = fopen("residents.txt", "r");
    if (file == NULL) {
        printf("No residents found!\n");
        return;
    }

    char name[50], address[100], line[200], deleteName[50];
    int age, found = 0;

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }

    printf("Enter Name of Resident to Delete: ");
    fgets(deleteName, sizeof(deleteName), stdin);
    deleteName[strcspn(deleteName, "\n")] = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^,], %d, %99[^\n]", name, &age, address);
        if (strcasecmp(name, deleteName) != 0) {
            fprintf(tempFile, "%s, %d, %s\n", name, age, address);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("residents.txt");
        rename("temp.txt", "residents.txt");
        printf("Resident deleted successfully!\n");
    } else {
        remove("temp.txt");
        printf("Resident not found.\n");
    }
}

// Custom Case-Insensitive Substring Search Function
char *strcasestr_custom(const char *haystack, const char *needle) {
    static char haystack_lower[100], needle_lower[100];

    strcpy(haystack_lower, haystack);
    strcpy(needle_lower, needle);

    for (int i = 0; haystack_lower[i]; i++) haystack_lower[i] = tolower(haystack_lower[i]);
    for (int i = 0; needle_lower[i]; i++) needle_lower[i] = tolower(needle_lower[i]);

    return strstr(haystack_lower, needle_lower);
}