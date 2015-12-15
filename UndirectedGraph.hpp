#ifndef UNDIRECTEDGRAPH_HPP
#define UNDIRECTEDGRAPH_HPP

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include "Vertex.hpp"

/**
 * Implements an undirected graph. Any edge in the graph
 * represents a bidirectional connection between two vertices.
 * 
 * Implements methods for producing a minimum spanning tree of the
 * graph, as well as calculating the total length of the shortest
 * paths between each pair of vertices.
 */
class UndirectedGraph {
  public:
    /**
     * Constructs an empty UndirectedGraph with no vertices and
     * no edges.
     */
    UndirectedGraph(){};
    
    std::string build(std::ifstream& in);

    int getTotalCost(std::ifstream& in) const;
    
    int getCheapestNetwork();

    int findMSTTransit();

    unordered_map<string, Vertex*> buildMinMap();

    int findShortestPaths(Vertex* v, unordered_map<string, Vertex*> vs);
 
    int findTotalTransit();
    /**
     * Destructs an UndirectedGraph.
     */
    ~UndirectedGraph();
    
  private:
    /**
     * Comparison functor for use with Dijkstra's algorithm. Allows Vertices
     * to be added to a priority queue more than once, with different weights.
     */
    class DijkstraVertexComparator {
      public:
        bool operator()(const std::pair<Vertex*, unsigned int> &left,
                const std::pair<Vertex*, unsigned int> &right){
          return get<1>(left) > get<1>(right);}
    };
   

    class MSTComparator{
      public:
        bool operator()(Edge& x, Edge& y) const
        {
          return y < x;}
    };
    /**
     * Map of vertex name to Vertex.
     */
    std::unordered_map<std::string, Vertex*> vertices;
};

#endif
