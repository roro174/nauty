#include "Graph.hpp"
#include "Partition.hpp"
#include <iostream>



void refinementFunction(const Graph& G, Partition& currentPartition, Partition& BetterPartition, vector<vector<int>> bestInvariant, vector<vector<int>> currentInvariant) {
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
            currentPartition.individualizeVertex(v);
            alpha.push_back(currentPartition.getCellById(v));
            Partition copyPartition = currentPartition;
            copyPartition.refineGraph(G,  alpha);
            refinementFunction(G, copyPartition, BetterPartition, bestInvariant, currentInvariant);
            alpha.pop_back();
            currentInvariant.pop_back();
            }
    }
}

int main() {
    return 0;
}
