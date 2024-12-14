#include "graph.h"
#include "postman.h"
#include <iostream>

int main() {
    int choice;
    std::cout << "1-json \n0-hand\nchoice:" ;
    std::cin >>  choice ;



    if (choice){
        Graph graph("graph.json");

        graph.printAdjMatrix();
        graph.toGraphviz("graph.dot");  // Export to Graphviz format

    }
    else {
        int vertices = 3;
        double saturation = 0.5;
        std::cout << "Enter the number of vertices: ";
        std::cin >> vertices;

        std::cout << "Enter the saturation: ";
        std::cin >> saturation;

        Graph graph(vertices, saturation);


        graph.printAdjMatrix();
        graph.toGraphviz("graph.dot");  // Export to Graphviz format


    }
    std::cin >> choice;

    return 0;
}
