#ifndef GRAPHRENDERING_H
#define GRAPHRENDERING_H

#include <SFML/Graphics.hpp>
#include "graph.h"
#include <unordered_map>

class GraphRendering {
private:
    Graph& graph;
    std::unordered_map<int, sf::Vector2f> vertexPositions;
    sf::RenderWindow window;
    const int windowSize = 600;

    void generateVertexPositions();
    void drawGraph();
    void drawTraversal(const std::vector<int>& path, sf::Color color);

public:
    GraphRendering(Graph& g);
    void renderTraversal(const std::vector<std::vector<int>>& paths);
};

#endif // GRAPHRENDERING_H

