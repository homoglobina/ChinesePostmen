#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
std::string getColor(int index) {
    static const std::vector<std::string> colors = {
        "#df01d6", "#0174df", "#04b404", "#ff8000", "#ff0000"
    };
    return colors[index % colors.size()];
}

int graphViz() {
    std::string inputFileName = "results.json";
    std::string outputFileName = "output.dot";

    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        return 1;
    }

    json inputData;
    inputFile >> inputData;
    inputFile.close();

    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file!" << std::endl;
        return 1;
    }
    outputFile << "graph G {\n";

    const auto& postmen = inputData["postmen"];
    for (size_t i = 0; i < postmen.size(); ++i) {
        std::string color = getColor(i);
        const auto& routes = postmen[i]["routes"];
        for (const auto& route : routes) {
            outputFile << "  " << route[0] << " -- " << route[1]
                       << " [color=\"" << color << "\"];\n";
        }
    }

    outputFile << "}\n";
    outputFile.close();

    std::cout << "DOT file has been created: " << outputFileName << std::endl;

    return 0;
}
