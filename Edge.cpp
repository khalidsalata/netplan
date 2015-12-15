#include "Edge.hpp"

// Method implementations here

/*
 * Let's be real, the only additional functionality you need here are
 * getters and setters. These are esseential to other implementations, 
 * but not much else happens down at this level.
 */

unsigned int Edge::getCost() const{
  return cost;}

Vertex* Edge::getTo() {
  return to;}

Vertex* Edge::getFrom() {
  return from;}

unsigned int Edge::getLength() const{
  return length;}
