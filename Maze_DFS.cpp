#define Graph_adj
#define DFS_BFS_C++
#define Maze_DFS

#ifdef Graph_adj
#include <iostream>

class Node {
public:
    int vertex;      // ������ ��Ÿ���� ������
    Node* next;      // ���� ��带 ����Ű�� ������

    // ������
    Node(int v) : vertex(v), next(nullptr) {}
};

class Graph {
private:
    int V;          // ������ ��
    Node** adjList; // ���� ����Ʈ (������ �迭)

public:
    // ������
    Graph(int vertices) : V(vertices) {
        adjList = new Node * [V]; // ���� ����ŭ ������ �迭 �Ҵ�
        for (int i = 0; i < V; ++i) {
            adjList[i] = nullptr; // �� �����͸� nullptr�� �ʱ�ȭ
        }
    }

    // ���� �߰� �Լ�
    void addEdge(int start, int end) {
        Node* newNode = new Node(end);  // �� ��� ����
        newNode->next = adjList[start]; // �� ����� next�� ������ ù ���� ����
        adjList[start] = newNode;       // ����Ʈ�� ��带 �� ���� ������Ʈ
    }

    // �׷��� ��� �Լ�
    void printGraph() {
        for (int i = 0; i < V; ++i) {
            Node* temp = adjList[i];
            std::cout << "Vertex " << i << ": ";
            while (temp != nullptr) {
                std::cout << temp->vertex << " -> ";
                temp = temp->next;
            }
            std::cout << "NULL\n";
        }
    }

    // �Ҹ���
    ~Graph() {
        for (int i = 0; i < V; ++i) {
            Node* temp = adjList[i];
            while (temp != nullptr) {
                Node* next = temp->next;
                delete temp;
                temp = next;
            }
        }
        delete[] adjList; // ���� �Ҵ�� �迭 �޸� ����
    }
};

int main() {
    int V = 4; // ������ ��
    Graph g(V);  // �׷��� ��ü ����
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    g.printGraph();  // �׷��� ���

    return 0;
}
#endif



#ifdef Maze_DFS
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