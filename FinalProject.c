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

    int no = 1; // Default NO for the first resident
    char line[200];

    // Find the last valid NO in the file
    while (fgets(line, sizeof(line), file)) {
        int tempNo;
        if (sscanf(line, "%d,", &tempNo) == 1) {
            if (tempNo > 0) { // Ensure the NO is valid
                no = tempNo + 1; // Increment the NO for the next resident
            }
        }
    }

    char name[50], address[100], contactNo[15], recordDate[20];
    int age;

    printf("Enter Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // Remove newline

    printf("Enter Age: ");
    scanf("%d", &age);
    while (getchar() != '\n'); // Clear input buffer

    printf("Enter Address: ");
    fgets(address, sizeof(address), stdin);
    address[strcspn(address, "\n")] = 0; // Remove newline

    printf("Enter Contact No.: ");
    fgets(contactNo, sizeof(contactNo), stdin);
    contactNo[strcspn(contactNo, "\n")] = 0; // Remove newline

    // Get the current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(recordDate, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    fprintf(file, "%d, %s, %d, %s, %s, %s\n", no, name, age, address, contactNo, recordDate);
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
        char name[50];
        int age;
        char address[100];
        char contactNo[15];
        char recordDate[20];
    } Resident;

    Resident residents[MAX_RESIDENTS];
    int count = 0;


    char line[200];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%*d, %49[^,], %d, %99[^,], %14[^,], %[^\n]", 
                   residents[count].name, 
                   &residents[count].age, 
                   residents[count].address, 
                   residents[count].contactNo, 
                   residents[count].recordDate) == 5) {
            count++;
        }
    }
    fclose(file);

    // Sort residents alphabetically by name
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(residents[i].name, residents[j].name) > 0) {
                Resident temp = residents[i];
                residents[i] = residents[j];
                residents[j] = temp;
            }
        }
    }

    // Display the residents
    printf("\n_____________________________________________________________________________________________________________________\n");
    printf("|%-2s| %-30s   | %-0s | %-40s  |%-15s |%-7s |\n", "NO", "NAME", "AGE", "ADDRESS", "CONTACT NO.", "DATE ADDED");
    printf("_____________________________________________________________________________________________________________________\n");

    for (int i = 0; i < count; i++) {
        printf("|%-2d| %-30s   | %-0d  | %-40s  |%-15s |%-7s |\n", 
               i + 1, 
               residents[i].name, 
               residents[i].age, 
               residents[i].address, 
               residents[i].contactNo, 
               residents[i].recordDate);
    }

    printf("_____________________________________________________________________________________________________________________\n");
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


    char searchTerm[50], name[50], address[100], contactNo[15], line[200], recordDate[20];
    int no, age, searchAge, isAgeSearch = 0, found = 0, resultNo = 1;

    printf("Enter Name, Age, Address, or Contact No. to Search: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0; // Remove newline

    if (sscanf(searchTerm, "%d", &searchAge) == 1) {
        isAgeSearch = 1;
    }

    printf("\n_____________________________________________________________________________________________________________________\n");
    printf("| %-2s | %-30s |%-0s | %-40s | %-15s | %-7s |\n", "NO", "NAME", "AGE", "ADDRESS", "CONTACT NO.", "DATE ADDED");
    printf("_____________________________________________________________________________________________________________________\n");
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d, %49[^,], %d, %99[^,], %14[^,], %[^\n]", &no, name, &age, address, contactNo, recordDate) == 6) {
            if ((isAgeSearch && age == searchAge) || 
                (!isAgeSearch && (strcasestr_custom(name, searchTerm) != NULL || 
                                  strcasestr_custom(address, searchTerm) != NULL || 
                                  strcasestr_custom(contactNo, searchTerm) != NULL))) {
                
                        printf("| %-2d | %-30s | %-0d | %-40s | %-15s | %-7s |\n", 
                       resultNo++, name, age, address, contactNo, recordDate);
                found = 1;
            }
        }
    }
        
    if (!found) {
        printf("| %-2s | %-30s | %-3s | %-40s | %-15s | %-7s |\n", "N/A", "No matching records found", "-", "-", "-", "-");
    }

    printf("======================================================================================================================\n");

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

    int noToDelete;
    printf("Enter the NO. of the resident to delete: ");
    scanf("%d", &noToDelete);
    while (getchar() != '\n'); // Clear input buffer

    if (noToDelete < 1 || noToDelete > total) {
        printf("Invalid NO. entered! Please try again.\n");
        system("pause");
        return;
    }

    FILE *file = fopen("residents.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening files!\n");
        return;
    }

    char line[200];
    int currentNo = 1, found = 0;

    // Write all residents except the one with the matching NO.
    while (fgets(line, sizeof(line), file)) {
        if (currentNo == noToDelete) {
            found = 1; // Skip the resident to be deleted
        } else {
            fputs(line, tempFile); // Write the resident to the temp file
        }
        currentNo++;
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the updated file
    if (remove("residents.txt") != 0 || rename("temp.txt", "residents.txt") != 0) {
        printf("Error updating the file!\n");
        return;
    }

    if (found) {
        printf("Resident deleted successfully!\n");
    } else {
        printf("Resident with NO. %d not found!\n", noToDelete);
    }

    system("pause");
}

// Function to Edit Resident
void editResident() {
    int total = listResidents();
    if (total == 0) return;

    int noToEdit;
    printf("Enter the NO. of the resident to edit: ");
    scanf("%d", &noToEdit);
    while (getchar() != '\n'); // Clear input buffer

    FILE *file = fopen("residents.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening files!\n");
        return;
    }

    char line[200], name[50], address[100], contactNo[15], newName[50], newAddress[100], newContactNo[15], recordDate[20];
    int no, age, newAge, found = 0, choice;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d, %49[^,], %d, %99[^,], %14[^,], %19[^\n]", &no, name, &age, address, contactNo, recordDate) == 6) {
            if (no == noToEdit) {
                found = 1;

                printf("\nWhat do you want to edit?\n");
                printf("1. Name\n");
                printf("2. Age\n");
                printf("3. Address\n");
                printf("4. Contact No.\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                while (getchar() != '\n'); // Clear input buffer

                switch (choice) {
                    case 1:
                        printf("Enter New Name: ");
                        fgets(newName, sizeof(newName), stdin);
                        newName[strcspn(newName, "\n")] = 0; // Remove newline
                        strcpy(name, newName); // Update name
                        break;
                    case 2:
                        printf("Enter New Age: ");
                        scanf("%d", &newAge);
                        while (getchar() != '\n'); // Clear input buffer
                        age = newAge; // Update age
                        break;
                    case 3:
                        printf("Enter New Address: ");
                        fgets(newAddress, sizeof(newAddress), stdin);
                        newAddress[strcspn(newAddress, "\n")] = 0; // Remove newline
                        strcpy(address, newAddress); // Update address
                        break;
                    case 4:
                        printf("Enter New Contact No.: ");
                        fgets(newContactNo, sizeof(newContactNo), stdin);
                        newContactNo[strcspn(newContactNo, "\n")] = 0; // Remove newline
                        strcpy(contactNo, newContactNo); // Update contact number
                        break;
                    default:
                        printf("Invalid choice! No changes made.\n");
                        break;
                }

                fprintf(tempFile, "%d, %s, %d, %s, %s, %s\n", no, name, age, address, contactNo, recordDate);
            } else {
                fprintf(tempFile, "%s", line);
            }
        }
    }

    fclose(file);
    fclose(tempFile);

    if (remove("residents.txt") != 0 || rename("temp.txt", "residents.txt") != 0) {
        printf("Error updating the file!\n");
        return;
    }

    if (found) {
        printf("Resident updated successfully!\n");
    } else {
        printf("Resident not found!\n");
    }

    system("pause");
}
