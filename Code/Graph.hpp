#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <iostream>
#include <string>

class Graph {
private:
    int n; // nombre de sommets
    bool isDirected;
    std::vector<std::vector<int>> adjMatrix;

    void buildFromSparse6(const std::string& sparse6Str);

public:
    // --- Constructeurs ---
    Graph(int numVertices, bool directed = false);             // 1. Graphe vide
    Graph(const std::string& sparse6Str, bool directed = false); // 2. Graphe depuis un sparse6

    void addEdge(int u, int v, int weight = 1);
    void removeEdge(int u, int v);
    bool hasEdge(int u, int v) const;
    void printMatrix() const;
    void printSparse6() const;

    int size() const;
    const std::vector<std::vector<int>>& getMatrix() const;
};

#endif // GRAPH_HPP
