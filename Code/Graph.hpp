#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <iostream>
#include <string>
#include <cstdint>
#include <bitset>

using namespace std;


/**
 * @brief  Class representing a graph. It contains the adjacency matrix and methods to manipulate the graph.
 */
class Graph {
private:
    size_t n; // number of vertices
    bool isDirected;
    std::vector<int> adjList;
    std::vector<std::vector<int>> neighbors;

    /**
     * @brief  create the adjacency matrix from a graph6 string
     * @param  graph6: Graph encoded in Graph6 format
     */

    void buildFromGraph6(const string& graph6);

    /**
     * @brief  return the position in the adjacency matrix for the edge (i,j)
     * @param  i: position of the first vertex
     * @param  j: position of the second vertex
     * @return: reference to the value in the adjacency matrix
     */
    int& at(size_t i, size_t j) { return adjList[i*n + j]; }

    /**
     * @brief: return the position in the adjacency matrix for the edge (i,j)
     * @note: we cannot modifie here, so we return a copy of the value
     * @param  i: position of the first vertex
     * @param  j: position of the second vertex
     * @return: position in the adjacency matrix for the edge (i,j)
     */
    int at(size_t i, size_t j) const { return adjList[i*n + j]; }

public:

    /**
     * @brief Creates an empty graph.
     *
     * @param numVertices Number of vertices.
     * @param directed Whether the graph is directed.
     */
    Graph(int numVertices, bool directed = false);


    /**
     * @brief Creates a graph from its Graph6 representation.
     *
     * @param graph6 Graph encoded in Graph6 format.
     * @param directed Whether the graph is directed.
     */
    Graph(const string& Graph6, bool directed = false); 


    /**
     * @brief: add an edge between two vertices with a given weight  
     * @param  u: first vertex
     * @param  v: second vertex
     * @param  weight: weight of the edge (default is 1)
     */
    void addEdge(size_t u, size_t v, int weight = 1);


    /**
     * @brief: remove an edge between two vertices   
     * @param  u: first vertex
     * @param  v: second vertex
     */
    void removeEdge(size_t u, size_t v);


    /**
     * @brief  verify if there is an edge between two vertices
     * @param  u: first vertex
     * @param  v: second vertex
     * @return: true if there is an edge between u and v, false otherwise
     */
    bool hasEdge(size_t u, size_t v) const;


    /**
     * @brief: return the neighbors of a vertex
     * @note: the neighbors are all the vertices that are connected to the vertex v by an edge
     * @param  v: vertex for which we want to get the neighbors
     * @return: a vector containing the neighbors of the vertex v
     */
    vector<int> getNeighbors(size_t v) const;

    /**
     * @brief:  print the adjacency matrix of the graph
     * @note: only use for debugging, not for large graphs
     */
    void printMatrix() const;

    /**
     * @brief:  print the graph in Graph6 format
     * @note: only use for debugging, not for large graphs 
     */
    void printGraph6() const;

    /**
     * @brief: compute the "N(x)" of the graph6 format(see documentation for more understanding) 
     * @return: a vector of uint8_t containing the "n" of the graph6 format
     */
    std::vector<uint8_t> nGraph6() const;

    /**
     * @brief: compute the "R(x)" of the graph6 format(see documentation for more understanding)
     * @note   
     * @param  x: the bigendian 36-bit binary form of n(number of vertices) in the graph6 format
     * @param  &result: value of N(x) in the graph6 format
     */
    void rGraph6(std::string x, std::vector<uint8_t> &result) const;

    /**
     * @brief:  Compute the upper triangle of the adjacency matrix as a string of 0s and 1s.
     * @return: A string representing the upper triangle of the adjacency matrix.
     */
    std::string upperTriangleMatrix() const;

    /**
     * @brief: Compare two graphs for equality.
     * @note  compare the adjacency matrices of the two graphs, if they are directed and have the same number of vertices, then they are equal
     * @param  other: another graph to compare with
     * @return: true if the graphs are equal, false otherwise
     */
    bool operator==(const Graph& other) const;

    /**
     * @brief: Compare two graphs for inequality.
     * @param  other: another graph to compare with
     * @return: true if the graphs are not equal, false otherwise
     */
    bool operator!=(const Graph& other) const;

    /**
     * @brief: compute the number of vertices in the graph
     * @return: n 
     */
    int size() const;

    /**
     * @brief: Apply a graph permutation(change in the vertices) to the original graph 
     * @note: only use to get the canonical form of the graph
     * @return: the modified graph
     */
    Graph applyPermutation(const std::vector<int> perm) const;

    /**
     * @brief: Acompute the Graph6 representation of the graph
     * @return: the graph in Graph6 format
     */
    std::string toGraph6() const;
};

#endif // GRAPH_HPP