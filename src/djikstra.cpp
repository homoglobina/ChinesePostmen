#include <queue>
#include <vector>
#include <limits>
#include "graph.h"
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;
int INT_MAX = 2147483647;

// Implement Dijkstra's algorithm to compute shortest path from source `start`
vector<int> Graph::dijkstra(int start) {
    vector<int> dist(vertices, numeric_limits<int>::max());
    dist[start] = 0;
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq; 
    pq.push({0, start});
    
    while (!pq.empty()) {
        int u = pq.top().second;
        int cost = pq.top().first;
        pq.pop();
        
        if (cost > dist[u]) continue;
        
        for (int v = 0; v < vertices; ++v) {
            if (adjMatrix[u][v] > 0 && dist[u] + adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + adjMatrix[u][v];
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

std::vector<int> dijkstra2(int start, int end, const std::vector<std::vector<int>>& adjMatrix, int vertices) {
    std::vector<int> distances(vertices, std::numeric_limits<int>::max());
    std::vector<int> prev(vertices, -1);
    distances[start] = 0;

    using Pair = std::pair<int, int>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [distance, u] = pq.top();
        pq.pop();

        if (distance > distances[u]) continue;

        for (int v = 0; v < vertices; ++v) {
            if (adjMatrix[u][v] > 0 && distances[u] + adjMatrix[u][v] < distances[v]) {
                distances[v] = distances[u] + adjMatrix[u][v];
                pq.push({distances[v], v});
                prev[v] = u;
            }
        }
    }

    // Reconstruct the path
    std::vector<int> path;
    for (int at = end; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    return path;
}
