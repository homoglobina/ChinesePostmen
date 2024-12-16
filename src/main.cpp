#include "graph.h"
#include <iostream>

int howManyPostmen();

int main() {
    int choice;
    std::cout << "1-json \n0-hand\nchoice:" ;
    std::cin >>  choice ;



    if (choice){   
        Graph graph("graph.json");
        graph.toGraphviz("graph.dot");  
        int numPostmen = howManyPostmen();
        graph.solveChinesePostman(numPostmen);



    }
    else {  
        int vertices = 3;
        double saturation = 0.5;
        std::cout << "Enter the number of vertices: ";
        std::cin >> vertices;

        std::cout << "Enter the saturation: ";
        std::cin >> saturation;

        Graph graph(vertices, saturation);

        graph.toGraphviz("graph.dot");  
        int numPostmen = howManyPostmen();
        graph.solveChinesePostman(numPostmen);



    }
    int res = graphViz();
    std::cout << "Graphviz: " << res ; 
    std::cin >> choice;
    return 0;
}


int howManyPostmen(){
    int numPostmen;
    std::cout << "How many postmen do you want: " << std::endl;
    std::cin >> numPostmen;
    return numPostmen;
}