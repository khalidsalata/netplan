#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <string>
#include <unordered_map>
#include <limits>
#include "Edge.hpp"

using namespace std;

/**
 * Represents a Vertex in a graph.
 *
 * Vertices are connected to other Vertices via Edges. Each Vertex
 * maintains a collection of all Edges that originate from it.
 */
class Vertex {
    // Graph needs access to Edge map for Dijkstra/Prim algorithms.
    friend class UndirectedGraph;
    
  public:
    /**
     * Initialize the Vertex with the given name.
     */
    Vertex(const std::string &name): name(name){}
   
    bool isVisited();
    
    Edge getSmallestEdge();
   
    std::string getName();

    int getDistance();

    void setDistance(int x);

    void resetVisited();
    
    void hasVisited();

  private:

    /**
     * Name of this Vertex.
     */
    std::string name;
    
    /**
     * Distance of this Vertex from initial Vertex.
     * Used by Dijkstra's algorithm.
     */
    unsigned int distance = numeric_limits<unsigned int>::max();
    //unsigned int distance = 10000;
    /**
     * Whether this node has been visited.
     * Used by Prim's algorithm.
     */
    bool visited = false;

    /**
     * Map of adjacent Vertex name to Edge describing the adjacency.
     */
    std::unordered_map<std::string, Edge> edges;
};

#endif
