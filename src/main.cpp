#include "graph.h"
#include <iostream>
#include <string>

int howManyPostmen();

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


    graph.solveChinesePostman(numPostmen);


    // graph.solveAnts(3);

    int gen = 100;
    std::cout << "Enter the number of generations: "; std::cin >> gen;

    graph.solveGenetic(numPostmen, gen);


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