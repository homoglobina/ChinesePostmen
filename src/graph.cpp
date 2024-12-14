#include "graph.h"
#include <limits>
#include <queue>
#include <random>
#include <iostream>
#include <algorithm> 
#include <string>
#include <fstream>  // For file handling
#include <iomanip> // For random colors

using namespace std;


void Graph::addEdge(int u, int v) {
    adjMatrix[u][v] = 1;
    adjMatrix[v][u] = 1;
    // cout << "added edge from " << u << " to " << v << endl;
}

Graph::Graph(int v, double saturation) : vertices(v), adjMatrix(v, vector<int>(v, 0)) {
    int totalEdges = v * (v - 1) / 2;
    int targetEdges = static_cast<int>(saturation * totalEdges);
    cout << "total: " << totalEdges << "  target: " << targetEdges << endl; 

    // Generate all possible edges
    vector<pair<int, int>> allEdges;
    for (int i = 0; i < v; i++) {
        for (int j = i + 1; j < v; j++) { 
            allEdges.emplace_back(i, j);
        }
    }

    // Shuffle edges
    random_device rd;
    mt19937 gen(rd());
    shuffle(allEdges.begin(), allEdges.end(), gen);

    for (int i = 0; i < targetEdges; i++) {
        int u = allEdges[i].first;
        int v = allEdges[i].second;
        addEdge(u, v);
    }
}

int Graph::getVertices() const {
    return vertices;
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
    // Open file for writing
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }

    
    std::random_device rd;
    std::mt19937 gen(rd());
    uniform_int_distribution<> colorDist(0, 255);
    
    outFile << "graph G {" << endl;
    for (int i = 0; i < vertices; ++i) {
        int r = colorDist(gen);
        int g = colorDist(gen);
        int b = colorDist(gen);
        
        // int color = colorDist(gen);
        for (int j = i + 1; j < vertices; ++j) {  
            if (adjMatrix[i][j] > 0) {
                

                // Add edge with color
                outFile << "  " << i << " -- " << j
                        << " [color=\"#" 
                        << std::hex << std::setw(2) << std::setfill('0') << r
                        << std::setw(2) << g
                        << std::setw(2) << b
                        << "\"];" << std::endl;

                // Reset std::hex formatting for subsequent output
                outFile << std::dec;
                        
            }
        }
    }

    outFile << "}" << endl;

    outFile.close();
    cout << "Graph exported to " << filename << " in Graphviz DOT format." << endl;
}
