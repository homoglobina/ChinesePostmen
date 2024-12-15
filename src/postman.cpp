#include "graph.h"
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <stdexcept>
#include <limits>


using namespace std;

void Graph::solveChinesePostman(int n) {
    // Step 1: Make the graph Eulerian
    makeGraphEulerian();

    // Step 2: Find the Euler cycle
    auto eulerCycle = findEulerCycle();

    // Step 3: Calculate the total cost of the cycle
    int totalCost = calculateCycleCost(eulerCycle);
    int avgCostPerPostman = totalCost / n;

    // Step 4: Divide the Euler cycle among `n` postmen
    vector<vector<pair<int, int>>> postmenRoutes(n); // Routes for each postman
    int currentCost = 0, postman = 0;
    for (const auto& edge : eulerCycle) {
        if (currentCost + getEdgeWeight(edge.first, edge.second) > avgCostPerPostman && postman < n - 1) {
            postman++; // Move to the next postman
            currentCost = 0;
        }
        postmenRoutes[postman].push_back(edge); // Assign edge to the current postman's route
        currentCost += getEdgeWeight(edge.first, edge.second);
    }

    // Step 5: Display routes
    for (int i = 0; i < n; ++i) {
        cout << "Postman " << i + 1 << ": ";
        for (const auto& edge : postmenRoutes[i]) {
            cout << "(" << edge.first << ", " << edge.second << ") ";
        }
        cout << endl;
    }
    cout << "Total cost: " << totalCost << endl;
}

void Graph::makeGraphEulerian() {
    // Find vertices with odd degree
    auto oddVertices = getOddDegreeVertices();

    if (oddVertices.size() % 2 != 0) {
        throw std::runtime_error("Odd number of vertices with odd degree!");
    }

    // Pair odd-degree vertices with minimal cost
    while (!oddVertices.empty()) {
        int u = oddVertices.back().first;
        oddVertices.pop_back();

        int bestV = -1;
        int bestCost = std::numeric_limits<int>::max();

        for (const auto& [v, degree] : oddVertices) {
            int cost = getEdgeWeight(u, v); // Assuming 1 if no explicit weight
            if (cost < bestCost) {
                bestV = v;
                bestCost = cost;
            }
        }

        // Add edge (u, bestV) to make degrees even
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


std::vector<std::pair<int, int>> Graph::findEulerCycle() {
    std::vector<std::pair<int, int>> eulerCycle;
    std::stack<int> stack;
    std::vector<std::vector<int>> localAdjMatrix = adjMatrix; // Make a copy

    stack.push(0); // Start at any vertex
    while (!stack.empty()) {
        int u = stack.top();
        bool hasEdge = false;

        for (int v = 0; v < vertices; ++v) {
            if (localAdjMatrix[u][v] > 0) {
                // Remove the edge
                localAdjMatrix[u][v]--;
                localAdjMatrix[v][u]--;
                stack.push(v);
                hasEdge = true;
                break;
            }
        }

        if (!hasEdge) {
            // Backtrack and add to Euler cycle
            stack.pop();
            if (!stack.empty()) {
                eulerCycle.emplace_back(stack.top(), u);
            }
        }
    }

    return eulerCycle;
}
