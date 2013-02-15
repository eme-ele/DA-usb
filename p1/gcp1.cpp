#include "graph.h"
#include <time.h>
#include <sys/time.h>

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

double timeval_diff(struct timeval *a, struct timeval *b){
	return
	(double)(a->tv_sec + (double)a->tv_usec/1000000) -
 	(double)(b->tv_sec + (double)b->tv_usec/1000000);
}


// ---- Greedy Algorithm for Graph Coloring (LDO, IDO, SDO)---------
// Executes the greedy algorithm for graph coloring given a
// with its vertexes sorted following a certain ordering policy.
//
// Input: graph * Graph to be colored.
//			int flag for type of algorithm
// Output: The graph has all its vertexes colored.
double greedyColoring(graph* graphInstance, int flag){
	int n = graphInstance->num_nodes();
	int vertex, legalColor;
	struct timeval t_ini, t_fin;
	double secs;

	gettimeofday(&t_ini, NULL);
	for(int i=1;i <= n; i++){
		if (flag == 0){
			vertex = i; 								// for First Fit
		} else if (flag == 1){
			vertex = graphInstance->max_degree();		// for LDO
		} else if (flag == 2){
			vertex = graphInstance->max_incidence();	// for SDO
		} else if (flag == 3){
			vertex = graphInstance->max_saturation();	// for IDO
		}
			legalColor = getLegalColor(graphInstance,vertex);
			graphInstance->set_color(vertex,legalColor);
			//cout << legalColor << "-";
	}
	//cout << endl;
	gettimeofday(&t_fin, NULL);	
	return timeval_diff(&t_fin, &t_ini);
}

int greedyIndependentSet(graph * graphInstance, set<int>& independent, set<int>& uncolored){
	set <int> u (uncolored);
	int arbitrary_vertex;

	while(!u.empty()){
		arbitrary_vertex = *u.begin();
		independent.insert(arbitrary_vertex); 
		set<int> neighbours;
		graphInstance->get_neighbours(arbitrary_vertex, neighbours);
		u.erase(arbitrary_vertex);
		for(set<int>::iterator it=neighbours.begin(); it != neighbours.end(); ++it) {
			u.erase(*it);
		}
	}
	return 0;
}

double independentSetBasedColoring(graph * graphInstance){
	set <int> uncolored; 
	graphInstance -> get_uncolored(uncolored);
	int color = 1;
	struct timeval t_ini, t_fin;
	double secs;

	gettimeofday(&t_ini, NULL);
	while(!uncolored.empty()){
		set<int> independent;
		greedyIndependentSet(graphInstance, independent, uncolored);
		for(set<int>::iterator it = independent.begin(); it != independent.end(); ++it){
			graphInstance->set_color(*it, color);
			uncolored.erase(*it);
		}
		color++;		
	}
	gettimeofday(&t_fin, NULL);	
	return timeval_diff(&t_fin, &t_ini);

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
	cout << "\nCargando grafo..." << endl;
	graph *graphInstance = load_graph(argv[2]);
	double secs;
	
	if(strcmp(argv[1],"-f")==0){
		// Run First Fit Algorithm
		cout<<endl<<"Ejecutando algoritmo First Fit..."<<endl;
		secs = greedyColoring(graphInstance,0);
	}else if(strcmp(argv[1],"-l")==0){
		// Run LDO Algorithm
		cout<<endl<<"Ejecutando algoritmo LDO..."<<endl;
		secs = greedyColoring(graphInstance,1);
	}else if(strcmp(argv[1],"-s")==0){
		// Run SDO Algorithm
		cout<<endl<<"Ejecutando algoritmo SDO..."<<endl;
		secs = greedyColoring(graphInstance,2);
	}else if(strcmp(argv[1],"-i")==0){
		// Run IDO Algorithm
		cout<<endl<<"Ejecutando algoritmo IDO..."<<endl;
		secs = greedyColoring(graphInstance,3);
	}else if(strcmp(argv[1],"-b")==0){
		// Run Independent Set Based Coloring Algorithm
		cout<<endl<<"Ejecutando algoritmo Independent Set Based Coloring..."<<endl;
		secs = independentSetBasedColoring(graphInstance);
		// TO DO
	}else{
		return -1;
	}
	cout << "\nTiempo: " << secs << endl;	
	cout << "Numero de colores: " << graphInstance->diff_colors() << endl;
	cout << "Vertice|Color: " << endl;
	graphInstance->print_colors();	
	
	return 0;
}
