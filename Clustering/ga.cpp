#include <stdlib.h>
#include <vector>
#include <set>
#include <functional>
#include <math.h> 
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <random>


// Imports for timing
#include <stack>
#include <ctime>

using namespace std;

random_device rdev{};

// TIMING FUNCTIONS
std::stack<clock_t> tictoc_stack;

void tic() {
    tictoc_stack.push(clock());
}

void toc() {
    std::cout << "% Time elapsed: "
              << ((double)(clock() - tictoc_stack.top())) / CLOCKS_PER_SEC
              << std::endl;
    tictoc_stack.pop();
}
// END TIMING FUNCTIONS


typedef vector <double> Element; 
typedef vector< Element > Clustering;
typedef vector< Clustering > Poblacion; 

vector< Element > dataset;


int load_dataset(string file, int num_feats) {
	
	double feat;
	ifstream data;
	
	data.open(file.c_str());
	if (data.is_open()) {
		string line;
		while(data.good()) {
			getline(data, line);
			if (line.compare("") == 0)
				break;
			Element instance;
			string word;
			stringstream ss_line(line);
			int count = 0;
			while(getline(ss_line,word,',') && count < num_feats){
				stringstream ss_double(word);
				ss_double >> feat;
				instance.push_back(feat);
				count++;
			}
			dataset.push_back(instance);
		}
	} else {
		cerr << "ERROR: No se pudo abrir el archivo" << endl;
		exit(1);
	}
	data.close();
	return 0;
}

int init_clustering(int num_clusters, Clustering &centroides){

	centroides.resize(num_clusters);

	//random_device rdev{};
	uniform_int_distribution<int> distribution(0, dataset.size()-1);
	mt19937 engine{rdev()};
	auto generator = std::bind(distribution, engine);
			
	set<int> random_nums; 
	while(random_nums.size() < num_clusters) {
		int r = generator();
		random_nums.insert( r ); 
	}

	int i = 0;
	for(set<int>::iterator it=random_nums.begin(); it!=random_nums.end(); ++it){

		Element elem(dataset[*it].begin(), dataset[*it].end());
		centroides[i] = elem;
		i++;
	}

	return 0; 

}

void print_centroides(Clustering &centroides){	
	cout << centroides.size() << endl;
	for(int i=0; i<centroides.size(); i++){
		for(int j=0; j<centroides[i].size(); j++){
			cout << centroides[i][j] << " ";
		}
		cout << endl;
	}
}


double euclidean_distance(vector<double> &example, vector<double> &centroid) {
	double distance; 
	for(int i=0; i<example.size(); i++) {
		distance = distance + pow(example[i]-centroid[i], 2); 
	}
	return sqrt(distance); 
}	

void assign_cluster(int example_id, vector<int> &clusters, Clustering &centroides) {
	double best_min = pow(10, 10);
	int current_cluster = 0;
	double distance;  

	for(int j=0; j<centroides.size(); j++) {
		distance = euclidean_distance(dataset[example_id], centroides[j]);
		cout << flush;
				
		if (distance < best_min) {
			best_min = distance; 
			current_cluster = j+1;
		}
	}

	if (clusters[example_id] != current_cluster){
		clusters[example_id] = current_cluster;
	}
	
}		
		
void create_solution(vector<int> &clusters, Clustering &centroides) {
	for(int i=0; i<dataset.size(); i++) {
		assign_cluster(i, clusters, centroides);
	}
}

double fitness(Clustering &centroides) {

	double total;
	vector<int> partition(dataset.size());
	create_solution(partition, centroides);

	for(int i=0; i<dataset.size(); i++){
		total = total + euclidean_distance(dataset[i], centroides[partition[i]-1]);
	}
	return pow(total,2);
}



void optimal_representatives(vector<int> &clusters, Clustering &centroides){
	int num_feats = centroides[0].size();
	Element totals;
	totals.resize(num_feats);
	int examples_in_cluster; 

	// para cada cluster
	for(int i=1; i<=centroides.size(); i++) {

		examples_in_cluster = 0; 
		fill (totals.begin(), totals.end(), 0.0); 

		// sumar vectores contenidos
		for(int j=0; j<dataset.size(); j++) {
			if(clusters[j] == i) {
				for(int k=0; k<num_feats; k++) {
					totals[k] = totals[k] + dataset[j][k]; 
				}
				examples_in_cluster++; 
			}
		}

		// calcular nuevo representativo
		if (examples_in_cluster > 0) {
			for(int k=0; k<num_feats; k++) {
				totals[k] = totals[k]/examples_in_cluster;
			}
			centroides[i-1] = totals; 
		}
	}

}


void kmeans_2(Clustering &centroides){
	vector<int> clusters(dataset.size());
	create_solution(clusters, centroides);
	for(int i=0; i<2; i++) {
		optimal_representatives(clusters, centroides);
		create_solution(clusters, centroides);
	}
}


void crossover(Clustering &centroides, Clustering &mate, int num_clusters) {

	//random_device rdev{};
	uniform_int_distribution<int> distribution(1, num_clusters-1);
	mt19937 engine{rdev()};
	auto generator = std::bind(distribution, engine);

	int random1 = generator();
	int random2 = generator();

	int point1 = min(random1, random2);
	int point2 = max(random1, random2);

	Clustering my_middle (centroides.begin()+point1, centroides.begin()+point2);
	Clustering mate_middle (mate.begin()+point1, mate.begin()+point2);

	for(int i=point1; i<point2; i++) {
		centroides[i] = mate_middle[i-point1];
		mate[i] = my_middle[i-point1];
	}

};

void mutar(int num_clusters, Clustering &centroides) {
	
	//random_device rdev{};
	uniform_int_distribution<int> distribution_centroides(0, num_clusters-1);
	uniform_int_distribution<int> distribution_dataset(0, dataset.size()-1);
	mt19937 engine{rdev()};
	auto generator_centroides = bind(distribution_centroides, engine);
	auto generator_dataset = bind(distribution_dataset, engine);

	int random_centroid = generator_centroides();
	int random_example = generator_dataset();

	centroides[random_centroid] = dataset[random_example];
	kmeans_2(centroides);

}


void pob_inicial(int tam_poblacion, int num_clusters, Poblacion &individuos) {

	individuos.resize(tam_poblacion);

	for(int i=0; i<tam_poblacion; i++) {
		Clustering indiv;
		init_clustering(num_clusters, indiv);
		individuos[i] = indiv;	
	}

}

void seleccion(Clustering &individuo, Poblacion &individuos) {
	//random_device rdev{};
	uniform_int_distribution<int> distribution(0, individuos.size()-1);
	mt19937 engine{rdev()};
	auto generator = std::bind(distribution, engine);

	int competidor_1 = generator();
	int competidor_2 = generator();

	double fit1 = fitness(individuos[competidor_1]);
	double fit2 = fitness(individuos[competidor_2]);

	if (fit1 < fit2) 
		individuo = individuos[competidor_1];
	else
		individuo = individuos[competidor_2];
	
	
}

void best_solution(Clustering &mejor, Poblacion &individuos) {
	double best_fitness = pow(10, 10);
	int best_indiv;
	for(int i=0; i<individuos.size(); i++) {
		double fit = fitness(individuos[i]);
		if (fit < best_fitness) {
			best_fitness = fit;
			best_indiv = i;
		}
	}
	cout << "% Min Fitness: " << best_fitness << endl;
	mejor = individuos[best_indiv];
}

int print_use(){
	cout << "Entrada invalida. Formato: ./ga [num_clusters] [data_file] [num_feats] [num_iter] [tam_poblacion] [prob_mutar] [prob_crossover]" << endl << flush;
	return 0;
}

int print_dataset(vector<int> &clusters) {
	for(int i=0; i<dataset.size(); i++){
		for(int j=0; j<dataset[i].size(); j++) {
			cout << dataset[i][j] << ",";
		}
		cout << clusters[i] << endl;
	}
}


int main(int argc, char *argv[]){

	if(argc != 8){
		print_use();
		return -1;
	}


	int num_clusters = atoi(argv[1]);
	string filename = argv[2]; 	
	int num_feats = atoi(argv[3]);
	int num_iteraciones = atoi(argv[4]);	
	int tam_poblacion = atoi(argv[5]);
	double prob_mutar = atof(argv[6]);
	double prob_crossover = atof(argv[7]);

	load_dataset(filename, num_feats);	

	// para generacion de numeros aleatorios
	//random_device rdev{};
	uniform_int_distribution<int> distribution(1, 100);
	mt19937 engine{rdev()};
	auto generator = std::bind(distribution, engine);

	tic();


	Poblacion p_o;
	pob_inicial(tam_poblacion, num_clusters, p_o);

	Poblacion p_i;
	
	
	for(int i=0; i<num_iteraciones; i++){

		int count = 1;
		
		while(p_i.size() < tam_poblacion ) {
			
			Clustering i_1, i_2;
			init_clustering(num_clusters, i_1);
			init_clustering(num_clusters, i_2);

			seleccion(i_1, p_o);	
			seleccion(i_2, p_o);

			if (generator() < prob_crossover) {
				crossover(i_1, i_2, num_clusters);	
			}

			if (generator() < prob_mutar) {
				mutar(num_clusters, i_1);
				mutar(num_clusters, i_2);
			}

			p_i.push_back(i_1);
			p_i.push_back(i_2);	

			count++;
			
		}

		p_o = p_i;
		p_i.clear();
		
		
	}

	Clustering best;
	best_solution(best, p_o);	

	toc();


	vector<int> solucion_final;
	solucion_final.resize(dataset.size());
	create_solution(solucion_final, best);

	print_dataset(solucion_final);

	
	
}
