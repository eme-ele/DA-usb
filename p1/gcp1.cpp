#include "graph.h"

using namespace std;

// ---- Legal Color Getter
// Given a vertex and the vector containing all edges, a legal color
// is return, considering the already colored adyacent vertexes.
// Input: graph * Graph to be colored.
//			  int vertex to consider.
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
//			int flag for type of algorithm
// Output: The graph has all its vertexes colored.
void greedyColoring(graph* graphInstance, int flag){
	int n = graphInstance->num_nodes();
	int vertex, legalColor;
	for(int i=0;i < n; i++){
		if (flag == 0){
		vertex = i; 							// for First Fit
	} else if (flag == 1){
		vertex = graphInstance->max_degree();	// for LDO
	} else if (flag == 2){
		graphInstance->max_incidence();			// for SDO
	} else if (flag == 3){
		graphInstance->max_saturation();		// for IDO
	}
		legalColor = getLegalColor(graphInstance,vertex);
		graphInstance->set_color(vertex,legalColor);
		cout << legalColor << "-";
	}
	cout << endl;
}

// error message
int print_use(){
	cout << "Entrada invalida. Formato: ./gcp1 [ -f | -l | -s | -i | -b ] <Archivo de entrada>" << endl;
	return 0;
}

int main(int argc,char *argv[]){
	printf("\nAlgoritmos Avidos para el Problema de Coloracion de Grafos\n");

	if(argc != 3){
		print_use();
		return -1;
	}
	
	// Parse input file
	cout<<"Generando Grafo..."<<endl;
	graph *graphInstance = load_graph(argv[2]);
	
	if(strcmp(argv[1],"-f")==0){
		// Run First Fit Algorithm
		cout<<endl<<"Ejecutando algoritmo First Fit..."<<endl;
		greedyColoring(graphInstance,0);
	}else if(strcmp(argv[1],"-l")==0){
		// Run LDO Algorithm
		cout<<endl<<"Ejecutando algoritmo LDO..."<<endl;
		greedyColoring(graphInstance,1);
	}else if(strcmp(argv[1],"-s")==0){
		// Run SDO Algorithm
		cout<<endl<<"Ejecutando algoritmo SDO..."<<endl;
		greedyColoring(graphInstance,2);
	}else if(strcmp(argv[1],"-i")==0){
		// Run IDO Algorithm
		cout<<endl<<"Ejecutando algoritmo IDO..."<<endl;
		greedyColoring(graphInstance,3);
	}else if(strcmp(argv[1],"-b")==0){
		// Run Independent Set Based Coloring Algorithm
		cout<<endl<<"Ejecutando algoritmo Independent Set Based Coloring..."<<endl;
		// TO DO
	}else{
		return -1;
	}
	
	return 0;
}
