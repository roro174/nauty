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

    explicit Partition(int nVertices) { // Tout en un cell(pi_0)
        Cell c;
        c.id = CellId++;
        c.verts.resize(nVertices);
        iota(c.verts.begin(), c.verts.end(), 0);
        idToIndex[c.id] = cells.size();
        cells.push_back(std::move(c));
    }

        size_t indexOf(int cellId) const {
        auto it = idToIndex.find(cellId);
        if (it == idToIndex.end()) throw std::overflow_error("Unknown cellId");
        return it->second;
    }

    void rebuildIndex() {
        idToIndex.clear();
        for (size_t i = 0; i < cells.size(); ++i) idToIndex[cells[i].id] = i;
    }

    bool isDiscrete() const {
        for (auto &c : cells) if (c.verts.size() > 1) return false;
        return true;
    }
};