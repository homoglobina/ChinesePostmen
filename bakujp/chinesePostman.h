#ifndef CHINESE_H
#define CHINESE_H
#include "graph.h"
#include <vector>



class ChinesePostman{
public:
    ChinesePostman(Graph& g, int numPostmen) : graph(g), numPostmen(numPostmen){};
    void findShortestRoute(); // greedy
    void findShortestRouteExact();
private:
    Graph& graph;
    int numPostmen;  

    // greddy
    std::vector<int> findOddDegreeVertices();
    std::vector<std::vector<int>> findShortestPaths(const std::vector<int>& oddVertices);
    std::vector<std::pair<int, int>> findMinimumWeightMatching(const std::vector<std::vector<int>>& shortestPaths, const std::vector<int>& oddVertices);
    void addMatchingEdgesToGraph(const std::vector<std::pair<int, int>>& matching, const std::vector<std::vector<int>>& shortestPaths, const std::vector<int>& oddVertices);
    std::vector<int> findEulerianCircuit();
    std::vector<std::vector<int>> splitCircuitAmongPostmen(const std::vector<int>& eulerianCircuit);
    void printRoutes(const std::vector<std::vector<int>>& routes);

    // exact
    std::vector<int> findOddDegreeVerticesExact();
    std::vector<std::vector<int>> findShortestPathsExact(const std::vector<int>& oddVertices);
    std::vector<std::pair<int, int>> findMinimumCostPerfectMatchingExact(const std::vector<std::vector<int>>& shortestPaths, const std::vector<int>& oddVertices);
    void addMatchingEdgesToGraphExact(const std::vector<std::pair<int, int>>& matching);
    std::vector<int> findEulerianCircuitExact();
    std::vector<std::vector<int>> splitCircuitAmongPostmenExact(const std::vector<int>& eulerianCircuit);
    void printRoutesExact(const std::vector<std::vector<int>>& routes);


};















#endif CHINESE_H