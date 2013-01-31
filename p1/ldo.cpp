#include <string>
#include <vector>
#include "graph.h"

using namespace std;

int main(int argc, char** argv) {
  string file = "instancias/DSJC500.1.col";
  graph* instance = load_graph(file); 
  vector<vertexDegree> ordered_vertices;
  instance->order(ordered_vertices);

  //instance->print();
  
 }


