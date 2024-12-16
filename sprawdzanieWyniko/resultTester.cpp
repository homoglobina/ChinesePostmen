#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <nlohmann/json.hpp> // Include the nlohmann/json library

using json = nlohmann::json;
using namespace std;

// Function to read JSON data from a file
json readJsonFromFile(const string& filePath) {
    ifstream file(filePath);
    if (!file) {
        throw runtime_error("Could not open file: " + filePath);
    }
    json data;
    file >> data;
    return data;
}

// Function to create a set of undirected edges from a list of edges
set<pair<int, int>> createEdgeSet(const vector<vector<int>>& edges) {
    set<pair<int, int>> edgeSet;
    for (const auto& edge : edges) {
        if (edge.size() != 2) continue;
        int u = edge[0], v = edge[1];
        if (u > v) std::swap(u, v); // Ensure (u, v) is ordered
        edgeSet.emplace(u, v);
    }
    return edgeSet;
}

int main() {
    try {
        json file1 = readJsonFromFile("graph.json");
        json file2 = readJsonFromFile("results.json");

        vector<vector<int>> edgesFile1 = file1["edges"];
        auto edgesSetFile1 = createEdgeSet(edgesFile1);

        set<pair<int, int>> edgesSetFile2;
        for (const auto& postman : file2["postmen"]) {
            for (const auto& route : postman["routes"]) {
                if (route.size() != 2) continue;
                int u = route[0], v = route[1];
                if (u > v) swap(u, v);
                edgesSetFile2.emplace(u, v);
            }
        }

        bool allEdgesIncluded = true;
        for (const auto& edge : edgesSetFile1) {
            if (edgesSetFile2.find(edge) == edgesSetFile2.end()) {
                std::cout << "Missing edge: (" << edge.first << ", " << edge.second << ")\n";
                allEdgesIncluded = false;
            }
        }

        if (allEdgesIncluded) {
            std::cout << "All edges from file1 are included in file2.\n";
        } else {
            std::cout << "Some edges from file1 are missing in file2.\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
