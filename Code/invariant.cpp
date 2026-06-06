#include "Partition.hpp"
#include "Graph.hpp"

const vector<int> InvariantTriangleByCell(const Graph &G, const Partition &P){
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