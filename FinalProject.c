#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h> //  Date added of the resident functions

#define MAX_RESIDENTS 300 // Maximum number of residents

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
int listResidents();
char *strcasestr_custom(const char *haystack, const char *needle);
void getNextResidentID(char *nextID);

// Custom Case-Insensitive Substring Search
char *strcasestr_custom(const char *haystack, const char *needle) {
    static char haystack_lower[100], needle_lower[100];

    strcpy(haystack_lower, haystack);
    strcpy(needle_lower, needle);

    for (int i = 0; haystack_lower[i]; i++) haystack_lower[i] = tolower(haystack_lower[i]);
    for (int i = 0; needle_lower[i]; i++) needle_lower[i] = tolower(needle_lower[i]);

    return strstr(haystack_lower, needle_lower);
}

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
        printf("\n\n");
        printf("_____________________________________________________________________________________________________________________ \n\n");
        printf("    ********    *******    ***       *******    ***         ***   *******   **********   *******   ****       ***  \n");
        printf("   **********  *********   ***      *********   ***         ***  *********  **********  *********  *****      ***  \n");
        printf("  *****   ***  ***   ***   ***      ***   ***   ***         ***  ***   ***  ***         ***   ***  ******     ***  \n");
        printf("  ****         ***   ***   ***      ***   ***   ***         ***  ***   ***  ***         ***   ***  *** ***    ***  \n");
        printf("   *****       *********   ***      *********   ***         ***  *********  ***  *****  *********  ***  ***   ***  \n");
        printf("     ******    *********   ***      *********   ***   ***   ***  *********  ***  *****  *********  ***   ***  ***  \n");
        printf("       *****   ***   ***   ***      ***   ***   ***  *****  ***  ***   ***  ***    ***  ***   ***  ***    *** ***   \n");
        printf("  ***   *****  ***   ***   ***      ***   ***   *** *** *** ***  ***   ***  ***    ***  ***   ***  ***     ******  \n");
        printf("  ***********  ***   ***   *******  ***   ***   *****     *****  ***   ***  **********  ***   ***  ***      *****  \n");
        printf("   *********   ***   ***   *******  ***   ***   ***         ***  ***   ***  **********  ***   ***  ***       ****  \n");
        printf("\n______________________________________________________________________________________________________________________\n\n");
        printf("                           BARANGAY SALAWAGAN MANAGEMENT SYSTEM                                                  \n");
        printf("______________________________________________________________________________________________________________________ \n");
        printf("1. Add Resident                                                                                                  \n");
        printf("2. View Residents                                                                                                \n");
        printf("3. Search Resident                                                                                               \n");
        printf("4. Delete Resident                                                                                               \n");
        printf("5. Edit Resident                                                                                                 \n");
        printf("6. Exit                                                                                                          \n");
        printf("_____________________________________________________________________________________________________________________ \n");
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
    FILE *file = fopen("residents.txt", "a+");
    if (file == NULL) {
        printf("Error opening residents file!\n");
        return;
    }

    char id[4], name[50], address[100], contactNo[15], recordDate[20];
    int age;
    getNextResidentID(id);

    printf("Enter Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter Age: ");
    scanf("%d", &age);
    while (getchar() != '\n');

    printf("Enter Address: ");
    fgets(address, sizeof(address), stdin);
    address[strcspn(address, "\n")] = 0;

    printf("Enter Contact No.: ");
    fgets(contactNo, sizeof(contactNo), stdin);
    contactNo[strcspn(contactNo, "\n")] = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(recordDate, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    fprintf(file, "%s, %s, %d, %s, %s, %s\n", id, name, age, address, contactNo, recordDate);
    fclose(file);

    printf("Resident added successfully!\n");
    system("pause");
}

// Function to View Residents
int listResidents() {
    FILE *file = fopen("residents.txt", "r");
    if (file == NULL) {
        printf("No residents found!\n");
        return 0;
    }

    typedef struct {
        char id[4];
        char name[50];
        int age;
        char address[100];
        char contactNo[15];
        char recordDate[20];
    } Resident;

    Resident residents[MAX_RESIDENTS];
    int count = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%3[^,], %49[^,], %d, %99[^,], %14[^,], %19[^\n]",
                   residents[count].id,
                   residents[count].name,
                   &residents[count].age,
                   residents[count].address,
                   residents[count].contactNo,
                   residents[count].recordDate) == 6) {
            count++;
        }
    }
    fclose(file);

    // Optional: Sort alphabetically by name
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(residents[i].name, residents[j].name) > 0) {
                Resident temp = residents[i];
                residents[i] = residents[j];
                residents[j] = temp;
            }
        }
    }

    printf("\n==================================================================================================================================\n");
    printf("| %-3s | %-5s | %-30s | %-3s | %-40s | %-15s | %-12s |\n", "NO", "ID", "NAME", "AGE", "ADDRESS", "CONTACT NO.", "DATE ADDED");
    printf("----------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("| %-3d | %-5s | %-30s | %-3d | %-40s | %-15s | %-12s |\n",
               i + 1,
               residents[i].id,
               residents[i].name,
               residents[i].age,
               residents[i].address,
               residents[i].contactNo,
               residents[i].recordDate);
    }

    printf("\n==================================================================================================================================\n");
    return count;
}

//Function to View Residents
void viewResidents() {
    if (listResidents() == 0) {
        printf("No residents to display.\n");
    }
    system("pause");
}

// Function to Search Resident
void searchResident() {
    FILE *file = fopen("residents.txt", "r");
    if (file == NULL) {
        printf("No residents found!\n");
        return;
    }

    char searchTerm[50], id[4], name[50], address[100], contactNo[15], line[200], recordDate[20];
    int age, searchAge, isAgeSearch = 0, found = 0, resultNo = 1;

    printf("Enter Name, Age, or Address to Search: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0; // Remove newline

    // Check if the search term is a number (for age search)
    if (sscanf(searchTerm, "%d", &searchAge) == 1) {
        isAgeSearch = 1;
    }

    printf("\n________________________________________________________________________________________________________________________________\n");
    printf("| %-2s | %-5s | %-30s | %-3s | %-40s | %-15s | %-10s |\n", "NO", "ID", "NAME", "AGE", "ADDRESS", "CONTACT NO.", "DATE ADDED");
    printf("________________________________________________________________________________________________________________________________\n");

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%3[^,], %49[^,], %d, %99[^,], %14[^,], %19[^\n]",
                   id, name, &age, address, contactNo, recordDate) == 6) {
            if ((isAgeSearch && age == searchAge) ||
                (!isAgeSearch && (strcasestr_custom(name, searchTerm) != NULL ||
                                  strcasestr_custom(address, searchTerm) != NULL))) {
                printf("| %-2d | %-5s | %-30s | %-3d | %-40s | %-15s | %-10s |\n",
                       resultNo++, id, name, age, address, contactNo, recordDate);
                found = 1;
            }
        }
    }

    if (!found) {
        printf("| %-2s | %-5s | %-30s | %-3s | %-40s | %-15s | %-10s |\n", "N/A", "-", "No matching records found", "-", "-", "-", "-");
    }
    printf("\n==================================================================================================================================\n");

    fclose(file);
    system("pause");
}

// Function to Delete Resident
void deleteResident() {
    int total = listResidents();
    if (total == 0) {
        printf("No residents to delete.\n");
        system("pause");
        return;
    }

    char idToDelete[4];
    printf("Enter the ID of the resident to delete: ");
    fgets(idToDelete, sizeof(idToDelete), stdin);
    idToDelete[strcspn(idToDelete, "\n")] = 0;

    FILE *file = fopen("residents.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening files!\n");
        return;
    }

    char line[256];
    char id[4], name[50], address[100], contactNo[15], recordDate[20];
    int age;
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%3[^,], %49[^,], %d, %99[^,], %14[^,], %19[^\n]", id, name, &age, address, contactNo, recordDate) == 6) {
            if (strcmp(id, idToDelete) != 0) {
                fprintf(tempFile, "%s, %s, %d, %s, %s, %s\n", id, name, age, address, contactNo, recordDate);
            } else {
                found = 1;
            }
        }
    }

    fclose(file);
    fclose(tempFile);

    if (remove("residents.txt") != 0 || rename("temp.txt", "residents.txt") != 0) {
        printf("Error updating the file!\n");
        return;
    }

    if (found)
        printf("Resident deleted successfully!\n");
    else
        printf("Resident with ID %s not found!\n", idToDelete);

    system("pause");
}

// Function to Edit Resident
void editResident() {
    int total = listResidents();
    if (total == 0) return;

    char idToEdit[4];
    printf("Enter the ID of the resident to edit: ");
    fgets(idToEdit, sizeof(idToEdit), stdin);
    idToEdit[strcspn(idToEdit, "\n")] = 0;

    FILE *file = fopen("residents.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening files!\n");
        return;
    }

    char line[256], id[4], name[50], address[100], contactNo[15], newName[50], newAddress[100], newContactNo[15], recordDate[20];
    int age, newAge, found = 0, choice;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%3[^,], %49[^,], %d, %99[^,], %14[^,], %19[^\n]", id, name, &age, address, contactNo, recordDate) == 6) {
            if (strcmp(id, idToEdit) == 0) {
                found = 1;
                printf("\nWhat do you want to edit?\n");
                printf("1. Name\n2. Age\n3. Address\n4. Contact No.\nEnter your choice: ");
                scanf("%d", &choice);
                while (getchar() != '\n');

                switch (choice) {
                    case 1:
                        printf("Enter New Name: ");
                        fgets(newName, sizeof(newName), stdin);
                        newName[strcspn(newName, "\n")] = 0;
                        strcpy(name, newName);
                        break;
                    case 2:
                        printf("Enter New Age: ");
                        scanf("%d", &newAge);
                        while (getchar() != '\n');
                        age = newAge;
                        break;
                    case 3:
                        printf("Enter New Address: ");
                        fgets(newAddress, sizeof(newAddress), stdin);
                        newAddress[strcspn(newAddress, "\n")] = 0;
                        strcpy(address, newAddress);
                        break;
                    case 4:
                        printf("Enter New Contact No.: ");
                        fgets(newContactNo, sizeof(newContactNo), stdin);
                        newContactNo[strcspn(newContactNo, "\n")] = 0;
                        strcpy(contactNo, newContactNo);
                        break;
                    default:
                        printf("Invalid choice! No changes made.\n");
                        break;
                }
            }
            fprintf(tempFile, "%s, %s, %d, %s, %s, %s\n", id, name, age, address, contactNo, recordDate);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (remove("residents.txt") != 0 || rename("temp.txt", "residents.txt") != 0) {
        printf("Error updating the file!\n");
        return;
    }

    if (found)
        printf("Resident updated successfully!\n");
    else
        printf("Resident with ID %s not found!\n", idToEdit);

    system("pause");
}

void getNextResidentID(char *nextID) {
    FILE *file = fopen("residents.txt", "r");
    int maxID = 0;
    char line[256], id[4], name[50], address[100], contactNo[15], recordDate[20];
    int age;
    if (file) {
        while (fgets(line, sizeof(line), file)) {
            if (sscanf(line, "%3[^,], %49[^,], %d, %99[^,], %14[^,], %19[^\n]", id, name, &age, address, contactNo, recordDate) == 6) {
                int idNum = atoi(id);
                if (idNum > maxID) maxID = idNum;
            }
        }
        fclose(file);
    }
    sprintf(nextID, "%03d", maxID + 1);
}
