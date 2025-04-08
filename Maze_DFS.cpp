#define Graph_adj
#define DFS_BFS_C++
#define Maze_DFS

#ifdef Graph_adj
#include <iostream>

class Node {
public:
    int vertex;      // 정점을 나타내는 데이터
    Node* next;      // 다음 노드를 가리키는 포인터

    // 생성자
    Node(int v) : vertex(v), next(nullptr) {}
};

class Graph {
private:
    int V;          // 정점의 수
    Node** adjList; // 인접 리스트 (포인터 배열)

public:
    // 생성자
    Graph(int vertices) : V(vertices) {
        adjList = new Node * [V]; // 정점 수만큼 포인터 배열 할당
        for (int i = 0; i < V; ++i) {
            adjList[i] = nullptr; // 각 포인터를 nullptr로 초기화
        }
    }

    // 간선 추가 함수
    void addEdge(int start, int end) {
        Node* newNode = new Node(end);  // 새 노드 생성
        newNode->next = adjList[start]; // 새 노드의 next를 현재의 첫 노드로 설정
        adjList[start] = newNode;       // 리스트의 헤드를 새 노드로 업데이트
    }

    // 그래프 출력 함수
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

    // 소멸자
    ~Graph() {
        for (int i = 0; i < V; ++i) {
            Node* temp = adjList[i];
            while (temp != nullptr) {
                Node* next = temp->next;
                delete temp;
                temp = next;
            }
        }
        delete[] adjList; // 동적 할당된 배열 메모리 해제
    }
};

int main() {
    int V = 4; // 정점의 수
    Graph g(V);  // 그래프 객체 생성
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    g.printGraph();  // 그래프 출력

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