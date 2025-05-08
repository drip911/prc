#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

void parallelBFS(const vector<vector<int>>& graph, int start) {
    int n = graph.size();
    vector<bool> visited(n, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "BFS Traversal: ";

    while (!q.empty()) {
        int levelSize = q.size();

        // Process all nodes at the current level in parallel
        vector<int> nextLevel;

        #pragma omp parallel for shared(q, visited) default(none) firstprivate(levelSize) schedule(dynamic)
        for (int i = 0; i < levelSize; ++i) {
            int u;

            #pragma omp critical
            {
                if (!q.empty()) {
                    u = q.front();
                    q.pop();
                    cout << u << " ";
                }
            }

            // Traverse neighbors
            for (int v : graph[u]) {
                if (!visited[v]) {
                    #pragma omp critical
                    {
                        if (!visited[v]) {
                            visited[v] = true;
                            nextLevel.push_back(v);
                        }
                    }
                }
            }
        }

        // Add all next level nodes to the queue
        for (int v : nextLevel) {
            q.push(v);
        }
    }

    cout << endl;
}

int main() {
    // Example graph as an adjacency list
    vector<vector<int>> graph = {
        {1, 2},    // Neighbors of node 0
        {0, 3, 4}, // Neighbors of node 1
        {0, 4},    // Neighbors of node 2
        {1, 5},    // Neighbors of node 3
        {1, 2, 5}, // Neighbors of node 4
        {3, 4}     // Neighbors of node 5
    };

    int startNode = 0;

    parallelBFS(graph, startNode);

    return 0;
}
