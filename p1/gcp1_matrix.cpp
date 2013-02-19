#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include <set>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <algorithm>


using namespace std;


vector<vector<int> > instance;
vector<int> color;

struct Pair{
	int vertex;
	int property;
	bool operator< (Pair other) const { return (property<other.property); }

};


vector<Pair> degree;


// load a graph from dimacs file
int load_graph(string file) {
	int v1;
	int v2;
	int num_nodes;
	ifstream data;
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
				instance.resize(num_nodes+1);
				for(int i=1; i < num_nodes+1; i++){
					instance[i].resize(num_nodes+1);
				}
				color.resize(num_nodes+1);
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
				instance[v1][v2] = 1;
				instance[v2][v1] = 1;
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
	return 0;
}

int vector_degree(){
	degree.resize(instance.size()-1);
	for(int i=1; i<instance.size(); i++){
		int d = 0;
		for(int j=1; j<instance.size(); j++){
			if (instance[i][j] == 1){
				d++;
			}
		}
		Pair p;
		p.vertex = i;
		p.property = d;
		degree[i-1] = p;
	}
	sort(degree.begin(), degree.end());
	return 0;
}


int getLegalColor(int vertex){
	int i, valid, c;
	int n = instance.size();
	for(i = 1; i<n; i++){
		valid = 1;
		for (int j=1; j<n; j++){
			// check if adjacent 
			if (instance[vertex][j] == 1){
				c = color[j];
				// if color invalid
				if(c == i){ 
					valid = 0;
					break;
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

double greedyColoring(int flag){
	int n = instance.size();
	int vertex, legalColor;
	struct timeval t_ini, t_fin;
	double secs;

	gettimeofday(&t_ini, NULL);

	for(int i=1; i<n; i++){
		if (flag==0){
			vertex = i;
		} else if (flag==1){
			vertex = degree[n-1-i].vertex;
		}
		legalColor = getLegalColor(vertex);
		color[vertex] = legalColor;
	}

	gettimeofday(&t_fin, NULL);	
	return timeval_diff(&t_fin, &t_ini);
}

int diff_colors(){
	set<int> dif;
	for(int i =0; i<color.size();i++){
		dif.insert(color[i]);
	}
	return dif.size();
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
	load_graph(argv[2]);


	double secs;
	
	if(strcmp(argv[1],"-f")==0){
		// Run First Fit Algorithm
		cout<<endl<<"Ejecutando algoritmo First Fit..."<<endl;
		secs = greedyColoring(0);
	}else if(strcmp(argv[1],"-l")==0){
		// Run LDO Algorithm
		vector_degree();
		cout<<endl<<"Ejecutando algoritmo LDO..."<<endl;
		secs = greedyColoring(1);
	}else if(strcmp(argv[1],"-s")==0){
		// Run SDO Algorithm
		cout<<endl<<"Ejecutando algoritmo SDO..."<<endl;
		//secs = greedyColoring(2);
	}else if(strcmp(argv[1],"-i")==0){
		// Run IDO Algorithm
		cout<<endl<<"Ejecutando algoritmo IDO..."<<endl;
		//secs = greedyColoring(3);
	}else if(strcmp(argv[1],"-b")==0){
		// Run Independent Set Based Coloring Algorithm
		cout<<endl<<"Ejecutando algoritmo Independent Set Based Coloring..."<<endl;
	//	secs = independentSetBasedColoring(graphInstance);
		// TO DO
	}else{
		return -1;
	}
	cout << "\nTiempo: " << secs << endl;	
	cout << "Numero de colores: " << diff_colors() << endl;
	//cout << "Vertice|Color: " << endl;
	//graphInstance->print_colors();	
	
	return 0;
}
