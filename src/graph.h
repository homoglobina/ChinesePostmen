#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>


class Graph {
private:
    int vertices;
    std::vector<std::vector<int>> adjMatrix;

public:
    Graph(int v, double satruation);
    void addEdge(int u, int v);
    int getVertices() const;
    // int findShortestPath(int u, int v) const;
    std::vector<std::pair<int, int>> getOddDegreeVertices() const;
    int getEdgeWeight(int u, int v) const;
    const std::vector<std::vector<int>>& getAdjMatrix() const;
    // void GenerateGraph(int v, int u);
    void printAdjMatrix();
    void toGraphviz(const std::string& filename) const;


};


#endif // GRAPH_H
