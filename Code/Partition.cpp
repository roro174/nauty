#include "Partition.hpp"

Partition::Partition(int nVertices) {
    Cell c;
    c.id = CellId++;
    c.verts.resize(nVertices);
    iota(c.verts.begin(), c.verts.end(), 0);
    idToIndex[c.id] = 0;
    cells.push_back(move(c));
}

size_t Partition::indexOf(int cellId) const {
    auto it = idToIndex.find(cellId);
    if (it == idToIndex.end()) throw runtime_error("Unknown cellId");
    return it->second;
}

void Partition::rebuildIndex() {
    idToIndex.clear();
    for (size_t i = 0; i < cells.size(); ++i) idToIndex[cells[i].id] = i;
}



bool Partition::isDiscrete() const {
    for (auto &c : cells)
        if (c.verts.size() > 1)
            return false;
    return true;
};

void Partition::print() const {
    std::cout << "Partition (cells = " << cells.size() << "):\n";
    for (auto &c : cells) {
        std::cout << "  id=" << c.id << " size=" << c.verts.size() << " : ";
        for (int v : c.verts) std::cout << v << " ";
        std::cout << "\n";
    }
}

void Partition::individualizeVertex(int v) {
 size_t idx = 65535;
    for (size_t i = 0; i < cells.size(); ++i) {
        for (int x : cells[i].verts) {
            if (x == v) { idx = i; break; }
        }
        if (idx != 65535) break;
    }
    if (idx == 65535) throw std::runtime_error("Vertex not found in any cell");

    Cell& old = cells[idx]; 
    if (old.verts.size() == 1) return; // déjà singleton

    // Nouvelle cellule
    Cell newCell;
    newCell.id = CellId++;
    newCell.verts.push_back(v);

    old.verts.erase(std::remove(old.verts.begin(), old.verts.end(), v), old.verts.end());

    cells.insert(cells.begin() + idx, std::move(newCell));
    rebuildIndex();
    return;
    }

void Partition::refineGraph(Graph &G, vector<int> &alpha) {
    while (!alpha.empty() && !isDiscrete()) {
        int W_id = alpha.front(); 
        alpha.erase(alpha.begin());

        for (auto cell : cells) {

            // todo
        }
    }
}
