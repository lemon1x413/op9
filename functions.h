#ifndef OP9_FUNCTIONS_H
#define OP9_FUNCTIONS_H

#include <sys/stat.h>
#include <dirent.h>
#include "color.h"
#include "sort.h"

#define SIGNATURE "QVNUUk9XT1JMRA=="
#define REGION_NAME_LENGTH 100
#define NAME_LENGTH 50
#define MAX_FILES 100
#define MAX_RECORDS 100
#define RECORD_LENGTH 1000
#define ESC 27

typedef struct {
    char region[REGION_NAME_LENGTH];
    float area;
    float population;
} record;

bool doesFileExist(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

bool validSignature(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        return 0;
    }
    char firstLine[17];
    if (fgets(firstLine, sizeof(firstLine), file) != NULL) {
        if (strncmp(firstLine, SIGNATURE, strlen(SIGNATURE)) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void backToMenu() {
    printf(YELLOW"Press any key to return to the main menu\n"RESET);
    getch();
}

void createFile(char *fileName) {
    FILE *file;
    do {
        inputString(YELLOW"Enter name of file you want to create (only latin letters and numbers):\n"RESET, fileName,
                    NAME_LENGTH, 'n');
        strcat(fileName, ".my");
        if (doesFileExist(fileName)) {
            printf(RED"File already exists. Please enter another name\n"RESET);
        }
    } while (doesFileExist(fileName));
    do {
        file = fopen(fileName, "w");
        if (file == NULL) {
            printf(RED"Error creating file"RESET);
        } else {
            printf(GREEN"File \"%s\" created successfully.\n"RESET, fileName);
            fprintf(file, SIGNATURE"\n");
        }
        fclose(file);
    } while (file == NULL);
}

//done
void listOfFiles(char files[][NAME_LENGTH+4], unsigned *fileCount) {
    struct dirent *res;
    struct stat sb;
    const char *path = ".";
    if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        DIR *folder = opendir(path);
        *fileCount = 0;
        if (access(path, F_OK) != -1) {
            if (folder) {
                while ((res = readdir(folder)) != NULL) {
                    if (res->d_name[0] != '.' && strstr(res->d_name, ".my") != NULL) {
                        if (validSignature(res->d_name)) {
                            strncpy(files[*fileCount], res->d_name, NAME_LENGTH+4);
                            (*fileCount)++;
                        }
                    }
                }
                closedir(folder);
            } else {
                printf(RED"Could not open the directory"RESET);
            }
        }
    } else {
        printf(RED"The \"%s\" it cannot be opened or is not a directory\n"RESET, path);
        return;
    }
}
//done
void selectFile(char *fileName) {
    char files[MAX_FILES][NAME_LENGTH+4];
    unsigned fileCount = 0;
    listOfFiles(files, &fileCount);
    if (fileCount == 0) {
        printf(RED"No supported files found.\n"RESET);
        return;
    }
    printf(GREEN"Available files:\n"RESET);
    for (int i = 0; i < fileCount; i++) {
        printf("%d) %s\n", i + 1, files[i]);
    }
    unsigned choice = validInputMultiChoice(YELLOW"Enter the number of the file you want to select:\n"RESET,fileCount);
    strncpy(fileName, files[choice - 1], NAME_LENGTH + 4);
    printf(GREEN"You selected file: %s\n"RESET, fileName);
}

void deleteFile(char *fileName) {
    if (!doesFileExist(fileName)) {
        printf(RED"None of the files was selected or selected file does not exist.\n"RESET);
        return;
    } else {
        printf(YELLOW"Are you sure you want to delete the file \"%s\"?\n"RESET
               RED"This action cannot be undone.\n"RESET, fileName);
        char fileDeletion = validInputChoice(GREEN"Press 1 to delete the file or 2 to cancel:\n"RESET, conditionDeletion);
        switch (fileDeletion) {
            case '1':
                if (remove(fileName) == 0) {
                    printf(GREEN"File \"%s\" deleted successfully.\n"RESET, fileName);
                    *fileName = '\0';
                } else {
                    printf(RED"Error deleting file \"%s\".\n"RESET, fileName);
                }
                break;
            case '2':
                return;
            default:
                printf(RED"Invalid choice.\n"RESET);
                break;
        }
    }
}

unsigned countRecords(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf(RED"Error opening file \"%s\" for reading.\n"RESET, fileName);
        return 0;
    }
    unsigned recordCount = 0;
    char line[RECORD_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Record") != NULL) {
            recordCount++;
        }
    }
    fclose(file);
    return recordCount;
}

void createRecord(char *fileName) {
    FILE *file = fopen(fileName, "a");
    if (file == NULL) {
        printf(RED"Error opening file \"%s\"\n"RESET, fileName);
        return;
    }
    unsigned existingRecordCount = countRecords(fileName);
    if (existingRecordCount >= MAX_RECORDS) {
        printf(RED"The file already contains the maximum number of records (%d)\n"RESET
                    RED"Try deleting unnecessary records first.\n"RESET, MAX_RECORDS);
        fclose(file);
        return;
    }
    unsigned currentRecordIndex = existingRecordCount + 1;
    record records = {"", 0, 0};
    printf(YELLOW"Writing record #%u to file \"%s\"\n"RESET,currentRecordIndex, fileName );
    inputString(GREEN"Enter region name (up to 50 latin letters):\n"RESET, records.region, REGION_NAME_LENGTH, 'r');
    records.area = validInputFloat(GREEN"Enter area of the region in square kilometers (from 0.0001 to 10000)\n"RESET, conditionFloat);
    records.population = validInputFloat(GREEN"Enter number of the population in millions (from 0.0001 to 10000)\n"RESET, conditionFloat);
    fprintf(file, "Record #%u:\n"
                                "Region: %s\n"
                                "Area: %f\n"
                                "Population: %f\n",
                                currentRecordIndex,
                                records.region,
                                records.area,
                                records.population);
    fclose(file);
    printf(GREEN"Record #%d have been successfully written in file \"%s\"\n"RESET, currentRecordIndex, fileName);
}

void readFile(char *fileName, char *message) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf(RED"Error opening file \"%s\"\n"RESET, fileName);
        return;
    }
    fseek(file, 0, SEEK_END);
    unsigned fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(fileSize + 1);
    if (buffer == NULL) {
        printf(RED"Error allocating memory\n"RESET);
        fclose(file);
        return;
    }
    unsigned charsRead = fread(buffer, 1, fileSize, file);
    if (charsRead == 0) {
        printf(RED"Error reading file\n"RESET);
        free(buffer);
        fclose(file);
        return;
    }
    buffer[charsRead] = '\0';
    char *start = buffer;
    char *lineEnd = strchr(start, '\n');
    if (lineEnd != NULL) {
        start = lineEnd + 1;
    }
    printf(GREEN"%s"RESET, message);
    do {
        lineEnd = strchr(start, '\n');
        if (lineEnd != NULL) {
            *lineEnd = '\0';
        }
        printf("%s\n", start);
        if (lineEnd != NULL) {
            start = lineEnd + 1;
        } else {
            break;
        }
    } while (*start != '\0');
    free(buffer);
    fclose(file);
}

void editRecord(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Failed to open file \"%s\"\n", fileName);
        return;
    }

    char line[RECORD_LENGTH];
    record records[MAX_RECORDS];
    int recordCount = 0;
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("File \"%s\" is empty or cannot be read\n", fileName);
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Record #")) {
            record currentRecord;
            fgets(line, sizeof(line), file);
            sscanf(line, "Region: %s", currentRecord.region);
            fgets(line, sizeof(line), file);
            sscanf(line, "Area: %f", &currentRecord.area);
            fgets(line, sizeof(line), file);
            sscanf(line, "Population: %f", &currentRecord.population);
            records[recordCount++] = currentRecord;
        }
    }
    fclose(file);
    if (recordCount == 0) {
        printf("No records found to edit\n");
        return;
    }

    readFile(fileName, "Available records:\n");
    unsigned recordNumber = validInputMultiChoice(YELLOW"Choose the record you want to edit:\n"RESET,recordCount);
    record *selectedRecord = &records[recordNumber-1];

    inputString(GREEN"Enter region name (up to 50 latin letters):\n"RESET, selectedRecord->region, REGION_NAME_LENGTH, 'r');
    selectedRecord->area = validInputFloat(GREEN"Enter area of the region in square kilometers (from 0.0001 to 10000)\n"RESET, conditionFloat);
    selectedRecord->population = validInputFloat(GREEN"Enter number of the population in millions (from 0.0001 to 10000)\n"RESET, conditionFloat);
    fprintf(file, "Record #%d:\n"
                                "Region: %s\n"
                                "Area: %f\n"
                                "Population: %f\n",
                                recordNumber,
                                selectedRecord->region,
                                selectedRecord->area,
                                selectedRecord->population);
    printf(GREEN"The record has been successfully changed\n"RESET);

    file = fopen(fileName, "w");
    if (file == NULL) {
        printf(RED"Failed to open file \"%s\" for writing\n"RESET, fileName);
        return;
    }
    fprintf(file, "%s\n", SIGNATURE);
    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "Record #%d:\n"
                                    "Region: %s\n"
                                    "Area: %f\n"
                                    "Population: %f\n",
                                    i + 1,
                                    records[i].region,
                                    records[i].area,
                                    records[i].population);
    }
    fclose(file);
    readFile(fileName, "Updated records:\n");
}

void sortRecords(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Failed to open file \"%s\"\n", fileName);
        return;
    }

    char line[RECORD_LENGTH];
    record records[MAX_RECORDS];
    int recordCount = 0;
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("File \"%s\" is empty or cannot be read\n", fileName);
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Record #")) {
            record currentRecord;
            fgets(line, sizeof(line), file);
            sscanf(line, "Region: %s", currentRecord.region);
            fgets(line, sizeof(line), file);
            sscanf(line, "Area: %f", &currentRecord.area);
            fgets(line, sizeof(line), file);
            sscanf(line, "Population: %f", &currentRecord.population);
            records[recordCount++] = currentRecord;
        }
    }
    fclose(file);
    if (recordCount == 0) {
        printf("No records found to sort\n");
        return;
    }
    char sortBy = validInputChoice(YELLOW"Choose the way to sort by:\n"RESET
                                "1 - Area\n"
                                "2 - Population\n"
                                "3 - Region name\n", conditionCharSortBy);
    char sortOrder = validInputChoice(YELLOW"Choose the order of sorting:\n"RESET
                                "1 - Ascending\n"
                                "2 - Descending\n", conditionCharSortOrder);

}

#endif //OP9_FUNCTIONS_H
