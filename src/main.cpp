#include "graph.h"
#include <iostream>

int menu();
int howManyPostmen();

int main() {
    int choice;
    std::cout << "1-json \n0-hand\nchoice:" ;
    std::cin >>  choice ;



    if (choice){    // use existing graph
        Graph graph("graph.json");

        // graph.printAdjMatrix();
        graph.toGraphviz("graph.dot");  // Export to Graphviz format

        // int x = menu();
        int numPostmen = howManyPostmen();
        graph.solveChinesePostman(numPostmen);



    }
    else {  // Generate a graph
        int vertices = 3;
        double saturation = 0.5;
        std::cout << "Enter the number of vertices: ";
        std::cin >> vertices;

        std::cout << "Enter the saturation: ";
        std::cin >> saturation;

        Graph graph(vertices, saturation);


        // graph.printAdjMatrix();
        graph.toGraphviz("graph.dot");  // Export to Graphviz format

        // int x = menu();
        int numPostmen = howManyPostmen();
        graph.solveChinesePostman(numPostmen);



    }
    int res = graphViz();
    std::cout << "Graphviz: " << res ; 
    std::cin >> choice;

    return 0;
}


int menu(){
    int choice;
    std::cout << "What kind of algorithm do you wish to choose? \n1-greedy \n0-exact\nchoice:" ;
    std::cin >> choice ;
    return choice;
}

int howManyPostmen(){
    int numPostmen;
    std::cout << "How many postmen do you want: " << std::endl;
    std::cin >> numPostmen;
    return numPostmen;
}