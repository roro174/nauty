#include "Graph.hpp"


Graph::Graph(int numVertices, bool directed)
    : n(numVertices), isDirected(directed), adjMatrix(numVertices, std::vector<int>(numVertices, 0)) {}


Graph::Graph(const std::string& sparse6Str, bool directed)
    : n(0), isDirected(directed) 
{
    buildFromSparse6(sparse6Str);
}

// TODO
void Graph::buildFromSparse6(const std::string& sparse6Str) {
}

void Graph::addEdge(int u, int v, int weight) {
    if (u < 0 || v < 0 || u >= n || v >= n) {
        std::cerr << "Erreur: sommet invalide (" << u << ", " << v << ")\n";
        return;
    }
    adjMatrix[u][v] = weight;
    if (!isDirected) adjMatrix[v][u] = weight;
}


void Graph::removeEdge(int u, int v) {
    if (u < 0 || v < 0 || u >= n || v >= n) {
        std::cerr << "Erreur: sommet invalide (" << u << ", " << v << ")\n";
        return;
    }
    adjMatrix[u][v] = 0;
    if (!isDirected) adjMatrix[v][u] = 0;
}

bool Graph::hasEdge(int u, int v) const {
    if (u < 0 || v < 0 || u >= n || v >= n) return false;
    return adjMatrix[u][v] != 0;
}


void Graph::printMatrix() const {
    std::cout << "Matrice d'adjacence (" << n << " sommets):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << adjMatrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}

// TODO
void Graph::printSparse6() const{

}


int Graph::size() const { return n; }

const std::vector<std::vector<int>>& Graph::getMatrix() const {
    return adjMatrix;
}
