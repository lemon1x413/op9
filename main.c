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
                case CREATE_FILE:
                    system("cls");
                    createFile(fileName);
                    backToMenu();
                    break;
                case SELECT_FILE:
                    system("cls");
                    selectFile(fileName);
                    backToMenu();
                    break;
                case DELETE_FILE:
                    system("cls");
                    deleteFile(fileName);
                    backToMenu();
                    break;
                default:
                    break;
            }
        } else {
            switch (choiceMenu) {
                case CREATE_FILE:
                    system("cls");
                    createFile(fileName);
                    backToMenu();
                    break;
                case SELECT_FILE:
                    system("cls");
                    selectFile(fileName);
                    backToMenu();
                    break;
                case DELETE_FILE:
                    system("cls");
                    deleteFile(fileName);
                    backToMenu();
                    break;
                case CREATE_RECORD:
                    system("cls");
                    createRecord(fileName);
                    backToMenu();
                    break;
                case READ_RECORDS:
                    system("cls");
                    readRecords(fileName, "File content:\n");
                    backToMenu();
                    break;
                case EDIT_RECORD:
                    system("cls");
                    editRecord(fileName);
                    backToMenu();
                    break;
                case SORT_RECORDS:
                    system("cls");
                    sortRecords(fileName);
                    backToMenu();
                    break;
                case INSERT_RECORD:
                    system("cls");
                    insertRecord(fileName);
                    backToMenu();
                    break;
                case DELETE_RECORD:
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
