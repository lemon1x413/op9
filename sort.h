#ifndef OP9_SORT_H
#define OP9_SORT_H

bool ascendingSort(char *mass1,char *mass2) {
    return (strcmp(mass1, mass2) > 0);
}

bool descendingSort(char *mass1,char *mass2) {
    return (strcmp(mass1, mass2) < 0);
}

void bubbleSort(char **mass, int n, bool (*cmp)(char *,char *)) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (cmp(mass[i], mass[j])) {
                char *tmp = 0;
                tmp = mass[i];
                mass[i] = mass[j];
                mass[j] = tmp;
            }
        }
    }
}

#endif //OP9_SORT_H
