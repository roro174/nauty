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
        /*
    * Normalement ici la fonction est bonne
    */
    auto it = idToIndex.find(cellId);
    if (it == idToIndex.end()) throw runtime_error("Unknown cellId");
    return it->second;
}



void Partition::rebuildIndex() {
        /*
    * Normalement ici la fonction est bonne
    */
    idToIndex.clear();
    for (size_t i = 0; i < cells.size(); ++i) idToIndex[cells[i].id] = i;
}



bool Partition::isDiscrete() const {
    /*
    * Normalement ici la fonction est bonne
    */
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
    /*
    * Normalement ici la fonction est bonne
    */
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
    /*
    * Normalement ici la fonction est bonne
    */
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
    while (!alpha.empty() and !isDiscrete()) {
        Cell W = alpha.front();
        alpha.erase(alpha.begin());
        for (auto cell : cells) {
            size_t XIdx = indexOf(cell.id);
            std::map<int, std::vector<int>> fragments = fragmentCellByCounts(G, XIdx, W.verts);
            if (fragments.size() <= 1) continue;
            std::vector<Cell> newCells = applyFragmentation(XIdx, fragments);
            bool inAlpha = false;
            size_t index = -1;
            for (const auto& c : alpha) {
                index++;
                if (c.id == cell.id) {
                    inAlpha = true;
                    break;
                }
            }
            if(inAlpha){
                    alpha.erase(alpha.begin() + index);
                    alpha.insert(alpha.begin() + index, newCells.begin(), newCells.end());
            }
            else{
                auto it = std::max_element(newCells.begin(), newCells.end(),
                    [](const Cell &a, const Cell &b) {
                        return a.verts.size() < b.verts.size();
                    }
                );

                newCells.erase(it);
                alpha.insert(alpha.end(), newCells.begin(), newCells.end());
            }
        }
    }
}

const Partition::Cell& Partition::targetCellSelector() const {
    constexpr size_t maxSizet{std::numeric_limits<size_t>::max()};
    size_t minSize = maxSizet;
    size_t targetIndex = 0;
    for (size_t i = 0; i < cells.size(); ++i) {
        size_t cellSize = cells[i].verts.size();
        if (cellSize < minSize and cellSize != 1) {
            minSize = cellSize;
            targetIndex = i;
        }
    }
    return cells[targetIndex];
}

const Partition::Cell& Partition::getCellByVertex(int vert) const {
    for (const auto& cell : cells) {
        if (std::find(cell.verts.begin(), cell.verts.end(), vert) != cell.verts.end()) {
            return cell;
        }
    }
    throw std::runtime_error("Vertex not found in any cell");
}

const vector<int> Partition::InvariantTriangleByCell(const Graph &G) const {
    vector<int> triangle(cells.size(), 0);

    for (size_t i = 0; i < cells.size(); ++i) {
        const auto verts = cells[i].verts;
        int n = verts.size();

        // On parcourt tous les triplets (a,b,c) de sommets dans la cellule
        for (int x = 0; x < n; ++x) {
            int v1 = verts[x];
            for (int y = x + 1; y < n; ++y) {
                int v2 = verts[y];
                if (!G.hasEdge(v1, v2)) continue; // arête absente, pas de triangle

                for (int z = y + 1; z < n; ++z) {
                    int v3 = verts[z];
                    // Vérifie que les 3 arêtes existent
                    if (G.hasEdge(v1, v3) && G.hasEdge(v2, v3)) {
                        triangle[i]++;
                    }
                }
            }
        }
    }

    return triangle;
}


