#ifndef OP9_IO_H
#define OP9_IO_H

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "color.h"

#define FLOAT_MIN (float) 1e-4
#define FLOAT_MAX (float) 1e4

bool conditionDeletion(char value) {
    return value != '1' && value != '2';
}

bool conditionFloat(float value) {
    return value < FLOAT_MIN || value > FLOAT_MAX;
}

bool conditionCharSortBy(char value) {
    return value != '1' && value != '2' && value != '3';
}

bool conditionCharSortOrder(char value) {
    return value != '1' && value != '2';
}

float validInputFloat(char *message, bool (*condition)(float)) {
    float value = 0;
    bool validInput = 0;
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

char validInputChoice(char *message, bool (*condition)(char)) {
    char value = 0;
    do {
        printf("%s", message);
        value = getch();
        if (condition(value)) {
            printf(RED"Invalid input\n"RESET);
        }
    } while (condition(value));
    return value;
}

unsigned validInputMultiChoice(char *message, unsigned max) {
    unsigned choice = 0;
    bool validInput = 0;
    do {
        printf("%s", message);
        validInput = scanf("%u", &choice);
        if (validInput != 1 || choice <= 0 || choice > max) {
            printf(RED"Invalid choice.\n"RESET);
        }
        fflush(stdin);
    } while (validInput != 1 || choice <= 0 || choice > max);
    return choice;
}

bool validInputString(char *arr, int size, char stringType) {
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
    switch (stringType) {
        case 'n':
            for (int i = 0; i < stringLen; i++) {
                if (!isalnum(arr[i])) {
                    printf(RED"File name must contain only latin letters and numbers.\n"RESET);
                    fflush(stdin);
                    return 0;
                }
            }
            break;
        case 'r':
            for (int i = 0; i < stringLen; i++) {
                if (!isalpha(arr[i])) {
                    printf(RED"Region name must contain only latin letters.\n"RESET);
                    fflush(stdin);
                    return 0;
                }
            }
            break;
        default:
            printf(RED"Invalid string type.\n"RESET);
            break;
    }
    fflush(stdin);
    return 1;
}

void inputString(char *message, char *arr, int size, char stringType) {
    do {
        printf("%s", message);
        fgets(arr, size + 2, stdin);
    } while (!validInputString(arr, size, stringType));
}

void printFileMenu(char *filename) {
    printf(YELLOW"This program allows you to work with files.\n"RESET
           GREEN"File currently working with: %s\n"RESET
           YELLOW"File operations:\n"RESET
           "1 - Create file\n"
           "2 - Select file\n"
           "3 - Delete file\n"
           "ESC - Exit program\n", filename);
}

void printRecordMenu(char *filename) {
    printf(YELLOW"This program allows you to work with files.\n"RESET
           GREEN"File currently working with: %s\n"RESET
           YELLOW"File operations:\n"RESET
           "1 - Create file\n"
           "2 - Select file\n"
           "3 - Delete file\n"
           YELLOW"Record operations:\n"RESET
           "4 - Create record\n"
           "5 - Read record\n"
           "6 - Edit record\n"
           "7 - Sort records\n"
           "8 - Insert record\n"
           "9 - Delete record\n"
           "ESC - Exit program\n", filename);
}

#endif //OP9_IO_H