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

    vector<vector<int>> localAdjMatrix = adjMatrix;

    makeGraphEulerian();

    // Step 2: Find the Euler cycle
    auto eulerCycle = findEulerCycle();
    


    cout << "\nEuler0" << ": ";
    for (const auto& edge : eulerCycle) {
        cout << "(" << edge.second << ", " << edge.first << ") ";
    }
    cout << endl;

    // test 
    vector<pair<int,int>> eulerCycle2;
    cout << "Euler" << ": ";
    for (const auto& edge : eulerCycle) {
        if (localAdjMatrix[edge.second][edge.first] == 0){
            // if in this loop then delete from eulerCycle the edge that is not in the original graph
            eulerCycle.erase(std::remove(eulerCycle.begin(), eulerCycle.end(), edge), eulerCycle.end());
            cout << "deleted";
        }
        else{
            cout << "(" << edge.second << ", " << edge.first << ") ";
            eulerCycle2.push_back(edge);

        }
    }


    cout << "\nEuler2" << ": ";
    for (const auto& edge : eulerCycle) {
        cout << "(" << edge.second << ", " << edge.first << ") ";
    }
    cout << endl;

    cout << "\nEulerEND" << ": ";
    for (const auto& edge : eulerCycle2) {
        cout << "(" << edge.second << ", " << edge.first << ") ";
    }
    cout << endl;

    // Step 3: Split the Euler cycle into `n` connected subpaths
    int totalEdges = eulerCycle.size();
    int edgesPerPostman = totalEdges / n;
    vector<vector<pair<int, int>>> postmenRoutes(n);

    // Split the Eulerian cycle into nearly equal subpaths
    auto it = eulerCycle.begin();
    for (int i = 0; i < n; ++i) {
        int count = 0;
        while (it != eulerCycle.end() && count < edgesPerPostman) {
            postmenRoutes[i].push_back(*it);
            ++it;
            ++count;
        }
    }

    // If there are remaining edges (in case totalEdges isn't divisible evenly), distribute them
    while (it != eulerCycle.end()) {
        postmenRoutes[n - 1].push_back(*it);
        ++it;
    }

    // Display the routes assigned to postmen
    int totalCost = 0;
    for (int i = 0; i < n; ++i) {
        cout << "Postman " << i + 1 << ": ";
        for (const auto& edge : postmenRoutes[i]) {
            cout << "(" << edge.second << ", " << edge.first << ") ";
        }
        cout << endl;
        totalCost += calculateCycleCost(postmenRoutes[i]);
    }

    cout << "Total cost: " << totalCost << endl;
}

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
