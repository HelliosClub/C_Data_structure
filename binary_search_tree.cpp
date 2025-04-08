//#define binary_search_tree_in_studentRecord
//#define equation_tree

#ifdef binary_search_tree_in_studentRecord
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct TreeNode {
    char name[6];
    int num;
    int score;
    struct TreeNode* left, * right;
} TreeNode;

typedef struct HeapNode {
    char name[6];
    int num;
    int score;
} HeapNode;

TreeNode* createTreeNode(char* name, int num, int score) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(newNode->name, name);
    newNode->num = num;
    newNode->score = score;
    newNode->left = newNode->right = NULL;
    return newNode;
}

TreeNode* insertTreeNode(TreeNode* node, char* name, int num, int score) {
    if (node == NULL) {
        return createTreeNode(name, num, score);
    }
    if (strcmp(name, node->name) < 0) {
        node->left = insertTreeNode(node->left, name, num, score);
    }
    else if (strcmp(name, node->name) > 0) {
        node->right = insertTreeNode(node->right, name, num, score);
    }
    return node;
}

void printRecord(TreeNode* node, char* name) {
    if (node == NULL) {
        printf("No record found for %s.\n", name);
        return;
    }
    if (strcmp(name, node->name) == 0) {
        printf("Found: Name: %s, Number: %d, Score: %d\n", node->name, node->num, node->score);
    }
    else if (strcmp(name, node->name) < 0) {
        printRecord(node->left, name);
    }
    else {
        printRecord(node->right, name);
    }
}

void printInOrder(TreeNode* node) {
    if (node != NULL) {
        printInOrder(node->left);
        printf("Name: %s, Number: %d, Score: %d\n", node->name, node->num, node->score);
        printInOrder(node->right);
    }
}

void maxHeapify(HeapNode arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].score > arr[largest].score)
        largest = left;

    if (right < n && arr[right].score > arr[largest].score)
        largest = right;

    if (largest != i) {
        HeapNode temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        maxHeapify(arr, n, largest);
    }
}

void buildMaxHeap(HeapNode arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        maxHeapify(arr, n, i);
}

void printHeap(HeapNode arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("Name: %s, Number: %d, Score: %d\n", arr[i].name, arr[i].num, arr[i].score);
}

void generateRandomName(char* name) {
    for (int i = 0; i < 5; i++) {
        name[i] = 'A' + rand() % 26;
    }
    name[5] = '\0';
}

int main() {
    srand(time(NULL));
    TreeNode* root = NULL;
    HeapNode heap[10];
    char input[20];
    int num = 1;

    for (int i = 0; i < 10; i++) {
        char name[6];
        generateRandomName(name);
        int score = rand() % 101;
        root = insertTreeNode(root, name, num, score);
        strcpy(heap[i].name, name);
        heap[i].num = num;
        heap[i].score = score;
        num++;
    }
    while (1) {
        printf("Enter 'BST' to sort by name, 'heap' to sort by score, or a name to search: ");
        scanf("%s", input);

        if (strcmp(input, "BST") == 0) {
            printInOrder(root);
        }
        else if (strcmp(input, "heap") == 0) {
            buildMaxHeap(heap, 10);
            printHeap(heap, 10);
        }
        else {
            printRecord(root, input);
        }

        return 0;
    }
}


#endif

#ifdef equation_tree
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char value;
    struct Node* left;
    struct Node* right;
} Node;

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

Node* createNode(char value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

void push(Node** stack, int* top, Node* node) {
    stack[++(*top)] = node;
}

Node* pop(Node** stack, int* top) {
    if (*top == -1) return NULL;
    return stack[(*top)--];
}

// 중위 표현식을 후위 표현식으로 변환
void infixToPostfix(char* exp, char* output) {
    Node* stack[100];
    int top = -1, index = 0;

    for (int i = 0; exp[i]; ++i) {
        char c = exp[i];

        // 숫자면 바로 출력
        if (isalnum(c)) {
            output[index++] = c;
        }
        else if (c == '(') {
            push(stack, &top, createNode(c));
        }
        else if (c == ')') {
            while (stack[top]->value != '(') {
                output[index++] = pop(stack, &top)->value;
            }
            pop(stack, &top);  // '(' 제거
        }
        else {  // 연산자인 경우
            while (top != -1 && precedence(stack[top]->value) >= precedence(c)) {
                output[index++] = pop(stack, &top)->value;
            }
            push(stack, &top, createNode(c));
        }
    }

    while (top != -1) {  // 스택에 남은 연산자 출력
        output[index++] = pop(stack, &top)->value;
    }
    output[index] = '\0';
}

// 후위 표현식으로부터 트리 구축
Node* constructExpressionTree(char* postfix) {
    Node* stack[100];
    int top = -1;

    for (int i = 0; postfix[i]; ++i) {
        char c = postfix[i];

        if (isalnum(c)) {
            push(stack, &top, createNode(c));
        }
        else {
            Node* node = createNode(c);
            node->right = pop(stack, &top);
            node->left = pop(stack, &top);
            push(stack, &top, node);
        }
    }
    return pop(stack, &top);
}

void printInOrder(Node* root) {
    if (root != NULL) {
        if (!isalnum(root->value)) printf("(");
        printInOrder(root->left);
        printf("%c", root->value);
        printInOrder(root->right);
        if (!isalnum(root->value)) printf(")");
    }
}

int main() {
    char infix[] = "(2+3*(4+2))";
    char postfix[100];

    infixToPostfix(infix, postfix);
    Node* root = constructExpressionTree(postfix);

    printf("Infix: %s\n", infix);
    printf("Postfix: %s\n", postfix);
    printf("Expression Tree (In-order): ");
    printInOrder(root);
    printf("\n");

    return 0;
}

#endif