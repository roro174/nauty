#include "Graph.hpp"
#include "Partition.hpp"
#include "bsgs.hpp"
#include "invariant.cpp"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct NautyResult {
    Graph canonicalGraph;
    vector<vector<int>> allPermutations;
    PermGroup automorphisms;
};



bool isOnBasePath(const vector<int>& baseVerts, const PermGroup& group) {
    return std::equal(baseVerts.begin(), baseVerts.end(), group.getBase().begin());
}


void checkAndInsertAutomorphism(const Graph& G, Partition& referenceLeaf,
                                 const vector<int>& perm, PermGroup& group) {
    auto reference = referenceLeaf.transformCellsToInt();
    if (G.applyPermutation(reference) == G.applyPermutation(perm)) {
        Perm sigma = composePerm(inversePerm(reference), perm);
        group.insertGenerator(sigma);
    }
}


void handleLeaf(const Graph& G, Partition& currentPartition,
                 Partition& BetterPartition, vector<vector<int>>& bestInvariant,
                 Partition& FirstLeaf, vector<vector<int>>& firstLeafInvariant, bool& firstLeafFound,
                 vector<vector<int>>& currentInvariant,
                 vector<vector<int>>* allPermutations,
                 PermGroup& group, vector<int>& baseVerts) {

    auto perm = currentPartition.transformCellsToInt();
    if (allPermutations != nullptr) allPermutations->push_back(perm);

    if (!firstLeafFound) {
        FirstLeaf = currentPartition;
        firstLeafInvariant = currentInvariant;
        firstLeafFound = true;
        BetterPartition = currentPartition;      
        bestInvariant = currentInvariant;
        group.setBase(baseVerts);
        return;
    }

    if (bestInvariant < currentInvariant) {
        BetterPartition = currentPartition;
        bestInvariant = currentInvariant;
    } else if (bestInvariant == currentInvariant) {
        checkAndInsertAutomorphism(G, BetterPartition, perm, group);
    }

    if (currentInvariant == firstLeafInvariant) {
        checkAndInsertAutomorphism(G, FirstLeaf, perm, group);
    }
}


bool shouldPrune(const vector<vector<int>>& currentInvariant,
                  const vector<vector<int>>& firstLeafInvariant,
                  const vector<vector<int>>& bestInvariant) {
    int lvl = (int)currentInvariant.size() - 1;
    bool NotTiesWithFirst = (currentInvariant.back() != firstLeafInvariant[lvl]); //P_B
    bool worseThanBest = (currentInvariant.back() < bestInvariant[lvl]); // P_A
    return NotTiesWithFirst && worseThanBest;
}


bool isRedundantByOrbit(PermGroup& group, int depth, const vector<int>& cellVerts,
                         int v, const vector<int>& tried) {// P_C
    auto orbits = group.orbitPartitionAtLevel(depth, cellVerts); 
    for (const auto& orb : orbits) {
        if (std::find(orb.begin(), orb.end(), v) == orb.end()) continue;
        for (int t : tried) {
            if (std::find(orb.begin(), orb.end(), t) != orb.end()) return true;
        }
        return false;
    }
    return false;
}

void generalNauty(const Graph& G, Partition& currentPartition, Partition& BetterPartition,
                   vector<vector<int>>& bestInvariant,
                   Partition& FirstLeaf, vector<vector<int>>& firstLeafInvariant, bool& firstLeafFound,
                   vector<vector<int>>& currentInvariant,
                   vector<vector<int>>* allPermutations,
                   PermGroup& group, vector<int>& baseVerts,
                   int depth) {

    currentInvariant.push_back(InvariantTriangleByCell(G, currentPartition));

    if (currentPartition.isDiscrete()) {
        handleLeaf(G, currentPartition, BetterPartition, bestInvariant,
FirstLeaf, firstLeafInvariant, firstLeafFound,
                   currentInvariant, allPermutations, group, baseVerts);
        return;
    }

    if (firstLeafFound && shouldPrune(currentInvariant, firstLeafInvariant, bestInvariant)) {
        return;
    }

    Partition::Cell targetCell = currentPartition.targetCellSelector();
    vector<int> tried;

for (int v : targetCell.verts) {
    if (firstLeafFound and isOnBasePath(baseVerts, group) and isRedundantByOrbit(group, depth, targetCell.verts, v, tried)) {
        continue;
    }
    tried.push_back(v);

    Partition copyPartition = currentPartition;
    copyPartition.individualizeVertex(v);
    copyPartition.refineGraph(G, {copyPartition.getCellByVertex(v)});

    baseVerts.push_back(v);
    generalNauty(G, copyPartition, BetterPartition, bestInvariant,
                 FirstLeaf, firstLeafInvariant, firstLeafFound,
                 currentInvariant, allPermutations, group, baseVerts, depth + 1);
    baseVerts.pop_back();
    currentInvariant.pop_back();
}
}

NautyResult preNauty(const Graph& mainGraph, bool storePerms = false) {
    Partition P(mainGraph.size());
    vector<Partition::Cell> alpha = P.getCells();
    P.refineGraph(mainGraph, alpha);
    Partition BestP = P;
    vector<vector<int>> bestInv;

    Partition FirstLeaf = P;
    vector<vector<int>> firstLeafInvariant;
    bool firstLeafFound = false;

    vector<vector<int>> currInv;
    vector<vector<int>> allPerms;
    vector<vector<int>>* ptrPerms = storePerms ? &allPerms : nullptr;

    PermGroup group(mainGraph.size());
    vector<int> baseVerts;

    generalNauty(mainGraph, P, BestP, bestInv,
                 FirstLeaf, firstLeafInvariant, firstLeafFound,
                 currInv, ptrPerms, group, baseVerts, 0);

    vector<int> perm = BestP.transformCellsToInt();
    Graph canonG = mainGraph.applyPermutation(perm);
    return {canonG, allPerms, group};
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage:\n";
        cout << "  iso graph6 graph6\n";
        cout << "  canonical graph6\n";
        cout << "  all graph6\n";
        cout << "  aut graph6\n";
        return 1;}
    string mode = argv[1];
    if (mode == "iso") {
        if (argc < 4) {
            cout << "Usage: iso graph6 graph6\n";
            return 1;
        }
        string g1_str = argv[2];
        string g2_str = argv[3];
        auto res1 = preNauty(Graph(g1_str));
        auto res2 = preNauty(Graph(g2_str));
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
        auto result = preNauty(mainGraph, storePerms);
        result.canonicalGraph.printGraph6();
        if (mode == "all") {
            for (const auto& perm : result.allPermutations) {
                Graph gPerm = mainGraph.applyPermutation(perm);
                auto res = preNauty(gPerm);
                res.canonicalGraph.printGraph6();
            }
        }
    }
    else if (mode == "aut") {
        if (argc < 3) {
            cout << "Usage: aut graph6\n";
            return 1;
        }
        Graph mainGraph(argv[2]);
        auto result = preNauty(mainGraph);
        result.canonicalGraph.printGraph6();
result.automorphisms.print(cout);

auto group = result.automorphisms.enumerateGroup();

cout << "\nToutes les permutations (" << group.size() << ") :\n";

for (const auto& p : group) {
    cout << "[";
    for (size_t i = 0; i < p.size(); ++i) {
        cout << p[i];
        if (i + 1 < p.size()) cout << ",";
    }
    cout << "]\n";
}
    }

    else cout << "Mode inconnu ! Utilise 'iso', 'canonical', 'all' ou 'aut'" << endl;

    return 0;
}