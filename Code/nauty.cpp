#include "nauty.hpp"
#include "invariant.hpp"
#include <algorithm>


/**
 * @brief Internal helper functions used by the Nauty implementation.
 */
namespace {

/**
 * @brief Checks whether a permutation is an automorphism and inserts it into the group.
 *
 * @param G: the graph being processed.
 * @param referenceLeaf: the reference leaf partition(The first leaf).
 * @param perm: the permutation to check.
 * @param group: the group to insert the automorphism into.
 */
void checkAndInsertAutomorphism(const Graph& G, Partition& referenceLeaf,
                                 const vector<int>& perm, PermGroup& group) {
    auto reference = referenceLeaf.transformCellsToInt();
    if (G.applyPermutation(reference) == G.applyPermutation(perm)) {
        Perm sigma = composePerm(inversePerm(reference), perm);
        group.insertGenerator(sigma);
    }
}

/**
 * @brief: Handles a leaf node in the search tree: Verify if it's the first leaf, a better Invariant or an automorphism
 * @param  G: The graph being processed.
 * @param  currentPartition: The current partition at the leaf node.
 * @param  BetterPartition: The best partition found so far.(The one with the best invariant)
 * @param  bestInvariant:  The best invariant found so far.
 * @param  FirstLeaf: The first leaf partition found.
 * @param  firstLeafInvariant: The invariant of the first leaf partition.
 * @param  firstLeafFound: Boolean indicating whether the first leaf has been found.
 * @param  currentInvariant: The invariant of the current partition.
 * @param  allPermutations: A pointer to a vector storing all permutations explored during the search (if requested).
 * @param  group: The group of automorphisms found so far.
 * @param  baseVerts: The current base vertices used for orbit pruning.(The path from the root to the current leaf)
 */
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

/**
 * @brief: Determines whether the current branch of the search tree should be pruned based on the invariants of the current partition, the first leaf, and the best partition found so far.
 * @param  currentInvariant: The invariant of the current partition.(The path from the root to the current node)
 * @param  firstLeafInvariant: The invariant of the first leaf partition found.
 * @param  bestInvariant: The best invariant found so far.
 * @return: a boolean indicating whether the current branch should be pruned (true) or not (false).
 */
bool shouldPrune(const vector<vector<int>>& currentInvariant,
                  const vector<vector<int>>& firstLeafInvariant,
                  const vector<vector<int>>& bestInvariant) {
    int lvl = (int)currentInvariant.size() - 1;
    bool NotTiesWithFirst = (currentInvariant.back() != firstLeafInvariant[lvl]); //P_B
    bool worseThanBest = (currentInvariant.back() < bestInvariant[lvl]); // P_A
    return NotTiesWithFirst && worseThanBest;
}


/**
 * @brief Tests whether exploring a vertex is redundant using orbit pruning.
 *
 * @param group: The group of automorphisms found so far.
 * @param baseVerts: The current base vertices used for orbit pruning.(The path from the root to the current leaf)
 * @param cellVerts: The vertices in the current cell being explored.
 * @param v: The vertex being considered for exploration.
 * @param tried: The vertices that have already been tried in the current cell.
 * @return: A boolean indicating whether exploring vertex v is redundant (true) or not (false).
 */
bool isRedundantByOrbit(const PermGroup& group, const vector<int>& baseVerts,
                         const vector<int>& cellVerts, int v, const vector<int>& tried) {// P_C
    Perm g;
    if (!group.findPathRepresentative(baseVerts, g)) return false;
    Perm gInv = inversePerm(g);

    vector<int> translatedCell;
    translatedCell.reserve(cellVerts.size());
    for (int x : cellVerts) translatedCell.push_back(gInv[x]);

    auto orbits = group.orbitPartitionAtLevel(static_cast<int>(baseVerts.size()), translatedCell);

    int vImg = gInv[v];
    for (const auto& orb : orbits) {
        if (std::find(orb.begin(), orb.end(), vImg) == orb.end()) continue;
        for (int t : tried) {
            int tImg = gInv[t];
            if (std::find(orb.begin(), orb.end(), tImg) != orb.end()) return true;
        }
        return false;
    }
    return false;
}


/**
 * @brief Recursively explores the search tree.
 * @note: It's the main function of the Nauty algorithm, implementing the backtracking search with pruning and automorphism detection.
 * @param G: The graph being processed.
 * @param currentPartition: The current partition at the node being explored.
 * @param BetterPartition: The best partition found so far.(The one with the best invariant)
 * @param bestInvariant: The best invariant found so far.
 * @param FirstLeaf: The first leaf partition found.
 * @param firstLeafInvariant: The invariant of the first leaf partition.
 * @param firstLeafFound: Boolean indicating whether the first leaf has been found.
 * @param currentInvariant: The invariant of the current partition.
 * @param allPermutations: A pointer to a vector storing all permutations explored during the search (if requested).
 * @param group: The group of automorphisms found so far.
 * @param baseVerts: The current base vertices used for orbit pruning.(The path from the root to the current leaf)
 * @param depth: The current depth in the search tree.
 */
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
    if (firstLeafFound and isRedundantByOrbit(group, baseVerts, targetCell.verts, v, tried)) {
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
}}
}

NautyResult preNauty(const Graph& mainGraph, bool storePerms) {
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