#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

vector<vector<double> > dataset;


// carga vectores de features de las instancias
int load_dataset(string file, int num_feats) {
	
	double feat;
	ifstream data;
	
	data.open(file.c_str());
	if (data.is_open()) {
		string line;
		while(data.good()) {
			getline(data, line);
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

int print_dataset() {
	for(int i=0; i<dataset.size(); i++){
		for(int j=0; j<dataset[i].size(); j++) {
			cout << dataset[i][j] << " ";
		}
		cout << endl;
	}
}

// mensaje de error 
int print_use(){
	cout << "Entrada invalida. Formato: ./kmeans [num_clusters] [data_file] [num_feats]" << endl;
	return 0;
}


int main(int argc, char *argv[]){
	if(argc != 4){
		print_use();
		return -1;
	}
	
	int num_clusters = atoi(argv[1]);
	string filename = argv[2];
	int num_feats = atoi(argv[3]);

	load_dataset(filename, num_feats);
	print_dataset();

}


