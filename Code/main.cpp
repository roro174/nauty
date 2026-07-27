#include "nauty.hpp"
#include <iostream>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage:\n";
        cout << "  iso graph6 graph6\n";
        cout << "  canonical graph6\n";
        cout << "  all graph6\n";
        cout << "  aut graph6\n";
        return 1;}
    string mode = argv[1];
    if (mode == "iso") { //only compare two graphs to see if they are isomorphes
        if (argc < 4) {
            cout << "Usage: iso graph6 graph6\n";
            return 1;
        }
        string g1_str = argv[2];
        string g2_str = argv[3];
        auto res1 = preNauty(Graph(g1_str));
        auto res2 = preNauty(Graph(g2_str));
        if (res1.canonicalGraph.toGraph6() == res2.canonicalGraph.toGraph6()) cout << "Isomorphes" << endl; 
        else cout << "Non isomorphes" << endl;
    }
    else if (mode == "canonical" || mode == "all") {// to see the canonical form of a graph, or all canonical forms of all permutations
        if (argc < 3) {
            cout << "Usage: canonical graph6\n";
            return 1;
        }
        string g_str = argv[2];
        Graph mainGraph(g_str);
        bool storePerms = (mode == "all");
        auto result = preNauty(mainGraph, storePerms);
        result.canonicalGraph.printGraph6();
        if (mode == "all") {//all permutations of the canonical form
            for (const auto& perm : result.allPermutations) {
                Graph gPerm = mainGraph.applyPermutation(perm);
                auto res = preNauty(gPerm);
                res.canonicalGraph.printGraph6();
            }
        }
    }
    else if (mode == "aut") {// to get the automorphisms of a graph
        if (argc < 3) {
            cout << "Usage: aut graph6\n";
            return 1;
        }
        Graph mainGraph(argv[2]);
        auto result = preNauty(mainGraph);
        result.canonicalGraph.printGraph6();
result.automorphisms.print(cout);

auto group = result.automorphisms.enumerateGroup();

cout << "\nToutes les permutations (" << group.size() << ") :\n";

for (const auto& p : group) {
    cout << "[";
    for (size_t i = 0; i < p.size(); ++i) {
        cout << p[i];
        if (i + 1 < p.size()) cout << ",";
    }
    cout << "]\n";
}
    }

    else cout << "Mode inconnu ! Utilise 'iso', 'canonical', 'all' ou 'aut'" << endl;

    return 0;
}