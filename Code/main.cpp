#include "Graph.hpp"

int main() {
    Graph g(5, false); // TEST

    g.addEdge(0, 2);
    g.addEdge(0, 4);
    g.addEdge(1, 3);
    g.addEdge(3, 4);

    g.printGraph6();
    return 0;
}
