#include <vector>
#include "graph.h"

using namespace std;

// ---- Legal Color Getter
// Given a vertex and the vector containing all edges, a legal color
// is return, considering the already colored adyacent vertexes.
// Input: vertexDegree vertex to consider
//        vector<set<int>> vector containing all edges of the graph
int getLegalColor(vector<vertexDegree> g, int vertex, int *color){
  set<int> adyacents;
  get_neighbours(vertex,&adyacents);
  int i;
  for(i = 0; color<g.size(); color++){
    for (set<int>::iterator it = g.begin() ; it != g.end(); ++it){
      // Get color of the vertex identified with 'it'
      
      // Check if the vertex is already colored (== -1)
	  // if colored, check if its color is equal to 'color'
	  //   if equal to 'color', break.
	  //   else, keep cheking other vertexes 
    }
  }
  color = i;
}

// ---- Greedy Algorithm for Graph Coloring
// Executes the greedy algorithm for graph coloring given a
// with its vertexes sorted following a certain ordering policy.
//
// Input: vertex<vertexDegree> A vertex vector ordered descendently 
//        according to the vertex degree. This degree will depend on 
//        the ordering policy for each algorithm.
//
// Output: vertex<int> which represents the color of each vertex of the
//         graph.
void greedyColoring(vector<vertexDegree> g, vector<set<int>> edges){	
  vector<int> colors;
  for (vector<vertexDegree>::iterator it = g.begin() ; it != g.end(); ++it){
    vertexDegree vertex = it;
    int legalColor;
    getLegalColor(g,it->vertex,&legalColor);
    it->color =  legalColor;
  }
	
  return colors;
}

