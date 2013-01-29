#include "graph.h"

using namespace std;

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

// fill a vector with vertices ordered by decreasing degree
void graph::order(vector<vertexDegree>& ordered_vertices){
  vertexDegree v; 
  //populate vertexDegree vector from graph
  for(map<int, set<int> >::iterator it = graph_container.begin(); 
      it != graph_container.end(); ++it) {
      v.vertex = it->first;
      v.degree = it->second.size();
      ordered_vertices.push_back(v);
  }
  sort(ordered_vertices.begin(), ordered_vertices.end()); 
}


// load a graph from dimacs file
graph * load_graph(string file) {
  int v1;
  int v2;
  string resto; 
  ifstream data;
  string rest; 
  graph * instance; 

  data.open(file.c_str());
  if (data.is_open()) {
    string line;
    while (data.good()) {
      getline(data, line);
      
      if (line.substr(0, 1) != "e") {
        // comments or other info
        continue;
      }

      if (line != "") {
        line.erase(0,2);
        int pos_space = line.find(" ");
        istringstream s((line.substr(0, pos_space)));
        s >> v1;
        rest = line.substr(pos_space+1, line.length());
        istringstream t(rest);
        t >> v2;
        instance = new graph();
        instance->add_edge(v1, v2);
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


