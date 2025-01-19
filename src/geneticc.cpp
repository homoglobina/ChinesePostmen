#include "graph.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

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
            if (verticesWithEdges.empty()) break;  // Prevents accessing empty vectors
            postmenRoutes[i].push_back(verticesWithEdges[(i + j) % verticesWithEdges.size()]);
        }
    }
    return postmenRoutes;
}


float Graph::testFitness(vector<vector<int>> route) {
    vector<vector<int>> adjMatrix = getAdjMatrix();
    int totalEdges = getEdges();

    float fitness = 0;
    int usedEdges = 0;
    vector<vector<bool>> visitedEdges(adjMatrix.size(), vector<bool>(adjMatrix.size(), false));
    
    // Track route lengths to penalize imbalance
    vector<int> routeLengths(route.size(), 0);

    for (int i = 0; i < route.size(); ++i) {
        for (int j = 0; j < route[i].size() - 1; ++j) {
            int from = route[i][j];
            int to = route[i][j + 1];

            if (adjMatrix[from][to] > 0) {
                if (!visitedEdges[from][to]) {
                    visitedEdges[from][to] = true;
                    usedEdges++;
                }
                fitness += adjMatrix[from][to];
            }

            routeLengths[i]++;
        }
    }

    // Penalize missing edges
    int missingEdges = totalEdges - usedEdges;
    fitness -= missingEdges * 5;  // Higher penalty for missing edges

    // Penalize route imbalance
    int maxLength = *max_element(routeLengths.begin(), routeLengths.end());
    int minLength = *min_element(routeLengths.begin(), routeLengths.end());
    int imbalance = maxLength - minLength;
    fitness -= imbalance * 1;  // Adjust penalty weight as needed

    // Penalize excessive total route length
    int totalRouteLength = accumulate(routeLengths.begin(), routeLengths.end(), 0);
    int excessLength = totalRouteLength - totalEdges;
    if (excessLength > 0) {
        fitness -= excessLength * 2;
    }

    return fitness;
}


vector<vector<int>> crossover(const vector<vector<int>>& population1, const vector<vector<int>>& population2) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, population1[0].size() - 1);  // Possible out-of-bounds

    vector<vector<int>> newPopulation(population1.size());

    for (int i = 0; i < population1.size(); ++i) {
        if (population1[i].empty() || population2[i].empty()) continue; // Prevents accessing empty vectors
        int crossoverPoint = dist(gen) % min(population1[i].size(), population2[i].size());

        for (int j = 0; j < crossoverPoint; ++j) {
            newPopulation[i].push_back(population1[i][j]);
        }
        for (int j = crossoverPoint; j < population2[i].size(); ++j) {
            newPopulation[i].push_back(population2[i][j]);
        }
    }
    return newPopulation;
}


void mutate(vector<vector<int>>& population, const vector<int>& verticesWithEdges, int totalEdges) {
    if (population.empty() || verticesWithEdges.empty()) return;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> postmanDist(0, population.size() - 1);
    uniform_int_distribution<> routeDist(0, totalEdges - 1);
    uniform_int_distribution<> vertexDist(0, verticesWithEdges.size() - 1);

    // Mutate multiple routes randomly
    for (int i = 0; i < 2; ++i) {  
        int postman = postmanDist(gen);
        if (population[postman].empty()) continue;

        int route = routeDist(gen) % population[postman].size();
        int vertex = vertexDist(gen);

        population[postman][route] = verticesWithEdges[vertex];
    }

    // Occasionally swap a section of routes
    if (population.size() > 1 && verticesWithEdges.size() > 2) {
        int p1 = postmanDist(gen);
        int p2 = postmanDist(gen);
        if (p1 != p2 && !population[p1].empty() && !population[p2].empty()) {
            swap(population[p1][0], population[p2][0]); // Swap first elements
        }
    }
}




// void Graph::solveGenetic(int n, int x) { // number of postmen, number of generations
//     vector<int> verticesWithEdges = shuffeledVertices(adjMatrix, getVertices());
//     if (n > verticesWithEdges.size()) {
//         cerr << "Number of postmen cannot be greater than the number of vertices." << endl;
//         return;
//     }

//     vector<int> postmenStart;
//     for (int i = 0; i < n; i++) {
//         postmenStart.push_back(verticesWithEdges[i]);
//     }

//     // basis for genetic algorithm
//     vector<vector<vector<int>>> populations;
//     vector<vector<int>> population;
//     vector<float> fitnessScores;

//     for (int gen = 0; gen < x; ++gen) {
//         populations.clear();
//         fitnessScores.clear();

//         for (int i = 0; i < 10; ++i) {
//             population = createPopulation(verticesWithEdges, n, getEdges());
//             float fitness = testFitness(population);
//             populations.push_back(population);
//             fitnessScores.push_back(fitness);
//         }

//         pair<int, int> bestPopulations = findBestPopulations(fitnessScores, populations, n);
//         vector<vector<int>> population1 = populations[bestPopulations.first];
//         vector<vector<int>> population2 = populations[bestPopulations.second];

//         vector<vector<int>> newPopulation = crossover(population1, population2);
//         mutate(newPopulation, verticesWithEdges, getEdges());

//         float newFitness = testFitness(newPopulation);
//         cout << "Generation " << gen + 1 << " new population fitness: " << newFitness << endl;

//         for (int i = 0; i < n; ++i) {
//             cout << "Postman " << i + 1 << " new route: ";
//             for (int vertex : newPopulation[i]) {
//                 cout << vertex << " ";
//             }
//             cout << endl;
//         }

//         populations.push_back(newPopulation);
//         fitnessScores.push_back(newFitness);
//     }
// }

int countValidEdges(const vector<vector<int>>& population, const vector<vector<int>>& adjMatrix) {
    int validEdges = 0;
    vector<vector<bool>> visitedEdges(adjMatrix.size(), vector<bool>(adjMatrix.size(), false));
    for (const auto& route : population) {
        for (size_t i = 0; i < route.size() - 1; ++i) {
            int from = route[i];
            int to = route[i + 1];
            if (adjMatrix[from][to] > 0 && !visitedEdges[from][to] && !visitedEdges[to][from]) {
                validEdges++;
                visitedEdges[from][to] = true;
                visitedEdges[to][from] = true; // Mark the reverse direction as visited
            }
        }
    }
    return validEdges;
}

void Graph::solveGenetic(int n, int x) { // number of postmen, number of generations
    vector<int> verticesWithEdges = shuffeledVertices(adjMatrix, getVertices());
    if (n > verticesWithEdges.size()) {
        cerr << "Number of postmen cannot be greater than the number of vertices." << endl;
        return;
    }

    vector<int> postmenStart;
    for (int i = 0; i < n; i++) {
        postmenStart.push_back(verticesWithEdges[i]);
    }

    // basis for genetic algorithm
    vector<vector<int>> population = createPopulation(verticesWithEdges, n, getEdges());
    float fitness = testFitness(population);

    // Print first generation
    cout << "Generation 1 fitness: " << fitness << endl;
    for (int i = 0; i < n; ++i) {
        cout << "Postman " << i + 1 << " route: ";
        for (int vertex : population[i]) {
            cout << vertex << " ";
        }
        cout << endl;
    }

    for (int gen = 1; gen < x; ++gen) {
        vector<vector<int>> newPopulation = createPopulation(verticesWithEdges, n, getEdges());
        float newFitness = testFitness(newPopulation);

        if (newFitness > fitness) {
            population = newPopulation;
            fitness = newFitness;
        }

        if (gen < x - 1) {
            vector<vector<int>> population1 = population;
            vector<vector<int>> population2 = createPopulation(verticesWithEdges, n, getEdges());

            vector<vector<int>> crossoverPopulation = crossover(population1, population2);
            mutate(crossoverPopulation, verticesWithEdges, getEdges());

            float crossoverFitness = testFitness(crossoverPopulation);
            if (crossoverFitness > fitness) {
                population = crossoverPopulation;
                fitness = crossoverFitness;
            }
        }
    }

    // Print last generation
    cout << "Generation " << x << " fitness: " << fitness << endl;
    int totalEdgesLastGen = 0;
    for (int i = 0; i < n; ++i) {
        cout << "Postman " << i + 1 << " route: ";
        for (int vertex : population[i]) {
            cout << vertex << " ";
        }
        totalEdgesLastGen += population[i].size() - 1; // Count edges in the route
        cout << endl;
    }

    float accuracy = 1;
    accuracy = (float)getEdges() / (float)totalEdgesLastGen; 
    cout << "Accuracy: " << accuracy * 100 <<"%"<< endl;
    cout << "Total edges in last generation: " << totalEdgesLastGen << endl;
    // cout << "get Edges: " << getEdges() << endl;

    int validEdges = countValidEdges(population, adjMatrix);
    cout << "Number of valid edges in the solution: " << validEdges << endl;
    cout << "Correctness: " << ( (float)validEdges / (float)getEdges() ) * 100 << "%" << endl;





    // Save results to JSON
    json result;
    for (int i = 0; i < n; ++i) {
        json postmanData;
        for (int j = 0; j < population[i].size() - 1; ++j) {
            postmanData.push_back({population[i][j], population[i][j + 1]});
        }
        result["postmen"][i] = {
            {"routes", postmanData},
            {"cost", testFitness(population)}
        };
    }


    ofstream file("resultsGenetic.json");
    if (file.is_open()) {
        file << result.dump();
        file.close();
        cout << "Results saved to resultsGenetic.json" << endl;
    } else {
        cerr << "Unable to open file for writing." << endl;
    }


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
