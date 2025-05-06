#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct list {
    int nr;
    int *status;
    char *filename;
    char **tasks;
}list;

list create_list(char *filename) {
    FILE *data = fopen(filename, "r");
    int nr;
    fscanf(data, "%d", &nr);
    nr++;
    int *status = malloc(nr * sizeof(int));
    status[0] = -1;
    for (int i = 1; i < nr; ++i) {
        fscanf(data, "%d", &status[i]);
    }
    char **actions = malloc(nr * sizeof(char *));
    actions[0] = "\033[31mInapoi\033[0m";
    char a[10];
    fgets(a, 100, data);
    for (int i = 1; i < nr; ++i) {
        char *action = malloc(sizeof(char) * 100);
        fgets(action, 100, data);
        action[strlen(action)-1] = '\0';
        actions[i] = action;
    }
    fclose(data);
    list l;
    l.nr = nr;
    l.status = status;
    l.filename = filename;
    l.tasks = actions;
    FILE *output = fopen("output.txt", "w");
    for (int i = 0; i < nr; ++i) {
        fprintf(output, "%d\n", l.status[i]);
        fprintf(output, "%s\n", l.tasks[i]);
    }
    fclose(output);
    return l;
}

void update_data(char *filename, int del) {
    FILE *data = fopen("data.txt", "r");
    int nr;
    fscanf(data, "%d", &nr);
    char **name = malloc(nr * sizeof(char *));
    char *a = malloc(sizeof(char) * 10);
    fgets(a, 100, data);
    free(a);
    for (int i = 0; i < nr; ++i) {
        char *action = malloc(sizeof(char) * 100);
        fgets(action, 100, data);
        action[strlen(action)-1] = '\0';
        name[i] = action;
    }
    fclose(data);
    if (del == 1) {
        int ok = 1;
        FILE *data_new = fopen("data.txt", "w");
        for (int i = 0; i < nr; ++i) {
            if (strcmp(name[i], filename) == 0) ok = 0;
        }
        if (ok) {
            fprintf(data_new, "%d\n", nr+1);
            for (int i = 0; i < nr; ++i) {
                fprintf(data_new, "%s\n", name[i]);
            }
            fprintf(data_new, "%s\n", filename);
        }else {
            fprintf(data_new, "%d\n", nr);
            for (int i = 0; i < nr; ++i) {
                fprintf(data_new, "%s\n", name[i]);
            }
        }
        fclose(data_new);
        free(name);
    }
    else {
        FILE *data_new = fopen("data.txt", "w");
        fprintf(data_new, "%d\n", nr-1);
        for (int i = 0; i < nr; ++i) {
            if (strcmp(name[i], filename) != 0)
                fprintf(data_new, "%s\n", name[i]);
        }
        fclose(data_new);
        free(name);
    }
}

void DeleteList() {
    int nr = 0;
    FILE *data = fopen("data.txt", "r");
    fscanf(data, "%d", &nr);
    if (nr == 0) {
        system("cls");
        printf("\033[33m-= Sterge Lista =-\033[0m\n\033[34m-=> \033[0m\033[31mInapoi\033[0m");
        int no_action = 1;
        while (no_action) {
            Sleep(100);
            if (GetAsyncKeyState(VK_RETURN)) {no_action = 0;getchar();}
        }
    }
    else {
        nr++;
        char **name = malloc((nr) * sizeof(char *));
        char *a = malloc(sizeof(char) * 10);
        fgets(a, 100, data);
        free(a);
        name[0] = "\033[31mInapoi\033[0m";
        for (int i = 1; i < nr; ++i) {
            char *action = malloc(sizeof(char) * 100);
            fgets(action, 100, data);
            action[strlen(action)-1] = '\0';
            name[i] = action;
        }
        int choice = 0;
        while (1) {
            system("cls");
            printf("\033[33m-= Sterge Lista =-\033[0m\n");
            for (int i = 0; i < nr; ++i) {
                if (choice == i) printf("\033[34m-=> \033[0m");
                printf("%s\n", name[i]);
            }
            int no_action = 1;
            while (no_action) {
                Sleep(100);
                if (GetAsyncKeyState(VK_UP)) {choice--; no_action = 0;}
                if (GetAsyncKeyState(VK_DOWN)) {choice++; no_action = 0;}
                if (GetAsyncKeyState(VK_RETURN)) {
                    getchar();
                    fclose(data);
                    if (choice == 0) {
                        free(name);
                        return;
                    }
                    remove(name[choice]);
                    update_data(name[choice], 0);
                    return DeleteList();
                }
                if (choice < 0) choice = nr-1;
                if (choice > nr-1) choice = 0;
            }
        }
    }
}

int verify_format_data(char *data) {
    if (strlen(data) != 10) return 0;
    if (data[0] < '0' || data[0] > '3') return 0;
    if (data[1] < '0' || data[1] > '9') return 0;
    if (data[2] != '.') return 0;
    if (data[3] < '0' || data[3] > '1') return 0;
    if (data[4] < '0' || data[4] > '9') return 0;
    if (data[5] != '.') return 0;
    if (data[6] < '0' || data[6] > '2') return 0;
    if (data[7] < '0' || data[7] > '9') return 0;
    if (data[8] < '0' || data[8] > '9') return 0;
    if (data[9] < '0' || data[9] > '9') return 0;
    return 1;
}

int verify_format_nr(char *nr) {
    for (int i = 0; nr[i] != '\0'; ++i) {
        if (nr[i] < '0' || nr[i] > '9') return 0;
    }
    return 1;
}

void AddList() {
    char filename[22];
    strcpy(filename, "Activitati ");
    printf("\033[33m-= Adauga Lista =-\033[0m\nIntroduceti data (\033[31mzz.ll.aaaa\033[0m):\n");
    char data[11];
    scanf("%s", data);
    while (verify_format_data(data) == 0) {
        printf("Introduceti data (\033[31mzz.ll.aaaa\033[0m):\n");
        scanf("%s", data);
    }
    strcat(filename, data);
    printf("Introduceti \033[31mnr de sarcini\033[0m sau \033[31m0 pentru a iesi\033[0m\n");
    int nr;
    char vf[10];
    scanf("%s", vf);
    while (verify_format_nr(vf) == 0) {
        printf("Introduceti \033[31mnr de sarcini\033[0m sau \033[31m0 pentru a iesi\033[0m\n");
        scanf("%s", vf);
    }

    nr = atoi(vf);
    if (nr == 0) {
        GetAsyncKeyState(VK_RETURN);
        getchar();
        return;
    }
    getchar();
    FILE *new_file = fopen(filename, "w");
    fprintf(new_file, "%d\n", nr);
    for (int i = 0; i < nr-1; ++i) fprintf(new_file, "0 ");
    fprintf(new_file, "0\n");
    for (int i = 0; i < nr; ++i) {
        char *action = malloc(sizeof(char) * 100);
        printf("Introduceti sarcina %d:\n", i+1);
        fgets(action, 100, stdin);
        action[strlen(action)-1] = '\0';
        fprintf(new_file, "%s\n", action);
        free(action);
    }
    update_data(filename, 1);
    fclose(new_file);
    while (nr--) {
        GetAsyncKeyState(VK_RETURN);
    }
}

int del_condition(char *filename) {
    FILE *data = fopen(filename, "r");
    int nr;
    fscanf(data, "%d", &nr);
    for (int i = 0; i < nr; ++i) {
        int status;
        fscanf(data, "%d", &status);
        if (status == 0) {
            fclose(data);
            return 0;
        }
    }
    fclose(data);
    return 1;
}

void update_list(list l) {
    FILE *data = fopen(l.filename, "w");
    fprintf(data, "%d\n", l.nr-1);
    for (int i = 1; i < l.nr-1; ++i) {
        fprintf(data, "%d ", l.status[i]);
    }
    fprintf(data, "%d\n", l.status[l.nr-1]);
    for (int i = 1; i < l.nr; ++i) {
        fprintf(data, "%s\n", l.tasks[i]);
    }
    fclose(data);
}

void open_list(char *filename) {
    list l = create_list(filename);
    int choice = 0;
    while (1) {
        system("cls");
        printf("\033[33m-= %s =-\033[0m\n", filename);
        for (int i = 0; i < l.nr; ++i) {
            if (choice == i) printf("\033[34m-=> \033[0m");
            if (l.status[i] == 1) printf("\033[32m[x] \033[0m");
            else if (l.status[i] == 0) printf("\033[32m[] \033[0m");
            printf("%s\n", l.tasks[i]);
        }
        int no_action = 1;
        while (no_action) {
            Sleep(100);
            if (GetAsyncKeyState(VK_UP)) {choice--; no_action = 0;}
            if (GetAsyncKeyState(VK_DOWN)) {choice++; no_action = 0;}
            if (GetAsyncKeyState(VK_RETURN)) {
                getchar();
                if (choice == 0) {
                    update_list(l);
                    free(l.tasks);
                    free(l.status);
                    return;
                }
                l.status[choice] = (1+l.status[choice]) % 2;
                no_action = 0;
            }
            if (choice < 0) choice = l.nr-1;
            if (choice > l.nr-1) choice = 0;
        }
    }
}

void Liste() {
    int nr = 0;
    FILE *data = fopen("data.txt", "r");
    fscanf(data, "%d", &nr);
    if (nr == 0) {
        system("cls");
        printf("\033[33m-= Liste =-\033[0m\n");
        printf("\033[34m-=> \033[0m");
        printf("\033[31mInapoi\033[0m");
        int no_action = 1;
        while (no_action) {
            Sleep(100);
            if (GetAsyncKeyState(VK_RETURN)) {no_action = 0; getchar();}
        }
    }
    else {
        nr++;
        char **name = malloc((nr) * sizeof(char *));
        char *a = malloc(sizeof(char) * 10);
        fgets(a, 100, data);
        free(a);
        name[0] = "\033[31mInapoi\033[0m";
        for (int i = 1; i < nr; ++i) {
            char *action = malloc(sizeof(char) * 100);
            fgets(action, 100, data);
            action[strlen(action)-1] = '\0';
            name[i] = action;
        }
        int choice = 0;
        while (1) {
            system("cls");
            printf("\033[33m-= Liste =-\033[0m\n");
            for (int i = 0; i < nr; ++i) {
                if (choice == i) printf("\033[34m-=> \033[0m");
                printf("%s\n", name[i]);
            }
            int no_action = 1;
            while (no_action) {
                Sleep(100);
                if (GetAsyncKeyState(VK_UP)) {choice--; no_action = 0;}
                if (GetAsyncKeyState(VK_DOWN)) {choice++; no_action = 0;}
                if (GetAsyncKeyState(VK_RETURN)) {
                    getchar();
                    fclose(data);
                    if (choice == 0) {
                        free(name);
                        return;
                    }
                    open_list(name[choice]);
                    if (del_condition(name[choice])) {
                        remove(name[choice]);
                        update_data(name[choice], 0);
                    }
                    return Liste();
                }
                if (choice < 0) choice = nr-1;
                if (choice > nr-1) choice = 0;
            }
        }
    }
}

void display_menu(int action) {
    printf("\033[33m-= Meniu =-\033[0m\n");
    char text[4][20] = {{"Liste"}, {"Adauga Lista"}, {"Sterge Lista"}, {"\033[31mExit\033[0m"}};
    for (int i = 0; i < 4; ++i) {
        if (action == i) {printf("\033[34m-=> \033[0m");printf("%s\n", text[i]);}
        else printf("%s\n", text[i]);
    }
}

void make_data_file() {
    FILE *data = fopen("data.txt", "r");
    if (data == NULL) {
        fclose(data);
        FILE *new_file = fopen("data.txt", "w");
        fprintf(new_file, "0\n");
        fclose(new_file);
        return;
    }
    fclose(data);
}

void start() {
    int choice = 0;
    while(1) {
        system("cls");
        display_menu(choice);
        int no_action = 1;
        while (no_action) {
            Sleep(100);
            if (GetAsyncKeyState(VK_UP)){choice--; no_action = 0;}
            if (GetAsyncKeyState(VK_DOWN)) {choice++; no_action = 0;}
            if (GetAsyncKeyState(VK_RETURN)) {
                getchar();
                no_action = 0;
                if (choice == 3) return;
                system("cls");
                switch (choice) {
                    case 0: Liste();break;
                    case 1: AddList();break;
                    case 2: DeleteList();break;
                    default: printf("NU MERGE :<");break;
                }
            }
            if (choice < 0) choice = 3;
            if (choice > 3) choice = 0;
        }
    }
}

int main() {
    make_data_file();
    start();
    return 0;
}
