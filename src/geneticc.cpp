#include "graph.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

vector<int> shuffeledVertices(vector<vector<int>> adjMatrix, int vertices){
    vector<int> verticesWithEdges;
    for (int i = 0; i < vertices; ++i) {
    for (int j = 0; j < vertices; ++j) {
            if (adjMatrix[i][j] > 0) {
                verticesWithEdges.push_back(i);
                break;
            }
        }
    }
    random_device rd;
    mt19937 gen(rd());
    shuffle(verticesWithEdges.begin(), verticesWithEdges.end(), gen);
    return verticesWithEdges;
}


vector<vector<int>> createPopulation(vector<int> verticesWithEdges, int n, int totalEdges){
    random_device rd;
    mt19937 gen(rd());
    vector<vector<int>> postmenRoutes(n);
    
    uniform_int_distribution<> dist(1, totalEdges);

    for (int i = 0; i < n; ++i) {
        int routeLength = dist(gen);
        for (int j = 0; j < routeLength; ++j) {
            postmenRoutes[i].push_back(verticesWithEdges[(i + j) % verticesWithEdges.size()]);
        }
    }
    return postmenRoutes;

}




float Graph::testFitness(vector<vector<int>> route){
    adjMatrix = getAdjMatrix();
    // for (int i = 0; i < adjMatrix.size(); ++i) {
    //     for (int j = 0; j < adjMatrix[i].size(); ++j) {
    //         cout << adjMatrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    
    float fitness = 0;
    int edges = 0;
    for (int i = 0; i < route.size(); ++i) {
        for (int j = 0; j < route[i].size() - 1; ++j) {
            edges++;
            fitness += adjMatrix[route[i][j]][route[i][j + 1]];
            // cout << "i: " << i << " j: " << j << " route[i][j] " << route[i][j] << " route[i][j+1]: " << route[i][j+1] << " fitness: " << fitness<< endl;            
        }
    }

    edges = edges - getEdges(); // conditon to not make solution obsuletely enourmous
    return fitness - edges*2;
}

// void mutate(vector<vector<int>> &population, vector<int> verticesWithEdges, int totalEdges){
//     random_device rd;
//     mt19937 gen(rd());
//     uniform_int_distribution<> dist(0, population.size() - 1);
//     uniform_int_distribution<> dist2(0, totalEdges - 1);
//     uniform_int_distribution<> dist3(0, verticesWithEdges.size() - 1);

//     int postman = dist(gen);
//     int route = dist2(gen);
//     int vertex = dist3(gen);

//     population[postman][route] = verticesWithEdges[vertex];
// }




void Graph::solveGenetic(int n){ // number of postmen
    
    vector<int> verticesWithEdges = shuffeledVertices(adjMatrix, getVertices());
    if (n > verticesWithEdges.size()){
        cerr << "Number of postmen cannot be greater than the number of vertices." << endl;
        return;
    }

    vector<int> postmenStart;
    for (int i = 0; i < n; i++){
        postmenStart.push_back(verticesWithEdges[i]);
    }


    // basis for genetic algorithm
    vector<vector<vector<int>>> populations;
    vector<vector<int>> population;
    vector<float> fitnessScores;

    for (int i = 0; i < 10; ++i) {
        population = createPopulation(verticesWithEdges, n, getEdges());
        float fitness = testFitness(population);
        cout << "Generation " << i + 1 << "  fitness: " << fitness << endl;
        
        for (int i = 0; i < n; ++i) {
            cout << "Postman " << i + 1 << " route: ";
            for (int vertex : population[i]) {
                cout << vertex << " ";
            }
            cout << endl;
        }
        populations.push_back(population);
        fitnessScores.push_back(fitness);
        
    }

    cout << endl << endl << endl;

    pair<int,int> bestPopulations = findBestPopulations(fitnessScores, populations, n);
    vector<vector<int>> population1, population2;
    population1 = populations[bestPopulations.first];
    population2 = populations[bestPopulations.second];


    



    // Find the two populations with the best fitness




    // // Use the best populations for further processing
    // vector<vector<int>> population2 = bestPopulation2;
    // population = bestPopulation1; // or bestPopulation2 depending on your needs



    // for (int i = 0; i < n; ++i) {
    //     cout << "Postman " << i + 1 << " route: ";
    //     for (int vertex : population2[i]) {
    //         cout << vertex << " ";
    //     }
    //     cout << endl;
    // }

    // mutate(population, verticesWithEdges, getEdges());
}

pair<int, int> Graph::findBestPopulations(std::vector<float> &fitnessScores, std::vector<std::vector<std::vector<int>>> &populations, int n)
{
    // Find the population with the best fitness score
    auto maxFitnessIt = max_element(fitnessScores.begin(), fitnessScores.end());
    int bestPopulationIndex = distance(fitnessScores.begin(), maxFitnessIt);
    vector<vector<int>> bestPopulation = populations[bestPopulationIndex];

    // Find the population with the second best fitness score
    float bestFitness = *maxFitnessIt;
    *maxFitnessIt = numeric_limits<float>::lowest(); // Temporarily set the best fitness to the lowest possible value
    auto secondMaxFitnessIt = max_element(fitnessScores.begin(), fitnessScores.end());
    int secondBestPopulationIndex = distance(fitnessScores.begin(), secondMaxFitnessIt);
    vector<vector<int>> secondBestPopulation = populations[secondBestPopulationIndex];
    *maxFitnessIt = bestFitness; // Restore the best fitness score

    cout << "Best population fitness: " << bestFitness << endl;
    cout << "Best population: " << bestPopulationIndex + 1 << endl;

    for (int i = 0; i < n; ++i)
    {
        cout << "Postman " << i + 1 << " best route: ";
        for (int vertex : bestPopulation[i])
        {
            cout << vertex << " ";
        }
        cout << endl;
    }

    cout << "Second best population fitness: " << *secondMaxFitnessIt << endl;
    cout << "Second best population: " << secondBestPopulationIndex + 1 << endl;

    for (int i = 0; i < n; ++i)
    {
        cout << "Postman " << i + 1 << " second best route: ";
        for (int vertex : secondBestPopulation[i])
        {
            cout << vertex << " ";
        }
        cout << endl;
    }

    return {bestPopulationIndex, secondBestPopulationIndex};
}
