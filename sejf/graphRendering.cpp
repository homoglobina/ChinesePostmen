#include "graphRendering.h"
#include <cmath>
#include <iostream>

GraphRendering::GraphRendering(Graph& g) : graph(g), window(sf::VideoMode(windowSize, windowSize), "Graph Traversal") {
    generateVertexPositions();
}

void GraphRendering::generateVertexPositions() {
    int vertices = graph.getVertices();
    float radius = windowSize / 3.0f;
    float angleStep = 2 * M_PI / vertices;
    sf::Vector2f center(windowSize / 2, windowSize / 2);

    for (int i = 0; i < vertices; ++i) {
        float angle = i * angleStep;
        vertexPositions[i] = center + sf::Vector2f(radius * std::cos(angle), radius * std::sin(angle));
    }
}

void GraphRendering::drawGraph() {
    sf::CircleShape vertexShape(10);
    vertexShape.setOrigin(10, 10);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }

    for (int i = 0; i < graph.getVertices(); ++i) {
        for (int j = i + 1; j < graph.getVertices(); ++j) {
            if (graph.getEdgeWeight(i, j) > 0) {
                sf::Vertex line[] = {
                    sf::Vertex(vertexPositions[i], sf::Color::White),
                    sf::Vertex(vertexPositions[j], sf::Color::White)
                };
                window.draw(line, 2, sf::Lines);
            }
        }
    }

    for (const auto& [vertex, position] : vertexPositions) {
        vertexShape.setPosition(position);
        vertexShape.setFillColor(sf::Color::Green);
        window.draw(vertexShape);

        sf::Text label(std::to_string(vertex), font, 15);
        label.setPosition(position.x - 5, position.y - 20);
        label.setFillColor(sf::Color::White);
        window.draw(label);
    }
}

void GraphRendering::drawTraversal(const std::vector<int>& path, sf::Color color) {
    for (size_t i = 1; i < path.size(); ++i) {
        sf::Vertex line[] = {
            sf::Vertex(vertexPositions[path[i - 1]], color),
            sf::Vertex(vertexPositions[path[i]], color)
        };
        window.draw(line, 2, sf::Lines);
    }
}

void GraphRendering::renderTraversal(const std::vector<std::vector<int>>& paths) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        drawGraph();

        sf::Color colors[] = {sf::Color::Red, sf::Color::Blue, sf::Color::Yellow, sf::Color::Cyan};
        for (size_t i = 0; i < paths.size(); ++i) {
            drawTraversal(paths[i], colors[i % 4]);
        }

        window.display();
    }
}

