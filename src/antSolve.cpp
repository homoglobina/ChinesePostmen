#include "graph.h"
#include <vector>
#include <iostream>

using namespace std;

int howDesirable(int v, int u, vector<vector<int>> adjMatrix, vector<pair<int, int>> visitedEdges);

void Graph::solveAnts(int n){
    vector<vector<int>> adjMatrix = getAdjMatrix();
    vector<pair<int, int>> visitedEdges;


    for (int i = 0; i < n; i++){
        vector<int> path;
        int start = 0;
        int end = 0;
        int sum = 0;
        while (visitedEdges.size() < adjMatrix.size()){
            path = dijkstra3(start, adjMatrix, adjMatrix.size());
            for (int j = 0; j < path.size(); j++){
                cout << path[j] << " ";
            }
            cout << endl;
            end = path[path.size() - 1];
            sum += howDesirable(start, end, adjMatrix, visitedEdges);
            visitedEdges.push_back(make_pair(start, end));
            start = end;
        }
        cout << "Sum: " << sum << endl;
    }
}

// int v - > int u
int howDesirable(int v, int u, vector<vector<int>> adjMatrix, vector<pair<int, int>> visitedEdges){
    int sum = 0;
    int isVisited = 0;
    for (int i = 0; i < adjMatrix.size(); i++){
        if (adjMatrix[u][i] > 0){  // jezeli istneije krawedz
            for (int j = 0; j < visitedEdges.size(); j++){ // sprawdzamy czy krawedz byla juz odwiedzona
                if (visitedEdges[j].first == v && visitedEdges[j].second == v){
                    isVisited++;
                }
                else if ((visitedEdges[j].first == u && visitedEdges[j].second == i) || (visitedEdges[j].first == i && visitedEdges[j].second == u)){
                    isVisited++;
                }
            }
            // jezeli krawedz nie byla odwiedzona to jest dodawana do oceny krawedzi
            if (isVisited == 0){
                sum += adjMatrix[u][i];
            }
            isVisited = 0;

        }
    }
    return sum;
}