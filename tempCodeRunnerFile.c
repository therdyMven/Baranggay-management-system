int listResidents() {
    FILE *file = fopen("residents.txt", "r");
    if (file == NULL) {
        printf("No residents found!\n");
        return 0;
    }

    char line[200], name[50], address[100], recordDate[20];
    int id, age, index = 0;

    printf("\n================================================================================================================\n");
    printf("| %-5s | %-25s | %-3s | %-40s | %-12s |\n", "ID", "NAME", "AGE", "ADDRESS", "DATE ADDED");
    printf("================================================================================================================\n");

    while (fgets(line, sizeof(line), file)) {
        // Adjusted sscanf to handle commas in the address
        if (sscanf(line, "%d, %49[^,], %d, %99[^,], %[^\n]", &id, name, &age, address, recordDate) == 5) {
            index++;
            printf("| %-5d | %-25s | %-3d | %-40s | %-12s |\n", id, name, age, address, recordDate);
        }
    }

    printf("=================================================================================================================\n");
    fclose(file);
    return index;
}

void viewResidents() {
    if (listResidents() == 0) {
        printf("No residents to display.\n");
    }
    system("pause");
}
