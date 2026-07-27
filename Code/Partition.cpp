#include "Partition.hpp"
#include <limits>

Partition::Partition(int nVertices) {
    vertexToCellId.resize(nVertices);
    Cell c;
    c.id = CellId++;
    c.verts.resize(nVertices);
    iota(c.verts.begin(), c.verts.end(), 0); // Fill with 0, 1, ..., nVertices-1
    idToIndex[c.id] = 0;
    for(int v : c.verts)
        vertexToCellId[v] = c.id;
    cells.push_back(move(c));
}

size_t Partition::indexOf(int cellId) const {
    auto it = idToIndex.find(cellId);
    if (it == idToIndex.end()) throw runtime_error("Unknown cellId");
    return it->second;
}



void Partition::rebuildIndex() {
    idToIndex.clear();
    vertexToCellId.assign(vertexToCellId.size(), -1);
    for (size_t i = 0; i < cells.size(); ++i) {
        idToIndex[cells[i].id] = i;
        for (int v : cells[i].verts)
            vertexToCellId[v] = cells[i].id;
    }
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
    int cellId = vertexToCellId[v];
    size_t idx = indexOf(cellId);
    Cell& old = cells[idx];
    if (old.verts.size() == 1) return; // déjà singleton

    // New cell for the individualized vertex
    Cell newCell;
    newCell.id = CellId++;
    newCell.verts.push_back(v);

    old.verts.erase(std::remove(old.verts.begin(), old.verts.end(), v), old.verts.end());

    cells.insert(cells.begin() + idx, std::move(newCell));
    rebuildIndex();
    }

std::map<int, std::vector<int>> Partition::fragmentCellByCounts(const Graph &G, size_t cellIndex, const vector<int> splitterVerts) const {
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

std::vector<Partition::Cell> Partition::applyFragmentation(size_t cellIndex, const std::map<int, std::vector<int>>& groups) {
    std::vector<Cell> newCells;
    for (const auto& [_, verts] : groups) {
        Cell newCell;
        newCell.id = CellId++;
        newCell.verts = verts;
        newCells.push_back(std::move(newCell));
    }
    cells.erase(cells.begin() + cellIndex);
    cells.insert(cells.begin() + cellIndex, newCells.begin(), newCells.end());
    rebuildIndex();
    return newCells;
}

void Partition::refineGraph(const Graph &G, vector<Cell> alpha) {
    while (!alpha.empty() && !isDiscrete()) {
        Cell W = alpha.front();
        alpha.erase(alpha.begin());
        vector<int> cellIds;
        for (const auto& c : cells) cellIds.push_back(c.id);
        for (int cellId : cellIds) { // pass by value to avoid invalidating the iterator
            size_t XIdx = indexOf(cellId);
            std::map<int, std::vector<int>> fragments = fragmentCellByCounts(G, XIdx, W.verts);

            if (fragments.size() <= 1) continue;

            std::vector<Cell> newCells = applyFragmentation(XIdx, fragments);
            bool inAlpha = false;
            size_t index = 0;
            for (const auto& c : alpha) {
                if (c.id == cellId) {
                    inAlpha = true;
                    break;
                }
                index++;
            }

            if (inAlpha) {
                alpha.erase(alpha.begin() + index);
                alpha.insert(alpha.begin() + index, newCells.begin(), newCells.end());
            } else {
                // add all new cells except the largest one to alpha
                auto it = std::max_element(newCells.begin(), newCells.end(),
                                           [](const Cell &a, const Cell &b) {
                                               return a.verts.size() < b.verts.size();
                                           });
                std::copy_if(newCells.begin(), newCells.end(),
                             std::back_inserter(alpha),
                             [&](const auto& cell){ return &cell != &(*it); });
            }
        }
    }
}

const Partition::Cell& Partition::targetCellSelector() const {
    auto it = std::min_element(cells.begin(), cells.end(),
        [](const Cell &a, const Cell &b) {
            if (a.verts.size() <= 1) return false;
            if (b.verts.size() <= 1) return true;
            return a.verts.size() < b.verts.size();
        });
    return *it;
}

const Partition::Cell& Partition::getCellByVertex(int vert) const {
    int cellId = vertexToCellId[vert];
    size_t idx = indexOf(cellId);
    return cells[idx];    
}

const vector<int> Partition::transformCellsToInt() const {
    vector<int> result;
    for (const auto& cell : cells) {
        result.push_back(cell.verts.at(0));
    }
    return result;
}
