#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include "graph.h"

using namespace std;

class MetaheuristicCPP {
public:
    MetaheuristicCPP(const Graph& g, int numPostmen) 
        : graph(g), n(numPostmen) {}

    void solve() {
        double temperature = 1000.0;
        double coolingRate = 0.995;
        const int maxIterations = 10000;

        // Initialize random route assignments for postmen
        vector<vector<pair<int, int>>> postmanRoutes(n);
        initializeRoutes(postmanRoutes);

        for (int iter = 0; iter < maxIterations; ++iter) {
            // Generate a neighbor solution by swapping two edges between postmen
            vector<vector<pair<int, int>>> newPostmanRoutes = postmanRoutes;
            swapEdges(newPostmanRoutes);

            double currentCost = calculateTotalCost(postmanRoutes);
            double newCost = calculateTotalCost(newPostmanRoutes);

            if (acceptanceProbability(currentCost, newCost, temperature) > (double)rand() / RAND_MAX) {
                postmanRoutes = newPostmanRoutes;
            }

            temperature *= coolingRate;  // Cool down
        }

        // Output final result
        double finalCost = calculateTotalCost(postmanRoutes);
        cout << "Final total cost for all postmen: " << finalCost << endl;
    }

private:
    Graph graph;
    int n;  // Number of postmen

    void initializeRoutes(vector<vector<pair<int, int>>>& postmanRoutes) {
        vector<pair<int, int>> edges;
        const auto& adj = graph.getAdjMatrix();

        // Collect all edges
        for (int i = 0; i < graph.getVertices(); ++i) {
            for (int j = i + 1; j < graph.getVertices(); ++j) {
                if (adj[i][j] > 0) {
                    edges.push_back({i, j});
                }
            }
        }

        // Randomly assign edges to postmen
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, n - 1);

        for (const auto& edge : edges) {
            int postman = dist(gen);
            postmanRoutes[postman].push_back(edge);
        }
    }

    void swapEdges(vector<vector<pair<int, int>>>& postmanRoutes) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, n - 1);

        // Swap edges between two random postmen
        int postman1 = dist(gen);
        int postman2 = dist(gen);
        if (postman1 != postman2 && !postmanRoutes[postman1].empty() && !postmanRoutes[postman2].empty()) {
            int idx1 = rand() % postmanRoutes[postman1].size();
            int idx2 = rand() % postmanRoutes[postman2].size();

            swap(postmanRoutes[postman1][idx1], postmanRoutes[postman2][idx2]);
        }
