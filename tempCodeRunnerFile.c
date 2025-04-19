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