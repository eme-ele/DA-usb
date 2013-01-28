#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

class graph {
  private:
    std::map<int, std::set<int> > graph_container;
  public:
    void add_edge(int v1, int v2);
}; 

// loads graph from dimacs file
graph * load_graph(string file); 
