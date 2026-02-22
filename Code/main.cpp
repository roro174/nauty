#include "Graph.hpp"
#include "Partition.hpp"
#include <iostream>



void refinementFunction(const Graph& G, Partition& currentPartition, Partition& BetterPartition, vector<vector<int>> &bestInvariant, vector<vector<int>> &currentInvariant, int depth = 0) {
    cout << string(depth, ' ') << "Depth: " << depth << " - Current Partition:\n";
    currentInvariant.push_back(currentPartition.InvariantTriangleByCell(G));
    if(currentPartition.isDiscrete()){
        cout << string(depth, ' ') << "Discrete partition reached.\n";
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
            cout << string(depth, ' ') << "Individualizing vertex: " << v << "\n";
            for(const auto& cell: copyPartition.cells) {
                cout << string(depth, ' ') << "Cell id=" << cell.id << " size=" << cell.verts.size() << " : ";
                for(int vert : cell.verts) cout << vert << " ";
                cout << "\n";
            }
            alpha.push_back(currentPartition.getCellByVertex(v));
            copyPartition.refineGraph(G,  alpha);
            refinementFunction(G, copyPartition, BetterPartition, bestInvariant, currentInvariant, depth+1);
            cout  << "return in depth: " << depth << "\n";
            alpha.pop_back();
            cout << "here" << endl;
            currentInvariant.pop_back();
            cout << "heure" << endl;
            }
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];
        Graph mainGraph(arg);
        Partition mainPartition(mainGraph.size());
        vector<Partition::Cell> alpha = mainPartition.cells;
        mainPartition.refineGraph(mainGraph, alpha);
        Partition BetterPartition = mainPartition;
        vector<vector<int>> bestInvariant;
        vector<vector<int>> currentInvariant;
        
        refinementFunction(mainGraph, mainPartition, BetterPartition, bestInvariant, currentInvariant);
        cout << "end" << endl;
        for(auto& cell : BetterPartition.cells) cell.print();


    } else {
        std::cout << "Aucun argument !" << std::endl;
    }

    return 0;
}