
//#define TEST_MATLAB
//#define TEST_MAZE_GEN
//#define TEST_MAZE_OUT





#ifdef TEST_MATLAB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_SIZE 10000



typedef int element;
typedef struct StackType {
    element* data;		// data는 구조체 안에 정의된 변수
    int top;

};
//행렬 구조체 선언
typedef struct Matrix2x2 {
    int m11, m12; // 첫 번째 행의 요소
    int m21, m22; // 두 번째 행의 요소
};

//행렬의 덧셈
Matrix2x2 add_matrix2x2(Matrix2x2* a, Matrix2x2* b) {
    Matrix2x2 result;
    result.m11 = a->m11 + b->m11;
    result.m12 = a->m12 + b->m12;
    result.m21 = a->m21 + b->m21;
    result.m22 = a->m22 + b->m22;
    return result;
}

//행렬의 뺄셈
Matrix2x2 sub_matrix2x2(Matrix2x2* a, Matrix2x2* b) {
    Matrix2x2 result;
    result.m11 = a->m11 - b->m11;
    result.m12 = a->m12 - b->m12;
    result.m21 = a->m21 - b->m21;
    result.m22 = a->m22 - b->m22;
    return result;
}

//행렬의 곱셈
Matrix2x2 multiply_matrix2x2(Matrix2x2* a, Matrix2x2* b) {
    Matrix2x2 result;
    result.m11 = a->m11 * b->m11 + a->m12 * b->m21;
    result.m12 = a->m11 * b->m12 + a->m12 * b->m22;
    result.m21 = a->m21 * b->m11 + a->m22 * b->m21;
    result.m22 = a->m21 * b->m12 + a->m22 * b->m22;
    return result;
}
//입력된 함수의 파싱
int parse_matrix(char* input, Matrix2x2* matrix) {

    char* format = "[%d %d ; %d %d]";

    // 불필요한 공백과 문자 제거
    char cleaned_input[100];  // 충분한 크기의 배열 선언
    int j = 0;
    for (int i = 0; input[i] != '\0'; ++i) {
        if (isdigit(input[i]) || input[i] == '[' || input[i] == ']' || input[i] == ';' || input[i] == ' ') {
            cleaned_input[j++] = input[i];
        }
    }
    cleaned_input[j] = '\0';  // 문자열 끝에 NULL 추가

    // sscanf를 사용하여 입력 문자열에서 행렬 데이터 추출
    int parsed = sscanf(cleaned_input, format, &matrix->m11, &matrix->m12, &matrix->m21, &matrix->m22);

    // 성공적으로 4개의 숫자를 읽었는지 확인
    return parsed == 4;
}
//함수의 출력
void print_matrix(Matrix2x2* matrix) {
    printf("[%d %d]\n[%d %d]\n", matrix->m11, matrix->m12, matrix->m21, matrix->m22);
}

//s는 구조체를 가리키는 구조체 포인터
//s->data = 는 s가 가리키는 구조체의 data에 값을 할당
// 스택 초기화 함수
void init_stack(StackType* s)
{
    s->top = -1;
    s->data = (element*)malloc(MAX_STACK_SIZE * sizeof(element));
    if (s->data == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
}
// 스택이 비어있는지 확인하는 함수
int is_empty(StackType* s) {

    return (s->top == -1);
}

// 스택이 가득 찼는지 확인하는 함수
int is_full(StackType* s) {
    return (s->top == (MAX_STACK_SIZE - 1));
}

// 스택에 요소를 추가하는 함수
void push(StackType* s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "Stack is full.\n");
        exit(1);
    }
    else s->data[++(s->top)] = item;
}

// 스택에서 요소를 제거하는 함수
element pop(StackType* s) {
    if (is_empty(s)) {
        printf("Stack is empty.\n");
        exit(1);
    }
    else return s->data[(s->top)--];
}

// 스택 메모리 해제 함수
void destroy_stack(StackType* s) {
    free(s->data);
    s->top = -1;
}
//연산자들의 우선순위 설정
int prec(char op)
{
    switch (op) {
    case '(': case ')': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    case '^': return 3;
    }
    return -1;
}
//스택의 맨위만 출력하는 함수
element peek(StackType* s) {
    if (is_empty(s)) {
        printf("Stack is empty.\n");
        exit(1);
    }
    return s->data[s->top];
}

//중위표기법을 후위표기법으로 바꾸는 함수
void infix_to_postfix(char* exp, char* postfix) {
    int i = 0, j = 0;  // 인덱스 변수 초기화
    char ch, e;        // 현재 문자와 스택에서 팝된 문자를 저장할 변수
    StackType s;       // 스택 정의

    init_stack(&s);    // 스택 초기화

    // exp의 각 문자를 순회
    while (exp[i] != '\0') {
        ch = exp[i];
        switch (ch) {
        case '+':
        case '-':
        case '*':
        case '/':
            // 스택이 비어있지 않고 스택의 맨 위 요소의 우선순위가 ch의 우선순위보다 크거나 같으면
            while (!is_empty(&s) && prec(peek(&s)) >= prec(ch)) {
                e = pop(&s);          // 스택에서 팝
                postfix[j++] = e;     // 팝된 연산자를 postfix에 추가
            }
            push(&s, ch);             // 현재 연산자를 스택에 푸시
            break;
        case '(':
            push(&s, ch);             // 왼쪽 괄호를 스택에 푸시
            break;
        case ')':
            e = pop(&s);              // 스택에서 팝
            // 팝된 요소가 왼쪽 괄호가 아닐 때까지 반복
            while (e != '(') {
                postfix[j++] = e;     // 팝된 연산자를 postfix에 추가
                e = pop(&s);          // 다음 요소를 팝
            }
            break;
        default:
            if (isdigit(ch)) {
                postfix[j++] = ch;    // 피연산자를 postfix에 추가
            }
            break;
        }
        i++;  // 다음 문자로 이동
    }

    // 스택에 남은 모든 연산자를 팝하여 postfix에 추가
    while (!is_empty(&s)) {
        e = pop(&s);
        postfix[j++] = e;
    }

    postfix[j] = '\0';  // 문자열 종료
    destroy_stack(&s);  // 스택 메모리 해제
}


int eval(char exp[]) {
    int op1, op2, value, i = 0;
    int len = strlen(exp);
    char ch;
    StackType s;
    init_stack(&s);
    for (i = 0; i < len; i++) {
        ch = exp[i];
        if (ch != '+' && ch != '-' && ch != '*' && ch != '/') {
            value = ch - '0';	// 입력이 피연산자이면
            push(&s, value);
        }
        else {	//연산자이면 피연산자를 스택에서 제거
            op2 = pop(&s);
            op1 = pop(&s);
            switch (ch) { //연산을 수행하고 스택에 저장 
            case '+': push(&s, op1 + op2); break;
            case '-': push(&s, op1 - op2); break;
            case '*': push(&s, op1 * op2); break;
            case '/': push(&s, op1 / op2); break;
            }
        }
    }
    return pop(&s);
}



//공백을 지우는 함수
void remove_spaces(char* source) {
    char* i = source;  // 원본 문자열을 가리키는 포인터
    char* j = source;  // 새로운 문자열을 작성할 포인터
    while (*i != '\0') {
        if (*i != ' ' && *i != '\n') {  // 현재 문자가 공백, 탭, 개행이 아니면
            *j++ = *i;  // j가 가리키는 위치에 i의 값을 복사하고, j를 1 증가시킴
        }
        i++;  // i는 다음 문자로 이동
    }
    *j = '\0';  // 새로운 문자열의 끝에 널 문자 추가
}
void process_script_file(char* filename) {
    FILE* file = fopen(filename, "r");  // 파일을 읽기 모드로 열기
    if (!file) {
        printf("Error: File does not exist: %s\n", filename);
        return;
    }

    char line[256];  // 한 줄씩 읽을 버퍼
    char postfix[MAX_STACK_SIZE];
    
    printf("Executing script from %s:\n", filename);
    while (fgets(line, sizeof(line), file) != NULL) {
        // 파일의 각 줄을 읽고 출력합니다. 실제 실행은 여기서 구현할 수 없습니다.
        printf("%s\n", line);  // 파일 내용 출력
        remove_spaces(line);  // 입력된 수식에서 공백 제거
        

        infix_to_postfix(line, postfix);  // 후위 표기법 변환
        printf("\n");
        int result = eval(postfix);
        printf("계산결과: %d\n", result);
        

    }

    fclose(file);  // 파일 닫기
}

//메인함수
int main() {
    char s[MAX_STACK_SIZE];  // 사용자로부터 입력받을 배열
    Matrix2x2 A, B, result;

    while (1) {
        printf(">> ");
        if (fgets(s, sizeof(s), stdin) == NULL) {
            printf("Invalid input. Please enter again\n");
            continue;
        }

        s[strcspn(s, "\n")] = 0;  // 개행 문자를 NULL 문자로 변환

        if (strcmp(s, "exit") == 0) {
            break;  // 프로그램 종료
        }
        else if (strncmp(s, "script", 6) == 0) {  // 'script' 또는 'script 파일이름'
            char filename[100];  // 파일 이름을 저장할 버퍼
            if (s[6] == ' ') {
                strcpy(filename, s + 7);  // 사용자가 입력한 파일 이름
            }
            else {
                strcpy(filename, "script.m");  // 기본 파일 이름
            }
            process_script_file(filename);
            
            
        }
        else if (strncmp(s, "A =", 3) == 0) {  // 'A ='로 시작하는 경우
            if (!parse_matrix(s + 3, &A)) {
                printf("Error: Failed to parse matrix A.\n");
            }
            else {
                printf("Enter next matrix (B):\n>> ");
                if (fgets(s, sizeof(s), stdin) == NULL) continue;
                s[strcspn(s, "\n")] = 0;
                if (strncmp(s, "B =", 3) == 0 && parse_matrix(s + 3, &B)) {
                    result = add_matrix2x2(&A, &B);
                    print_matrix(&result);
                }
                else {
                    printf("Error: Failed to parse matrix B.\n");
                }
            }
        }
        else if (s[0] == '(') {
            // 수식 처리
            int the_result;
            char postfix[MAX_STACK_SIZE];
            remove_spaces(s);  // 입력된 수식에서 공백 제거

            infix_to_postfix(s, postfix);
            (printf("%s\n", postfix));


            the_result = eval(postfix);
            printf("계산결과값: %d\n", the_result);
            printf("\n");

        }
        else {
            printf("Invalid command. Please enter again.\n");
        }

    }

    return 0;  // 프로그램 종료
}
#endif


#ifdef TEST_MAZE_GEN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAZE_SIZE 6
int maze[MAZE_SIZE][MAZE_SIZE];
int visited[MAZE_SIZE][MAZE_SIZE] = { 0 };


typedef struct {		// 교체!
    short r;
    short c;
} element;
typedef struct {
    element* data;		// data은 포인터로 정의된다. 
    int capacity;		// 현재 크기
    int top;
} StackType;

#define MAX_STACK_SIZE 100
// 스택이 비어있는지 확인하는 함수
int is_empty(StackType* s) {

    return (s->top == -1);
}

// 스택이 가득 찼는지 확인하는 함수
int is_full(StackType* s) {
    return (s->top == (MAX_STACK_SIZE - 1));
}
void push(StackType* s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "Stack is full.\n");
        exit(1);
}
    else s->data[++(s->top)] = item;
}
element pop(StackType* s) {
    if (is_empty(s)) {
        printf("Stack is empty.\n");
        exit(1);
    }
    else return s->data[(s->top)--];
}
// 스택 초기화 함수
void init_stack(StackType* s)
{
    s->top = -1;
    s->data = (element*)malloc(MAX_STACK_SIZE * sizeof(element));
    if (s->data == NULL) {
        printf("Memory allocation failed.\n");
        free(s);
        exit(1);
    }
}

// 스택에 위치를 푸시하는 조건을 검사하는 함수
void push_loc(StackType* s, int r, int c) {
    if (r < 0 || c < 0) return;
    if (maze[r][c] != '1' && !visited[r][c]) {
        element tmp;
        tmp.r = r;
        tmp.c = c;
        push(s, tmp);
    }
}

element here = { 1,0 }, entry = { 1,0 };

// 가능한 이동 방향 (상, 하, 좌, 우)
int dx[4] = { 0, 0, -1, 1 };
int dy[4] = { -1, 1, 0, 0 };

void print_maze() {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (maze[i][j] == 1)
                printf("1 "); // 벽
            else
                printf("0 "); // 길
        }
        printf("\n");
    }
}

void shuffle(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        int r = rand() % size;
        int temp = arr[i];
        arr[i] = arr[r];
        arr[r] = temp;
    }
}

void generate_dfs(int x, int y) {
    int directions[4] = { 0, 1, 2, 3 };
    shuffle(directions, 4); // 방향을 랜덤하게 섞어서 무작위성을 높임

    for (int i = 0; i < 4; i++) {
        int dir = directions[i];
        int nx = x + dx[dir] * 2;
        int ny = y + dy[dir] * 2;

        if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE && maze[nx][ny] == 1) {
            // 현재 칸과 다음 칸 사이의 벽을 제거
            maze[nx][ny] = 0;
            maze[x + dx[dir]][y + dy[dir]] = 0;
            generate_dfs(nx, ny); // 새 위치에서 다시 DFS 수행
        }
    }
}

void generate_maze() {
    // 모든 칸을 벽으로 초기화
    for (int i = 0; i < MAZE_SIZE; i++)
        for (int j = 0; j < MAZE_SIZE; j++)
            maze[i][j] = 1;

    // 시작 위치를 임의로 (1, 1)로 설정하고 여기서부터 탐색 시작
    maze[1][1] = 0;
    generate_dfs(1, 1);

    // 입구와 출구를 강제로 열기
    maze[1][0] = 0;
    maze[4][5] = 0;
}
int main() {
    srand(time(NULL)); // 난수 초기화
    generate_maze();
    print_maze();
    StackType path_stack;
    init_stack(&path_stack); // 스택 초기화
    visited[1][0] = 1;       // 입구를 방문했다고 표시

    
   

    return 0;
}

#endif

#ifdef TEST_MAZE_OUT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAZE_SIZE 6
int maze[MAZE_SIZE][MAZE_SIZE];
int visited[MAZE_SIZE][MAZE_SIZE] = { 0 };


typedef struct {		// 교체!
    short r;
    short c;
} element;
typedef struct {
    element* data;		// data은 포인터로 정의된다. 
    		
    int top;
} StackType;

#define MAX_STACK_SIZE 100
// 스택이 비어있는지 확인하는 함수
int is_empty(StackType* s) {

    return (s->top == -1);
}

// 스택이 가득 찼는지 확인하는 함수
int is_full(StackType* s) {
    return (s->top == (MAX_STACK_SIZE - 1));
}
void push(StackType* s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "Stack is full.\n");
        exit(1);
    }
    else s->data[++(s->top)] = item;
}
element pop(StackType* s) {
    if (is_empty(s)) {
        printf("Stack is empty.\n");
        exit(1);
    }
    else return s->data[(s->top)--];
}
// 스택 초기화 함수
void init_stack(StackType* s)
{
    s->top = -1;
    s->data = (element*)malloc(MAX_STACK_SIZE * sizeof(element));
    if (s->data == NULL) {
        printf("Memory allocation failed.\n");
        free(s);
        exit(1);
    }
}

// 스택에 위치를 푸시하는 조건을 검사하는 함수
void push_loc(StackType* s, int r, int c) {
    if (r < 0 || c < 0) return;
    if (maze[r][c] != '1' && !visited[r][c]) {
        element tmp;
        tmp.r = r;
        tmp.c = c;
        push(s, tmp);
    }
}

element here = { 1,0 }, entry = { 1,0 };

// 가능한 이동 방향 (상, 하, 좌, 우)
int dx[4] = { 0, 0, -1, 1 };
int dy[4] = { -1, 1, 0, 0 };

void print_maze() {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (maze[i][j] == 1)
                printf("1 "); // 벽
            else
                printf("0 "); // 길
        }
        printf("\n");
    }
}

void shuffle(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        int r = rand() % size;
        int temp = arr[i];
        arr[i] = arr[r];
        arr[r] = temp;
    }
}

void generate_dfs(int x, int y) {
    int directions[4] = { 0, 1, 2, 3 };
    shuffle(directions, 4); // 방향을 랜덤하게 섞어서 무작위성을 높임

    for (int i = 0; i < 4; i++) {
        int dir = directions[i];
        int nx = x + dx[dir] * 2;
        int ny = y + dy[dir] * 2;

        if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE && maze[nx][ny] == 1) {
            // 현재 칸과 다음 칸 사이의 벽을 제거
            maze[nx][ny] = 0;
            maze[x + dx[dir]][y + dy[dir]] = 0;
            generate_dfs(nx, ny); // 새 위치에서 다시 DFS 수행
        }
    }
}
void find_exit_path(StackType* stack, int x, int y) {
    // 현재 위치 방문 처리
    visited[x][y] = 1;
    push(stack, element{ (short)x, (short)y });
    maze[x][y] = '.';  // 방문한 위치를 표시

    if (x == 4 && y == 5) {  // 출구에 도달했을 경우
        // 스택에 있는 경로 출력
        for (int i = 0; i <= stack->top; i++) {
            printf("(%d, %d) ", stack->data[i].r, stack->data[i].c);
        }
        printf("\n");
        return;
    }

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i], ny = y + dy[i];
        if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE &&
            maze[nx][ny] == 0 && !visited[nx][ny]) { // 방문하지 않은 길만 탐색
            find_exit_path(stack, nx, ny);
        }
    }

    // 경로 되돌리기 (백트래킹)
    pop(stack);
    visited[x][y] = 0;  // 방문 상태 복구
    maze[x][y] = 0;     // 미로 상태를 이전 상태로 되돌림
}

void generate_maze() {
    // 모든 칸을 벽으로 초기화
    for (int i = 0; i < MAZE_SIZE; i++)
        for (int j = 0; j < MAZE_SIZE; j++)
            maze[i][j] = 1;

    // 시작 위치를 임의로 (1, 1)로 설정하고 여기서부터 탐색 시작
    maze[1][1] = 0;
    generate_dfs(1, 1);

    // 입구와 출구를 강제로 열기
    maze[1][0] = 0;
    maze[4][5] = 0;
}
int main() {
    srand(time(NULL)); // 난수 초기화
    generate_maze();
    print_maze();
    StackType path_stack;
    init_stack(&path_stack); // 스택 초기화
    visited[1][0] = 1;       // 입구를 방문했다고 표시

    printf("Finding path from entrance to exit:\n");
    find_exit_path(&path_stack, 1, 0); // 경로 찾기

    return 0;
    
}
#endif

