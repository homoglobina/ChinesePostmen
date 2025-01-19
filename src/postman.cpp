#include "graph.h"
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include <fstream>
#include <nlohmann/json.hpp> 
#include <utility>


using json = nlohmann::json;
using namespace std;

/**
 * @file graph.cpp
 * @brief Solves the Chinese Postman Problem for the given graph.
 * 
 * This function makes the graph Eulerian, finds an Euler cycle, and then 
 * distributes the edges of the cycle among the given number of postmen. 
 * It calculates the total cost of the routes and saves the results in a JSON file.
 * 
 * @param n The number of postmen.
 * 
 * The function performs the following steps:
 * 1. Copies the adjacency matrix to a local variable.
 * 2. Makes the graph Eulerian by adding necessary edges.
 * 3. Finds an Euler cycle in the graph.
 * 4. Reconstructs the Euler cycle to ensure all edges are covered.
 * 5. Distributes the edges of the Euler cycle among the postmen.
 * 6. Calculates the total cost of the routes.
 * 7. Prints the routes and total cost.
 * 8. Calculates and prints the accuracy of the solution.
 * 9. Saves the results in a JSON file named "results.json".
 * 
 * The JSON file contains:
 * - The routes for each postman.
 * - The cost of each postman's route.
 * - The total cost of all routes.
 * 
 * @note The function assumes that the graph is connected and all vertices have even degree.
 */
void Graph::solveChinesePostman(int n) {
    vector<vector<int>> localAdjMatrix = adjMatrix;
    makeGraphEulerian();
    auto eulerCycle = findEulerCycle();
    vector<pair<int,int>> eulerCycle2;

    for (const auto& edge : eulerCycle) {
        if (localAdjMatrix[edge.second][edge.first] == 1){
            eulerCycle2.push_back({edge.second, edge.first});
            
        }
        else{
            auto parent = dijkstra3(edge.second, localAdjMatrix, vertices);
            auto shortestPath = reconstructPath(edge.second, edge.first, parent);
            for (const auto& edgePath : shortestPath) {
                eulerCycle2.push_back({edgePath.first, edgePath.second});
            }
        }
    }

    // cout << "\nEulerEND" << ": ";
    // for (const auto& edge : eulerCycle2) {
    //     cout << "(" << edge.first << ", " << edge.second << ") ";
    // }
    // cout << endl;

    int totalEdges = eulerCycle2.size();
    int edgesPerPostman = totalEdges / n;
    vector<vector<pair<int, int>>> postmenRoutes(n);

    auto it = eulerCycle2.begin();
    for (int i = 0; i < n; ++i) {
        int count = 0;
        while (it != eulerCycle2.end() && count < edgesPerPostman) {
            postmenRoutes[i].push_back(*it);
            ++it;
            ++count;
        }
    }

    while (it != eulerCycle2.end()) {
        postmenRoutes[n - 1].push_back(*it);
        ++it;
    }

    int totalCost = 0;
    for (int i = 0; i < n; ++i) {
        cout << "Postman " << i + 1 << ": ";
        for (const auto& edge : postmenRoutes[i]) {
            cout << "(" << edge.first << ", " << edge.second << ") ";
        }
        cout << endl;
        totalCost += calculateCycleCost(postmenRoutes[i]);
    }
    cout << "Total cost: " << totalCost << endl;
    float accuracy = 1;
    accuracy = (float)getEdges() / (float)totalCost; 
    cout << "Accuracy: " << accuracy * 100 <<"%"<< endl;

    json result;

    for (int i = 0; i < n; ++i) {
        json postmanData;
        for (const auto& edge : postmenRoutes[i]) {
            postmanData.push_back({edge.first, edge.second});
        }
        result["postmen"][i] = {
            {"routes", postmanData},
            {"cost", calculateCycleCost(postmenRoutes[i])}
        };
        totalCost += calculateCycleCost(postmenRoutes[i]);
    }
    result["totalCost"] = totalCost;
    ofstream file("results.json");
    if (file.is_open()) {
        file << result.dump();
        file.close();
        cout << "Results saved to results.json" << endl;
    } else {
        cerr << "Unable to open file for writing." << endl;
    }
}

/**
 * @brief Reconstructs the shortest path from start to end using the distance vector.
 * 
 * This function takes the start and end vertices along with the distance vector
 * and reconstructs the shortest path from start to end by backtracking through
 * the adjacency matrix.
 * 
 * @param start The starting vertex of the path.
 * @param end The ending vertex of the path.
 * @param dist The distance vector containing the shortest distances from the start vertex.
 * @return A vector containing the vertices in the shortest path from start to end.
 */
vector<int> Graph::reconstructShortestPath(int start, int end, const vector<int>& dist) {
    vector<int> path;
    int current = end;
    while (current != start) {
        path.push_back(current);
        for (int i = 0; i < vertices; ++i) {
            if (adjMatrix[i][current] > 0 && dist[current] - adjMatrix[i][current] == dist[i]) {
                current = i;
                break;
            }
        }
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

/**
 * @brief Converts the graph to an Eulerian graph by adding the minimum number of edges.
 * 
 * This function modifies the graph to make it Eulerian by ensuring all vertices have even degrees.
 * It identifies vertices with odd degrees and pairs them up by adding edges with the minimum cost.
 * 
 * @throws std::runtime_error if the number of vertices with odd degrees is odd.
 */
void Graph::makeGraphEulerian() {
    auto oddVertices = getOddDegreeVertices();
    if (oddVertices.size() % 2 != 0) {
        throw std::runtime_error("Odd number of vertices with odd degree!");
    }
    while (!oddVertices.empty()) {
        int u = oddVertices.back().first;
        oddVertices.pop_back();

        int bestV = -1;
        int bestCost = std::numeric_limits<int>::max();

        for (const auto& [v, degree] : oddVertices) {
            int cost = getEdgeWeight(u, v); 
            if (cost < bestCost) {
                bestV = v;
                bestCost = cost;
            }
        }
        if (bestV != -1) {
            addEdge(u, bestV);
            oddVertices.erase(
                std::remove_if(oddVertices.begin(), oddVertices.end(),
                               [bestV](const std::pair<int, int>& p) { return p.first == bestV; }),
                oddVertices.end());
        }
    }
}

int Graph::calculateCycleCost(const std::vector<std::pair<int, int>>& cycle) {
    int totalCost = 0;
    for (const auto& edge : cycle) {
        totalCost += getEdgeWeight(edge.first, edge.second);
    }
    return totalCost;
}

/**
 * @brief Finds an Eulerian cycle in the graph.
 * 
 * This function uses Hierholzer's algorithm to find an Eulerian cycle in the graph.
 * An Eulerian cycle is a cycle that visits every edge exactly once.
 * 
 * @return std::vector<std::pair<int, int>> A vector of pairs representing the edges in the Eulerian cycle.
 * Each pair contains two integers representing the vertices connected by the edge.
 */
std::vector<std::pair<int, int>> Graph::findEulerCycle() {
    std::vector<std::pair<int, int>> eulerCycle;
    std::stack<int> stack;
    std::vector<std::vector<int>> localAdjMatrix = adjMatrix; 
    stack.push(0); 
    while (!stack.empty()) {
        int u = stack.top();
        bool hasEdge = false;

        for (int v = 0; v < vertices; ++v) {
            if (localAdjMatrix[u][v] > 0) {
                localAdjMatrix[u][v]--;
                localAdjMatrix[v][u]--;
                stack.push(v);
                hasEdge = true;
                break;
            }
        }
        if (!hasEdge) {
            stack.pop();
            if (!stack.empty()) {
                eulerCycle.emplace_back(stack.top(), u);
            }
        }
    }
    return eulerCycle;
}
