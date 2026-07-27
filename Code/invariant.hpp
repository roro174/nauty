#pragma once

#include "Graph.hpp"
#include "Partition.hpp"
#include <vector>


/**
 * @brief: InvariantTriangleByCell computes the number of triangles in each cell of a partition of a graph.
 * @param  G: The graph where triangles are to be counted.
 * @param  P: The partition of the graph into cells.
 * @return: A vector of integers where each integer represents the number of triangles in the corresponding cell of the partition.
 */
std::vector<int> InvariantTriangleByCell(const Graph& G, const Partition& P);
