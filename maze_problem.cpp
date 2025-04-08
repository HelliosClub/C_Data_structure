
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
    element* data;		// data�� ����ü �ȿ� ���ǵ� ����
    int top;

};
//��� ����ü ����
typedef struct Matrix2x2 {
    int m11, m12; // ù ��° ���� ���
    int m21, m22; // �� ��° ���� ���
};

//����� ����
Matrix2x2 add_matrix2x2(Matrix2x2* a, Matrix2x2* b) {
    Matrix2x2 result;
    result.m11 = a->m11 + b->m11;
    result.m12 = a->m12 + b->m12;
    result.m21 = a->m21 + b->m21;
    result.m22 = a->m22 + b->m22;
    return result;
}

//����� ����
Matrix2x2 sub_matrix2x2(Matrix2x2* a, Matrix2x2* b) {
    Matrix2x2 result;
    result.m11 = a->m11 - b->m11;
    result.m12 = a->m12 - b->m12;
    result.m21 = a->m21 - b->m21;
    result.m22 = a->m22 - b->m22;
    return result;
}

//����� ����
Matrix2x2 multiply_matrix2x2(Matrix2x2* a, Matrix2x2* b) {
    Matrix2x2 result;
    result.m11 = a->m11 * b->m11 + a->m12 * b->m21;
    result.m12 = a->m11 * b->m12 + a->m12 * b->m22;
    result.m21 = a->m21 * b->m11 + a->m22 * b->m21;
    result.m22 = a->m21 * b->m12 + a->m22 * b->m22;
    return result;
}
//�Էµ� �Լ��� �Ľ�
int parse_matrix(char* input, Matrix2x2* matrix) {

    char* format = "[%d %d ; %d %d]";

    // ���ʿ��� ����� ���� ����
    char cleaned_input[100];  // ����� ũ���� �迭 ����
    int j = 0;
    for (int i = 0; input[i] != '\0'; ++i) {
        if (isdigit(input[i]) || input[i] == '[' || input[i] == ']' || input[i] == ';' || input[i] == ' ') {
            cleaned_input[j++] = input[i];
        }
    }
    cleaned_input[j] = '\0';  // ���ڿ� ���� NULL �߰�

    // sscanf�� ����Ͽ� �Է� ���ڿ����� ��� ������ ����
    int parsed = sscanf(cleaned_input, format, &matrix->m11, &matrix->m12, &matrix->m21, &matrix->m22);

    // ���������� 4���� ���ڸ� �о����� Ȯ��
    return parsed == 4;
}
//�Լ��� ���
void print_matrix(Matrix2x2* matrix) {
    printf("[%d %d]\n[%d %d]\n", matrix->m11, matrix->m12, matrix->m21, matrix->m22);
}

//s�� ����ü�� ����Ű�� ����ü ������
//s->data = �� s�� ����Ű�� ����ü�� data�� ���� �Ҵ�
// ���� �ʱ�ȭ �Լ�
void init_stack(StackType* s)
{
    s->top = -1;
    s->data = (element*)malloc(MAX_STACK_SIZE * sizeof(element));
    if (s->data == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
}
// ������ ����ִ��� Ȯ���ϴ� �Լ�
int is_empty(StackType* s) {

    return (s->top == -1);
}

// ������ ���� á���� Ȯ���ϴ� �Լ�
int is_full(StackType* s) {
    return (s->top == (MAX_STACK_SIZE - 1));
}

// ���ÿ� ��Ҹ� �߰��ϴ� �Լ�
void push(StackType* s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "Stack is full.\n");
        exit(1);
    }
    else s->data[++(s->top)] = item;
}

// ���ÿ��� ��Ҹ� �����ϴ� �Լ�
element pop(StackType* s) {
    if (is_empty(s)) {
        printf("Stack is empty.\n");
        exit(1);
    }
    else return s->data[(s->top)--];
}

// ���� �޸� ���� �Լ�
void destroy_stack(StackType* s) {
    free(s->data);
    s->top = -1;
}
//�����ڵ��� �켱���� ����
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
//������ ������ ����ϴ� �Լ�
element peek(StackType* s) {
    if (is_empty(s)) {
        printf("Stack is empty.\n");
        exit(1);
    }
    return s->data[s->top];
}

//����ǥ����� ����ǥ������� �ٲٴ� �Լ�
void infix_to_postfix(char* exp, char* postfix) {
    int i = 0, j = 0;  // �ε��� ���� �ʱ�ȭ
    char ch, e;        // ���� ���ڿ� ���ÿ��� �˵� ���ڸ� ������ ����
    StackType s;       // ���� ����

    init_stack(&s);    // ���� �ʱ�ȭ

    // exp�� �� ���ڸ� ��ȸ
    while (exp[i] != '\0') {
        ch = exp[i];
        switch (ch) {
        case '+':
        case '-':
        case '*':
        case '/':
            // ������ ������� �ʰ� ������ �� �� ����� �켱������ ch�� �켱�������� ũ�ų� ������
            while (!is_empty(&s) && prec(peek(&s)) >= prec(ch)) {
                e = pop(&s);          // ���ÿ��� ��
                postfix[j++] = e;     // �˵� �����ڸ� postfix�� �߰�
            }
            push(&s, ch);             // ���� �����ڸ� ���ÿ� Ǫ��
            break;
        case '(':
            push(&s, ch);             // ���� ��ȣ�� ���ÿ� Ǫ��
            break;
        case ')':
            e = pop(&s);              // ���ÿ��� ��
            // �˵� ��Ұ� ���� ��ȣ�� �ƴ� ������ �ݺ�
            while (e != '(') {
                postfix[j++] = e;     // �˵� �����ڸ� postfix�� �߰�
                e = pop(&s);          // ���� ��Ҹ� ��
            }
            break;
        default:
            if (isdigit(ch)) {
                postfix[j++] = ch;    // �ǿ����ڸ� postfix�� �߰�
            }
            break;
        }
        i++;  // ���� ���ڷ� �̵�
    }

    // ���ÿ� ���� ��� �����ڸ� ���Ͽ� postfix�� �߰�
    while (!is_empty(&s)) {
        e = pop(&s);
        postfix[j++] = e;
    }

    postfix[j] = '\0';  // ���ڿ� ����
    destroy_stack(&s);  // ���� �޸� ����
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
            value = ch - '0';	// �Է��� �ǿ������̸�
            push(&s, value);
        }
        else {	//�������̸� �ǿ����ڸ� ���ÿ��� ����
            op2 = pop(&s);
            op1 = pop(&s);
            switch (ch) { //������ �����ϰ� ���ÿ� ���� 
            case '+': push(&s, op1 + op2); break;
            case '-': push(&s, op1 - op2); break;
            case '*': push(&s, op1 * op2); break;
            case '/': push(&s, op1 / op2); break;
            }
        }
    }
    return pop(&s);
}



//������ ����� �Լ�
void remove_spaces(char* source) {
    char* i = source;  // ���� ���ڿ��� ����Ű�� ������
    char* j = source;  // ���ο� ���ڿ��� �ۼ��� ������
    while (*i != '\0') {
        if (*i != ' ' && *i != '\n') {  // ���� ���ڰ� ����, ��, ������ �ƴϸ�
            *j++ = *i;  // j�� ����Ű�� ��ġ�� i�� ���� �����ϰ�, j�� 1 ������Ŵ
        }
        i++;  // i�� ���� ���ڷ� �̵�
    }
    *j = '\0';  // ���ο� ���ڿ��� ���� �� ���� �߰�
}
void process_script_file(char* filename) {
    FILE* file = fopen(filename, "r");  // ������ �б� ���� ����
    if (!file) {
        printf("Error: File does not exist: %s\n", filename);
        return;
    }

    char line[256];  // �� �پ� ���� ����
    char postfix[MAX_STACK_SIZE];
    
    printf("Executing script from %s:\n", filename);
    while (fgets(line, sizeof(line), file) != NULL) {
        // ������ �� ���� �а� ����մϴ�. ���� ������ ���⼭ ������ �� �����ϴ�.
        printf("%s\n", line);  // ���� ���� ���
        remove_spaces(line);  // �Էµ� ���Ŀ��� ���� ����
        

        infix_to_postfix(line, postfix);  // ���� ǥ��� ��ȯ
        printf("\n");
        int result = eval(postfix);
        printf("�����: %d\n", result);
        

    }

    fclose(file);  // ���� �ݱ�
}

//�����Լ�
int main() {
    char s[MAX_STACK_SIZE];  // ����ڷκ��� �Է¹��� �迭
    Matrix2x2 A, B, result;

    while (1) {
        printf(">> ");
        if (fgets(s, sizeof(s), stdin) == NULL) {
            printf("Invalid input. Please enter again\n");
            continue;
        }

        s[strcspn(s, "\n")] = 0;  // ���� ���ڸ� NULL ���ڷ� ��ȯ

        if (strcmp(s, "exit") == 0) {
            break;  // ���α׷� ����
        }
        else if (strncmp(s, "script", 6) == 0) {  // 'script' �Ǵ� 'script �����̸�'
            char filename[100];  // ���� �̸��� ������ ����
            if (s[6] == ' ') {
                strcpy(filename, s + 7);  // ����ڰ� �Է��� ���� �̸�
            }
            else {
                strcpy(filename, "script.m");  // �⺻ ���� �̸�
            }
            process_script_file(filename);
            
            
        }
        else if (strncmp(s, "A =", 3) == 0) {  // 'A ='�� �����ϴ� ���
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
            // ���� ó��
            int the_result;
            char postfix[MAX_STACK_SIZE];
            remove_spaces(s);  // �Էµ� ���Ŀ��� ���� ����

            infix_to_postfix(s, postfix);
            (printf("%s\n", postfix));


            the_result = eval(postfix);
            printf("�������: %d\n", the_result);
            printf("\n");

        }
        else {
            printf("Invalid command. Please enter again.\n");
        }

    }

    return 0;  // ���α׷� ����
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


typedef struct {		// ��ü!
    short r;
    short c;
} element;
typedef struct {
    element* data;		// data�� �����ͷ� ���ǵȴ�. 
    int capacity;		// ���� ũ��
    int top;
} StackType;

#define MAX_STACK_SIZE 100
// ������ ����ִ��� Ȯ���ϴ� �Լ�
int is_empty(StackType* s) {

    return (s->top == -1);
}

// ������ ���� á���� Ȯ���ϴ� �Լ�
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
// ���� �ʱ�ȭ �Լ�
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

// ���ÿ� ��ġ�� Ǫ���ϴ� ������ �˻��ϴ� �Լ�
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

// ������ �̵� ���� (��, ��, ��, ��)
int dx[4] = { 0, 0, -1, 1 };
int dy[4] = { -1, 1, 0, 0 };

void print_maze() {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (maze[i][j] == 1)
                printf("1 "); // ��
            else
                printf("0 "); // ��
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
    shuffle(directions, 4); // ������ �����ϰ� ��� ���������� ����

    for (int i = 0; i < 4; i++) {
        int dir = directions[i];
        int nx = x + dx[dir] * 2;
        int ny = y + dy[dir] * 2;

        if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE && maze[nx][ny] == 1) {
            // ���� ĭ�� ���� ĭ ������ ���� ����
            maze[nx][ny] = 0;
            maze[x + dx[dir]][y + dy[dir]] = 0;
            generate_dfs(nx, ny); // �� ��ġ���� �ٽ� DFS ����
        }
    }
}

void generate_maze() {
    // ��� ĭ�� ������ �ʱ�ȭ
    for (int i = 0; i < MAZE_SIZE; i++)
        for (int j = 0; j < MAZE_SIZE; j++)
            maze[i][j] = 1;

    // ���� ��ġ�� ���Ƿ� (1, 1)�� �����ϰ� ���⼭���� Ž�� ����
    maze[1][1] = 0;
    generate_dfs(1, 1);

    // �Ա��� �ⱸ�� ������ ����
    maze[1][0] = 0;
    maze[4][5] = 0;
}
int main() {
    srand(time(NULL)); // ���� �ʱ�ȭ
    generate_maze();
    print_maze();
    StackType path_stack;
    init_stack(&path_stack); // ���� �ʱ�ȭ
    visited[1][0] = 1;       // �Ա��� �湮�ߴٰ� ǥ��

    
   

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


typedef struct {		// ��ü!
    short r;
    short c;
} element;
typedef struct {
    element* data;		// data�� �����ͷ� ���ǵȴ�. 
    		
    int top;
} StackType;

#define MAX_STACK_SIZE 100
// ������ ����ִ��� Ȯ���ϴ� �Լ�
int is_empty(StackType* s) {

    return (s->top == -1);
}

// ������ ���� á���� Ȯ���ϴ� �Լ�
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
// ���� �ʱ�ȭ �Լ�
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

// ���ÿ� ��ġ�� Ǫ���ϴ� ������ �˻��ϴ� �Լ�
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

// ������ �̵� ���� (��, ��, ��, ��)
int dx[4] = { 0, 0, -1, 1 };
int dy[4] = { -1, 1, 0, 0 };

void print_maze() {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (maze[i][j] == 1)
                printf("1 "); // ��
            else
                printf("0 "); // ��
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
    shuffle(directions, 4); // ������ �����ϰ� ��� ���������� ����

    for (int i = 0; i < 4; i++) {
        int dir = directions[i];
        int nx = x + dx[dir] * 2;
        int ny = y + dy[dir] * 2;

        if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE && maze[nx][ny] == 1) {
            // ���� ĭ�� ���� ĭ ������ ���� ����
            maze[nx][ny] = 0;
            maze[x + dx[dir]][y + dy[dir]] = 0;
            generate_dfs(nx, ny); // �� ��ġ���� �ٽ� DFS ����
        }
    }
}
void find_exit_path(StackType* stack, int x, int y) {
    // ���� ��ġ �湮 ó��
    visited[x][y] = 1;
    push(stack, element{ (short)x, (short)y });
    maze[x][y] = '.';  // �湮�� ��ġ�� ǥ��

    if (x == 4 && y == 5) {  // �ⱸ�� �������� ���
        // ���ÿ� �ִ� ��� ���
        for (int i = 0; i <= stack->top; i++) {
            printf("(%d, %d) ", stack->data[i].r, stack->data[i].c);
        }
        printf("\n");
        return;
    }

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i], ny = y + dy[i];
        if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE &&
            maze[nx][ny] == 0 && !visited[nx][ny]) { // �湮���� ���� �游 Ž��
            find_exit_path(stack, nx, ny);
        }
    }

    // ��� �ǵ����� (��Ʈ��ŷ)
    pop(stack);
    visited[x][y] = 0;  // �湮 ���� ����
    maze[x][y] = 0;     // �̷� ���¸� ���� ���·� �ǵ���
}

void generate_maze() {
    // ��� ĭ�� ������ �ʱ�ȭ
    for (int i = 0; i < MAZE_SIZE; i++)
        for (int j = 0; j < MAZE_SIZE; j++)
            maze[i][j] = 1;

    // ���� ��ġ�� ���Ƿ� (1, 1)�� �����ϰ� ���⼭���� Ž�� ����
    maze[1][1] = 0;
    generate_dfs(1, 1);

    // �Ա��� �ⱸ�� ������ ����
    maze[1][0] = 0;
    maze[4][5] = 0;
}
int main() {
    srand(time(NULL)); // ���� �ʱ�ȭ
    generate_maze();
    print_maze();
    StackType path_stack;
    init_stack(&path_stack); // ���� �ʱ�ȭ
    visited[1][0] = 1;       // �Ա��� �湮�ߴٰ� ǥ��

    printf("Finding path from entrance to exit:\n");
    find_exit_path(&path_stack, 1, 0); // ��� ã��

    return 0;
    
}
#endif

