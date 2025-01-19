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
