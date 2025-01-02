#include <stdio.h>
#include <conio.h>
#include "io.h"
#include "functions.h"

int main(void) {
    char choiceMenu = 0;
    char fileName[NAME_LENGTH] = {};
    do {
        system("cls");
        *fileName == '\0'
                    ? printFileMenu(fileName)
                    : printRecordMenu(fileName);
        choiceMenu = getch();
        if (fileName[0] == '\0') {
            switch (choiceMenu) {
                case '1':
                    system("cls");
                    createFile(fileName);
                    backToMenu();
                    break;
                case '2':
                    system("cls");
                    selectFile(fileName);
                    backToMenu();
                    break;
                case '3':
                    system("cls");
                    deleteFile(fileName);
                    backToMenu();
                    break;
                default:
                    break;
            }
        } else {
            switch (choiceMenu) {
                case '1':
                    system("cls");
                    createFile(fileName);
                    backToMenu();
                    break;
                case '2':
                    system("cls");
                    selectFile(fileName);
                    backToMenu();
                    break;
                case '3':
                    system("cls");
                    deleteFile(fileName);
                    backToMenu();
                    break;
                case '4':
                    system("cls");
                    createRecord(fileName);
                    backToMenu();
                    break;
                case '5':
                    system("cls");
                    readFile(fileName, "File content:\n");
                    backToMenu();
                    break;
                case '6':
                    system("cls");
                    editRecord(fileName);
                    backToMenu();
                    break;
                case '7':
                    system("cls");
                    sortRecords(fileName);
                    backToMenu();
                    break;
                case '8':
                    system("cls");
                    insertRecord(fileName);
                    backToMenu();
                    break;
                case '9':
                    system("cls");
                    deleteRecord(fileName);
                    backToMenu();
                    break;
                default:
                    break;
            }
        }
    } while (choiceMenu != ESC);
    return 0;
}
