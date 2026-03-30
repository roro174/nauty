#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <iostream>
#include <string>
#include <cstdint>
#include <bitset>

using namespace std;



class Graph {
private:
    size_t n; // nombre de sommets
    bool isDirected;
    std::vector<int> adjList;
    std::vector<std::vector<int>> neighbors;

    /**
     * @brief  
     * @note   
     * @param  graph6: 
     * @retval None
     */

    void buildFromGraph6(const string& graph6);

    /**
     * @brief  
     * @note   
     * @param  i: 
     * @param  j: 
     * @retval 
     */
    int& at(size_t i, size_t j) { return adjList[i*n + j]; }

    /**
     * @brief  
     * @note   
     * @param  i: 
     * @param  j: 
     * @retval 
     */
    int at(size_t i, size_t j) const { return adjList[i*n + j]; }

public:

    /**
     * @brief  
     * @note   
     * @param  numVertices: 
     * @param  directed: 
     * @retval 
     */
    Graph(int numVertices, bool directed = false);


    /**
     * *  */   
    Graph(const string& Graph6, bool directed = false); 


    /**
     * @brief  
     * @note   
     * @param  u: 
     * @param  v: 
     * @param  weight: 
     * @retval None
     */
    void addEdge(size_t u, size_t v, int weight = 1);


    /**
     * @brief  
     * @note   
     * @param  u: 
     * @param  v: 
     * @retval None
     */
    void removeEdge(size_t u, size_t v);


    /**
     * @brief  
     * @note   
     * @param  u: 
     * @param  v: 
     * @retval 
     */
    bool hasEdge(size_t u, size_t v) const;


    /**
     * @brief  
     * @note   
     * @param  v: 
     * @retval 
     */
    vector<int> getNeighbors(size_t v) const;

    /**
     * @brief  
     * @note   
     * @retval None
     */
    void printMatrix() const;

    /**
     * @brief  
     * @note   
     * @retval None
     */
    void printGraph6() const;

    /**
     * @brief  
     * @note   
     * @retval 
     */
    std::vector<uint8_t> nGraph6() const;

    /**
     * @brief  
     * @note   
     * @param  x: 
     * @param  &result: 
     * @retval None
     */
    void rGraph6(std::string x, std::vector<uint8_t> &result) const;

    /**
     * @brief  
     * @note   
     * @retval 
     */
    std::string upperTriangleMatrix() const;

    /**
     * @brief  
     * @note   
     * @param  other: 
     * @retval 
     */
    bool operator==(const Graph& other) const;

    /**
     * @brief  
     * @note   
     * @param  other: 
     * @retval 
     */
    bool operator!=(const Graph& other) const;

    /**
     * @brief  
     * @note   
     * @retval 
     */
    int size() const;

    /**
     * @brief  
     * @note   
     * @param  perm: 
     * @retval 
     */
    Graph applyPermutation(const std::vector<int> perm) const;

    /**
     * @brief  
     * @note   
     * @retval 
     */
    std::string toGraph6() const;
};

#endif // GRAPH_HPP