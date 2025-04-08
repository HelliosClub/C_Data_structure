#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


struct student {     //학생들의 정보를 저장하는 구조체의 선언
    int  num;
    char name[6];
    int  score;
};

#define N_ST 100
//로그파일의 작성
void logCommandResult(const char *logFilename, const char* message) {
    FILE* filep = fopen(logFilename, "a"); // 로그 파일을 추가 모드로 열기
    if (filep == NULL) {
        printf("Log file open error.\n");
        return;
    }

    fprintf(filep, "%s\n", message); // 명령 결과를 로그 파일에 기록
    fclose(filep);
}
//insert함수의 설정
void insert(struct student* x, int* n, int num, char* name, int score) {
    if (num < 0 || score < 0 || strlen(name) == 0) {
        printf("잘못된 입력입니다. 학번과 점수는 양수이어야 하고, 이름은 비워둘 수 없습니다.\n");
        return; // 잘못된 입력이므로 로그를 남기지 않고 함수를 종료
    }

    int insertposition = *n;
    for (int i = 0; i < *n; i++) {
        if (x[i].num >= num) {
            insertposition = i;
            break; // 삽입 위치를 찾으면 루프 종료
        }
    }

    for (int i = *n; i > insertposition; i--) {
        x[i] = x[i - 1];
    }

    x[insertposition].num = num;
    strncpy(x[insertposition].name, name, 5);
    x[insertposition].name[5] = '\0'; // 문자열 종료 보장
    x[insertposition].score = score;

    // 올바른 입력일 때만 변경사항 로그에 기록
    char logMessage[100];
    sprintf(logMessage, "Inserted: %d %s %d", num, name, score);
    logCommandResult("commandLog.txt", logMessage);

    (*n)++;
}

//파일에 리스트작성하는 함수의 설정
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
//delete 함수의 설정
void deletename(struct student* x, int* n, char* name) {
    int i, j;
    char logMessage[100]; // 로그 메시지를 저장할 배열

    for (i = 0; i < *n; i++) {
        if (strncmp(x[i].name, name, 5) == 0) {
            // 일치하는 이름 찾음, 해당 학생 정보 삭제
            for (j = i; j < *n - 1; j++) {
                x[j] = x[j + 1]; // 레코드 한 칸씩 앞으로 이동
            }
            (*n)--; // 학생 수 감소

            // 삭제 성공 로그 메시지 기록
            sprintf(logMessage, "Deleted: %s", name);
            logCommandResult("commandLog.txt", logMessage);

            printf("Record deleted.\n");
            return; // 삭제 후 함수 종료
        }
    }

    // 일치하는 이름이 없는 경우 로그 메시지 기록
    sprintf(logMessage, "Delete failed: %s not found", name);
    logCommandResult("commandLog.txt", logMessage);

    printf("Name not found.\n");
}
//list 함수의 설정
void listName(struct student* x, int n, char* name) {
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (strncmp(x[i].name, name, 5) == 0) {
            printf(" % d % s % d\n", x[i].num, x[i].name, x[i].score);
            found = 1;
            break; // 이름이 일치하는 첫 번째 레코드를 찾으면 종료
        }
    }

}
//name으로 sort하는 함수의 설정
void sortName(struct student* x, int n) {
    int i, j;
    struct student temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (strcmp(x[j].name, x[j + 1].name) > 0) {
                // 이름이 알파벳 순으로 되어 있지 않으면 두 학생의 정보를 교환
                temp = x[j];
                x[j] = x[j + 1];
                x[j + 1] = temp;
            }
        }
    }
}

//score로 Sort하는 함수의 설정
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
//Record의 개수 세는 함수의 설정
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
//ReadDB 함수의 설정
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



//메인함수
int main() {
    srand(time(NULL));

    //동적메모리의 할당
    struct student* x = (struct student*)malloc(sizeof(struct student) * N_ST);
    if (x == NULL) {
        // 메모리 할당 실패 처리
        printf("Memory allocation failed\n");
        return -1;
    }

    int n = 0; //n의 값을 초기화

    for (int i = 0; i < N_ST; i++) {
        int num = rand() % 100 + 1; // 학생 번호를 1~100 사이의 랜덤 숫자로 설정
        char name[6];
        for (int j = 0; j < 5; j++) { // 간단한 무작위 이름 생성 (A~Z)
            name[j] = 'A' + (rand() % 26);
        }
        name[5] = '\0'; // 문자열 끝 표시
        int score = rand() % 101; // 점수를 0~100 사이의 랜덤 숫자로 설정

        // 생성된 학생 정보를 배열에 추가
        insert(x, &n, num, name, score);
    }

    // 생성된 학생 정보를 파일에 기록
    writeFile(x, n, "test.txt");




    // 메인 루프 내의 사용자 입력 처리 부분
    while (1) {
        printf(">> ");
        char cmd[1024]; // 충분히 큰 크기로 버퍼 크기 조정
        if (!fgets(cmd, sizeof(cmd), stdin)) {

            continue; // 사용자가 계속 시도할 수 있도록 함
        }



        char* token = strtok(cmd, " \n"); // 공백과 개행 문자를 구분자로 사용
        if (token == NULL) {
            continue; // 토큰이 없으면 다시 시작
        }

        if (strcmp(token, "exit") == 0) {
            break; // 프로그램 종료
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
            token = strtok(NULL, ""); // 나머지 부분을 전부 파일 이름으로 처리
            if (token) {
                char* filename = token; // 공백이 포함된 파일 이름도 처리 가능
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
            token = strtok(NULL, ""); // 나머지 부분을 전부 파일 이름으로 처리
            if (token) {
                char* filename = token; // 공백이 포함된 파일 이름도 처리 가능
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