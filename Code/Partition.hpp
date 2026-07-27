#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <map>
#include <stdexcept>
#include <deque>
#include <iostream>
#include <cstddef>
#include <unordered_set>
#include "Graph.hpp"

/**
 * @brief Class representing a partition(colouring) of the vertices of a graph. It contains all the necessary methods to refine the partition and individualize vertices.
 */
struct Partition {
public:

    /**
     * @brief Struct representing a cell of the partition. It contains the id of the cell and the vertices that belong to it.
     */
    struct Cell {
        int id;
        std::vector<int> verts;

        /**
         * @brief: Prints the cell id and the vertices that belong to it.(only for debugging purposes)
         */
        void print() const {
            std::cout << "Cell id=" << id << " size=" << verts.size() << " : ";
            for (int v : verts) std::cout << v << " ";
            std::cout << "\n";
        };
    };

private:
    std::vector<Cell> cells;
    unordered_map<int, size_t> idToIndex;
    vector<int> vertexToCellId;
    int CellId = 0;

public:

    /**
     * @brief: Constructs a partition with nVertices vertices in only one cell.
     * @param nVertices
     */
    explicit Partition(int nVertices);

    /**
     * @brief: Checks if the partition is discrete, meaning that all cells are singletons.
     * @return: True if the partition is discrete, false otherwise.
     */
    bool isDiscrete() const;

    /**
     * @brief: Returns the index of the cell with the given id in the cells vector.
     * @note: We use an index because the cells vector can be modified during the refinement process, so we need a way to quickly find the index of a cell by its id. 
     * @param cellId: The id of the cell to find the index for.
     * @return: The index of the cell with the given id in the cells vector.
     */
    size_t indexOf(int cellId) const;

    /**
     * @brief: Rebuilds the idToIndex and vertexToCellId maps after the cells vector has been modified. This is necessary to keep the maps consistent with the current state of the cells vector.
     */
    void rebuildIndex();

    /**
     * @brief: Prints the partition, showing the id and size of each cell and the vertices that belong to it. (only for debugging purposes)
     */
    void print() const;

    /**
     * @brief: Individualizes a vertex v by creating a new cell containing only v and removing v from its original cell. It updates the partition accordingly.
     * @param v: The vertex to be individualized.
     */
    void individualizeVertex(int v);

    /**
     * @brief: Fragments a cell into new cells based on the counts of neighbors in the splitter cell. It returns a map where the key is the count of neighbors in the splitter cell and the value is a vector of vertices that have that count. Each entry in the map represents a new cell to be created.
     * @param G: The graph to be used for counting neighbors.
     * @param cellIndex: The index of the cell to be fragmented in the partition.
     * @param splitterVerts: A vector of vertices that are used to count neighbors in the cell to be fragmented.
     * @return: A map where the key is the count of neighbors in the splitter cell and the value is a vector of vertices that have that count. Each entry in the map represents a new cell to be created.
     */
    std::map<int, std::vector<int>> fragmentCellByCounts(
        const Graph& G,
        size_t cellIndex,
        const vector<int> splitterVerts
    ) const;

    /**
     * @brief: Applies the fragmentation of a cell into new cells based on the provided groups. It replaces the original cell with the new cells in the partition.
     * @param cellIndex: The index of the cell to be fragmented in the partition.
     * @param groups: A map where the key is the count of neighbors in the splitter cell and the value is a vector of vertices that have that count. Each entry in the map represents a new cell to be created.
     * @return: A vector of the new cells created from the fragmentation.
     */
    std::vector<Cell> applyFragmentation(
        size_t cellIndex,
        const std::map<int, std::vector<int>>& groups
    );

    /**
     * @brief: Refinement algorithm as prescribed in the Nauty paper. It refines the partition by splitting cells based on their adjacency to the vertices in the cells of alpha.
     * @param G: The graph to be refined.
     * @param alpha: A vector of cells that are used to refine the partition. 
     */
    void refineGraph(const Graph& G, vector<Cell> alpha);

    /**
     * @brief: Selects the target cell for individualization. The target cell is the first non-singleton cell of minimum size in the partition.
     * @return: A const reference to the target cell.
     */
    const Cell& targetCellSelector() const;

    /**
     * @brief: get the cell of the partition that contains the vertex v.
     * @param vert: The vertex for which we want to find the cell.
     * @return: A const reference to the cell that contains the vertex v.
     */
    const Cell& getCellByVertex(int vert) const;

    /**
     * @brief: transforms the cells of the partition into a vector of integers, where each integer represents the first vertex of each cell.
     * note: this function is used only when the partition is discrete, meaning that all cells are singletons.
     * @return: A vector of integers representing the first vertex of each cell.
     */
    const vector<int> transformCellsToInt() const;

    /**
     * @brief: Returns the cells of the partition.
     * @return: A const reference to the vector of cells.
     */
    const std::vector<Cell>& getCells() const { return cells; }
};