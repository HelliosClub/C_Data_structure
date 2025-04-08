#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


struct student {     //�л����� ������ �����ϴ� ����ü�� ����
    int  num;
    char name[6];
    int  score;
};

#define N_ST 100
//�α������� �ۼ�
void logCommandResult(const char *logFilename, const char* message) {
    FILE* filep = fopen(logFilename, "a"); // �α� ������ �߰� ���� ����
    if (filep == NULL) {
        printf("Log file open error.\n");
        return;
    }

    fprintf(filep, "%s\n", message); // ��� ����� �α� ���Ͽ� ���
    fclose(filep);
}
//insert�Լ��� ����
void insert(struct student* x, int* n, int num, char* name, int score) {
    if (num < 0 || score < 0 || strlen(name) == 0) {
        printf("�߸��� �Է��Դϴ�. �й��� ������ ����̾�� �ϰ�, �̸��� ����� �� �����ϴ�.\n");
        return; // �߸��� �Է��̹Ƿ� �α׸� ������ �ʰ� �Լ��� ����
    }

    int insertposition = *n;
    for (int i = 0; i < *n; i++) {
        if (x[i].num >= num) {
            insertposition = i;
            break; // ���� ��ġ�� ã���� ���� ����
        }
    }

    for (int i = *n; i > insertposition; i--) {
        x[i] = x[i - 1];
    }

    x[insertposition].num = num;
    strncpy(x[insertposition].name, name, 5);
    x[insertposition].name[5] = '\0'; // ���ڿ� ���� ����
    x[insertposition].score = score;

    // �ùٸ� �Է��� ���� ������� �α׿� ���
    char logMessage[100];
    sprintf(logMessage, "Inserted: %d %s %d", num, name, score);
    logCommandResult("commandLog.txt", logMessage);

    (*n)++;
}

//���Ͽ� ����Ʈ�ۼ��ϴ� �Լ��� ����
void writeFile(struct student* x, int n, const char* filename) {
    FILE* filep = fopen(filename, "wt");
    if (filep == NULL) {
        printf("File open error.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        //printf("Writing to file: %d %s %d\n", x[i].num, x[i].name, x[i].score);

        fprintf(filep, "%d %s %d\n", x[i].num, x[i].name, x[i].score);
    }

    fclose(filep);
}
//delete �Լ��� ����
void deletename(struct student* x, int* n, char* name) {
    int i, j;
    char logMessage[100]; // �α� �޽����� ������ �迭

    for (i = 0; i < *n; i++) {
        if (strncmp(x[i].name, name, 5) == 0) {
            // ��ġ�ϴ� �̸� ã��, �ش� �л� ���� ����
            for (j = i; j < *n - 1; j++) {
                x[j] = x[j + 1]; // ���ڵ� �� ĭ�� ������ �̵�
            }
            (*n)--; // �л� �� ����

            // ���� ���� �α� �޽��� ���
            sprintf(logMessage, "Deleted: %s", name);
            logCommandResult("commandLog.txt", logMessage);

            printf("Record deleted.\n");
            return; // ���� �� �Լ� ����
        }
    }

    // ��ġ�ϴ� �̸��� ���� ��� �α� �޽��� ���
    sprintf(logMessage, "Delete failed: %s not found", name);
    logCommandResult("commandLog.txt", logMessage);

    printf("Name not found.\n");
}
//list �Լ��� ����
void listName(struct student* x, int n, char* name) {
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (strncmp(x[i].name, name, 5) == 0) {
            printf(" % d % s % d\n", x[i].num, x[i].name, x[i].score);
            found = 1;
            break; // �̸��� ��ġ�ϴ� ù ��° ���ڵ带 ã���� ����
        }
    }

}
//name���� sort�ϴ� �Լ��� ����
void sortName(struct student* x, int n) {
    int i, j;
    struct student temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (strcmp(x[j].name, x[j + 1].name) > 0) {
                // �̸��� ���ĺ� ������ �Ǿ� ���� ������ �� �л��� ������ ��ȯ
                temp = x[j];
                x[j] = x[j + 1];
                x[j + 1] = temp;
            }
        }
    }
}

//score�� Sort�ϴ� �Լ��� ����
void sortScore(struct student* x, int n) {
    int i, j;
    struct student temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (x[j].score > x[j + 1].score) {
                temp = x[j];
                x[j] = x[j + 1];
                x[j + 1] = temp;
            }
        }
    }
}
//Record�� ���� ���� �Լ��� ����
int countRecords(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File open error.\n");
        return -1;
    }

    int count = 0;
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        count++;
    }

    fclose(file);
    return count;
}
//ReadDB �Լ��� ����
struct student* readDatabase(const char* filename, int* count) {
    int totalRecords = countRecords(filename);
    if (totalRecords <= 0) {
        printf("No records found or error occurred.\n");
        return NULL;
    }

    struct student* students = (struct student*)malloc(sizeof(struct student) * totalRecords);
    if (!students) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        free(students);
        printf("File open error.\n");
        return NULL;
    }

    int i = 0;
    while (i < totalRecords && fscanf(file, "%d %5s %d\n", &students[i].num, students[i].name, &students[i].score) == 3) {
        i++;
    }

    *count = totalRecords; 
    fclose(file);

    return students;
}



//�����Լ�
int main() {
    srand(time(NULL));

    //�����޸��� �Ҵ�
    struct student* x = (struct student*)malloc(sizeof(struct student) * N_ST);
    if (x == NULL) {
        // �޸� �Ҵ� ���� ó��
        printf("Memory allocation failed\n");
        return -1;
    }

    int n = 0; //n�� ���� �ʱ�ȭ

    for (int i = 0; i < N_ST; i++) {
        int num = rand() % 100 + 1; // �л� ��ȣ�� 1~100 ������ ���� ���ڷ� ����
        char name[6];
        for (int j = 0; j < 5; j++) { // ������ ������ �̸� ���� (A~Z)
            name[j] = 'A' + (rand() % 26);
        }
        name[5] = '\0'; // ���ڿ� �� ǥ��
        int score = rand() % 101; // ������ 0~100 ������ ���� ���ڷ� ����

        // ������ �л� ������ �迭�� �߰�
        insert(x, &n, num, name, score);
    }

    // ������ �л� ������ ���Ͽ� ���
    writeFile(x, n, "test.txt");




    // ���� ���� ���� ����� �Է� ó�� �κ�
    while (1) {
        printf(">> ");
        char cmd[1024]; // ����� ū ũ��� ���� ũ�� ����
        if (!fgets(cmd, sizeof(cmd), stdin)) {

            continue; // ����ڰ� ��� �õ��� �� �ֵ��� ��
        }



        char* token = strtok(cmd, " \n"); // ����� ���� ���ڸ� �����ڷ� ���
        if (token == NULL) {
            continue; // ��ū�� ������ �ٽ� ����
        }

        if (strcmp(token, "exit") == 0) {
            break; // ���α׷� ����
        }
        else if (strcmp(token, "insert") == 0) {
            token = strtok(NULL, " ");
            int num = token ? atoi(token) : -1;

            token = strtok(NULL, " ");
            char name[6] = "";
            if (token) strncpy(name, token, sizeof(name) - 1);

            token = strtok(NULL, " ");
            int score = token ? atoi(token) : -1;

            if (num != -1 && score != -1 && strlen(name) > 0) {
                insert(x, &n, num, name, score);
                writeFile(x, n, "test.txt");
            }
            else {
                printf("Invalid insert command.\n");
            }
        }
        else if (strcmp(token, "delete") == 0) {
            token = strtok(NULL, " ");
            if (token) {
                char nameToDelete[6] = "";
                strncpy(nameToDelete, token, sizeof(nameToDelete) - 1);
                deletename(x, &n, nameToDelete);
                writeFile(x, n, "test.txt");
            }
            else {
                printf("Invalid delete command. Use 'delete name'.\n");
            }
        }
        else if (strcmp(token, "list") == 0) {
            token = strtok(NULL, " ");
            if (token) {
                char nameToList[6] = "";
                strncpy(nameToList, token, sizeof(nameToList) - 1);
                listName(x, n, nameToList);
            }
            else {
                printf("Invalid list command. Use 'list name'.\n");
            }
        }
        else if (strcmp(token, "sort") == 0) {
            token = strtok(NULL, " ");
            if (token && strcmp(token, "name") == 0) {
                sortName(x, n);
                writeFile(x, n, "test.txt");
                printf("Sorted by name.\n");
            }
            else if (token && strcmp(token, "score") == 0) {
                sortScore(x, n);
                writeFile(x, n, "test.txt");
                printf("Sorted by score.\n");
            }
            else {
                printf("Invalid sort command. Use 'sort name' or 'sort score'.\n");
            }
        }
        else if (strcmp(token, "readDB") == 0) {
            token = strtok(NULL, ""); // ������ �κ��� ���� ���� �̸����� ó��
            if (token) {
                char* filename = token; // ������ ���Ե� ���� �̸��� ó�� ����
                FILE* file = fopen(filename, "r");
                if (!file) {
                    printf("File open error.\n");
                    continue;
                }

                int count = 0;
                struct student temp;
                while (fscanf(file, "%d %5s %d", &temp.num, temp.name, &temp.score) == 3) {
                    printf("%d %s %d\n", temp.num, temp.name, temp.score);
                    count++;
                }

                printf("Total records: %d\n", count);
                fclose(file);
            }
            else {
                printf("Filename not specified.\n");
            }
        }
        else if (strcmp(token, "writeDB") == 0) {
            token = strtok(NULL, ""); // ������ �κ��� ���� ���� �̸����� ó��
            if (token) {
                char* filename = token; // ������ ���Ե� ���� �̸��� ó�� ����
                FILE* logFile = fopen("commandLog.txt", "r");
                if (!logFile) {
                    printf("Unable to open log file.\n");
                    continue;
                }

                FILE* targetFile = fopen(filename, "w");
                if (!targetFile) {
                    printf("Unable to open target file: %s\n", filename);
                    fclose(logFile);
                    continue;
                }

                char buffer[1024];
                while (fgets(buffer, sizeof(buffer), logFile) != NULL) {
                    fputs(buffer, targetFile);
                }

                printf("Log contents successfully written to %s\n", filename);
                fclose(logFile);
                fclose(targetFile);
            }
            else {
                printf("Filename not specified.\n");
            }
        }



    }

    free(x);
    return 0;
}