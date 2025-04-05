#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// Function Prototypes
void addResident();
void viewResidents();
void searchResident();
void deleteResident();
void editResident();
void menu();

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
        printf("4. Edit Resident\n");
        printf("5. Delete Resident\n");
        printf("6. Exit\n");
        printf("==========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1: addResident(); break;
            case 2: viewResidents(); break;
            case 3: searchResident(); break;
            case 4: editResident(); break;
            case 5: deleteResident(); break;
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
    name[strcspn(name, "\n")] = 0; // Remove newline

    printf("Enter Age: ");
    scanf("%d", &age);
    while (getchar() != '\n'); // Clear buffer

    printf("Enter Address: ");
    fgets(address, sizeof(address), stdin);
    address[strcspn(address, "\n")] = 0;

    fprintf(file, "%s, %d, %s\n", name, age, address);
    fclose(file);

    printf("Resident added successfully!\n");
    system("pause");
}

// Function to View Residents (with numbered list)
void viewResidents() {
    FILE *file = fopen("residents.txt", "r");
    if (file == NULL) {
        printf("No residents found!\n");
        return;
    }

    char line[200];
    int count = 1;

    printf("\nResidents List:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%d. %s ... (◦◦◦ Edit/Delete)\n", count++, line); // Showing dots as an indicator
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

    char nameSearch[50];
    int ageSearch, searchChoice;
    char name[50], address[100], line[200];
    int age;
    int found = 0;

    printf("Search by:\n");
    printf("1. Name\n");
    printf("2. Age\n");
    printf("3. Both Name and Age\n");
    printf("Enter choice: ");
    scanf("%d", &searchChoice);
    while (getchar() != '\n'); // Clear buffer

    if (searchChoice == 1 || searchChoice == 3) {
        printf("Enter Name: ");
        fgets(nameSearch, sizeof(nameSearch), stdin);
        nameSearch[strcspn(nameSearch, "\n")] = 0;
    }
    if (searchChoice == 2 || searchChoice == 3) {
        printf("Enter Age: ");
        scanf("%d", &ageSearch);
        while (getchar() != '\n'); // Clear buffer
    }

    printf("\nSearch Results:\n");
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^,], %d, %99[^\n]", name, &age, address);

        if ((searchChoice == 1 && strcmp(name, nameSearch) == 0) ||
            (searchChoice == 2 && age == ageSearch) ||
            (searchChoice == 3 && strcmp(name, nameSearch) == 0 && age == ageSearch)) {
            printf("%s, %d, %s\n", name, age, address);
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
void editResident() {
    FILE *file = fopen("residents.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Error accessing files!\n");
        return;
    }

    char nameToEdit[50], line[200], name[50], address[100];
    int age, found = 0;

    printf("Enter Name of Resident to Edit: ");
    fgets(nameToEdit, sizeof(nameToEdit), stdin);
    nameToEdit[strcspn(nameToEdit, "\n")] = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^,], %d, %99[^\n]", name, &age, address);

        if (strcmp(name, nameToEdit) == 0) {
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

// Function to Delete Resident
void deleteResident() {
    // (Same deletion logic as before)
}
