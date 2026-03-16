#include "Graph.hpp"
#include "Partition.hpp"
#include <iostream>



void refinementFunction(const Graph& G, Partition& currentPartition, Partition& BetterPartition, vector<vector<int>> &bestInvariant, vector<vector<int>> &currentInvariant) {
    currentInvariant.push_back(currentPartition.InvariantTriangleByCell(G));
    if(currentPartition.isDiscrete()){
        if(!BetterPartition.isDiscrete()) {
            BetterPartition = currentPartition;
            bestInvariant = currentInvariant;
        }
        else if (bestInvariant < currentInvariant) {
                BetterPartition = currentPartition;
                bestInvariant = currentInvariant;} 
    }
    else{
        std::vector<Partition::Cell> alpha;
        Partition::Cell targetCell = currentPartition.targetCellSelector();
        for(int v : targetCell.verts){
            Partition copyPartition = currentPartition;
            copyPartition.individualizeVertex(v);
            alpha.push_back(currentPartition.getCellByVertex(v));
            copyPartition.refineGraph(G,  alpha);
            refinementFunction(G, copyPartition, BetterPartition, bestInvariant, currentInvariant);
            alpha.pop_back();
            currentInvariant.pop_back();
            }
    }
}

#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage:\n";
        std::cout << "  iso graph6 graph6\n";
        std::cout << "  canonical graph6\n";
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "iso") {
        if (argc < 4) {
            std::cout << "Usage: iso graph6 graph6\n";
            return 1;
        }

        std::string g1_str = argv[2];
        std::string g2_str = argv[3];

        Graph G1(g1_str);
        Graph G2(g2_str);
    }
    else if (mode == "canonical") {
        if (argc < 3) {
            std::cout << "Usage: canonical graph6\n";
            return 1;
        }

        std::string g_str = argv[2];
        Graph mainGraph(g_str);
        Partition mainPartition(mainGraph.size());
        vector<Partition::Cell> alpha = mainPartition.getCells();
        mainPartition.refineGraph(mainGraph, alpha);
        Partition BetterPartition = mainPartition;
        vector<vector<int>> bestInvariant;
        vector<vector<int>> currentInvariant;
        refinementFunction(mainGraph, mainPartition, BetterPartition, bestInvariant, currentInvariant);
        vector<int> intBetterPartition = BetterPartition.transformCellsToInt();
        Graph canonicalGraph = mainGraph.applyPermutation(intBetterPartition);
        canonicalGraph.printGraph6();
    }

    else {
        std::cout << "Mode inconnu ! Utilise 'iso' ou 'canonical'" << std::endl;
    }

    return 0;
}