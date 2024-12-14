#include "graph.h"
#include "postman.h"
#include <iostream>

int main() {
    int vertices = 3;
    double saturation = 0.5;
    std::cout << "Enter the number of vertices: ";
    std::cin >> vertices;

    std::cout << "Enter the saturation: ";
    std::cin >> saturation;

    Graph graph(vertices, saturation);

    // graph.addEdge(0, 1, 2);
    // graph.addEdge(1, 2, 3);
    // graph.addEdge(2, 3, 4);
    // graph.addEdge(3, 4, 5);
    // graph.addEdge(4, 5, 6);
    // graph.addEdge(5, 0, 7);
    // graph.addEdge(1, 3, 2);
    // graph.addEdge(2, 4, 1);

    // int postmen = 2;
    // Postman postman(graph, postmen);
    // postman.solve();

    graph.printAdjMatrix();

    graph.toGraphviz("graph.dot");  // Export to Graphviz format

    return 0;
}
