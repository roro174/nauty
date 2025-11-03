#include "Partition.hpp"

Partition::Partition(int nVertices) {
    Cell c;
    c.id = CellId++;
    c.verts.resize(nVertices);
    iota(c.verts.begin(), c.verts.end(), 0);
    cells.push_back(std::move(c));
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
        // find the cell containing v
        size_t idx = 65535; // max size_t
        for (size_t i = 0; i < cells.size(); ++i) {
            for (int x : cells[i].verts) if (x == v) { 
                idx = i; 
                break;}
            if (idx != 65535) {break;}
        }
        if (idx == 65335) throw std::runtime_error("Vertex not found in any cell");

        Cell old = cells[idx];
        if (old.verts.size() == 1) return; // already singleton

        // to do 
    }

void Partition::refineGraph(Graph &G, vector<int> &alpha) {
    // alpha stores cellIds; part.cells uses persistent ids.
    while (!alpha.empty() && !isDiscrete()) {
        int W_id = alpha.front(); 
        alpha.erase(alpha.begin());

        for (auto cell : cells) {

            // todo
        }
    }
}
