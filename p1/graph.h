#include <stdlib.h>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

class graph {
  private:
    vector<set<int> > graph_container;
    vector<int> colors; 
  public:
    graph();
    graph(int num_nodes);
    void add_edge(int v1, int v2); 
    void get_neighbours(int v, set<int>& neighbours);
	void get_uncolored(set<int>& uncolored);
    void print();
	void print_colors();
    void set_color(int v, int k);
    int get_color(int v);
    int degree(int v);
    int max_degree();
    int num_nodes();
    int saturation(int v);
    int incidence(int v);
    int max_saturation();
    int max_incidence();
	int diff_colors();
}; 

// loads graph from dimacs file
graph * load_graph(string file); 

