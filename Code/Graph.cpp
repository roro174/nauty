#include "Graph.hpp"


Graph::Graph(int numVertices, bool directed)
    : n(numVertices), isDirected(directed), adjMatrix(numVertices*numVertices, 0) {}

Graph::Graph(const string& graph6, bool directed)
    : n(0), isDirected(directed) 
{
    buildFromGraph6(graph6);
}


void Graph::buildFromGraph6(const string& graph6) { 
    size_t index = 0; 

    unsigned char c = graph6[index++];
    if (c != 126) {
        n = c - 63;
    } else if (c == 126 and graph6[index] != 126) {
        std::string bits;
        for (int i = 0; i < 3; i++)
            bits += std::bitset<6>(graph6[index++] - 63).to_string();
        n = std::stoi(bits.substr(bits.size() - 18), nullptr, 2);
    } else if (c == 126 and graph6[index] == 126) { 
        index++;
        std::string bits;
        for (int i = 0; i < 6; i++)
            bits += std::bitset<6>(graph6[index++] - 63).to_string();
        n = std::stoi(bits.substr(bits.size() - 36), nullptr, 2);
    }

    std::string bitString;
    for (size_t index = 1 ; index < graph6.size(); index++)
        bitString += std::bitset<6>(graph6[index] - 63).to_string();

    bitString = bitString.substr(0, n * (n - 1) / 2);
    adjMatrix.assign(n*n, 0);

    int pos = 0;
    for (size_t i = 1; i < n; ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (bitString[pos++] == '1') {
                at(i,j) = 1;
                at(j,i) = 1;
            }
        }
    }
}


void Graph::addEdge(size_t u, size_t v, int weight) {
    if (u >= n || v >= n) { cerr << "Erreur: sommet invalide\n"; return; }
    at(u,v) = weight;
    if (!isDirected) at(v,u) = weight;
}

void Graph::removeEdge(size_t u, size_t v) {
    if (u >= n || v >= n) { cerr << "Erreur: sommet invalide\n"; return; }
    at(u,v) = 0;
    if (!isDirected) at(v,u) = 0;
}

bool Graph::hasEdge(size_t u, size_t v) const {
    if (u >= n || v >= n) return false;
    return at(u,v) != 0;
}


vector<int> Graph::getNeighbors(size_t v) const {
    vector<int> neighbors;
    if (v >= n) return neighbors;
    for (size_t j = 0; j < n; ++j)
        if (at(v,j) != 0) neighbors.push_back(j);
    return neighbors;
}


void Graph::printMatrix() const {
    cout << "Matrice d'adjacence (" << n << " sommets):\n";
    for (size_t i = 0; i < n; ++i){
        for (size_t j = 0; j < n; ++j)
            cout << at(i,j) << " ";
        cout << "\n";
    }
}

void Graph::printGraph6() const {
    std::string bitString = upperTriangleMatrix();
    std::vector<uint8_t> nGraph = nGraph6();
    rGraph6(bitString, nGraph);
    for (auto b : nGraph) std::cout << static_cast<char>(b);
    std::cout << std::endl;
}

// --- Upper triangle ---
std::string Graph::upperTriangleMatrix() const {
    std::string result;
    for(size_t i=1; i < n; ++i)
        for(size_t j=0; j < i; ++j)
            result += (at(i,j) == 0 ? '0' : '1');
    return result;
}


void Graph::rGraph6(std::string x, std::vector<uint8_t> &result) const {
    int rest = x.size() % 6;
    if (rest != 0) x.append(6 - rest, '0'); 
    for (size_t i = 0; i < x.size(); i += 6) {
        std::string group = x.substr(i, 6);
        int value = std::stoi(group, nullptr, 2);
        result.push_back(static_cast<uint8_t>(value + 63));
    }
}

std::vector<uint8_t> Graph::nGraph6() const {
    std::vector<uint8_t> result;
    if(n <= 62) result.push_back(n + 63);
    else if(n <= 258047) {
        result.push_back(126);
        std::bitset<18> bits(n);
        rGraph6(bits.to_string(), result);
    } else if(n <= 68719476735) {
        result.push_back(126); result.push_back(126);
        std::bitset<36> bits(n);
        rGraph6(bits.to_string(), result);
    }
    return result;
}


bool Graph::operator==(const Graph& other) const {
    return n == other.n && isDirected == other.isDirected && adjMatrix == other.adjMatrix;
}

bool Graph::operator!=(const Graph& other) const {
    return !(*this == other);
}

int Graph::size() const { return n; }


Graph Graph::applyPermutation(const std::vector<int> perm) const {
    if (perm.size() != n) throw std::invalid_argument("Permutation size mismatch");

    Graph result(n, isDirected);
    for (size_t i = 0; i < n; ++i)
        for (size_t j = i; j < n; ++j) {
            int val = at(perm[i], perm[j]);
            result.at(i,j) = val;
            result.at(j,i) = val;
        }
    return result;
}