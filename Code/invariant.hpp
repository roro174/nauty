#pragma once

#include "Graph.hpp"
#include "Partition.hpp"
#include <vector>


/**
 * @brief: InvariantTriangleByCell computes the number of triangles in each cell of a partition of a graph.
 * @param  G: The graph.
 * @param  P: The partition of the graph into cells.
 * @return: A vector of integers where each integer represents the number of triangles in the corresponding cell of the partition.
 */
std::vector<int> InvariantTriangleByCell(const Graph& G, const Partition& P);


/**
 * @brief: InvariantTwoPathsByCell computes for each cell of a partition of a graph, the sum of idCell where the idCell represents the cell index of the 2-neighbors of the vertices in that cell. 
 * @param  G: The graph.
 * @param  P: The partition of the graph into cells.
 * @return: A vector of integers where each integer represents the two-paths invariant of the corresponding cell of the partition.
 */
std::vector<int> InvariantTwoPathsByCell(const Graph& G, const Partition& P);

/**
 * @brief: InvariantCellTriplesByCell implements nauty's celltrips invariant. See Invariant triple in the Nauty user guide for further details.
 * @param  G: The graph.
 * @param  P: The partition of the graph into cells.
 * @return: A vector of integers, one per cell.
 */
std::vector<int> InvariantCellTriplesByCell(const Graph& G, const Partition& P);