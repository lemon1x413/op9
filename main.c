#include <stdio.h>
#include <conio.h>
#include <dirent.h>
#include "io.h"
#include "functions.h"
#include "color.h"

FILE *file;

int main(void) {
    char choiceMenu = 0;
    char fileName[NAME_LENGTH] = {};
    do {
        system("cls");
        fileName[0] == '\0'
                    ? printFileMenu(fileName)
                    : printRecordMenu(fileName);
        choiceMenu = getch();
        //make menu (1 variant make big switch and two small switches inside; 2 variant)
        if (fileName[0] == '\0') {
            switch (choiceMenu) {
                case '1':
                    system("cls");
                    createFile(fileName);
                    break;
                case '2':
                    system("cls");
                    openFile(fileName);
                    break;
                case '3':
                    system("cls");
                    deleteFile(fileName);
                    break;
                default:
                    printf(RED"Invalid choice\n"RESET);
                    break;
            }
        } else {
            switch (choiceMenu) {
                case '1':
                    system("cls");
                    createFile(fileName);
                    break;
                case '2':
                    system("cls");
                    openFile(fileName);
                    break;
                case '3':
                    system("cls");
                    deleteFile(fileName);
                    break;
                case '4':
                    system("cls");
                    break;
                case '5':
                    system("cls");
                    readFile(fileName);
                    break;
                case '6':
                    system("cls");
                    break;
                case '7':
                    system("cls");
                    break;
                case '8':
                    system("cls");
                    break;
                case '9':
                    system("cls");
                    break;
                default:
                    printf(RED"Invalid choice\n"RESET);
                    break;
            }
        }

    } while (choiceMenu != ESC);
    return 0;
}
