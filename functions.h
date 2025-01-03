#ifndef OP9_FUNCTIONS_H
#define OP9_FUNCTIONS_H

#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include "colors.h"

#define CREATE_FILE '1'
#define SELECT_FILE '2'
#define DELETE_FILE '3'
#define CREATE_RECORD '4'
#define READ_RECORDS '5'
#define EDIT_RECORD '6'
#define SORT_RECORDS '7'
#define INSERT_RECORD '8'
#define DELETE_RECORD '9'
#define ESC 27

#define SIGNATURE "Uk9ERU9CSVRUU01BU1RST1dPUkxEVVRPUElB"
#define REGION_NAME_LENGTH 100
#define NAME_LENGTH 100
#define MAX_FILES 100
#define MAX_RECORDS 100
#define RECORD_LENGTH 16537

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
    char firstLine[strlen(SIGNATURE) + 1];
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
        inputString(YELLOW"Enter name of file you want to create using latin letters or numbers\n"RESET
                    YELLOW"Max length of file name is 100\n"RESET, fileName,
                    NAME_LENGTH, 'n');
        strcat(fileName, ".my");
        if (doesFileExist(fileName)) {
            printf(RED"File already exists. Please enter another name\n"RESET);
        }
    } while (doesFileExist(fileName));
    file = fopen(fileName, "w");
    if (file == NULL) {
        printf(RED"Error creating file"RESET);
        return;
    }
    printf(GREEN"File \"%s\" created successfully\n"RESET, fileName);
    fprintf(file, SIGNATURE"\n");
    fclose(file);
}

void listOfFiles(char filesList[][NAME_LENGTH + 4], unsigned *fileCount) {
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
                            strncpy(filesList[*fileCount], res->d_name, NAME_LENGTH + 4);
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

void selectFile(char *fileName) {
    char filesList[MAX_FILES][NAME_LENGTH + 4];
    unsigned fileCount = 0;
    listOfFiles(filesList, &fileCount);
    if (fileCount == 0) {
        printf(RED"No supported files found\n"RESET);
        return;
    }
    printf(GREEN"Available files:\n"RESET);
    for (int i = 0; i < fileCount; i++) {
        printf("%d) %s\n", i + 1, filesList[i]);
    }
    unsigned choice = validInputMultiChoice(YELLOW"Enter the number of the file you want to select:\n"RESET,fileCount);
    strncpy(fileName, filesList[choice - 1], NAME_LENGTH + 4);
    printf(GREEN"You selected file: %s\n"RESET, fileName);
}

void deleteFile(char *fileName) {
    if (!doesFileExist(fileName)) {
        printf (RED"None of the files was selected or selected file does not exist\n"RESET);
        return;
    }
    printf(YELLOW"Are you sure you want to delete the file \"%s\"?\n"RESET
                    RED"This action cannot be undone\n"RESET, fileName);
    char fileDeletion = validInputChoice(GREEN"Press 1 to delete the file or 2 to cancel:\n"RESET, conditionDeletion);
    switch (fileDeletion) {
        case '1':
            if (remove(fileName) == 0) {
                printf(GREEN"File \"%s\" deleted successfully\n"RESET, fileName);
                *fileName = '\0';
            } else {
                printf(RED"Error deleting file \"%s\"\n"RESET, fileName);
            }
            break;
        case '2':
            printf(GREEN"Deleting file \"%s\" cancelled\n"RESET, fileName);
            break;
        default:
            printf(RED"Invalid input\n"RESET);
            break;
    }
}

unsigned countRecords(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf(RED"Error opening file \"%s\"\n"RESET, fileName);
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
                    RED"Try deleting unnecessary records first\n"RESET, MAX_RECORDS);
        fclose(file);
        return;
    }
    unsigned currentRecordIndex = existingRecordCount + 1;
    record records = {"", 0, 0};
    printf(YELLOW"Writing record #%u to file \"%s\"\n"RESET, currentRecordIndex, fileName );
    inputString(GREEN"Enter region name (up to 100 latin letters):\n"RESET, records.region, REGION_NAME_LENGTH, 'r');
    records.area = validInputFloat(GREEN"Enter area of the region in square kilometers (from 0.0001 to 10000)\n"RESET, conditionFloat);
    records.population = validInputFloat(GREEN"Enter number of the population in millions (from 0.0001 to 10000)\n"RESET, conditionFloat);
    fprintf(file, "Record #%u:\n"
                                " Region: %s\n"
                                " Area: %.4f\n"
                                " Population: %.4f\n",
                                currentRecordIndex,
                                records.region,
                                records.area,
                                records.population);
    fclose(file);
    printf(GREEN"Record #%d have been successfully written in file \"%s\"\n"RESET, currentRecordIndex, fileName);
}

void readRecords(char *fileName, char *message) {
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

void fillFile(char *fileName, record *records, unsigned recordCount) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf(RED"Failed to open file \"%s\" for writing\n"RESET, fileName);
        return;
    }
    fprintf(file, "%s\n", SIGNATURE);
    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "Record #%d:\n"
                      " Region: %s\n"
                      " Area: %.4f\n"
                      " Population: %.4f\n",
                i + 1,
                records[i].region,
                records[i].area,
                records[i].population);
    }
    fclose(file);
}

void editRecord(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf(RED"Failed to open file \"%s\"\n"RESET, fileName);
        return;
    }

    char line[RECORD_LENGTH];
    record records[MAX_RECORDS];
    int recordCount = 0;
    if (fgets(line, sizeof(line), file) == NULL) {
        printf(RED"File \"%s\" is empty or cannot be read\n"RESET, fileName);
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Record #")) {
            record currentRecord;
            fgets(line, sizeof(line), file);
            sscanf(line, " Region: %s", currentRecord.region);
            fgets(line, sizeof(line), file);
            sscanf(line, " Area: %f", &currentRecord.area);
            fgets(line, sizeof(line), file);
            sscanf(line, " Population: %f", &currentRecord.population);
            records[recordCount++] = currentRecord;
        }
    }
    fclose(file);
    if (recordCount == 0) {
        printf(RED"No records found to edit\n"RESET);
        return;
    }

    readRecords(fileName, "Available records:\n");
    unsigned recordNumber = validInputMultiChoice(YELLOW"Choose the record you want to edit:\n"RESET,recordCount);
    record *selectedRecord = &records[recordNumber-1];

    inputString(GREEN"Enter region name (up to 100 latin letters):\n"RESET, selectedRecord->region, REGION_NAME_LENGTH, 'r');
    selectedRecord->area = validInputFloat(GREEN"Enter area of the region in square kilometers (from 0.0001 to 10000)\n"RESET, conditionFloat);
    selectedRecord->population = validInputFloat(GREEN"Enter number of the population in millions (from 0.0001 to 10000)\n"RESET, conditionFloat);
    printf(GREEN"The record has been successfully changed\n"RESET);

    fillFile(fileName, records, recordCount);
    readRecords(fileName, "Updated records:\n");
}

bool ascendingSortRegion(record *records1, record *records2) {
    return (strcmp (records1->region, records2->region) > 0);
}

bool descendingSortRegion(record *record1, record *record2) {
    return (strcmp (record1->region, record2->region) < 0);
}

bool ascendingSortArea(record *record1, record *record2) {
    return (record1->area < record2->area);
}

bool descendingSortArea(record *record1, record *record2) {
    return (record1->area > record2->area);
}

bool ascendingSortPopulation(record *record1, record *record2) {
    return (record1->population < record2->population);
}

bool descendingSortPopulation(record *record1, record *record2) {
    return (record1->population > record2->population);
}

void bubbleSort(record *records, int n, bool (*cmp)(record *, record *)) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (cmp(&records[i], &records[j])) {
                record tmp = records[i];
                records[i] = records[j];
                records[j] = tmp;
            }
        }
    }
}

void sortRecords(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf(RED"Failed to open file \"%s\"\n"RESET, fileName);
        return;
    }

    char line[RECORD_LENGTH];
    record records[MAX_RECORDS];
    int recordCount = 0;
    if (fgets(line, sizeof(line), file) == NULL) {
        printf(RED"File \"%s\" is empty or cannot be read\n"RESET, fileName);
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Record #")) {
            record currentRecord;
            fgets(line, sizeof(line), file);
            sscanf(line, " Region: %s", currentRecord.region);
            fgets(line, sizeof(line), file);
            sscanf(line, " Area: %f", &currentRecord.area);
            fgets(line, sizeof(line), file);
            sscanf(line, " Population: %f", &currentRecord.population);
            records[recordCount++] = currentRecord;
        }
    }
    fclose(file);
    if (recordCount == 0) {
        printf(RED"No records found to sort\n"RESET);
        return;
    }
    bool (*sortChoice)(record *, record *) = 0;
    char sortBy = validInputChoice(YELLOW"Choose the way to sort by:\n"RESET
                                "1 - Region name\n"
                                "2 - Area\n"
                                "3 - Population\n", conditionCharSortBy);
    char sortOrder = validInputChoice(YELLOW"Choose the order of sorting:\n"RESET
                                "1 - Ascending\n"
                                "2 - Descending\n", conditionCharSortOrder);
    switch (sortOrder) {
        case '1':
            switch (sortBy) {
                case '1':
                    sortChoice = ascendingSortRegion;
                    break;
                case '2':
                    sortChoice = ascendingSortArea;
                    break;
                case '3':
                    sortChoice = ascendingSortPopulation;
                    break;
                default:
                    printf(RED"Invalid choice\n"RESET);
                    break;
            }
            break;
        case '2':
            switch (sortBy) {
                case '1':
                    sortChoice = descendingSortRegion;
                    break;
                case '2':
                    sortChoice = descendingSortArea;
                    break;
                case '3':
                    sortChoice = descendingSortPopulation;
                    break;
                default:
                    printf(RED"Invalid choice\n"RESET);
                    break;
            }
            break;
        default:
            printf(RED"Invalid choice\n"RESET);
            return;
    }
    readRecords(fileName, "Before sorting:\n");
    bubbleSort(records, recordCount, sortChoice);
    fillFile(fileName, records, recordCount);
    readRecords(fileName, "After sorting:\n");
}

void insertRecord(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf(RED"Failed to open file \"%s\"\n"RESET, fileName);
        return;
    }

    char line[RECORD_LENGTH];
    record records[MAX_RECORDS];
    int recordCount = 0;
    if (fgets(line, sizeof(line), file) == NULL) {
        printf(RED"File \"%s\" is empty or cannot be read\n"RESET, fileName);
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Record #")) {
            record currentRecord;
            fgets(line, sizeof(line), file);
            sscanf(line, " Region: %s", currentRecord.region);
            fgets(line, sizeof(line), file);
            sscanf(line, " Area: %f", &currentRecord.area);
            fgets(line, sizeof(line), file);
            sscanf(line, " Population: %f", &currentRecord.population);
            records[recordCount++] = currentRecord;
        }
    }
    fclose(file);
    if (recordCount == 0) {
        printf(RED"No records found to edit\n"RESET);
        return;
    }

    readRecords(fileName, "Current records:\n");

    unsigned insertionPosition = validInputMultiChoice(YELLOW"Choose the position where you want to insert the new record:\n"RESET,recordCount);
    record newRecord;


    inputString(GREEN"Enter region name (up to 50 latin letters):\n"RESET, newRecord.region, REGION_NAME_LENGTH, 'r');
    newRecord.area = validInputFloat(GREEN"Enter area of the region in square kilometers (from 0.0001 to 10000)\n"RESET, conditionFloat);
    newRecord.population = validInputFloat(GREEN"Enter number of the population in millions (from 0.0001 to 10000)\n"RESET, conditionFloat);

    for (int i = recordCount; i >= insertionPosition; i--) {
        records[i] = records[i - 1];
    }
    records[insertionPosition - 1] = newRecord;
    recordCount++;

    fillFile(fileName, records, recordCount);
    readRecords(fileName, "Updated records:\n");
}

void deleteRecord(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf(RED"Failed to open file \"%s\"\n"RESET, fileName);
        return;
    }
    char line[RECORD_LENGTH];
    record records[MAX_RECORDS];
    unsigned recordCount = 0, newRecordCount = 0;
    if (fgets(line, sizeof(line), file) == NULL) {
        printf(RED"File \"%s\" is empty or cannot be read\n"RESET, fileName);
        fclose(file);
        return;
    }
    unsigned existingRecordCount = countRecords(fileName);
    if (existingRecordCount == 0) {
        printf(RED"No records found to edit\n"RESET);
        return;
    }

    readRecords(fileName, "Available records:\n");
    unsigned recordNumber = validInputMultiChoice(YELLOW"Choose the record you want to delete:\n"RESET, existingRecordCount);

    printf(YELLOW"Are you sure you want to delete record #%u from the file \"%s\"?\n"RESET
           RED"This action cannot be undone\n"RESET, recordNumber, fileName);
    char recordDeletion = validInputChoice(GREEN"Press 1 to delete record or 2 to cancel:\n"RESET, conditionDeletion);
    switch (recordDeletion) {
        case '1':
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "Record #")) {
                    if (recordNumber != recordCount + 1) {
                        record currentRecord;
                        fgets(line, sizeof(line), file);
                        sscanf(line, " Region: %s", currentRecord.region);
                        fgets(line, sizeof(line), file);
                        sscanf(line, " Area: %f", &currentRecord.area);
                        fgets(line, sizeof(line), file);
                        sscanf(line, " Population: %f", &currentRecord.population);
                        records[newRecordCount++] = currentRecord;
                        recordCount++;
                    } else {
                        recordCount++;
                    }
                }
            }
            fclose(file);
            if (recordCount == 0) {
                printf(RED"No records found to edit\n"RESET);
                return;
            }
            fillFile(fileName, records, newRecordCount);
            readRecords(fileName, "Updated records:\n");
            break;
        case'2':
            printf(RED"Record deletion canceled\n"RESET);
            fclose(file);
            break;
        default:
            printf(RED"Invalid input\n"RESET);
            break;
    }
}

#endif