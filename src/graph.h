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
    int getEdges() const;
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

    void solveGenetic(int n);
    std::pair<int, int> findBestPopulations(std::vector<float> &fitnessScores, std::vector<std::vector<std::vector<int>>> &populations, int n);
    float testFitness(std::vector<std::vector<int>> route);
    

    void solveAnts(int n);


};





std::vector<int> dijkstra2(int start, int end, const std::vector<std::vector<int>>& adjMatrix, int vertices);
std::vector<int> dijkstra3(int start, const std::vector<std::vector<int>>& adjMatrix, int vertices);
std::vector<std::pair<int, int>> reconstructPath(int start, int end, const std::vector<int>& parent);

std::string getColor(int index);
int graphViz();


#endif // GRAPH_H
