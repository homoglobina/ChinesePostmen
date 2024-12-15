#include "graph.h"
#include "chinesePostman.h"
#include <limits>
#include <algorithm> // for std::min and std::max
#include <iostream>  // for debugging output if needed

void ChinesePostman::findShortestRouteExact() {
    // Step 1: Find all odd degree vertices
    std::vector<int> oddVertices = findOddDegreeVerticesExact();

    // Step 2: Find the shortest path between all pairs of odd degree vertices
    std::vector<std::vector<int>> shortestPaths = findShortestPathsExact(oddVertices);

    // Step 3: Solve Minimum-Cost Perfect Matching problem using exact algorithm
    std::vector<std::pair<int, int>> matching = findMinimumCostPerfectMatchingExact(shortestPaths, oddVertices);

    // Step 4: Add the matching edges to the graph
    addMatchingEdgesToGraphExact(matching);

    // Step 5: Find Eulerian circuit
    std::vector<int> eulerianCircuit = findEulerianCircuitExact();

    // Step 6: Split the Eulerian circuit among the postmen
    std::vector<std::vector<int>> routes = splitCircuitAmongPostmenExact(eulerianCircuit);

    // Print the routes
    printRoutesExact(routes);
}

std::vector<int> ChinesePostman::findOddDegreeVerticesExact() {
    std::vector<int> oddVertices;
    const auto& adjMatrix = graph.getAdjMatrix();

    for (int i = 0; i < graph.getVertices(); ++i) {
        int degree = 0;
        for (int j = 0; j < graph.getVertices(); ++j) {
            if (adjMatrix[i][j] > 0) {
                degree++;
            }
        }
        if (degree % 2 != 0) {
            oddVertices.push_back(i);
        }
    }

    return oddVertices;
}

std::vector<std::vector<int>> ChinesePostman::findShortestPathsExact(const std::vector<int>& oddVertices) {
    int n = graph.getVertices();
    std::vector<std::vector<int>> dist = graph.getAdjMatrix();

    // Use Floyd-Warshall algorithm to find shortest paths
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] != std::numeric_limits<int>::max() && 
                    dist[k][j] != std::numeric_limits<int>::max()) {
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // Extract distances between odd-degree vertices
    std::vector<std::vector<int>> shortestPaths(oddVertices.size(), std::vector<int>(oddVertices.size()));
    for (size_t i = 0; i < oddVertices.size(); ++i) {
        for (size_t j = 0; j < oddVertices.size(); ++j) {
            shortestPaths[i][j] = dist[oddVertices[i]][oddVertices[j]];
        }
    }

    return shortestPaths;
}

std::vector<std::pair<int, int>> ChinesePostman::findMinimumCostPerfectMatchingExact(
    const std::vector<std::vector<int>>& shortestPaths, const std::vector<int>& oddVertices) {
    int n = shortestPaths.size();
    std::vector<std::pair<int, int>> matching;

    // Dynamic programming (bitmask) to solve the matching
    std::vector<int> dp(1 << n, std::numeric_limits<int>::max());
    std::vector<int> parent(1 << n, -1);
    dp[0] = 0;

    for (int mask = 0; mask < (1 << n); ++mask) {
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {
                for (int j = i + 1; j < n; ++j) {
                    if (mask & (1 << j)) {
                        int newMask = mask ^ (1 << i) ^ (1 << j);
                        int cost = dp[newMask] + shortestPaths[i][j];
                        if (cost < dp[mask]) {
                            dp[mask] = cost;
                            parent[mask] = (i << 16) | j;
                        }
                    }
                }
            }
        }
    }

    int mask = (1 << n) - 1;
    while (parent[mask] != -1) {
        int pairData = parent[mask];
        int i = (pairData >> 16) & 0xFFFF;
        int j = pairData & 0xFFFF;
        matching.emplace_back(oddVertices[i], oddVertices[j]);
        mask ^= (1 << i) ^ (1 << j);
    }

    return matching;
}

void ChinesePostman::addMatchingEdgesToGraphExact(const std::vector<std::pair<int, int>>& matching) {
    for (const auto& edge : matching) {
        graph.addEdge(edge.first, edge.second);
    }
}

std::vector<int> ChinesePostman::findEulerianCircuitExact() {
    // Stub for finding the Eulerian circuit. Implement this method.
    std::vector<int> eulerianCircuit;
    // TODO: Add your logic here
    return eulerianCircuit;
}

std::vector<std::vector<int>> ChinesePostman::splitCircuitAmongPostmenExact(const std::vector<int>& eulerianCircuit) {
    // Stub for splitting the Eulerian circuit among postmen. Implement this method.
    std::vector<std::vector<int>> routes;
    // TODO: Add your logic here
    return routes;
}

void ChinesePostman::printRoutesExact(const std::vector<std::vector<int>>& routes) {
    for (size_t i = 0; i < routes.size(); ++i) {
        std::cout << "Route " << i + 1 << ": ";
        for (int vertex : routes[i]) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    }
}
