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
    int n; // nombre de sommets
    bool isDirected;
    std::vector<vector<int>> adjMatrix;

    void buildFromGraph6(const string& graph6);

public:
    // --- Constructeurs ---
    Graph(int numVertices, bool directed = false);   
    Graph(const string& Graph6, bool directed = false); 

    void addEdge(int u, int v, int weight = 1);
    void removeEdge(int u, int v);
    bool hasEdge(int u, int v) const;
    vector<int> getNeighbors(int v) const;
    void printMatrix() const;
    void printGraph6() const;
    std::vector<uint8_t> nGraph6() const;
    void rGraph6(std::string x, std::vector<uint8_t> &result) const;
    std::string upperTriangleMatrix() const;

    int size() const;
    const vector<vector<int>>& getMatrix() const;
};

#endif // GRAPH_HPP
