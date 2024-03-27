#include <iostream>
#include <vector>
#include <algorithm>
#include <windows.h>

using namespace std;
#define MAX_SIZE 100 //迷宫大小

struct Graph {
    int g[MAX_SIZE][MAX_SIZE];
    int height, width;

    Graph(int h, int w) : height(h), width(w) {
        // 初始化g为全0
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                g[i][j] = 0;
            }
        }
    }
};

struct Node {
    int x, y;
    Node* parent;
    int g, h, f;

    Node(int x, int y) : x(x), y(y), parent(nullptr), g(0), f(0), h(0) {}
};

int heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

Node* findLowestNode(vector<Node*> openList) {
    Node* lowestNode = openList[0];
    for (Node* node : openList) {
        if (node->f < lowestNode->f) {
            lowestNode = node;
        }
    }

    return lowestNode;
}

vector<Node*> getNeighbors(Graph* graph, Node* currentNode, int endX, int endY) {
    vector<Node*> neighbors;
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; i++) {
        int newX = currentNode->x + dx[i];
        int newY = currentNode->y + dy[i];

        if (newX >= 0 && newX < graph->width && newY >= 0 && newY < graph->height && graph->g[newY][newX] == 0) {
            Node* neighbor = new Node(newX, newY);
            neighbor->parent = currentNode;
            neighbor->g = currentNode->g + 1;
            neighbor->h = heuristic(newX, newY, endX, endY);
            neighbor->f = neighbor->g + neighbor->h;
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
}

vector<Node*> Astar(Graph* graph, int startX, int startY, int endX, int endY) {
    vector<Node*> openList;
    vector<Node*> closeList;

    Node* startNode = new Node(startX, startY);
    openList.push_back(startNode);

    while (!openList.empty()) {
        Node* currentNode = findLowestNode(openList);
        openList.erase(remove(openList.begin(), openList.end(), currentNode), openList.end());
        closeList.push_back(currentNode);

        if (currentNode->x == endX && currentNode->y == endY) { //找到终点，退出A*
            vector<Node*> path;
            while (currentNode != nullptr) {
                path.push_back(currentNode);
                currentNode = currentNode->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        //开始判断周围节点
        vector<Node*> neighbors = getNeighbors(graph, currentNode, endX, endY);

        for (Node* neighbor : neighbors) {
            bool inCloseList = false;
            for (Node* node : closeList) {
                if (node->x == neighbor->x && node->y == neighbor->y) {
                    inCloseList = true;
                    break;
                }
            }
            if (inCloseList) {
                continue;
            }

            Node* it = nullptr;
            for (Node* node : openList) {
                if (node->x == neighbor->x && node->y == neighbor->y) {
                    it = node;
                    break;
                }
            }

            if (it == nullptr) {
                openList.push_back(neighbor);
            }
            else {
                if (neighbor->g < it->g) {
                    it->parent = currentNode;
                    it->g = neighbor->g;
                    it->f = neighbor->g + neighbor->h;
                }
                delete neighbor;
            }
        }
    }

    return vector<Node*>();
}

Graph* createGraph(int h, int w) {
    return new Graph(h, w);
}

void printGraph(Graph* g) {
    for (int i = 0; i < g->width + 2; i++) {
        cout << "■";
    }
    cout << endl;
    for (int i = 0; i < g->height; i++) {
        cout << "■";
        for (int j = 0; j < g->width; j++) {
            switch (g->g[i][j]) {
            case 0:
                cout << "  ";
                break;
            case 1:
                cout << "■";
                break;
            case 2:
                cout << "○";
                break;
            }
        }
        cout << "■" << endl;
    }
    for (int i = 0; i < g->width + 2; i++) {
        cout << "■";
    }
    cout << endl;
}

void setBlock(Graph* g, int x1, int y1, int x2, int y2) { //设置阻碍
    int temp;
    if (x1 > x2) {
        temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y1 > y2) {
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    
    for (int i = 0; i < y2 - y1 + 1; i++) {
        for (int j = 0; j < x2 - x1 + 1; j++) {
            g->g[y1 + i - 1][x1 + j - 1] = 1;
        }
    }
}

void printPath(Graph* graph, vector<Node*> path) {
    for (Node* node : path) {
        graph->g[node->y][node->x] = 2;  // 用 2 标记路径
        system("CLS");
        printGraph(graph);
        Sleep(50);
    }
    for (Node* node : path) {
        graph->g[node->y][node->x] = 0;  // 删除路径
    }
}

int main()
{
    Graph* graph = createGraph(15, 23);
    setBlock(graph, 4, 1, 2, 7);
    printGraph(graph);
    printPath(graph, Astar(graph, 0, 0, graph->width - 1, graph->height - 1));

    setBlock(graph, 6, 2, 8, 14);
    printGraph(graph);
    printPath(graph, Astar(graph, 0, 0, graph->width - 1, graph->height - 1));

    setBlock(graph, 8, 14, 15, 14);
    printGraph(graph);
    printPath(graph, Astar(graph, 0, 0, graph->width - 1, graph->height - 1));

    setBlock(graph, 17, 9, 17, 15);
    printGraph(graph);
    printPath(graph, Astar(graph, 0, 0, graph->width - 1, graph->height - 1));

    setBlock(graph, 19, 2, 19, 14);
    setBlock(graph, 21, 2, 21, 15);
    printGraph(graph);
    printPath(graph, Astar(graph, 0, 0, graph->width - 1, graph->height - 1));

    system("pause");
    return 0;
}

