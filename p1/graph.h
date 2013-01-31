#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

struct vertexDegree {
  int vertex; 
  int degree;
  int color;
  // default init
  vertexDegree(){
    color = -1;
  }
}; 

class graph {
  private:
    vector<set<int> > graph_container;
  public:
    graph(int num_nodes);
    void add_edge(int v1, int v2);
    void order(vector<vertexDegree>& ordered_vertices); 
    void get_neighbours(int v, set<int>& neighbours);
    void print();
}; 

// loads graph from dimacs file
graph * load_graph(string file); 

