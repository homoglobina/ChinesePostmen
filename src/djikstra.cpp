#include <queue>
#include <vector>
#include <limits>
#include "graph.h"
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;
/**
 * @brief Computes the shortest paths from a starting vertex to all other vertices in the graph using Dijkstra's algorithm.
 * 
 * This function uses a priority queue to efficiently find the shortest path from the starting vertex to all other vertices.
 * The graph is represented by an adjacency matrix, where adjMatrix[u][v] holds the weight of the edge from vertex u to vertex v.
 * 
 * @param start The starting vertex for Dijkstra's algorithm.
 * @return A vector of integers where the value at each index represents the shortest distance from the starting vertex to that vertex.
 */
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

/**
 * @brief Computes the shortest path from a start vertex to an end vertex using Dijkstra's algorithm.
 *
 * This function uses Dijkstra's algorithm to find the shortest path from a given start vertex to a specified end vertex
 * in a graph represented by an adjacency matrix. It returns the path as a vector of vertex indices.
 *
 * @param start The starting vertex.
 * @param end The ending vertex.
 * @param adjMatrix The adjacency matrix representing the graph. adjMatrix[u][v] holds the weight of the edge from vertex u to vertex v.
 * @param vertices The number of vertices in the graph.
 * @return A vector of integers representing the shortest path from the start vertex to the end vertex. If no path exists, the vector will contain only the end vertex.
 */
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
    std::vector<int> path;
    for (int at = end; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

/**
 * @brief Computes the shortest path tree using Dijkstra's algorithm.
 *
 * This function uses Dijkstra's algorithm to compute the shortest path tree from a given start vertex
 * to all other vertices in a graph represented by an adjacency matrix.
 *
 * @param start The starting vertex for the shortest path tree.
 * @param adjMatrix The adjacency matrix representing the graph. adjMatrix[u][v] should be the weight of the edge from vertex u to vertex v.
 * @param vertices The number of vertices in the graph.
 * @return A vector of parent vertices where the index represents the vertex and the value at that index represents the parent vertex in the shortest path tree.
 */
std::vector<int> dijkstra3(int start, const std::vector<std::vector<int>>& adjMatrix, int vertices) {
    std::vector<int> dist(vertices, std::numeric_limits<int>::max());
    std::vector<int> parent(vertices, -1);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [cost, u] = pq.top();
        pq.pop();

        if (cost > dist[u]) continue;

        for (int v = 0; v < vertices; v++) {
            if (adjMatrix[u][v] > 0 && dist[u] + adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + adjMatrix[u][v];
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    return parent; 
}

/**
 * @brief Reconstructs the path from the start node to the end node using the parent vector.
 * 
 * This function takes the start and end nodes along with a parent vector and reconstructs
 * the path from the start node to the end node. The parent vector contains the parent of
 * each node in the path.
 * 
 * @param start The starting node of the path.
 * @param end The ending node of the path.
 * @param parent A vector where each index represents a node and the value at that index
 *               represents the parent of that node in the path.
 * @return A vector of pairs representing the path from the start node to the end node.
 *         Each pair contains two integers: the parent node and the current node.
 */
std::vector<std::pair<int, int>> reconstructPath(int start, int end, const std::vector<int>& parent) {
    std::vector<std::pair<int, int>> path;
    int current = end;

    while (current != -1 && current != start) {
        path.emplace_back(parent[current], current);
        current = parent[current];
    }

    std::reverse(path.begin(), path.end()); 
    return path;
}
