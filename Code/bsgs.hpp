#pragma once

#include <vector>
#include <unordered_map>
#include <ostream>
#include <queue>
#include <algorithm>
#include <deque>

using Perm = std::vector<int>;

/**
 * @brief: Creates an identity permutation of size n.
 * @param n: The size of the permutation.
 * @return: A permutation where each element maps to itself.
 */
Perm identityPerm(int n);

/**
 * @brief Compose two permutations under the right-action convention
 *        composePerm(g, h)[i] = h[g[i]].
 * @param g The permutation applied first.
 * @param h The permutation applied second.
 * @return The composed permutation g∘h.
 */
Perm composePerm(const Perm& g, const Perm& h);

/**
 * @brief Compute the inverse of a permutation.
 * @param p The permutation to invert.
 * @return The permutation p^{-1} such that composePerm(p, inversePerm(p))
 *         is the identity permutation.
 */
Perm inversePerm(const Perm& p);

/**
 * @brief: Test whether a permutation is the identity.
 * @param p: The permutation to test.
 * @return: true if p[i] == i for every point i, false otherwise.
 */
bool isIdentityPerm(const Perm& p);

/**
 * @brief: Incremental representation of a permutation group of degree n via
 *        a base and strong generating set (BSGS), built and maintained with
 *        the Schreier-Sims algorithm.
 */
class PermGroup {
public:

    /**
     * @brief: Constructs a permutation group of degree n.
     * @param n: The degree of the permutation group.
     */
    explicit PermGroup(int n = 0);

    /**
     * @brief: Sets the base of the permutation group and initializes the strong generating set.
     * @param b: A vector of integers representing the base of the permutation group.
     */
    void setBase(const std::vector<int>& b);

    /**
     * @brief: Inserts a new generator into the permutation group, updating the strong generating set and orbits as necessary.
     * @param g: A permutation to be added as a generator of the group.
     */
    void insertGenerator(const Perm& g);

    /**
     * @brief: Computes the orbit partition of a set of points under the action of the group at a specified level.
     * @param level: The level in the base at which to compute the orbit partition.
     * @param points: The set of points to partition into orbits
     * @return: A vector of vectors, where each inner vector represents an orbit of points under the group action at the specified level.
     */
    std::vector<std::vector<int>> orbitPartitionAtLevel(
        int level,
        const std::vector<int>& points
    ) const;

    /**
     * @brief: Finds a representative of the group element corresponding to a given path in the base.(The path is a sequence of images of the base points under the group action.)
     * @param path: A vector of integers representing the images of the base points under the group action.
     * @param outRep: Set to the composed representative permutation when the path is realizable
     * @return: true if the path is realizable by some group element, false otherwise.
     */
    bool findPathRepresentative(
        const std::vector<int>& path,
        Perm& outRep
    ) const;

    /**
     * @brief: Compute the order of the group from its BSGS.
     * @return: The order of the permutation group as an unsigned long long integer.
     */
    unsigned long long order() const;

    /**
     * @brief: Get the base of the permutation group.
     * @return: A constant reference to the vector representing the base of the permutation group.
     */
    const std::vector<int>& getBase() const { return base; }

    /**
     * @brief: Get all generators of the permutation group in a flat vector.
     * @return: A vector containing all generators of the permutation group, flattened from the strong generating set.
     */
    std::vector<Perm> allGeneratorsFlat() const;

    /**
     * @brief: Prints the details of the permutation group, including its base, order, and generators, to the specified output stream.
     * @param os: The output stream to which the group details will be printed.
     */
    void print(std::ostream& os) const;

    /**
     * @brief: Enumerates all elements of the permutation group by performing a breadth-first search starting from the identity permutation and applying all generators.
     * @return: A vector containing all elements of the permutation group.
     */
    std::vector<Perm> enumerateGroup() const;

private:

    int n; // The degree of the permutation group (the number of points being permuted).
    std::vector<int> base;
    std::vector<std::vector<Perm>> strongGens;
    std::vector<std::vector<int>> orbit;
    std::vector<std::unordered_map<int, Perm>> transversal;

    /**
     * @brief: Inserts a new generator into the permutation group at a specified level, updating the strong generating set and orbits as necessary.
     * @param g: A permutation to be added as a generator of the group.
     * @param level: The level in the base at which to insert the generator.
     */
    void insertGeneratorAt(const Perm& g, int level);

    /**
     * @brief:Recompute/extend orbit[level] and transversal[level] by breadth-first expansion from base[level] under the generators
     *        currently available at that level.
     * @param level: The level in the base at which to grow the orbit.
     */
    void growOrbit(int level);

    /**
     * @brief Verify and complete the stabilizer chain at a given level by sifting the Schreier generators (products of transversal
     *        elements and strong generators) through strip/sift, and inserting any non-trivial residues into the next level.
     * @param level The level at which the stabilizer is closed/verified.
     */
    void closeStabilizer(int level);

    /**
     * @brief: Extends the base of the permutation group by adding a new point and updating the strong generating set and orbits accordingly.
     * @param h: A permutation representing the new point to be added to the base.
     */
    void extendBase(const Perm& h);
};