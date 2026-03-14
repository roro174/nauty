#ifndef PARTITION_HPP
#define PARTITION_HPP

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

struct Partition {
    struct Cell {
        int id;
        std::vector<int> verts;

    bool operator<(const Cell& other) const {
        return verts < other.verts;
    }
    void print() const {
        std::cout << "Cell id=" << id << " size=" << verts.size() << " : ";
        for (int v : verts) std::cout << v << " ";
        std::cout << "\n";
    };
    };

    std::vector<Cell> cells; 
    unordered_map<int, size_t> idToIndex;               
    int CellId = 0;                    

    explicit Partition(int nVertices);

    bool isDiscrete() const;

    size_t indexOf(int cellId) const;

    void rebuildIndex();

    void print() const;

    void individualizeVertex(int v);

    std::map<int, std::vector<int>> fragmentCellByCounts(const Graph &G, size_t cellIndex, const vector<int> splitterVerts) const;

    std::vector<Cell> applyFragmentation(size_t cellIndex, const std::map<int, std::vector<int>>& groups);

    void refineGraph(const Graph &G, vector<Cell> alpha);

    const Cell& targetCellSelector() const;

    const Cell& getCellByVertex(int vert) const;

    const vector<int> InvariantTriangleByCell(const Graph &G) const;

    const vector<int> transformCellsToInt() const;
};

#endif
