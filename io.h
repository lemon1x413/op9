#ifndef OP9_IO_H
#define OP9_IO_H
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "color.h"

bool conditionFloat(float value) {
    return value < 1e-3 || value > 1e3;
}

double validInputFloat(char *message, bool (*condition)(float)) {
    float value = (float) 0.0;
    int validInput = 0;
    do {
        printf("%s", message);
        validInput = scanf("%f", &value);
        if (validInput != 1 || condition(value)) {
            printf(RED"Invalid input.\n"RESET);
        }
        fflush(stdin);
    } while (validInput != 1 || condition(value));
    return value;
}

char validInputChoice(char *message, char choice1, char choice2) {
    char value = 0;
    do {
        printf("%s", message);
        value = getch();
        if (value != choice1 && value != choice2) {
            printf(RED"Invalid input\n"RESET);
        }
    } while (value != choice1 && value != choice2);
    return value;
}

bool validInputString(char *arr, int size) {
    unsigned stringLen = strlen(arr);
    if (stringLen == 0 || *arr == '\n') {
        printf(RED"Invalid input. Entered string is empty.\n"RESET);
        fflush(stdin);
        return 0;
    }
    if (arr[stringLen - 1] != '\n') {
        printf(RED"Invalid input. Max length of string is: %d\n"RESET, size);
        fflush(stdin);
        return 0;
    }
    if (arr[stringLen - 1] == '\n') {
        arr[stringLen - 1] = '\0';
        stringLen--;
    }
    for (int i = 0; i < stringLen; i++) {
        if (!isalnum(arr[i])) {
            printf(RED"File name must contain only letters and numbers.\n"RESET);
            fflush(stdin);
            return 0;
        }
    }
    fflush(stdin);
    return 1;
}

void inputString(char *message, char *arr, int size) {
    do {
        printf("%s", message);
        fgets(arr, size + 2, stdin);
    } while (!validInputString(arr, size));
}

void printFileMenu(char *filename) {
    printf(YELLOW"This program allows you to work with files.\n"RESET
            GREEN"File currently working with: %s\n"RESET
            YELLOW"File operations:\n"RESET
           "1 - Create file\n"
           "2 - Open file\n"
           "3 - Delete file\n"
           "ESC - Exit program\n", filename);
}

void printRecordMenu(char *filename) {
    printf(YELLOW"This program allows you to work with files.\n"RESET
            GREEN"File currently working with: %s\n"RESET
           YELLOW"File operations:\n"RESET
           "1 - Create file\n"
           "2 - Open file\n"
           "3 - Delete file\n"
            YELLOW"Record operations:\n"RESET
           "4 - Create record\n"
           "5 - Read record\n"
           "6 - Edit record\n"
           "7 - Sort records\n"
           "8 - Insert record\n"
           "9 - Delete record\n", filename);
}
#endif //OP9_IO_H