#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <iostream>
#include <string>
#include <cstdint>
#include <bitset>

using namespace std;

class Graph {
private:
    size_t n; // nombre de sommets
    bool isDirected;
    std::vector<int> adjMatrix;
    std::vector<std::vector<int>> neighbors;

    void buildFromGraph6(const string& graph6);

    int& at(size_t i, size_t j) { return adjMatrix[i*n + j]; }
    int at(size_t i, size_t j) const { return adjMatrix[i*n + j]; }

public:
    // --- Constructeurs ---
    Graph(int numVertices, bool directed = false);   
    Graph(const string& Graph6, bool directed = false); 

    void addEdge(size_t u, size_t v, int weight = 1);
    void removeEdge(size_t u, size_t v);
    bool hasEdge(size_t u, size_t v) const;
    vector<int> getNeighbors(size_t v) const;
    void printMatrix() const;
    void printGraph6() const;
    std::vector<uint8_t> nGraph6() const;
    void rGraph6(std::string x, std::vector<uint8_t> &result) const;
    std::string upperTriangleMatrix() const;
    bool operator==(const Graph& other) const;
    bool operator!=(const Graph& other) const;
    int size() const;
    Graph applyPermutation(const std::vector<int> perm) const;
};

#endif // GRAPH_HPP