#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>
using namespace std;

const int V = 6; // Number of vertices

// Find the vertex with minimum distance value not yet included in shortest path
int minDistance(const vector<int>& dist, const vector<bool>& visited) {
    int min = INT_MAX, min_index = -1;

    #pragma omp parallel for
    for (int v = 0; v < V; v++) {
        if (!visited[v] && dist[v] < min) {
            #pragma omp critical
            {
                if (dist[v] < min) {
                    min = dist[v];
                    min_index = v;
                }
            }
        }
    }

    return min_index;
}

// Dijkstra algorithm
void dijkstra(int graph[V][V], int src) {
    vector<int> dist(V, INT_MAX); // Distance from source to each vertex
    vector<bool> visited(V, false); // Visited vertices

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, visited);
        visited[u] = true;

        // Update dist value of adjacent vertices of u
        #pragma omp parallel for
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Print the result
    cout << "Vertex\tDistance from Source\n";
    for (int i = 0; i < V; i++)
        cout << i << "\t" << dist[i] << "\n";
}

int main() {
    int graph[V][V] = {
        {0, 2, 0, 1, 0, 0},
        {2, 0, 4, 0, 3, 0},
        {0, 4, 0, 5, 1, 0},
        {1, 0, 5, 0, 2, 8},
        {0, 3, 1, 2, 0, 6},
        {0, 0, 0, 8, 6, 0}
    };
    cout << endl;
    dijkstra(graph, 0);

    return 0;
}
