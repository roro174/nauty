#include "Graph.hpp"


std::vector<int> refinement(Graph g, std::vector<int> v, std::string s) {
    std::vector<int> result;
    return result;
}


int main() {
    Graph g(5, false); // TEST

    g.addEdge(0, 2);
    g.addEdge(0, 4);
    g.addEdge(1, 3);
    g.addEdge(3, 4);

    g.printGraph6();

    Graph g2("DcQ", false);
    g2.printMatrix();
    return 0;
}
