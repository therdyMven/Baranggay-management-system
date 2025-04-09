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
void editResident();
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
        printf("5. Edit Resident\n");
        printf("6. Exit\n");
        printf("==========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addResident(); break;
            case 2: viewResidents(); break;
            case 3: searchResident(); break;
            case 4: deleteResident(); break;
            case 5: editResident(); break;
            case 6: printf("Exiting...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 6);
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

// Function to Search Resident
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

// Function to Delete Resident by Selecting from a Numbered List
void deleteResident() {
    FILE *file = fopen("residents.txt", "r");
    if (file == NULL) {
        printf("No residents found!\n");
        return;
    }

    char name[50], address[100], line[200], residents[MAX][200];
    int age, index = 0, selection, found = 0;

    while (fgets(line, sizeof(line), file)) {
        strcpy(residents[index++], line);
    }
    fclose(file);

    viewResidents();
    printf("Select Resident Number to Delete: ");
    scanf("%d", &selection);
    while (getchar() != '\n');

    FILE *tempFile = fopen("temp.txt", "w");
    for (int i = 0; i < index; i++) {
        if (i + 1 != selection) {
            fprintf(tempFile, "%s", residents[i]);
        } else {
            found = 1;
        }
    }
    fclose(tempFile);

    if (found) {
        remove("residents.txt");
        rename("temp.txt", "residents.txt");
        printf("Resident deleted successfully!\n");
    } else {
        printf("Invalid selection.\n");
    }
}

// Function to Edit Resident Details
void editResident() {
    FILE *file = fopen("residents.txt", "r");
    if (file == NULL) {
        printf("No residents found!\n");
        return;
    }

    char name[50], address[100], line[200], residents[MAX][200];
    int age, index = 0, selection;

    while (fgets(line, sizeof(line), file)) {
        strcpy(residents[index++], line);
    }
    fclose(file);

    viewResidents();
    printf("Select Resident Number to Edit: ");
    scanf("%d", &selection);
    while (getchar() != '\n');

    printf("Enter Updated Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter Updated Age: ");
    scanf("%d", &age);
    while (getchar() != '\n');

    printf("Enter Updated Address: ");
    fgets(address, sizeof(address), stdin);
    address[strcspn(address, "\n")] = 0;

    sprintf(residents[selection - 1], "%s, %d, %s\n", name, age, address);

    FILE *tempFile = fopen("residents.txt", "w");
    for (int i = 0; i < index; i++) {
        fprintf(tempFile, "%s", residents[i]);
    }
    fclose(tempFile);

    printf("Resident updated successfully!\n");
}