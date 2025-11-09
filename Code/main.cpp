#include "Graph.hpp"
#include "Partition.hpp"
#include <iostream>


int main() {
    Partition P(5);

    std::cout << "Partition init" << std::endl;
    P.print();
    P.individualizeVertex(2);
    std::cout << "\nafter indiv 2" << std::endl;
    P.print();
    P.individualizeVertex(4);
    std::cout << "\nafter indiv 4" << std::endl;
    P.print();

    return 0;
}
