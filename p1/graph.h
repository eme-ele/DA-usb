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
  friend bool operator<(vertexDegree a, vertexDegree b){
        return a.degree > b.degree;
    }
  
}; 

class graph {
  private:
    map<int, std::set<int> > graph_container;
  public:
    void add_edge(int v1, int v2);
    void order(vector<vertexDegree>& ordered_vertices); 
    void print();
}; 

// loads graph from dimacs file
graph * load_graph(string file); 

