#include "UndirectedGraph.hpp"
#include "Edge.hpp"
#include "Vertex.hpp"
#include <string>
#include <queue>
#include <iostream>
#include <fstream>
// Method implementations here

using namespace std;

/**
 * A method to construct an undirected graph
 * using an input file that's formatted in the way
 * described in the write-up
 */
string UndirectedGraph::build(std::ifstream& in){
  string v1,v2,cost,length = "";
  string toReturn = "";
  Vertex* vert1; 
  Vertex* vert2;
  while(in.good()){//while the file has readable input
  in >> v1;
  toReturn = v1;
  in >> v2;
  in >> cost;
  in >> length;//fill up the necessary info, again following write-up order
  //This series of conditional statments determines whether we make a 
  //new vertex, or update an existing one
  if(this->vertices.count(v1) != 0){
    vert1 = this->vertices[v1];}
  else{
    vert1 = new Vertex(v1);}
  if(this->vertices.count(v2) != 0){
    vert2 = this->vertices[v2];}
  else{
    vert2 = new Vertex(v2);}
  //Convert the string values into integers
  unsigned int icost = 0;
  if(cost != ""){
    icost = stoi(cost);}
  unsigned int ilength = 0;
  if(length != ""){
    ilength = stoi(length);}
  //I'm creating pointers to edges so we can make sure they're on the heap
  Edge* edge1 = new Edge(vert1, vert2, icost, ilength);
  Edge* edge2 = new Edge(vert2, vert1, icost, ilength);
  //Now we add the edges to the vertices we've extracted
  vert1->edges.insert(std::make_pair(v2, *edge1));
  vert2->edges.insert(std::make_pair(v1, *edge2));
  //Now we add the vertices to the unordered map we're making
  this->vertices.insert(make_pair(v1, vert1));
  this->vertices.insert(make_pair(v2, vert2));
  //And here is our end condition
  if(in.peek() == -1){
    break;} 
  }
  return toReturn;

}

/**
 * Function adds up all the costs of the edges in the graph
 * No fancy Data Structure stuff here, just re-reading the input file.
 */
int UndirectedGraph::getTotalCost(ifstream& in) const{
  string toAdd = "";
  string toIgnore = "";
  int toReturn = 0;
  //We ignore every input except for cost
  while(in.good()){
    in >> toIgnore;
    in >> toIgnore;
    in >> toAdd;
    if(toAdd != ""){
    toReturn = toReturn + stoi(toAdd);}
    if(in.peek() == -1){//Same kind of break logic as build()
      break;}
    in >> toIgnore;
  }
  //An integer is the best thing to return, as you can imagine
  if(toAdd != ""){
  toReturn = toReturn -stoi(toAdd);}
  return toReturn;
}

//These two unordered maps are important in implementing union-find
unordered_map<Vertex*, Vertex*> p;
unordered_map<Vertex*, int> r;

/**
 * Looks for the topmost nod of a grouping of vertices
 * Useful in union-find
 */
Vertex* find(Vertex* v){
  if(p[v] == v){
    return p[v];}
  else{
    return find(p[v]);}}

//This unordered map is important for part 5: finding total cost on the MST
unordered_map<string, Edge> newMinEdges;

/**
 * Method to obtain an MST. Notice we're using union-find and an 
 * implemntation of Kruskal's algorithm to achieve this. An integer
 * return value is needed, representing the sum of the edges we ended up
 * using.
 */
int UndirectedGraph::getCheapestNetwork(){
  int minPath = 0;
  //A priority queue helps us select only the next best edge to include
  //in our MST
  priority_queue<Edge, vector<Edge>, MSTComparator> mstE1;
  //A nested loop giving us access to the edges
  for(auto it = this->vertices.begin(); it != this->vertices.end(); ++it){
    for(auto ie = it->second->edges.begin(); ie != it->second->edges.end(); ++ie){
      mstE1.push(ie->second);}}
  //Why two priority queues? You'll see!
  priority_queue<Edge, vector<Edge>, MSTComparator> mstE;
  while(!mstE1.empty()){//this gets rid of duplicates in our first min-heap
      mstE.push(mstE1.top());
      mstE1.pop();
      mstE1.pop();}
  //So Now we can populate our two unordered maps
  for(auto i = this->vertices.begin(); i != this->vertices.end(); ++i){
    p[i->second] = i->second;
    r[i->second] = 0;}
  
  //Here's where union-find becomes helpful. While we pop off our 
  //queue, we can see whether it's been included in our prospective 
  //MST, and if the edges should be added
  while(!mstE.empty()){
    Edge dummy = mstE.top();
    Vertex* v1 = find(dummy.getFrom());
    Vertex* v2 = find(dummy.getTo());
    if(v1 != v2){
      //Some foresight: we don't want to forget what edges ended up
      //making our MST. This is important in part 5
      newMinEdges.insert(make_pair(v1->getName(),dummy));
      minPath = minPath + dummy.getCost();
      if(r[v1] > r[v2]){
        p[v2] = v1;
        r[v1]++;}
      else{
        p[v1] = v2;
        r[v2]++;}
  }
    mstE.pop();
  }
  return minPath;
}

/**
 * We use the findShortestPaths algorithim, but apply it to every vertex
 * in order to get the total of the shortest paths.
 */
int UndirectedGraph::findTotalTransit(){
  int toReturn = 0;
  for(auto it = this->vertices.begin(); it != this->vertices.end(); ++it){
    toReturn = toReturn + findShortestPaths(it->second, this->vertices);      
  }
  return toReturn;}

/**
 * The same logic applies here, but instead of using the original
 * undirected graph, we use the undirected graph that represents our MST.
 * Notice we build that MST, and then apply findShortestPaths.
 */
int UndirectedGraph::findMSTTransit(){
  int toReturn = 0;
  unordered_map<string, Vertex*> newMSTMap = buildMinMap();

  for(auto it = newMSTMap.begin(); it != newMSTMap.end(); ++it){
    toReturn  = toReturn + findShortestPaths(it->second, newMSTMap);
  }
  return toReturn;
}

/**
 * The function responsible for making good use of newMinEdges.
 * We take the edges from our MST and create a separate undirected graph
 * from them. Since my original build algorithm worked before, why
 * reinvent the wheel right? 
 */
unordered_map<string, Vertex*> UndirectedGraph::buildMinMap(){
  Vertex* vert1;
  Vertex* vert2;
  string v1 = "";
  string v2 = ""; 
  unordered_map<string, Vertex*> newMinMap;
  for(auto i = newMinEdges.begin(); i != newMinEdges.end(); ++i){
    Edge dummy = i->second;
    v1 = dummy.getFrom()->getName();
    v2 = dummy.getTo()->getName();
    if(newMinMap.count(v1) != 0){
     vert1 = newMinMap[v1];}
   else{
     vert1 = new Vertex(v1);}
   if(newMinMap.count(v2) != 0){
     vert2 = newMinMap[v2];}
   else{
     vert2 = new Vertex(v2);}
   unsigned int icost = dummy.getCost();
   unsigned int ilength = dummy.getLength();
   Edge* edge1 = new Edge(vert1, vert2, icost, ilength);
   Edge* edge2 = new Edge(vert2, vert1, icost, ilength);
   vert1->edges.insert(make_pair(v2, *edge1));
   vert2->edges.insert(make_pair(v1, *edge2));
   newMinMap.insert(make_pair(v1, vert1));
   newMinMap.insert(make_pair(v2, vert2));}
  
  return newMinMap;
}

/**
 * This algorithm is an implementation of Djikstra's shortest path 
 * algorithm. We maintain a heap that has our tentative best paths from
 * one vertex to all the others. Notice that this function returns only 
 * the sum of shortest paths for one vertex. A separate funciton is
 * meant to feed the correct number of vertices into this algorithm and 
 * manage summing them.
 */
int UndirectedGraph::findShortestPaths(Vertex* v, unordered_map<string, Vertex*> vs){
  int toReturn = 0;
  //This resets the values in our vertices so there's no unnecessary 
  //feedback from previous implementations of this function
  for(auto i = vs.begin(); i != vs.end(); ++i){
    i->second->resetVisited();
    i->second->setDistance(10000);}
  //A heap that makes use of the Djikstra comparator class in the header
  priority_queue<pair<Vertex*, unsigned int>,vector<pair<Vertex*, unsigned int>>, DijkstraVertexComparator> pq;
  //Add the first vertex, so we no where to find paths from
  pq.push(make_pair(v,0));
  v->setDistance(0);
  //And now we continuously add vertices to the heap and manage their
  //costs, only considering the values that are smallest
  while(!pq.empty()){
    Vertex* top = get<0>(pq.top());
    pq.pop();
    if(!top->isVisited()){
    top->hasVisited();
    //The next loop will determine what we can push to our heap
    //We also set the distances of vertices after we can assume we
    //actually found their shortest path
    for(auto it = top->edges.begin(); it != top->edges.end(); ++it){
      Vertex* to = it->second.getTo();
      Vertex* from = it->second.getFrom();
      if(!to->isVisited()){
        int best = it->second.getLength() + from->getDistance(); 
        if(best < to->getDistance()){
          to->setDistance(best);
          pq.push(make_pair(to, best));
          to->setDistance(best);
          }  
            }
            } 
    //Managing summing, like I told ya
    toReturn = toReturn + top->getDistance();
    
    }        
  }
  return toReturn;}
