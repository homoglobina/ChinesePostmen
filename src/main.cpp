#include "graph.h"
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>

int howManyPostmen();

/**
 * @file main.cpp
 * @brief Entry point for the Chinese Postman problem solver.
 *@version 1.0
 *author Jakub Doman
 * Compilation
 * Make compile
 * 
 * This program reads a graph from a JSON file, initializes the graph, and solves the Chinese Postman problem
 * using different algorithms.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 *             - argv[0]: Program name
 *             - argv[1]: Path to the JSON file containing the graph
 *             - argv[2]: Number of postmen
 *             - argv[3]: Seed for random number generation
 *
 * @return int Exit status of the program.
 *             - 0: Success
 *             - 1: Error
 *
 * Usage:
 * @code
 * ./main <json file> <number of postmen> <seed>
 * @endcode
 */

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <json file>  <number of postmen>  <seed>" << std::endl;
        return 1;
    }

    std::string jsonFile = argv[1];
    Graph graph(jsonFile);

    int seed = std::stoi(argv[3]);
    graph.setSeed(seed);

    int numPostmen = std::stoi(argv[2]);
    if (numPostmen > graph.getEdges()) {
        std::cerr << "Number of postmen cannot be greater than the number of edges." << std::endl;
        return 1;
    }
    graph.toGraphviz("graph.dot");  

    int gen = 500;


    

    auto start = std::chrono::high_resolution_clock::now();
    graph.solveChinesePostman(numPostmen);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> chinesePostmanTime = end - start;

    start = std::chrono::high_resolution_clock::now();
    graph.solveGenetic(numPostmen, gen);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> geneticTime = end - start;

    std::ofstream csvFile("wykres.csv", std::ios::app);



    if (csvFile.is_open()) {
        // csvFile << graph.getEdges() << "," << chinesePostmanTime.count() << "," << geneticTime.count() << "," << graph.getVertices() << "\n";
        csvFile << graph.getEdges() << "," << chinesePostmanTime.count() << "," << 0 << "," << graph.getVertices() << "\n";
        csvFile.close();
    } else {
        std::cerr << "Unable to open file wykres.csv" << std::endl;
    }

}
    






// int main() {
//     int choice;
//     std::cout << "1-json \n0-hand\nchoice:" ;
//     std::cin >>  choice ;

//     if (choice){   
//         Graph graph("graph.json");
//         graph.toGraphviz("graph.dot");  
//         int numPostmen = howManyPostmen();
//         graph.solveChinesePostman(numPostmen);
//     }
//     else {  
//         int vertices = 3;
//         double saturation = 0.5;
//         std::cout << "Enter the number of vertices: ";
//         std::cin >> vertices;

//         std::cout << "Enter the saturation: ";
//         std::cin >> saturation;

//         Graph graph(vertices, saturation);

//         graph.toGraphviz("graph.dot");  
//         int numPostmen = howManyPostmen();
//         graph.solveChinesePostman(numPostmen);



//     }
//     int res = graphViz();
//     std::cout << "Graphviz: " << res ; 
//     std::cin >> choice;
//     return 0;
// }


int howManyPostmen(){
    int numPostmen;
    std::cout << "How many postmen do you want: " << std::endl;
    std::cin >> numPostmen;
    return numPostmen;
}