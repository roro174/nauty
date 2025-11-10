#include "Partition.hpp"
#include <limits>

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
    constexpr size_t maxSizet{std::numeric_limits<size_t>::max()};
    size_t idx = maxSizet;
    for (size_t i = 0; i < cells.size(); ++i) {
        for (int x : cells[i].verts) {
            if (x == v) { idx = i; break; }
        }
        if (idx != maxSizet) break;
    }
    if (idx == maxSizet) throw std::runtime_error("Vertex not found in any cell");

    Cell& old = cells[idx];
    if (old.verts.size() == 1) return; // déjà singleton

    // Nouvelle cellule
    Cell newCell;
    newCell.id = CellId++;
    newCell.verts.push_back(v);

    old.verts.erase(std::remove(old.verts.begin(), old.verts.end(), v), old.verts.end());

    cells.insert(cells.begin() + idx, std::move(newCell));
    rebuildIndex();
    }

std::map<int, std::vector<int>> Partition::fragmentCellByCounts(const Graph &G, size_t cellIndex, const vector<int>& splitterVerts) const {
    const vector<int> &Xverts = cells[cellIndex].verts;
    std::map<int, std::vector<int>> groups;
    std::unordered_set<int> S(splitterVerts.begin(), splitterVerts.end());
    for (int v : Xverts) {
        int cnt = 0;
        for (int neighbor : G.getNeighbors(v))
            if (S.find(neighbor) != S.end()) ++cnt;
        groups[cnt].push_back(v);
    }
    return groups;
}

void Partition::applyFragmentation(size_t cellIndex, const std::map<int, std::vector<int>>& groups) {
    Cell &oldCell = cells[cellIndex];
    std::vector<Cell> newCells;
    for (const auto& [key, verts] : groups) {
        Cell newCell;
        newCell.id = CellId++;
        newCell.verts = verts;
        newCells.push_back(std::move(newCell));
    }
    cells.erase(cells.begin() + cellIndex);
    cells.insert(cells.begin() + cellIndex, newCells.begin(), newCells.end());
    rebuildIndex();
}


// surement lier les deux fonctions applyFragmentation
void Partition::applyFragmentationAlpha(size_t cellIndex, const std::map<int, std::vector<int>>& groups, vector<Cell> &alpha) {
    Cell &oldCell = alpha[cellIndex];
    std::vector<Cell> newCells;
    for (const auto& [key, verts] : groups) {
        Cell newCell;
        newCell.id = CellId++;
        newCell.verts = verts;
        newCells.push_back(std::move(newCell));
    }
    alpha.erase(alpha.begin() + cellIndex);
    alpha.insert(alpha.begin() + cellIndex, newCells.begin(), newCells.end());
}

void Partition::refineGraph(Graph &G, vector<Cell> &alpha) {
    while (!alpha.empty() && !isDiscrete()) {
        Cell W = alpha.front();
        size_t WIndex = indexOf(W.id);
        alpha.erase(alpha.begin());
        for (auto cell : cells) {
            size_t XIdx = indexOf(cell.id);
            std::map<int, std::vector<int>> fragments = fragmentCellByCounts(G, XIdx, W.verts);
            if (fragments.size() <= 1) continue;
            applyFragmentation(XIdx, fragments);
            bool inAlpha = false;
            size_t index = -1;
            for (const auto& c : alpha) {
                index++;
                if (c.id == XIdx) {
                    inAlpha = true;
                    break;
                }
            }
            if(inAlpha){
                applyFragmentationAlpha(index, fragments, alpha);
            }
            else{
                std::vector<Cell> newCells;
                for (const auto& [key, verts] : fragments) {
                    Cell newCell;
                    newCell.id = CellId++;
                    newCell.verts = verts;
                    newCells.push_back(std::move(newCell));
                }
                alpha.insert(alpha.end(), newCells.begin(), newCells.end());
                alpha.pop_back();
            }
        }
    }
}
