#include <iostream>
#include <nlohmann/json.hpp> 
#include <fstream>
#include <vector>
#include <random>

using json = nlohmann::json;
using namespace std;

int main(int argc, char* argv[]) { //  ./genJsonTests 123 5 0.7 (seed,number_of_vertices,saturation)
    int v = 3;
    double saturation = 0.5;
    unsigned int seed = random_device{}(); 

    if (argc < 3){
        cout << "Usage: " << argv[0] << " [seed] [vertices] [saturation]" << endl;
        return 1;
    }
    seed = static_cast<unsigned int>(stoi(argv[1]));
    v = stoi(argv[2]);
    saturation = stod(argv[3]);

    string filNr = to_string(v);



    // if (argc > 1) {
    //     seed = static_cast<unsigned int>(stoi(argv[1]));
    // }
    // if (argc > 2) {
    //     v = stoi(argv[2]);
    // } else {
    //     cout << "Enter the number of vertices: ";
    //     cin >> v;
    // }
    // if (argc > 3) {
    //     saturation = stod(argv[3]);
    // } else {
    //     cout << "Enter the saturation (0.0 to 1.0): ";
    //     cin >> saturation;
    // }

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

    ofstream outFile("graphTest" + filNr + ".json");
    if (outFile.is_open()) {
        outFile << graphJson.dump(); 
        outFile.close();
        cout << "Graph saved to graphTest" << filNr <<".json" << endl;
    } else {
        cerr << "Failed to open file for writing." << endl;
    }

    return 0;
}
