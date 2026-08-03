#include "invariant.hpp"
#include <algorithm>
#include <unordered_set>
#include <iterator>

using namespace std;

vector<int> InvariantTriangleByCell(const Graph &G, const Partition &P){
    vector<Partition::Cell> Pcells = P.getCells();
    vector<int> triangle(Pcells.size(), 0);

    for (size_t i = 0; i < Pcells.size(); ++i) {
        const auto& verts = Pcells[i].verts;
        unordered_set<int> inCell(verts.begin(), verts.end());
        for (int v1 : verts) {
            const auto& N1 = G.getNeighbors(v1);
            for (int v2 : N1) {
                if (!inCell.count(v2) or v2 <= v1) continue;
                vector<int> common;
                set_intersection(
                    N1.begin(), N1.end(),
                    G.getNeighbors(v2).begin(), G.getNeighbors(v2).end(),
                    back_inserter(common)
                );
                for (int v3 : common) {
                    if (inCell.count(v3) and v3 > v2)
                        triangle[i]++;
                }
            }
        }
    }

    return triangle;
}


std::vector<int> InvariantTwoPathsByCell(const Graph& G, const Partition& P) {
    const auto& cells = P.getCells();
    int n = G.getN();
    std::vector<int> cellOf(n, -1);
    for (size_t c = 0; c < cells.size(); ++c) {
        for (int v : cells[c].verts) {
            cellOf[v] = static_cast<int>(c);
        }
    }
    std::vector<int> neighborCellSum(n, 0);
    for (int u = 0; u < n; ++u) {
        int sum = 0;
        for (int w : G.getNeighbors(u)) {
            sum += cellOf[w];
        }
        neighborCellSum[u] = sum;
    }
    std::vector<int> result;
    result.reserve(cells.size());
    for (const auto& cell : cells) {
        int total = 0;
        for (int v : cell.verts) {
            for (int u : G.getNeighbors(v)) {
                total += neighborCellSum[u];
            }
        }
        result.push_back(total);
    }


    return result;
}


std::vector<int> InvariantCellTriplesByCell(const Graph& G, const Partition& P) {
    const auto& cells = P.getCells();
    int n = G.getN();
    auto tripleWeight = [&](int a, int b, int c) {
        int w = 0;
        for (int x = 0; x < n; ++x) {
            if ((G.hasEdge(x, a) + G.hasEdge(x, b) + G.hasEdge(x, c)) % 2 == 1) {
                ++w;
            }
        }
        return w;
    };

std::vector<int> result(cells.size(), 0);
for (size_t u = 0; u < cells.size(); ++u) {
    const auto& verts = cells[u].verts;
    int k = static_cast<int>(verts.size());
    if (k < 3) continue;

    for (int i = 0; i < k; ++i)
        for (int j = i + 1; j < k; ++j)
            for (int l = j + 1; l < k; ++l) {
                int w = tripleWeight(verts[i], verts[j], verts[l]);
                result[u] += w;
            }
}

    return result;
}