#include <iostream>
#include <vector>
#include <algorithm>
#include "graph.h"
#include "postman.h"

using namespace std;

class GreedyChinesePostman {
public:
    GreedyChinesePostman(const Graph& g, int numPostmen) 
        : graph(g), n(numPostmen) {}

    void solve() {
        vector<pair<int, int>> edges;
        const auto& adj = graph.getAdjMatrix();

        // Step 1: Collect all edges
        for (int i = 0; i < graph.getVertices(); ++i) {
            for (int j = i + 1; j < graph.getVertices(); ++j) {
                if (adj[i][j] > 0) {
                    edges.push_back({i, j});
                }
            }
        }

        // Step 2: Sort edges by weight (or distance)
        sort(edges.begin(), edges.end(), [&](const pair<int, int>& a, const pair<int, int>& b) {
            return graph.getEdgeWeight(a.first, a.second) < graph.getEdgeWeight(b.first, b.second);
        });

        // Step 3: Distribute edges to postmen (round-robin)
        vector<vector<pair<int, int>>> postmanRoutes(n);
        int postmanIndex = 0;
        for (const auto& edge : edges) {
            postmanRoutes[postmanIndex].push_back(edge);
            postmanIndex = (postmanIndex + 1) % n;
        }

        // Step 4: Print the routes
        for (int i = 0; i < n; ++i) {
            cout << "Postman " << i + 1 << " route: ";
            for (const auto& edge : postmanRoutes[i]) {
                cout << "(" << edge.first << " -> " << edge.second << ") ";
            }
            cout << endl;
        }

        // Step 5: Calculate the total distance for each postman
        for (int i = 0; i < n; ++i) {
            double totalCost = 0.0;
            for (const auto& edge : postmanRoutes[i]) {
                totalCost += graph.getEdgeWeight(edge.first, edge.second);
            }
            cout << "Postman " << i + 1 << " total distance: " << totalCost << endl;
        }
    }

private:
    Graph graph;
    int n;  // Number of postmen
};
