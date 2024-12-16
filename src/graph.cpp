#include "graph.h"
#include <limits>
#include <random>
#include <iostream>
#include <algorithm> 
#include <string>
#include <fstream>  
#include <iomanip> 
#include <nlohmann/json.hpp> 
#include <stack>

using namespace std;

using json = nlohmann::json;


void Graph::addEdge(int u, int v) {
    adjMatrix[u][v] = 1;
    adjMatrix[v][u] = 1;
}


Graph::Graph(const std::string& jsonFile) {
    std::ifstream file(jsonFile);
    if (!file) {
        throw std::runtime_error("Error: Could not open JSON file " + jsonFile);
    }
    json j;
    file >> j;
    if (!j.contains("vertices") || !j.contains("edges")) {
        throw std::runtime_error("Invalid JSON format: missing 'vertices' or 'edges' fields.");
    }
    vertices = j["vertices"];
    adjMatrix.resize(vertices, std::vector<int>(vertices, 0));
    for (const auto& edge : j["edges"]) {
        int u = edge[0];
        int v = edge[1];
        if (u >= vertices || v >= vertices) {
            throw std::runtime_error("Invalid edge: vertex out of bounds.");
        }
        addEdge(u, v);
    }
}

Graph::Graph(int v, double saturation) : vertices(v), adjMatrix(v, vector<int>(v, 0)) {
    int totalEdges = v * (v - 1) / 2;
    int targetEdges = static_cast<int>(saturation * totalEdges);
    cout << "total: " << totalEdges << "  target: " << targetEdges << endl; 
    vector<pair<int, int>> allEdges;
    for (int i = 0; i < v; i++) {
        for (int j = i + 1; j < v; j++) { 
            allEdges.emplace_back(i, j);
        }
    }
    random_device rd;
    mt19937 gen(rd());
    shuffle(allEdges.begin(), allEdges.end(), gen);

    for (int i = 0; i < targetEdges; i++) {
        int u = allEdges[i].first;
        int v = allEdges[i].second;
        addEdge(u, v);
    }
}

vector<pair<int,int>> Graph::findEuler(){
    vector<pair<int,int>> eulerCycle;
    stack<int> stack;
    vector<vector<int>> localAdjMatrix = adjMatrix; 

    stack.push(0); 
    while (!stack.empty()) {
        int u = stack.top();
        int v = 0;
        while (v < vertices && localAdjMatrix[u][v] == 0) {
            v++;
        }
        if (v == vertices) {
            eulerCycle.emplace_back(u, -1);
            stack.pop();
        } else {
            stack.push(v);
            localAdjMatrix[u][v] = 0;
            localAdjMatrix[v][u] = 0;
        }
    }

    return eulerCycle;

}



int Graph::getVertices() const {
    return vertices;
}

bool Graph::isEulerian() const {
    for (int i = 0; i < vertices; ++i) {
        int degree = 0;
        for (int j = 0; j < vertices; ++j) {
            if (adjMatrix[i][j] > 0) {
                degree++;
            }
        }
        if (degree % 2 != 0) {
            return false;
        }
    }
    return true;
}


vector<pair<int, int>> Graph::getOddDegreeVertices() const {
    vector<pair<int, int>> oddVertices;
    for (int i = 0; i < vertices; ++i) {
        int degree = 0;
        for (int j = 0; j < vertices; ++j) {
            if (adjMatrix[i][j] > 0) {
                degree++;
            }
        }
        if (degree % 2 != 0) {
            oddVertices.emplace_back(i, degree);
        }
    }
    return oddVertices;
}

int Graph::getEdgeWeight(int u, int v) const {
    return adjMatrix[u][v];
}

const vector<vector<int>>& Graph::getAdjMatrix() const {
    return adjMatrix;
}

void Graph::printAdjMatrix(){
    for (const auto& row : adjMatrix) {
        for (int weight : row) {
            cout << weight << " ";
        }
        cout << endl;
    }
}

void Graph::toGraphviz(const string& filename) const {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }

    
    std::random_device rd;
    std::mt19937 gen(rd());
    uniform_int_distribution<> colorDist(0, 255);
    int r = colorDist(gen);
    int g = colorDist(gen);
    int b = colorDist(gen);
    
    outFile << "graph G {" << endl;
        
    for (int i = 0; i < vertices; ++i) {
        for (int j = i + 1; j < vertices; ++j) {  
            if (adjMatrix[i][j] > 0) {
                outFile << "  " << i << " -- " << j
                        << " [color=\"#" 
                        << std::hex << std::setw(2) << std::setfill('0') << r
                        << std::setw(2) << g
                        << std::setw(2) << b
                        << "\"];" << std::endl;
                outFile << std::dec;
                        
            }
        }
    }

    outFile << "}" << endl;

    outFile.close();
    cout << "Graph exported to " << filename << " in Graphviz DOT format." << endl;
}
