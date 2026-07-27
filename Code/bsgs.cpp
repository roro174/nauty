#include "bsgs.hpp"


Perm identityPerm(int n) {
    Perm p(n);
    for (int i = 0; i < n; ++i) p[i] = i;
    return p;
}

Perm composePerm(const Perm& g, const Perm& h) {
    Perm r(g.size());
    for (size_t x = 0; x < g.size(); ++x) r[x] = h[g[x]];
    return r;
}

Perm inversePerm(const Perm& p) {
    Perm r(p.size());
    for (size_t x = 0; x < p.size(); ++x) r[p[x]] = static_cast<int>(x);
    return r;
}

bool isIdentityPerm(const Perm& p) {
    for (size_t x = 0; x < p.size(); ++x)
        if (p[x] != static_cast<int>(x)) return false;
    return true;
}

PermGroup::PermGroup(int n) : n(n) {}

void PermGroup::setBase(const std::vector<int>& b) {
    base = b;
    strongGens.assign(base.size(), {});
    orbit.assign(base.size(), {});
    transversal.assign(base.size(), {});
    for (size_t i = 0; i < base.size(); ++i) {
        orbit[i] = {base[i]};
        transversal[i][base[i]] = identityPerm(n);
    }
}

void PermGroup::insertGenerator(const Perm& g) {
    if (g.empty() || isIdentityPerm(g)) return;
    insertGeneratorAt(g, 0);
}

void PermGroup::insertGeneratorAt(const Perm& g, int level) {
    if (level == static_cast<int>(base.size())) {
        if (!isIdentityPerm(g)) extendBase(g);
        return;
    }

    Perm h = g;
    for (int i = level; i < static_cast<int>(base.size()); ++i) {
        int image = h[base[i]];
        auto it = transversal[i].find(image);
        if (it == transversal[i].end()) {
            strongGens[i].push_back(h);
            growOrbit(i);
            closeStabilizer(i);
            return;
        }
        h = composePerm(h, inversePerm(it->second));
    }

    if (!isIdentityPerm(h)) extendBase(h);
}

void PermGroup::growOrbit(int level) {
    orbit[level].clear();
    transversal[level].clear();
    int b = base[level];
    orbit[level].push_back(b);
    transversal[level][b] = identityPerm(n);

    for (size_t qi = 0; qi < orbit[level].size(); ++qi) {
        int pt = orbit[level][qi];
        Perm u = transversal[level][pt];
        for (int lvl = level; lvl < static_cast<int>(strongGens.size()); ++lvl) {
            for (const Perm& g : strongGens[lvl]) {
                int img = g[pt];
                if (transversal[level].find(img) == transversal[level].end()) {
                    transversal[level][img] = composePerm(u, g);
                    orbit[level].push_back(img);
                }
            }
        }
    }
}

void PermGroup::closeStabilizer(int level) {
    std::vector<int> pts = orbit[level];
    for (int lvl = level; lvl < static_cast<int>(strongGens.size()); ++lvl) {
        for (const Perm& s : strongGens[lvl]) {
            for (int pt : pts) {
                Perm u_pt = transversal[level].at(pt);
                int image = s[pt];
                Perm u_image = transversal[level].at(image);
                Perm schreier = composePerm(composePerm(u_pt, s), inversePerm(u_image));
                if (!isIdentityPerm(schreier)) insertGeneratorAt(schreier, level + 1);
            }
        }
    }
}

void PermGroup::extendBase(const Perm& h) {
    int newPoint = -1;
    for (int x = 0; x < n; ++x) {
        if (h[x] != x) { newPoint = x; break; }
    }
    if (newPoint == -1) return; 

    base.push_back(newPoint);
    strongGens.push_back({});
    orbit.push_back({newPoint});
    transversal.push_back({{newPoint, identityPerm(n)}});

    insertGeneratorAt(h, static_cast<int>(base.size()) - 1);
}

std::vector<std::vector<int>> PermGroup::orbitPartitionAtLevel(int level, const std::vector<int>& points) const {
    std::vector<int> parent(n);
    for (int i = 0; i < n; ++i) parent[i] = i;

    auto find = [&](int x) {
        while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; }
        return x;
    };
    auto unite = [&](int a, int b) {
        a = find(a); b = find(b);
        if (a != b) parent[a] = b;
    };

    for (int lvl = level; lvl < static_cast<int>(strongGens.size()); ++lvl)
        for (const Perm& g : strongGens[lvl])
            for (int x : points) unite(x, g[x]);

    std::unordered_map<int, std::vector<int>> groups;
    for (int x : points) groups[find(x)].push_back(x);

    std::vector<std::vector<int>> result;
    result.reserve(groups.size());
    for (auto& kv : groups) result.push_back(std::move(kv.second));
    return result;
}

unsigned long long PermGroup::order() const {
    unsigned long long total = 1;
    for (const auto& o : orbit) total *= o.size();
    return total;
}

std::vector<Perm> PermGroup::allGeneratorsFlat() const {
    std::vector<Perm> all;
    for (const auto& lvlGens : strongGens)
        for (const auto& g : lvlGens) all.push_back(g);
    return all;
}

void PermGroup::print(std::ostream& os) const {
    os << "base : [";
    for (size_t i = 0; i < base.size(); ++i) os << base[i] << (i + 1 < base.size() ? "," : "");
    os << "]\n";
    os << "ordre : " << order() << "\n";
    os << "generateurs :\n";
    for (size_t lvl = 0; lvl < strongGens.size(); ++lvl) {
        for (const Perm& g : strongGens[lvl]) {
            os << "  [";
            for (size_t i = 0; i < g.size(); ++i) os << g[i] << (i + 1 < g.size() ? "," : "");
            os << "]  (niveau " << lvl << ")\n";
        }
    }
}

std::vector<Perm> PermGroup::enumerateGroup() const {
    std::vector<Perm> generators = allGeneratorsFlat();

    std::vector<Perm> elements;
    std::queue<Perm> q;

    Perm id = identityPerm(n);

    auto contains = [&](const Perm& p) {
        return std::find_if(elements.begin(), elements.end(), [&](const Perm& elem) { return elem == p; }) != elements.end();
    };

    elements.push_back(id);
    q.push(id);

    while (!q.empty()) {
        Perm current = q.front();
        q.pop();

        for (const Perm& g : generators) {
            Perm next = composePerm(current, g);

            if (!contains(next)) {
                elements.push_back(next);
                q.push(next);
            }
        }
    }

    return elements;
}

bool PermGroup::findPathRepresentative(const std::vector<int>& path, Perm& outRep) const {
    Perm g = identityPerm(n);

    for (size_t j = 0; j < path.size(); ++j) {
        if (j >= base.size()) return false;

        int wanted = path[j];
        if (g[base[j]] == wanted) continue;

        int target = inversePerm(g)[wanted];
        std::unordered_map<int, int> parentPoint;
        std::unordered_map<int, const Perm*> parentGen;
        std::deque<int> queue;
        parentPoint[base[j]] = base[j];
        queue.push_back(base[j]);
        bool found = (base[j] == target);

        while (!queue.empty() && !found) {
            int pt = queue.front();
            queue.pop_front();
            for (int lvl = static_cast<int>(j); lvl < static_cast<int>(strongGens.size()) && !found; ++lvl) {
                for (const Perm& gen : strongGens[lvl]) {
                    int img = gen[pt];
                    if (parentPoint.find(img) == parentPoint.end()) {
                        parentPoint[img] = pt;
                        parentGen[img] = &gen;
                        queue.push_back(img);
                        if (img == target) { found = true; break; }
                    }
                }
            }
        }

        if (!found) return false;
        std::vector<const Perm*> chain;
        int cur = target;
        while (cur != base[j]) {
            chain.push_back(parentGen[cur]);
            cur = parentPoint[cur];
        }
        Perm s = identityPerm(n);
        for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
            s = composePerm(s, **it);
        }

        g = composePerm(s, g); 
    }

    outRep = g;
    return true;
}