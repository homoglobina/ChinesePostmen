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
    Graph(const std::string& jsonFile);
    void addEdge(int u, int v);
    int getVertices() const;
    std::vector<std::pair<int, int>> getOddDegreeVertices() const;
    int getEdgeWeight(int u, int v) const;
    const std::vector<std::vector<int>>& getAdjMatrix() const;
    bool isEulerian() const;
    void printAdjMatrix();
    void toGraphviz(const std::string& filename) const;

    void solveChinesePostman(int n);
    void makeGraphEulerian();
    std::vector<std::pair<int, int>> findEulerCycle();
    int calculateCycleCost(const std::vector<std::pair<int, int>>& cycle);
    std::vector<std::pair<int,int>> findEuler();
    std::vector<int> dijkstra(int start);
    std::vector<int> reconstructShortestPath(int start, int end, const std::vector<int>& dist);
};

std::vector<int> dijkstra2(int start, int end, const std::vector<std::vector<int>>& adjMatrix, int vertices);



#endif // GRAPH_H
