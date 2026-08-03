#include "nauty.hpp"
#include "invariant.hpp"
#include <iostream>
#include <fstream>

using namespace std;

InvariantFunction getInvariant(int argc, char* argv[], int index){
    if (argc < index + 1)
        return InvariantTriangleByCell;

    string inv = argv[index];

    if (inv == "triangle")
        return InvariantTriangleByCell;
    if (inv == "paths")
        return InvariantTwoPathsByCell;
    if (inv == "triple")
        return InvariantCellTriplesByCell;

    cout << "Invariant inconnu, utilisation de 'triangle' par défaut.\n";
    return InvariantTriangleByCell;
}

void runIso(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Usage: iso graph6 graph6 [triangle|paths|triple]\n";
        return;
    }

    InvariantFunction inv = getInvariant(argc, argv, 4);

    Graph g1(argv[2]);
    Graph g2(argv[3]);

    auto res1 = preNauty(g1, inv);
    auto res2 = preNauty(g2, inv);

    if (res1.canonicalGraph.toGraph6() == res2.canonicalGraph.toGraph6())
        cout << "Isomorphes\n";
    else
        cout << "Non isomorphes\n";
}

void runCanonical(int argc, char* argv[], bool all) {
    if (argc < 3) {
        cout << "Usage: canonical graph6 [triangle|paths|triple]\n";
        return;
    }
    InvariantFunction inv = getInvariant(argc, argv, 3);
    Graph g(argv[2]);
    auto result = preNauty(g, inv, all);
    result.canonicalGraph.printGraph6();
    if (!all)
        return;
    for (const auto& perm : result.allPermutations) {
        Graph gPerm = g.applyPermutation(perm);
        auto res = preNauty(gPerm, inv);
        res.canonicalGraph.printGraph6();
    }
}

void runAut(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: aut graph6 [triangle|paths|triple]\n";
        return;
    }
    InvariantFunction inv = getInvariant(argc, argv, 3);
    Graph g(argv[2]);
    auto result = preNauty(g, inv);
    result.canonicalGraph.printGraph6();
    result.automorphisms.print(cout);
    auto group = result.automorphisms.enumerateGroup();
    cout << "\nToutes les permutations (" << group.size() << ") :\n";
    for (const auto& p : group) {
        cout << "[";
        for (size_t i = 0; i < p.size(); ++i) {
            cout << p[i];
            if (i + 1 < p.size())
                cout << ",";
        }
        cout << "]\n";
    }
}

void runFile(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Usage: file fichier_entree fichier_sortie [triangle|paths|triple]\n";
        return;
    }

    InvariantFunction inv = getInvariant(argc, argv, 4);

    ifstream in(argv[2]);
    if (!in) {
        cerr << "Impossible d'ouvrir le fichier d'entrée : " << argv[2] << "\n";
        return;
    }

    ofstream out(argv[3]);
    if (!out) {
        cerr << "Impossible de créer le fichier de sortie : " << argv[3] << "\n";
        return;
    }

    string line;
    int numLigne = 0;
    int numTraites = 0;

    while (getline(in, line)) {
        ++numLigne;

        while (!line.empty() && isspace((unsigned char)line.back()))
            line.pop_back();

        if (line.empty())
            continue; 

        try {
            Graph g(line);
            auto result = preNauty(g, inv);
            out << result.canonicalGraph.toGraph6() << "\n";
            ++numTraites;
        } catch (const std::exception& e) {
            cerr << "Erreur ligne " << numLigne << " (\"" << line << "\") : " << e.what() << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage:\n";
        cout << "  iso graph6 graph6 [triangle|paths|triple]\n";
        cout << "  canonical graph6 [triangle|paths|triple]\n";
        cout << "  all graph6 [triangle|paths|triple]\n";
        cout << "  aut graph6 [triangle|paths|triple]\n";
        cout << "  file fichier_entree fichier_sortie [triangle|paths|triple]\n";
        return 1;
    }

    string mode = argv[1];

    if (mode == "iso") {
        runIso(argc, argv);
    }
    else if (mode == "canonical") {
        runCanonical(argc, argv, false);
    }
    else if (mode == "all") {
        runCanonical(argc, argv, true);
    }
    else if (mode == "aut") {
        runAut(argc, argv);
    }
    else if (mode == "file") {
        runFile(argc, argv);
    }
    else {
        cout << "Mode inconnu ! Utilise 'iso', 'canonical', 'all', 'aut' ou 'batch'.\n";
        return 1;
    }

    return 0;
}