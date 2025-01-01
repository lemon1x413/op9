#ifndef OP9_FUNCTIONS_H
#define OP9_FUNCTIONS_H

#include <sys/stat.h>

#define SIGNATURE "QVNUUk9XT1JMRA=="
#define NAME_LENGTH 40
#define MAX_FILES 100
/*#define
#define*/
#define ESC 27


typedef struct {
    char region[NAME_LENGTH];
    float area;
    float population;
} record;



bool doesFileExist(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

bool validSignature(const char *filename) {
    FILE *file = fopen(filename, "r");
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

void createFile(char *filename) {
    FILE *file;
    do {
        inputString(YELLOW"Enter name of file you want to create (only latin letters and numbers):\n"RESET, filename, NAME_LENGTH);
        strcat(filename, ".my");
        if (doesFileExist(filename)) {
            printf(RED"File already exists. Please enter another name\n"RESET);
        }
    } while (doesFileExist(filename));
    do {
        file = fopen(filename, "w");
        if (file == NULL) {
            printf(RED"Error creating file"RESET);
        } else {
            printf(GREEN"File \"%s\" created successfully.\n"RESET, filename);
            fprintf(file, SIGNATURE"\n");
        }
        fclose(file);
    } while (file == NULL);
    printf(YELLOW"Press any key to return to the main menu\n"RESET);
    getch();
}
//done
void listOfFiles(char files[][NAME_LENGTH], unsigned *fileCount) {
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
                            strncpy(files[*fileCount], res->d_name, NAME_LENGTH);
                            (*fileCount)++;
                        }
                    }
                }
                closedir(folder);
            } else {
                printf("Could not open the directory");
            }
        }
    } else {
        printf("The \"%s\" it cannot be opened or is not a directory\n", path);
        return;
    }
}

//done
void openFile(char *filename) {
    FILE *file;
    char files[MAX_FILES][NAME_LENGTH];
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
    int choice = 0;
    int validInput = 0;
    do {
        printf(YELLOW"Enter the number of the file you want to select:\n"RESET);
        validInput = scanf("%d", &choice);
        if (validInput != 1 || choice <= 0 || choice > fileCount) {
            printf(RED"Invalid choice.\n"RESET);
        } else {
            strncpy(filename, files[choice - 1], NAME_LENGTH);
            printf(GREEN"You selected file: %s"RESET, filename);
            printf("\n");
        }
        fflush(stdin);
    } while (validInput != 1 || choice <= 0 || choice > fileCount);
    printf(YELLOW"Press any key to return to the main menu.\n"RESET);
    getch();
}

void deleteFile(char *fileName) {
    printf(YELLOW"Are you sure you want to delete the file \"%s\"?\n"RESET
                RED"This action cannot be undone.\n"RESET, fileName);
    char fileDeletion = validInputChoice(GREEN"Press 1 to delete the file or 2 to cancel:\n"RESET, '1', '2');
    switch (fileDeletion) {
        case '1':
            if (remove(fileName) == 0) {
                printf(GREEN"File \"%s\" deleted successfully.\n"RESET, fileName);
                fileName[0] = '\0';
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
    printf(YELLOW"Press any key to return to the main menu.\n"RESET);
    getch();
}

void readFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf(RED"Error opening file \"%s\" for reading.\n"RESET, fileName);
        return;
    }
    fseek(file, 0, SEEK_END);
    unsigned fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(fileSize + 1);
    if (buffer == NULL) {
        printf(RED"Error allocating memory.\n"RESET);
        fclose(file);
        return;
    }
    unsigned bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead == 0) {
        printf(RED"Error reading file.\n"RESET);
        free(buffer);
        fclose(file);
        return;
    }
    buffer[bytesRead] = '\0';
    char *start = buffer;
    char *lineEnd = strchr(start, '\n');
    if (lineEnd != NULL) {
        start = lineEnd + 1;
    }
    printf(GREEN"File content:\n"RESET);
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
    printf(GREEN"\nFile reading complete.\n"RESET);
    printf(YELLOW"Press any key to return to the main menu.\n"RESET);
    getch();
}

#endif //OP9_FUNCTIONS_H
