#include "Vertex.hpp"
#include <iostream>

using namespace std;

//Typical getter method. Pretty neat huh?
int Vertex::getDistance(){
  return distance;}
  
/*
 * This iterates through the "edges" unordered map and returns the 
 * smallest edge that has not been visited. This is quite important to 
 * the success of our shortest path algorithm back in UndirectedGraph.cpp
 */
Edge Vertex::getSmallestEdge(){
  auto it = this->edges.begin();
  Edge minEdge = it->second;
  for(auto it = this->edges.begin(); it != this->edges.end(); ++it){
    if(minEdge.getCost() > it->second.getCost() &&
        !it->second.getTo()->isVisited()){
        minEdge = it->second;}
  }
  //We set the edge we've chosen to "visited" before returning
  minEdge.getTo()->hasVisited();
  return minEdge;
}


//More getters and setters. Good for resetting your values and 
//"cleaning up after yourself" between function calls.

bool Vertex::isVisited(){
  return visited;}

void Vertex::hasVisited(){
  this->visited = true;}

void Vertex::resetVisited(){
  this->visited = false;}

string Vertex::getName(){
  return name;}

void Vertex::setDistance(int x){
  this->distance = x;}
