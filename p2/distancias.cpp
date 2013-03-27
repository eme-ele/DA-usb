#include <stdlib.h>
#include <string>
#include <sstream>
#include <map>
#include <iostream>


/*
	Ricardo Lunar 08-10655	
	Maria Leonor Pacheco 07-41302

	Proyecto # 2
	Evaluacion de metricas de distancia de Strings 
	en el apareamiento de nombres y registros
*/

using namespace std;

map<string, int> memo_levenshtein;

double get_min(double x, double y, double z){
	return min(x, min(y, z));
}

// funcion que calcula distancia levenshtein usando memoization
double levenshtein(string a, int i, int len_a, string b, int j, int len_b) {

	// transformar numeros a strings
	stringstream i_s, lena_s, lenb_s, j_s;
	i_s << i;
	j_s << j;
	lena_s << len_a;
	lenb_s << len_b;

	// crear clave para memorizacion
	string key = i_s.str() + "," + lena_s.str() + "," + j_s.str() + "," + lenb_s.str();

	if (memo_levenshtein.count(key) > 0) { 
		return memo_levenshtein[key]; 
	}
	if (len_a == 0) {
		return len_b;
	} 
	if (len_b == 0) {
		return len_a;
	}

	int cost;
	if (a[i] != b[j]) {
		cost = 1;
	}
	
	double dist = get_min(levenshtein(a, i+1, len_a-1, b, j, len_b)+1,
						  levenshtein(a, i, len_a, b, j+1, len_b-1)+1,
						  levenshtein(a, i+1, len_a-1, b, j+1, len_b-1)+cost);
	memo_levenshtein[key] = dist;
	return dist;
	
}

int main(int argc,char *argv[]){
	string a = argv[1];
	string b = argv[2];
	cout << levenshtein(a, 0, a.length(), b, 0, b.length()) << endl;
	return 0;
}

