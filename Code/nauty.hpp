#pragma once

#include "Graph.hpp"
#include "Partition.hpp"
#include "bsgs.hpp"
#include <vector>

using InvariantFunction = std::vector<int>(*)(const Graph&, const Partition&);

/**
 * @brief Stores the result of the Nauty algorithm.
 */
struct NautyResult {
    Graph canonicalGraph;
    std::vector<std::vector<int>> allPermutations;
    PermGroup automorphisms;
};

/**
 * @brief Computes the canonical form and automorphism group of a graph.
 * @param mainGraph Graph to process.
 * @param storePerms Whether to store all leaf permutations explored during the search.
 * @return Result containing the canonical graph, explored permutations (if requested),
 *         and the automorphism group.
 */
NautyResult preNauty(const Graph& mainGraph, InvariantFunction invariant, bool storePerms = false);