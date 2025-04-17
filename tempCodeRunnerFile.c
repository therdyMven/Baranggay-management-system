
// Function to Search Resident
void searchResident() {
    FILE *file = fopen("residents.txt", "r");
    if (file == NULL) {
        printf("No residents found!\n");
        return;
    }

    char searchTerm[50], name[50], address[100], line[200], recordDate[20];
    }