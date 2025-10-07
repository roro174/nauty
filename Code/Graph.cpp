#include "Graph.hpp"



Graph::Graph(int numVertices, bool directed)
    : n(numVertices), isDirected(directed), adjMatrix(numVertices, vector<int>(numVertices, 0)) {
    }


Graph::Graph(const string& graph6, bool directed)
    : n(0), isDirected(directed) 
{
    buildFromGraph6(graph6);
}

// TODO
void Graph::buildFromGraph6(const string& graph6) {
}

void Graph::addEdge(int u, int v, int weight) {
    if (u < 0 || v < 0 || u >= n || v >= n) {
        cerr << "Erreur: sommet invalide (" << u << ", " << v << ")\n";
        return;
    }
    adjMatrix[u][v] = weight;
    if (!isDirected) adjMatrix[v][u] = weight;
}


void Graph::removeEdge(int u, int v) {
    if (u < 0 || v < 0 || u >= n || v >= n) {
        cerr << "Erreur: sommet invalide (" << u << ", " << v << ")\n";
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
    cout << "Matrice d'adjacence (" << n << " sommets):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << adjMatrix[i][j] << " ";
        }
        cout << "\n";
    }
}


void Graph::printGraph6() const{
    std::string bitString = upperTriangleMatrix();
    std::vector<uint8_t> nGraph = nGraph6();
    for (auto b : nGraph)
    std::cout << static_cast<int>(b) << " ";
std::cout << std::endl;

    rGraph6(bitString, nGraph);
    for (auto b : nGraph)
        std::cout << static_cast<int>(b) << " ";
    std::cout << std::endl;
    for (auto b : nGraph)
    std::cout << static_cast<char>(b);
std::cout << std::endl;










}

std::string Graph::upperTriangleMatrix() const{
    std::string result;
    for(int i=1; i < adjMatrix.size(); i++){
        for(int j=0; j < adjMatrix[i].size();j++){
            if(j < i){
                result += (adjMatrix[j][i] == 0 ? '0' : '1');
            }
        }
    }

    return result;

}
void Graph::rGraph6(std::string x, std::vector<uint8_t> &result) const {
    int rest = x.size() % 6;
    if (rest != 0) {
        x.append(6 - rest, '0'); 
    }

    for (size_t i = 0; i < x.size(); i += 6) {
        std::string group = x.substr(i, 6);
        int value = std::stoi(group, nullptr, 2);
        result.push_back(static_cast<uint8_t>(value + 63));
    }
}



std::vector<uint8_t> Graph::nGraph6() const{
    std::vector<uint8_t> result;
    if(n >= 0 && n <= 62){
        result.push_back(n + 63);
        }
    else if(n>= 63 && n <= 258047){
        result.push_back(126);
        std::bitset<18> bits(n);
        std::string x = bits.to_string();
        rGraph6(x, result);
    }
    else if(n>= 258048 && n <= 68719476735){
        result.push_back(126);
        result.push_back(126);
        std::bitset<36> bits(n);
        std::string x = bits.to_string();
        rGraph6(x, result);
    }
    return result;
}

int Graph::size() const { return n; }

const vector<vector<int>>& Graph::getMatrix() const {
    return adjMatrix;
}
