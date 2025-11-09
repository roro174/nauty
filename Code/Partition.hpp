#ifndef PARTITION_HPP
#define PARTITION_HPP

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <deque>
#include <iostream>
#include <cstddef>
#include "Graph.hpp"

struct Partition {
    struct Cell {
        int id;
        std::vector<int> verts;
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

    void refineGraph(Graph &G, vector<int> &alpha);

};

#endif
