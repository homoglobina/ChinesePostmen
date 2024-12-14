#ifndef POSTMAN_H
#define POSTMAN_H

#include "graph.h"
#include <vector>



class GreedyChinesePostman {
public:
    GreedyChinesePostman(const Graph& g, int numPostmen) 
        : graph(g), n(numPostmen) {}

    void solve();

private:
    Graph graph;
    int n;  
};




#endif // POSTMAN_H
