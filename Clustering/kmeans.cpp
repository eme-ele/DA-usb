#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <set>
#include <math.h>
#include <random>
#include <functional>

// Imports for timing
#include <stack>
#include <ctime>

using namespace std;

// for random number generation 
std::random_device rdev{};
mt19937 engine{rdev()};


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

// indice del vector identifica ejemplo en dataset
vector<vector<double> > dataset;
// indice del vector identifica ejemplo en dataset, <int> identifica id de cluster
vector<int> clusters; 
// indice del vector id de cluster
vector<vector<double> > centroids; 

// inicializar clusters y fijar centroides iniciales
int init_clusters_centroids(int num_clusters){
	std::uniform_int_distribution<int> distribution(0, dataset.size()-1);
	auto generator = std::bind(distribution, engine);
	
	set<int> random_nums; 
	while(random_nums.size() < num_clusters) {
		random_nums.insert( generator() ); 
	}

	// inicializar clusters y centroides 
	int cluster_id = 1;
	for(set<int>::iterator it=random_nums.begin(); it!=random_nums.end(); ++it){
		clusters[*it] = cluster_id;
		centroids[cluster_id] = dataset[*it];
		cluster_id++;
	}
	return 0; 
}

// calcular el nuevo centroide de cada cluster 
int recalculate_centroids(int num_clusters, int num_feats) {
	vector<double> totals (num_feats);
	int examples_in_cluster; 

	// para cada cluster
	for(int i=1; i<num_clusters+1; i++) {

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

		// calcular nuevo centroide
		if (examples_in_cluster > 0) {
			for(int k=0; k<num_feats; k++) {
				totals[k] = totals[k]/examples_in_cluster;
			}
			centroids[i] = totals; 
		}
	}
	return 0; 
}

// calcula la distancia euclidea entre el ejemplo y el centroide 
double euclidean_distance(vector<double> &example, vector<double> &centroid, int num_feats) {
	double distance; 
	for(int i=0; i<num_feats; i++) {
		distance = distance + pow(example[i]-centroid[i], 2); 
	}
	return sqrt(distance); 
}

// actualiza los clusters 
int update_clusters(int num_clusters, int num_feats) {
	int is_still_moving = 0; 
	double best_min;
	int current_cluster; 
	double distance;  

	for(int i=0; i<dataset.size(); i++) {
		best_min = pow(10,10);
		current_cluster = 0; 

		for(int j=1; j<num_clusters+1; j++) {
			distance = euclidean_distance(dataset[i], centroids[j], num_feats);
			cout << flush;
			if (distance < best_min) {
				best_min = distance; 
				current_cluster = j;
			}
		}

		if(clusters[i] == 0 || clusters[i] != current_cluster ) {
			clusters[i] = current_cluster;
			is_still_moving = 1;
		}
	}

	return is_still_moving; 
}

// carga vectores de features de las instancias
int load_dataset(string file, int num_feats, int flag) {
	
	double feat;
	int label;
	ifstream data;
	
	data.open(file.c_str());
	if (data.is_open()) {
		string line;
		while(data.good()) {
			getline(data, line);
			if (line.compare("") == 0)
				break;
			 if (line.substr(0, 1) == "%") {
                continue;
            }
			vector<double> instance;
			string word;
			stringstream ss_line(line);
			int count = 0;
			while(getline(ss_line,word,',') && count <= num_feats){
				// partiendo de una clasificacion inicial
				if (count == num_feats && flag) {
					stringstream ss_int(word);
					ss_int >> label;
					clusters.push_back(label);
				}
				// sin clasificacion
				else if(count < num_feats) {
					stringstream ss_double(word);
					ss_double >> feat;
					instance.push_back(feat);
				}
				count++;
			}
			dataset.push_back(instance);
		}
	} else {
		cerr << "ERROR: No se pudo abrir el archivo" << endl;
	}

	if (!flag) {
		clusters.resize(dataset.size());	
	}
	
	data.close();
	return 0;
}

int print_dataset() {
	for(int i=0; i<dataset.size(); i++){
		for(int j=0; j<dataset[i].size(); j++) {
			cout << dataset[i][j] << ",";
		}
		cout << clusters[i] << endl;
	}
}

int print_centroids() {
	for(int i=1; i<centroids.size(); i++){
		for(int j=0; j<centroids[i].size(); j++) {
			cout << centroids[i][j] << " ";
		}
		cout << endl;
	}
}


// mensaje de error 
int print_use(){
	cout << "Entrada invalida. Formato: ./kmeans [num_clusters] [data_file] [num_feats] [1 (opcional para tomar en cuenta clasificacion inicial)]" << endl;
	return 0;
}


int main(int argc, char *argv[]){
	if(argc != 4 && argc != 5){
		print_use();
		return -1;
	}
	
	int num_clusters = atoi(argv[1]);
	string filename = argv[2];
	int num_feats = atoi(argv[3]);
	// empezar desde cero
	int flag = 0; 
	int is_still_moving = 1; 
	int loop_count = 1; 

	// empezar desde un optimo 
	if(argc == 5){
		flag = 1; 
	}


	load_dataset(filename, num_feats, flag);
	// fijar el tamaño de las estructuras
	
	// no hay cluster "0", por lo que centroids[0] se ignora
	centroids.resize(num_clusters+1);

	tic();

	init_clusters_centroids(num_clusters);
	init_clusters_centroids(num_clusters);

	while(is_still_moving) {
		recalculate_centroids(num_clusters, num_feats);
		is_still_moving = update_clusters(num_clusters, num_feats);
		loop_count++;
	}
	
	toc();
	cout << "% Iteraciones: "<< loop_count << endl;	

	print_dataset();


}


