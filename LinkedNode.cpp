//#define linked_Student
//#define linked_stack_queue
//#define maze_with_linkedlist


#ifdef linked_student
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 연결 리스트를 위한 노드 구조체
struct ListNode {
    int num;
    char name[6];
    int score;
    struct ListNode* next;
};

// 연결 리스트 삽입 함수
void insert(struct ListNode** head, int num, char* name, int score) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }
    newNode->num = num;
    strncpy(newNode->name, name, 5);
    newNode->name[5] = '\0'; // 널 문자를 추가하여 정확한 문자열을 만듭니다.
    newNode->score = score;
    newNode->next = NULL;

    // 삽입 위치 찾기
    struct ListNode** current = head;
    while (*current != NULL && (*current)->num < num) {
        current = &(*current)->next;
    }
    // 노드 삽입
    newNode->next = *current;
    *current = newNode;

    printf("Insert successful: %d %s %d\n", num, name, score);
}

// 연결 리스트 삭제 함수
void delete_list(struct ListNode** head, char* name) {
    struct ListNode* current = *head;
    struct ListNode* prev = NULL;

    while (current != NULL && strncmp(current->name, name, 5) != 0) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Name not found.\n");
        return;
    }

    if (prev == NULL) {
        *head = current->next;
    }
    else {
        prev->next = current->next;
        
    }

    free(current);
}



// 연결 리스트 출력 함수
void printList(struct ListNode* head) {
    struct ListNode* current = head;
    while (current != NULL) {
        printf("%d %s %d\n", current->num, current->name, current->score);
        current = current->next;
    }
}

// 연결 리스트 해제 함수
void freeList(struct ListNode* head) {
    struct ListNode* current = head;
    while (current != NULL) {
        struct ListNode* temp = current;
        current = current->next;
        free(temp);
    }
}
void swap(struct ListNode* a, struct ListNode* b) {
    int tempNum = a->num;
    char tempName[6];
    strcpy(tempName, a->name);
    int tempScore = a->score;

    a->num = b->num;
    strcpy(a->name, b->name);
    a->score = b->score;

    b->num = tempNum;
    strcpy(b->name, tempName);
    b->score = tempScore;
}
void sortName(struct ListNode** head) {
    int swapped;
    struct ListNode* ptr1;
    struct ListNode* lptr = NULL;

    /* Checking for empty list */
    if (*head == NULL)
        return;

    do {
        swapped = 0;
        ptr1 = *head;

        while (ptr1->next != lptr) {
            if (strcmp(ptr1->name, ptr1->next->name) > 0) {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    printf("List sorted by name.\n");
}

void sortScore(struct ListNode** head) {
    int swapped;
    struct ListNode* ptr1;
    struct ListNode* lptr = NULL;

    /* Checking for empty list */
    if (*head == NULL)
        return;

    do {
        swapped = 0;
        ptr1 = *head;

        while (ptr1->next != lptr) {
            if (ptr1->score > ptr1->next->score) {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    printf("List sorted by score.\n");
}

void writeFile(struct ListNode* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file %s for writing.\n", filename);
        return;
    }

    struct ListNode* current = head;
    while (current != NULL) {
        fprintf(file, "%d %s %d\n", current->num, current->name, current->score);
        current = current->next;
    }

    fclose(file);
}
void generateRandomStudents(struct ListNode** head, int num_students) {
    srand(time(NULL)); // 무작위 수 생성기 초기화

    for (int i = 0; i < num_students; i++) {
        int num = rand() % 1000 + 1; // 학생 번호를 1~1000 사이의 랜덤 숫자로 설정
        char name[6];
        for (int j = 0; j < 5; j++) { // 간단한 무작위 이름 생성 (A~Z)
            name[j] = 'A' + (rand() % 26);
        }
        name[5] = '\0'; // 문자열 끝 표시
        int score = rand() % 101; // 점수를 0~100 사이의 랜덤 숫자로 설정

        // 생성된 학생 정보를 연결 리스트에 추가
        insert(head, num, name, score);
    }
}

int main() {
    struct ListNode* head = NULL;
    char input[100];
    char filename[] = "test.txt"; // 파일 이름을 지정합니다.

    struct ListNode* head = NULL; // 연결 리스트 초기화
    int num_students = 1000; // 생성할 학생 수

    generateRandomStudents(&head, num_students); // 랜덤 학생 정보 생성 및 리스트에 추가

    writeFile(head, "test.txt"); // 생성된 학생 정보를 파일에 기록

    
    while (1) {
        printf(">> ");
        if (!fgets(input, sizeof(input), stdin)) {
            continue; // Error or end of file
        }

        char* command = strtok(input, " \n");

        if (command && strcmp(command, "insert") == 0) {
            int num = atoi(strtok(NULL, " \n"));
            char* name = strtok(NULL, " \n");
            int score = atoi(strtok(NULL, " \n"));
            if (name && strlen(name) == 5) {
                insert(&head, num, name, score);
                writeFile(head, filename);
            }
            else {
                printf("Invalid name length.\n");
            }
        }
        else if (command && strcmp(command, "delete") == 0) {
            char* name = strtok(NULL, " \n");
            if (name) {
                delete_list(&head, name);
                writeFile(head, filename);
                printf("Delete successful: %s\n", name);
            }
            else {
                printf("Invalid name.\n");
            }
        }
        
        else if (command && strcmp(command, "sortname") == 0) {
            writeFile(head, filename);
            sortName(&head);
        }
        else if (command && strcmp(command, "sortscore") == 0) {
            writeFile(head, filename);
            sortScore(&head);
        }
        
        else if (command && strcmp(command, "exit") == 0) {
            writeFile(head, filename); // 프로그램을 종료하기 전에 리스트를 파일에 저장합니다.
            break;
        }
        else {
            printf("Invalid command.\n");
        }
    }

    freeList(head);
    return 0;
}
#endif

#ifdef linked_stack_queue

//#define stack_with_linkedlist
#ifdef stack_with_linkedlist
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

void push(struct Node** top, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    newNode->data = data;
    newNode->next = *top;
    *top = newNode;
}

int pop(struct Node** top) {
    if (*top == NULL) {
        printf("Stack is empty\n");
        return -1; // Stack underflow
    }
    struct Node* temp = *top;
    int data = temp->data;
    *top = temp->next;
    free(temp);
    return data;
}

#endif

//#define Queue_with_linkedlist
#ifdef Queue_with_linkedlist
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Queue {
    struct Node* front;
    struct Node* rear;
};

void enqueue(struct Queue* q, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    newNode->data = data;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    }
    else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

int dequeue(struct Queue* q) {
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return -1; // Queue underflow
    }
    struct Node* temp = q->front;
    int data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return data;
}

#endif
#endif

#ifdef maze_with_linkedlist
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>



typedef struct {
    int x;
    int y;
} position;

typedef struct Node {
    position data;
    struct Node* next;
} Node;

void push(Node** top, position data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    newNode->data = data;
    newNode->next = *top;
    *top = newNode;
}

position pop(Node** top) {
    position data = { -1, -1 };  // Return -1, -1 if stack is empty
    if (*top == NULL) {
        printf("Stack is empty\n");
        return data;
    }
    Node* temp = *top;
    data = temp->data;
    *top = temp->next;
    free(temp);
    return data;
}

int isEmpty(Node* top) {
    return top == NULL;
}

int maze[6][6] = {
    {0, 1, 0, 0, 0, 1},
    {0, 1, 0, 1, 0, 0},
    {0, 0, 0, 1, 0, 1},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 1, 0, 0, 0},
    {1, 0, 0, 1, 1, 0}
};

int visited[6][6] = { 0 }; // 초기화되지 않은 모든 요소는 0으로 설정됩니다.
void printMaze(int maze[6][6]) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            printf("%d ", maze[i][j]);
        }
        printf("\n");
    }
}
void printPath(Node* path) {
    if (path != NULL) {
        printPath(path->next);  // Recursively go to the end of the list
        printf("(%d, %d) -> ", path->data.x, path->data.y);  // Print each position
    }
}

void findPath(int startX, int startY) {
    Node* stack = NULL;
    push(&stack, (position { startX, startY }));
    visited[startX][startY] = 1;

    while (!isEmpty(stack)) {
        position current = pop(&stack);
        int x = current.x;
        int y = current.y;

        if (x == 5 && y == 5) {
            printf("Reached the exit at (%d, %d)\n", x, y);
            printPath(stack);  // Print the path from start to one step before the exit
            printf("(%d, %d)\n", x, y);  // Print the exit step
            return;
        }

        int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        for (int i = 0; i < 4; i++) {
            int nextX = x + directions[i][0];
            int nextY = y + directions[i][1];

            if (nextX >= 0 && nextX < 6 && nextY >= 0 && nextY < 6 &&
                maze[nextX][nextY] == 0 && !visited[nextX][nextY]) {
                push(&stack, (position { nextX, nextY }));
                visited[nextX][nextY] = 1;
            }
        }
    }

    printf("No path to the exit found.\n");
}


int main() {
    printMaze(maze);
    findPath(0, 0);
    return 0;
}

#endif