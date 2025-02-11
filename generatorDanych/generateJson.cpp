#include <iostream>
#include <nlohmann/json.hpp> 
#include <fstream>
#include <vector>
#include <random>

using json = nlohmann::json;
using namespace std;

int main(int argc, char* argv[]) {
    int v = 3;
    double saturation = 0.5;
    unsigned int seed = random_device{}(); 

    if (argc > 1) {
        seed = static_cast<unsigned int>(stoi(argv[1]));
    }

    cout << "Enter the number of vertices: ";
    cin >> v;

    cout << "Enter the saturation (0.0 to 1.0): ";
    cin >> saturation;

    int totalEdges = v * (v - 1) / 2;
    int targetEdges = static_cast<int>(saturation * totalEdges);

    vector<pair<int, int>> allEdges;
    for (int i = 0; i < v; i++) {
        for (int j = i + 1; j < v; j++) {
            allEdges.emplace_back(i, j);
        }
    }

    mt19937 gen(seed);
    shuffle(allEdges.begin(), allEdges.end(), gen);

    vector<pair<int, int>> selectedEdges;
    for (int i = 0; i < targetEdges; i++) {
        selectedEdges.emplace_back(allEdges[i]);
    }

    json graphJson;
    graphJson["vertices"] = v;

    for (const auto& edge : selectedEdges) {
        graphJson["edges"].push_back({edge.first, edge.second});
    }

    ofstream outFile("graph.json");
    if (outFile.is_open()) {
        outFile << graphJson.dump(); 
        outFile.close();
        cout << "Graph saved to graph.json" << endl;
    } else {
        cerr << "Failed to open file for writing." << endl;
    }

    return 0;
}
