#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// Function Prototypes
void addResident();
void viewResidents();
void searchResident();
void menu();
void editResident(char *nameToEdit);

// Main Function
int main() {
    menu();
    return 0;
}

// Menu Function
void menu() {
    int choice;
    do {
        #ifdef _WIN32
            system("cls"); // Clears console for Windows
        #else
            system("clear"); // Clears console for Linux/macOS
        #endif

        printf("\n==========================================\n");
        printf("   BARANGAY SALAWAGAN MANAGEMENT SYSTEM\n");
        printf("==========================================\n");
        printf("1. Add Resident\n");
        printf("2. View Residents\n");
        printf("3. Search Resident\n");
        printf("4. Exit\n");
        printf("==========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addResident(); break;
            case 2: viewResidents(); break;
            case 3: searchResident(); break;
            case 4: printf("Exiting...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 4);
}

// Function to Add Resident (Includes immediate editing option)
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

    // Prompt user for immediate edit
    char choice;
    printf("Do you want to edit this resident now? (y/n): ");
    scanf(" %c", &choice);
    while (getchar() != '\n');

    if (choice == 'y' || choice == 'Y') {
        editResident(name);
    }
}

// Function to View Residents with headers
void viewResidents() {
    FILE *file = fopen("residents.txt", "r");
    if (file == NULL) {
        printf("No residents found!\n");
        return;
    }

    char name[50], address[100], line[200];
    int age;

    printf("\n==========================================\n");
    printf("   NAME                 AGE   ADDRESS\n");
    printf("==========================================\n");

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^,], %d, %99[^\n]", name, &age, address);
        printf("%-20s %3d   %-30s\n", name, age, address);
    }
    fclose(file);

    system("pause");
}

// Function to Search Resident (Case-insensitive filtering)
#ifdef _WIN32
    #define strcasecmp _stricmp
#endif

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

    printf("\nSearch Results:\n");
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^,], %d, %99[^\n]", name, &age, address);

        if ((isAgeSearch && age == searchAge) || (!isAgeSearch && strcasecmp(name, searchTerm) == 0)) {
            printf("Name: %s, Age: %d, Address: %s\n", name, age, address);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching records found.\n");
    }

    fclose(file);
    system("pause");
}

// Function to Edit Resident
void editResident(char *nameToEdit) {
    FILE *file = fopen("residents.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Error accessing files!\n");
        return;
    }

    char name[50], address[100], line[200];
    int age, found = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^,], %d, %99[^\n]", name, &age, address);

        if (strcasecmp(name, nameToEdit) == 0) {
            printf("Enter New Name: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;

            printf("Enter New Age: ");
            scanf("%d", &age);
            while (getchar() != '\n');

            printf("Enter New Address: ");
            fgets(address, sizeof(address), stdin);
            address[strcspn(address, "\n")] = 0;

            fprintf(temp, "%s, %d, %s\n", name, age, address);
            found = 1;
        } else {
            fprintf(temp, "%s\n", line);
        }
    }

    fclose(file);
    fclose(temp);
    remove("residents.txt");
    rename("temp.txt", "residents.txt");

    if (found) {
        printf("Resident updated successfully!\n");
    } else {
        printf("Resident not found!\n");
    }

    system("pause");
}