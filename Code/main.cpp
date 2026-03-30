#include "Graph.hpp"
#include "Partition.hpp"
#include <iostream>
#include <vector>

using namespace std;

struct NautyResult {
    Graph canonicalGraph;
    vector<vector<int>> allPermutations;
};


bool compareLexicoLess(const vector<vector<int>>& a, const vector<vector<int>>& b) { // True si a < b
    return std::lexicographical_compare(a.begin(),a.begin() + b.size(),b.begin(),b.end());
    }

void refinementFunction(const Graph& G,Partition& currentPartition, Partition& BetterPartition,vector<vector<int>> &bestInvariant,
                        vector<vector<int>> &currentInvariant,vector<vector<int>>* allPermutations){
    currentInvariant.push_back(currentPartition.InvariantTriangleByCell(G));
    if (currentPartition.isDiscrete()) {
        if (allPermutations != nullptr) allPermutations->push_back(currentPartition.transformCellsToInt());
        if (!BetterPartition.isDiscrete()) {
            BetterPartition = currentPartition;
            bestInvariant = currentInvariant;}
        else if (bestInvariant < currentInvariant) {
            BetterPartition = currentPartition;
            bestInvariant = currentInvariant;}
    }

    // Prunning P_a + P_b
    else if(BetterPartition.isDiscrete() and (!compareLexicoLess(bestInvariant, currentInvariant))) {
        return;
    }

    else {
        vector<Partition::Cell> alpha;
        Partition::Cell targetCell = currentPartition.targetCellSelector();
        for (int v : targetCell.verts) {
            Partition copyPartition = currentPartition;
            copyPartition.individualizeVertex(v);
            alpha.push_back(copyPartition.getCellByVertex(v));
            copyPartition.refineGraph(G, alpha);
            refinementFunction(G,copyPartition,BetterPartition,bestInvariant,currentInvariant,allPermutations);
            alpha.pop_back();
            currentInvariant.pop_back();}
    }
}

NautyResult computeNauty(const Graph& mainGraph, bool storePerms = false) {
    Partition P(mainGraph.size());
    vector<Partition::Cell> alpha = P.getCells();
    P.refineGraph(mainGraph, alpha);
    Partition BestP = P;
    vector<vector<int>> bestInv;
    vector<vector<int>> currInv;
    vector<vector<int>> allPerms;
    vector<vector<int>>* ptrPerms = storePerms ? &allPerms : nullptr;
    refinementFunction(mainGraph,P,BestP,bestInv,currInv,ptrPerms);
    vector<int> perm = BestP.transformCellsToInt();
    Graph canonG = mainGraph.applyPermutation(perm);
    return {canonG, allPerms};
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage:\n";
        cout << "  iso graph6 graph6\n";
        cout << "  canonical graph6\n";
        cout << "  all graph6\n";
        return 1;}
    string mode = argv[1];
    if (mode == "iso") {
        if (argc < 4) {
            cout << "Usage: iso graph6 graph6\n";
            return 1;
        }
        string g1_str = argv[2];
        string g2_str = argv[3];
        auto res1 = computeNauty(Graph(g1_str));
        auto res2 = computeNauty(Graph(g2_str));
        if (res1.canonicalGraph.toGraph6() == res2.canonicalGraph.toGraph6()) cout << "Isomorphes" << endl; 
        else cout << "Non isomorphes" << endl;
    }
    else if (mode == "canonical" || mode == "all") {
        if (argc < 3) {
            cout << "Usage: canonical graph6\n";
            return 1;
        }
        string g_str = argv[2];
        Graph mainGraph(g_str);
        bool storePerms = (mode == "all");
        auto result = computeNauty(mainGraph, storePerms);
        result.canonicalGraph.printGraph6();
        if (mode == "all") {
            for (const auto& perm : result.allPermutations) {
                Graph gPerm = mainGraph.applyPermutation(perm);
                auto res = computeNauty(gPerm);
                res.canonicalGraph.printGraph6();
            }
        }
    }

    else cout << "Mode inconnu ! Utilise 'iso' ou 'canonical' ou 'all'" << endl;

    return 0;
}