#include "Graph.hpp"
#include "Partition.hpp"
#include <iostream>



void refinementFunction(const Graph& G,Partition& currentPartition,Partition& BetterPartition,vector<vector<int>> &bestInvariant,
vector<vector<int>> &currentInvariant,vector<vector<int>> &allPermutations) {
    currentInvariant.push_back(currentPartition.InvariantTriangleByCell(G));
    if(currentPartition.isDiscrete()) {
        allPermutations.push_back(currentPartition.transformCellsToInt());
        if(!BetterPartition.isDiscrete()) {
            BetterPartition = currentPartition;
            bestInvariant = currentInvariant;
        }
        else if (bestInvariant < currentInvariant) {
            BetterPartition = currentPartition;
            bestInvariant = currentInvariant;
        }
    }
    else {
        std::vector<Partition::Cell> alpha;
        Partition::Cell targetCell = currentPartition.targetCellSelector();
        for(int v : targetCell.verts){
            Partition copyPartition = currentPartition;
            copyPartition.individualizeVertex(v);
            alpha.push_back(copyPartition.getCellByVertex(v));
            copyPartition.refineGraph(G, alpha);
            refinementFunction(G,copyPartition,BetterPartition,bestInvariant,currentInvariant,allPermutations);
            alpha.pop_back();
            currentInvariant.pop_back();
        }
    }
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage:\n";
        std::cout << "  iso graph6 graph6\n";
        std::cout << "  canonical graph6\n";
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "iso") { // premier mode comparaison entre 2 graphes
    if (argc < 4) {
        std::cout << "Usage: iso graph6 graph6\n";
        return 1;
    }

    std::string g1_str = argv[2];
    std::string g2_str = argv[3];
    Graph G1(g1_str);
    Graph G2(g2_str);

    // --- Canonical form de G1 ---
    Partition P1(G1.size());
    vector<Partition::Cell> alpha1 = P1.getCells();
    P1.refineGraph(G1, alpha1);
    Partition BestP1 = P1;
    vector<vector<int>> bestInv1;
    vector<vector<int>> currInv1;
    vector<vector<int>> allPerms1;
    refinementFunction(G1, P1, BestP1, bestInv1, currInv1, allPerms1);
    vector<int> perm1 = BestP1.transformCellsToInt();
    Graph canonG1 = G1.applyPermutation(perm1);

    // --- Canonical form de G2 ---
    Partition P2(G2.size());
    vector<Partition::Cell> alpha2 = P2.getCells();
    P2.refineGraph(G2, alpha2);
    Partition BestP2 = P2;
    vector<vector<int>> bestInv2;
    vector<vector<int>> currInv2;
    vector<vector<int>> allPerms2;
    refinementFunction(G2, P2, BestP2, bestInv2, currInv2, allPerms2);
    vector<int> perm2 = BestP2.transformCellsToInt();
    Graph canonG2 = G2.applyPermutation(perm2);

    std ::cout << "Canonical form of G1: ";
    canonG1.printGraph6();

    std::cout << "Canonical form of G2: ";
    canonG2.printGraph6();

    // --- Comparaison ---
    if (canonG1 == canonG2) {
        std::cout << "Isomorphes ✅" << std::endl;
    } else {
        std::cout << "Non isomorphes ❌" << std::endl;
    }
}
else if (mode == "canonical" or mode == "all") {
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
    vector<vector<int>> allPerms;
    refinementFunction(mainGraph, mainPartition, BetterPartition, bestInvariant, currentInvariant, allPerms);
    vector<int> intBetterPartition = BetterPartition.transformCellsToInt();
    Graph canonicalGraph = mainGraph.applyPermutation(intBetterPartition);
    canonicalGraph.printGraph6();
    if(mode == "all") {
        for (const auto& perm : allPerms) {
            Graph gPerm = mainGraph.applyPermutation(perm);
            Partition P(gPerm.size());
            vector<Partition::Cell> alpha2 = P.getCells();
            P.refineGraph(gPerm, alpha2);
            Partition BestP = P;
            vector<vector<int>> bestInv;
            vector<vector<int>> currInv;
            vector<vector<int>> dummyPerms;
            refinementFunction(gPerm, P, BestP, bestInv, currInv, dummyPerms);
            vector<int> permCanon = BestP.transformCellsToInt();
            Graph canon = gPerm.applyPermutation(permCanon);
            canon.printGraph6();
        }
    }
}
    else {
        std::cout << "Mode inconnu ! Utilise 'iso' ou 'canonical'" << std::endl;
    }

    return 0;
}