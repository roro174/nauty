#ifndef PARTITION_HPP
#define PARTITION_HPP

#include <vector>
#include <unordered_map>
#include <numeric>
#include <stdexcept>

struct Partition {
    struct Cell {
        int id;
        std::vector<int> verts;
    };

    std::vector<Cell> cells;                   
    std::unordered_map<int, size_t> idToIndex;  
    int CellId = 0;                    

    explicit Partition(int nVertices);

    size_t indexOf(int cellId) const;

    void rebuildIndex();

    bool isDiscrete() const;
};

#endif
