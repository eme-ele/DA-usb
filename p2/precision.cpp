#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>


#include "distancias.h"

/*
	Ricardo Lunar 08-10655	
	Maria Leonor Pacheco 07-41302

*/

using namespace std;

struct Objeto{
	string clave;
	string contenido;
};

struct Pair {
	Objeto o1;
	Objeto o2;
	int leven;
	double smith;
	double monger;
	double jaro;
	int relevante;	
};

bool leven_ord (Pair i, Pair j) { return (i.leven<j.leven); }
bool jaro_ord (Pair i, Pair j) { return (i.jaro>j.jaro); }


// relaciones a ser comparadas y numero de pares relevantes
map <string, vector<Objeto> > relaciones;
int relevantes = 0;

int parse_file(string file) {
	string nombre;
	string clave;
	string contenido;
	ifstream data;
	int pos_tab;
	string rest; 

	data.open(file.c_str());
	if (data.is_open()) {
		string line;
		while (data.good()) {
			getline(data, line);
			if ( !line.empty() ) {
				pos_tab = line.find("\t");
				istringstream s((line.substr(0, pos_tab)));
				s >> nombre;

				rest = line.substr(pos_tab+1, line.length());
				pos_tab = rest.find("\t");
				istringstream x((rest.substr(0, pos_tab)));
				x >> clave;

				rest = rest.substr(pos_tab+1, rest.length());
				contenido = rest;
				transform(contenido.begin(), contenido.end(), contenido.begin(), ::tolower);
				

				Objeto o;
				o.clave = clave;
				o.contenido = contenido;
				relaciones[nombre].push_back(o);
			}
		}
	} else {
		cerr << "ERROR: No se pudo abrir el archivo" << endl;
		exit(1);
	}
	data.close();
	return 0;
}

int make_pairs(vector<Pair>& distancias) {
	map<string, vector<Objeto> >::iterator it;
	it = relaciones.begin();
	vector<Objeto> A = it->second;
	++it;
	vector<Objeto> B = it->second;

	for (int i=0; i<A.size(); i++) {
		for(int j=0; j<B.size(); j++) {
			Pair p; 
			p.o1 = A[i];
			p.o2 = B[j];
			p.relevante = 0;
			p.leven = levenshtein(p.o1.contenido, p.o2.contenido);
			p.jaro = jaro_winkler(p.o1.contenido, p.o2.contenido);

			if (p.o1.clave == p.o2.clave) {
				p.relevante = 1;
				relevantes++;
			} 
			distancias.push_back(p);
		}
	}

	cout << distancias.size() << " distancias calculadas" << endl;
	cout << relevantes << " instancias relevantes" << endl;

}

double avg_prec(vector<Pair>& distancias, int metrica) {
	if (metrica == 1)
		sort(distancias.begin(), distancias.end(), leven_ord);		
	if (metrica == 4)
		sort(distancias.begin(), distancias.end(), jaro_ord);

	int acum_correctos = 0;
	int correcto = 0;
	double sumatoria = 0.0;

	for (int i=0; i<1000; i++) {
		correcto = distancias[i].relevante;
		acum_correctos = acum_correctos + correcto;
		sumatoria = sumatoria + (acum_correctos*correcto)/(i+1.0);
	}
	cout << sumatoria << endl;
	cout << acum_correctos << " ejemplos rankeados correctamente" << endl;
	return sumatoria/relevantes;
}

// error message
int print_use(){
	cout << "Entrada invalida. Formato: ./precision <archivo de la instancia>" << endl;
	return 0;
}


int main(int argc, char *argv[]){
	if(argc != 2){
		print_use();
		return -1;
	}
	parse_file(argv[1]);

	vector<Pair> distancias;
	make_pairs(distancias);
	cout << "levenshtein: " << avg_prec(distancias, 1) << endl;
//	cout << "jaro: " << avg_prec(distancias, 4) << endl;
}
	



