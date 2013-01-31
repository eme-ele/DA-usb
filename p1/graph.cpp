#include "graph.h"

using namespace std;

graph::graph(int num_nodes){
  graph_container.resize(num_nodes);
}

// add edge to graph 
void graph::add_edge(int v1, int v2){
      // v1 -> v2
      set<int> adjacent; 
      adjacent = graph_container[v1];

      adjacent.insert(v2);
      graph_container[v1] = adjacent;
      // v2 -> v1
      adjacent = graph_container[v2];
      adjacent.insert(v1);
      graph_container[v2] = adjacent; 
}

bool decreasing(vertexDegree a, vertexDegree b){
  return a.degree > b.degree;
}

// fill a vector with vertices ordered by decreasing degree
void graph::order(vector<vertexDegree>& ordered_vertices){
  vertexDegree v; 
  //populate vertexDegree vector from graph
  for(int i=1; i < graph_container.size(); i++) {
      v.vertex = i;
      v.degree = graph_container[i].size();
      ordered_vertices.push_back(v);
  }
    sort(ordered_vertices.begin(), ordered_vertices.end(), decreasing); 
}

void graph::get_neighbours(int v, set<int>& neighbours) {
  neighbours = graph_container[v];
}

// prints graph on console
void graph::print(){
  for(int i=1; i < graph_container.size(); i++) {
    cout << i << "|";
    for (set<int>:: iterator it=graph_container[i].begin(); it!=graph_container[i].end(); ++it){
      cout << *it << " ";
    }
    cout << endl; 
  }
}

// load a graph from dimacs file
graph * load_graph(string file) {
  int v1;
  int v2;
  int num_nodes;
  ifstream data;
  graph * instance;
  //aux 
  int pos_space;
  string rest; 

  data.open(file.c_str());
  if (data.is_open()) {
    string line;
    while (data.good()) {
      getline(data, line);
      // line that specifies number of nodes 
      if (line.substr(0,1) == "p") {
        line.erase(0,7);
        pos_space = line.find(" ");
        istringstream s((line.substr(0, pos_space)));
        s >> num_nodes;
        instance = new graph(num_nodes+1);    
      }
      // comments
      if (line.substr(0, 1) == "c") {
        continue;
      }
      // edge to be added 
      if (line.substr(0, 1) == "e"){
        line.erase(0,2);
        int pos_space = line.find(" ");
        istringstream s((line.substr(0, pos_space)));
        s >> v1;
        rest = line.substr(pos_space+1, line.length());
        istringstream t(rest);
        t >> v2;
        instance->add_edge(v1, v2);
      }   
      // any other case
      else {
        continue;
      }
    }      
  } 
  else {
    cerr << "ERROR: No se pudo abrir el archivo" << endl;
    exit(1);
  }
  data.close();
  return instance; 
}


