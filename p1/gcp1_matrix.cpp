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

// grafo y coloracion
vector<vector<int> > instance;
vector<int> color;

// par  vertice-grado para ordenacion
struct Pair{
	int vertex;
	int property;
	bool operator< (Pair other) const { return (property<other.property); }

};

// propiedades de los vertices s
vector<Pair> degree;
vector<set <int> > saturation;
vector<int> incidence;


// lee un grafo en formato dimacs y llena la estructura
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
				saturation.resize(num_nodes+1);
				incidence.resize(num_nodes+1);
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

// obtiene el menor color posible para un vertice
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


// calcular diferencia de tiempo
double timeval_diff(struct timeval *a, struct timeval *b){
	return
	(double)(a->tv_sec + (double)a->tv_usec/1000000) -
 	(double)(b->tv_sec + (double)b->tv_usec/1000000);
}

double first_fit(){
	int n = instance.size();
	int vertex, legalColor;
	struct timeval t_ini, t_fin;
	double secs;
	gettimeofday(&t_ini, NULL);
	for(int i=1; i<n; i++){
		vertex = i;
		legalColor = getLegalColor(vertex);
		color[vertex] = legalColor;
	}
	gettimeofday(&t_fin, NULL);	
	return timeval_diff(&t_fin, &t_ini);
}

// ordena los vertices de acuerdo a su grado
int degree_ordering(){
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

// obtiene el vertice no coloreado con mayor grado
int max_degree(){
	int vertex;
	int n = degree.size()-1;
	for(int i=n; i>0; i--){
		vertex = degree[i].vertex;
		if(color[vertex] == 0){
			return vertex;
		}
	}
}


double latest_degree_ordering(){
	int n = color.size();
	int vertex, legalColor;
	struct timeval t_ini, t_fin;
	double secs;
	gettimeofday(&t_ini, NULL);
	degree_ordering();
	for (int i=n-1; i>0; i--){
		vertex = max_degree();
		legalColor = getLegalColor(vertex);
		color[vertex] = legalColor;
	}
	gettimeofday(&t_fin, NULL);	
	return timeval_diff(&t_fin, &t_ini);
}

// obtiene el vertice no coloreado con maxima saturacion
int max_saturation(){
	int sat;
	int vertex;
	int max_sat = 0;
	for(int i=1; i<color.size(); i++){
		if (color[i] == 0){
			sat = saturation[i].size();
			if(sat > max_sat){
				vertex = i;
				max_sat = sat;
			}
		}
	}
	return vertex;
}

// actualiza saturaciones de vecinos
int update_neighbours_saturation(int vertex, int color){
	int n = instance[vertex].size();
	for (int i=1; i<n; i++){
		if (instance[vertex][i] == 1){
			saturation[i].insert(color);		
		}
	}
	return 0;
}

// obtiene el vertice no coloreado con mayor incidencia
int max_incidence(){
	int inc;
	int vertex;
	int max_inc = 0;
	for(int i=1; i<color.size(); i++){
		if (color[i] == 0){
			inc = incidence[i];
			if(inc > max_inc){
				vertex = i;
				max_inc = inc;
			}
		}
	}
	return vertex;
}

// actualiza las incidencias de vecinos
int update_neighbours_incidence(int vertex){
	int n = instance[vertex].size();
	for (int i=1; i<n; i++){
		if (instance[vertex][i] == 1){
			incidence[i]++;
		}
	}
}

double saturation_degree_ordering(){
	int n = color.size();
	int legalColor, vertex;
	struct timeval t_ini, t_fin;
	double secs;
	gettimeofday(&t_ini, NULL);
	color[1] = 1;
	update_neighbours_saturation(1, 1);
	for (int i=2; i<n; i++){
		vertex = max_saturation();
		legalColor = getLegalColor(vertex);
		color[vertex] = legalColor;
		update_neighbours_saturation(vertex, legalColor);
	}
	gettimeofday(&t_fin, NULL);	
	return timeval_diff(&t_fin, &t_ini);
}

double incidence_degree_ordering(){
	int n = color.size();
	int legalColor, vertex;
	struct timeval t_ini, t_fin;
	double secs;
	gettimeofday(&t_ini, NULL);
	color[1] = 1;
	update_neighbours_incidence(1);
	for (int i=2; i<n; i++){
		vertex = max_incidence();
		legalColor = getLegalColor(vertex);
		color[vertex] = legalColor;
		update_neighbours_incidence(vertex);
	}
	gettimeofday(&t_fin, NULL);	
	return timeval_diff(&t_fin, &t_ini);

	
}

// calcula el numero de colores usados
int diff_colors(){
	set<int> dif;
	for(int i =0; i<color.size();i++){
		dif.insert(color[i]);
	}
	return dif.size();
}

// imprime coloracion obtenida
int print_colors(){
	for(int i=1; i< color.size(); i++){
		cout << "\t" <<  i << "|" << color[i];
		if (i % 10 == 0){
			cout << endl;
		}
	}
	cout << endl;
}

// mensaje de error 
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
		secs = first_fit();
	}else if(strcmp(argv[1],"-l")==0){
		// Run LDO Algorithm
		cout<<endl<<"Ejecutando algoritmo LDO..."<<endl;
		secs = latest_degree_ordering();
	}else if(strcmp(argv[1],"-s")==0){
		// Run SDO Algorithm
		cout<<endl<<"Ejecutando algoritmo SDO..."<<endl;
		secs = saturation_degree_ordering();
	}else if(strcmp(argv[1],"-i")==0){
		// Run IDO Algorithm
		cout<<endl<<"Ejecutando algoritmo IDO..."<<endl;
		secs = incidence_degree_ordering();
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
	cout << "Vertice|Color: " << endl;
	print_colors();	
	
	return 0;
}
