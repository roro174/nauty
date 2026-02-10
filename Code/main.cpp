#include "Graph.hpp"
#include "Partition.hpp"
#include <iostream>



void refinementFunction(const Graph& G, Partition& currentPartition, std::vector<Partition::Cell>& alpha, std::vector<Partition>& searchTree, Partition& BetterPartition, int depth = 0) {
    currentPartition.refineGraph(G,  alpha);
    if (size(searchTree) == 1) {
        alpha.clear();
    }
    if(currentPartition.isDiscrete()){ 
        if (BetterPartition.isDiscrete() && currentPartition.cells < BetterPartition.cells) { // si égaux je dois faire un random entre les 2 non ?
                BetterPartition = currentPartition;
        } else if(!BetterPartition.isDiscrete()) {
            BetterPartition = currentPartition;
        }

    }
    else{
        Partition copyPartition = currentPartition;
        searchTree.push_back(copyPartition);
        Partition::Cell targetCell = currentPartition.targetCellSelector();
        for(int v : targetCell.verts){
            currentPartition.individualizeVertex(v);
            Partition::Cell newCell = currentPartition.getCellById(v);
            alpha.push_back(newCell);
            refinementFunction(G, searchTree[depth], alpha, searchTree, BetterPartition, depth + 1);
            alpha.pop_back();}
    }
}

int main() {
    return 0;
}
