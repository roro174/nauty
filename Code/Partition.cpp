#include "Partition.hpp"

Partition::Partition(int nVertices) {
    Cell c;
    c.id = CellId++;
    c.verts.resize(nVertices);
    iota(c.verts.begin(), c.verts.end(), 0);
    idToIndex[c.id] = cells.size();
    cells.push_back(std::move(c));
}

size_t Partition::indexOf(int cellId) const {
    auto it = idToIndex.find(cellId);
    if (it == idToIndex.end()) 
        throw std::overflow_error("Unknown cellId");
    return it->second;
}

void Partition::rebuildIndex() {
    idToIndex.clear();
    for (size_t i = 0; i < cells.size(); ++i)
        idToIndex[cells[i].id] = i;
}

bool Partition::isDiscrete() const {
    for (auto &c : cells)
        if (c.verts.size() > 1)
            return false;
    return true;
}
