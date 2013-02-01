#include "graph.h"

using namespace std;

// ---- Legal Color Getter
// Given a vertex and the vector containing all edges, a legal color
// is return, considering the already colored adyacent vertexes.
// Input: graph * Graph to be colored.
//        int vertex to consider.
// Output: int legal color for the given vertex.
int getLegalColor(graph* graphInstance,int vertex){
  set<int> adyacents;
  graphInstance->get_neighbours(vertex,adyacents);
  int i;
  int n = graphInstance->num_nodes();
  for(i = 1; i<n+1; i++){
    int valid = 1;
    for (set<int>::iterator it = adyacents.begin() ; it != adyacents.end() && valid == 1; ++it){
      // Get color of the vertex identified with 'it'
      int c = graphInstance->get_color(*it);
      if(c != 0){ 
		// Is colored
        if(c == i){
			// Try another color
			valid = 0;
		}
	  }
    }
    if(valid == 1) break;
  }
  return i;
}

// ---- Greedy Algorithm for Graph Coloring (LDO, IDO, SDO)---------
// Executes the greedy algorithm for graph coloring given a
// with its vertexes sorted following a certain ordering policy.
//
// Input: graph * Graph to be colored.
// Output: The graph has all its vertexes colored.
void greedyColoring(graph* graphInstance){
  int i = 0;
  int n = graphInstance->num_nodes();
  while( i < n ){
	int vertex = i; // for First Fit
    //int vertex = graphInstance->max_degree(); // for LDO
    //int vertex = graphInstance->max_incidence(); // for IDO
    //int vertex = graphInstance->max_saturation(); // for SDO
    int legalColor = getLegalColor(graphInstance,vertex);
    graphInstance->set_color(vertex,legalColor);
    i++;
  }
}

// Executes the greedy algorithm for graph coloring given a
// with its vertexes sorted following First Fit policy.
//
// Input: graph * Graph to be colored.
// Output: The graph has all its vertexes colored.
void FirstFit(graph* graphInstance){
  int i = 0;
  int n = graphInstance->num_nodes();
  while( i < n ){
    int vertex = i; 
    int legalColor = getLegalColor(graphInstance,vertex);
    graphInstance->set_color(vertex,legalColor);
    cout<<legalColor<<"-";
    i++;
  }
  cout<<endl;
}

// Executes the greedy algorithm for graph coloring given a
// with its vertexes sorted following largest degree ordering policy.
//
// Input: graph * Graph to be colored.
// Output: The graph has all its vertexes colored.
void LDO(graph* graphInstance){
  int i = 0;
  int n = graphInstance->num_nodes();
  while( i < n ){
    int vertex = graphInstance->max_degree(); 
    int legalColor = getLegalColor(graphInstance,vertex);
    graphInstance->set_color(vertex,legalColor);
    cout<<legalColor<<"-";
    i++;
  }
  cout<<endl;
}

// Executes the greedy algorithm for graph coloring given a
// with its vertexes sorted following incidence degree ordering policy.
//
// Input: graph * Graph to be colored.
// Output: The graph has all its vertexes colored.
void IDO(graph* graphInstance){
  int i = 0;
  int n = graphInstance->num_nodes();
  while( i < n ){
    int vertex = graphInstance->max_incidence();
    int legalColor = getLegalColor(graphInstance,vertex);
    graphInstance->set_color(vertex,legalColor);
    cout<<legalColor<<"-";
    i++;
  }
}

// Executes the greedy algorithm for graph coloring given a
// with its vertexes sorted following saturation degree ordering policy.
//
// Input: graph * Graph to be colored.
// Output: The graph has all its vertexes colored.
void SDO(graph* graphInstance){
  int i = 0;
  int n = graphInstance->num_nodes();
  while( i < n ){
    int vertex = graphInstance->max_saturation();
    int legalColor = getLegalColor(graphInstance,vertex);
    graphInstance->set_color(vertex,legalColor);
    cout<<legalColor<<"-";
    i++;
  }
}

int main(int argc,char *argv[]){
  printf("\nAlgoritmos Avidos para el Problema de Coloracion de Grafos\n");
  
  if(argc == 3){
	// Parse input file
	cout<<"Generando Grafo..."<<endl;
	graph *graphInstance = load_graph(argv[2]);
	
    if(strcmp(argv[1],"-f")==0){
      // Run First Fit Algorithm
      cout<<endl<<"Ejecutando algoritmo First Fit..."<<endl;
      FirstFit(graphInstance);
    }else if(strcmp(argv[1],"-l")==0){
      // Run LDO Algorithm
      cout<<endl<<"Ejecutando algoritmo LDO..."<<endl;
      LDO(graphInstance);
    }else if(strcmp(argv[1],"-s")==0){
      // Run SDO Algorithm
      cout<<endl<<"Ejecutando algoritmo SDO..."<<endl;
      SDO(graphInstance);
    }else if(strcmp(argv[1],"-i")==0){
      // Run IDO Algorithm
      cout<<endl<<"Ejecutando algoritmo IDO..."<<endl;
      IDO(graphInstance);
    }else if(strcmp(argv[1],"-b")==0){
      // Run Independent Set Based Coloring Algorithm
      cout<<endl<<"Ejecutando algoritmo Independent Set Based Coloring..."<<endl;
      // TO DO
    }else{
      cout<<endl<<"Entrada invalida. Formato: ./gcp1 [ -f | -l | -s | -i | -b ] <Archivo de entrada>"<<endl;
    }
  }else{
    cout<<endl<<"Entrada invalida. Formato: ./gcp1 [ -f | -l | -s | -i | -b ] <Archivo de entrada>"<<endl;
  }
	
  return 0;
}
