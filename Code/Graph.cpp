#include "Graph.hpp"

// and et or au lieu de && et size_t pour n 

Graph::Graph(int numVertices, bool directed)
    : n(numVertices), isDirected(directed), adjMatrix(numVertices, vector<int>(numVertices, 0)) {
    }


Graph::Graph(const string& graph6, bool directed)
    : n(0), isDirected(directed) 
{
    buildFromGraph6(graph6);
}

void Graph::buildFromGraph6(const string& graph6) { 
    size_t index = 0; 

    unsigned char c = graph6[index++];
    if (c !=126) {
        n = c - 63;
    } else if (c == 126 && graph6[index] != 126) {
        std::string bits;
        for (int i = 0; i < 3; i++) {
            bits += std::bitset<6>(graph6[index++] - 63).to_string();
        }
        n = std::stoi(bits.substr(bits.size() - 18), nullptr, 2);
    } else if (c == 126 && graph6[index] == 126) { 
        index++;
        std::string bits;
        for (int i = 0; i < 6; i++) {
            bits += std::bitset<6>(graph6[index++] - 63).to_string();
        }
        n = std::stoi(bits.substr(bits.size() - 36), nullptr, 2);
    }

    std::string bitString;
    for (int index = 1 ; index < graph6.size(); index++) {
        unsigned char c = graph6[index];
        int val = c - 63;
        bitString += std::bitset<6>(val).to_string();
    }

    bitString = bitString.substr(0, n * (n - 1) / 2);
    adjMatrix.assign(n, std::vector<int>(n, 0));
    int pos = 0;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (bitString[pos++] == '1') {
                adjMatrix[i][j] = 1;
                adjMatrix[j][i] = 1;
                
            }
        }
    }


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

    rGraph6(bitString, nGraph);
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

vector<vector<int>>& Graph::getMatrix() {
    return adjMatrix;
}

vector<int> Graph::getNeighbors(int v) const {
    vector<int> neighbors;
    if (v < 0 || v >= n) return neighbors;

    for (int j = 0; j < n; ++j) {
        if (adjMatrix[v][j] != 0) {
            neighbors.push_back(j);
        }
    }
    return neighbors;
}

bool Graph::operator==(const Graph& other) const {
    if (n != other.n) return false;
    if (isDirected != other.isDirected) return false;
    return adjMatrix == other.adjMatrix;
}

bool Graph::operator!=(const Graph& other) const {
    return !(*this == other);
}

Graph Graph::applyPermutation(const std::vector<int> perm) const {
    if (perm.size() != n) {
        throw std::invalid_argument("Permutation size does not match number of vertices");
    }

    Graph result(n, isDirected);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result.getMatrix()[i][j] = adjMatrix[perm[i]][perm[j]];
        }
    }

    return result;
}