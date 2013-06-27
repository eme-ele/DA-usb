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

using namespace std;

// indice del vector identifica ejemplo en dataset
vector<vector<double> > dataset;

int num_clusters;
int num_feats;

int print_centroids(vector<vector<double> > &representatives) {
	for(int i=1; i<representatives.size(); i++){
		for(int j=0; j<representatives[i].size(); j++) {
			cout << representatives[i][j] << " ";
		}
		cout << endl;
	}
}

int print_elem(vector<vector<double> > &container, int id) {
	for(int i=0; i<container[id].size(); i++) {
		cout << container[id][i] << " ";
	}
	cout << endl;
	return 0;
}

int print_dataset(vector<int> &clusters) {
	for(int i=0; i<dataset.size(); i++){
		cout << i << ": ";
		for(int j=0; j<dataset[i].size(); j++) {
			cout << dataset[i][j] << " ";
		}
		cout << ": " << clusters[i] << endl;
	}
}

double euclidean_distance(vector<double> &example, vector<double> &centroid) {
	double distance; 
	for(int i=0; i<num_feats; i++) {
		distance = distance + pow(example[i]-centroid[i], 2); 
	}
	return sqrt(distance); 
}

double objective_function(vector<vector<double> > &centroids, vector<int> &partition) {
	double total;
	for(int i=0; i<dataset.size(); i++){
		total = total + euclidean_distance(dataset[i], centroids[partition[i]]);
	}
	return pow(total,2);
}



// inicializar representacion de clusters con ejemplos al azar
int init_representatives(vector<vector<double> > &representatives) {
	std::random_device rdev{};
	std::uniform_int_distribution<int> distribution(0, dataset.size()-1);
	std::mt19937 engine{rdev()}; // Mersenne twister MT19937
	auto generator = std::bind(distribution, engine);
	
	set<int> random_nums; 
	while(random_nums.size() < num_clusters) {
		random_nums.insert( generator() ); 
	}
	
	int cluster_id = 1;
	for(set<int>::iterator it=random_nums.begin(); it!=random_nums.end(); ++it){
		representatives[cluster_id] = dataset[*it];
		cluster_id++;
	}
	return 0; 
}

// intercambian una rep aleatoria de un cluster por un ejemplo al azar
int random_swap(int &obsolete_cluster, int &added_cluster, vector<int> &clusters, vector<vector<double> > &representatives){
	std::random_device rdev{};
	std::uniform_int_distribution<int> distribution_example(0, dataset.size()-1);
	std::uniform_int_distribution<int> distribution_cluster(1, representatives.size()-1);
	std::mt19937 engine{rdev()}; // Mersenne twister MT19937
	auto generator_example = std::bind(distribution_example, engine);
	auto generator_cluster = std::bind(distribution_cluster, engine);
	

// por alguna extraña razon tengo que hacerlo dos veces para que difiera del random de init_representtaives
	int random_example = generator_example();	
	int random_cluster = generator_cluster();

	added_cluster = random_cluster; 
	obsolete_cluster = clusters[random_example];

	representatives[random_cluster] = dataset[random_example]; 
	clusters[random_example] = random_cluster; 
	
	return 0;
}

int assign_cluster(int example_id, vector<int> &clusters, vector<vector<double> > &representatives) {
	double best_min = pow(10, 10);
	int current_cluster = 0;
	double distance;  

	for(int j=1; j<num_clusters+1; j++) {
			distance = euclidean_distance(dataset[example_id], representatives[j]);
			cout << flush;
			if (distance < best_min) {
				best_min = distance; 
				current_cluster = j;
			}
	}

	clusters[example_id] = current_cluster;
	return 0;	
}

int init_solution(vector<int> &clusters, vector<vector<double> > &representatives) {
	for(int i=0; i<dataset.size(); i++) {
		assign_cluster(i, clusters, representatives);
	}
	return 0;
}


int object_rejection(int cluster_id, vector<int> &clusters, vector<vector<double> > &representatives) {
	for(int i=0; i<clusters.size(); i++){
		if(clusters[i] == cluster_id) {
			assign_cluster(i, clusters, representatives);
		}
	}
}

int object_attraction(int cluster_id, vector<int> &clusters, vector<vector<double> > &representatives){
	for(int i=0; i<dataset.size(); i++){
		if (clusters[i] != cluster_id) {
			double current_distance = euclidean_distance(dataset[i], representatives[clusters[i]]); 
			double attraction_distance = euclidean_distance(dataset[i], representatives[cluster_id]);
			if (attraction_distance < current_distance) {			
				clusters[i] = cluster_id;
			} 
		}	
	}
	return 0;
}

int local_repartition(int obsolete_cluster, int added_cluster, vector<int> &clusters, vector<vector<double> > &representatives){
	object_rejection(obsolete_cluster, clusters, representatives);
	object_attraction(added_cluster, clusters, representatives);
}

int optimal_representatives(vector<int> &clusters, vector<vector<double> > &representatives){
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

		// calcular nuevo representativo
		if (examples_in_cluster > 0) {
			for(int k=0; k<num_feats; k++) {
				totals[k] = totals[k]/examples_in_cluster;
			}
			representatives[i] = totals; 
		}
	}
	return 0; 

}

// carga vectores de features de las instancias
int load_dataset(string file) {
	
	double feat;
	ifstream data;
	
	data.open(file.c_str());
	if (data.is_open()) {
		string line;
		while(data.good()) {
			getline(data, line);
			if (line.compare("") == 0)
				break;
			vector<double> instance;
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

// mensaje de error 
int print_use(){
	cout << "Entrada invalida. Formato: ./ls [num_clusters] [data_file] [num_feats] [num_iter]" << endl << flush;
	return 0;
}


int main(int argc, char *argv[]){
	if(argc != 5){
		print_use();
		return -1;
	}
	
	num_clusters = atoi(argv[1]);
	num_feats = atoi(argv[3]);	
	string filename = argv[2];
	int num_iter = atoi(argv[4]);

	load_dataset(filename);

	vector<int> clusters, new_clusters; 
	vector<vector<double> > representatives, new_representatives; 

	int obsolete_cluster, added_cluster;

	
	// fijar el tamaño de las estructuras
	clusters.resize(dataset.size());
	new_clusters.resize(dataset.size());
	// no hay cluster "0", por lo que centroids[0] y repr[0] se ignora
	representatives.resize(num_clusters+1);	
	new_representatives.resize(num_clusters+1);

	init_representatives(representatives);
	//print_centroids(representatives);

	init_solution(clusters, representatives);
	//print_dataset(clusters);

	int min_counter = 0; 

	new_representatives = representatives;
	new_clusters = clusters; 


	double f = objective_function(representatives, clusters); 
	cout << f << endl;
	double new_f; 
	
	for(int i=0; i<num_iter; i++){

		//print_centroids(representatives);
		random_swap(obsolete_cluster, added_cluster, new_clusters, new_representatives);
		//print_centroids(new_representatives);
		//exit(-1);
		local_repartition(obsolete_cluster, added_cluster, new_clusters, new_representatives);
		//exit(-1);
		optimal_representatives(new_clusters, new_representatives);

		// minimizar 
		new_f = objective_function(new_representatives, new_clusters);
	//	exit(-1);
		
		if(new_f < f ){
			// seguir avanzando 
			f = new_f;
			cout << new_f << " " << i << endl;
			min_counter++;
			representatives = new_representatives;
			clusters = new_clusters; 
		} else {
			new_representatives = representatives;
			new_clusters = clusters; 
		}
		//representatives = new_representatives;
		//clusters = new_clusters; 

	}

	print_dataset(clusters);

}



