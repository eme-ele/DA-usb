#include <stdlib.h>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstring>

/*
	Ricardo Lunar 08-10655	
	Maria Leonor Pacheco 07-41302

	Proyecto # 2
	Evaluacion de metricas de distancia de Strings 
	en el apareamiento de nombres y registros
*/

using namespace std;

map<string, int> memo_levenshtein;
map<string, int> memo_matches;

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

double jaro_winkler(string a, int len_a, string b, int len_b) {

	if (len_a == 0 || len_b == 0) {
		return 0;
	}

	int match_window = floor(max(len_a, len_b/2)) - 1;
	int a_pos = 0;
	int b_pos = 0;
	vector<int> a_seenin_b (len_b, 0);
	vector<int> b_seenin_a (len_a, 0);
	int transpositions = 0;
	int matches = 0;
	char c, a_char, b_char; 

	while (a_pos < len_a || b_pos < len_b) {

		// encontrar el siguiente match de a en b
			while (a_pos < len_a) {
			c = a[a_pos];
			for (int i = max(0, a_pos-match_window); i < min(len_b, a_pos+match_window); i++ ) {
				if (a_seenin_b[i]) { continue; }
				if (c == b[i]) {
					a_char = c;
					a_seenin_b[i] = 1;
					break;
				}
			}
			a_pos++;
			if (a_char != '\0') {
				break;
			}				
		}

		// encontrar el siguiente match de b en a
		while (b_pos < len_b){
			c = b[b_pos];
			for (int i = max(0, b_pos-match_window); i < min(len_a, b_pos+match_window); i++ ) {
				if (a_seenin_b[i]) { continue; }
				if (c == a[i]) {
					b_char = c;
					b_seenin_a[i] = 1;
					break;
				}
			}
			b_pos++;
			if (b_char != '\0') {
				break;
			}		
		}
		
		if (a_char == '\0' && b_char == '\0') { break; }
		if (a_char != '\0' && b_char == '\0') { return 0;}
		if (a_char == '\0' && b_char != '\0') { return 0;}
					
		matches++;
		if (a_char != b_char) { transpositions++; }
	}
	
	if (matches == 0) { return 0;}

	// calculo de la distancia jaro
	double jaro_dist = 1/3*(matches/len_a + matches/len_b + (matches-transpositions)/matches);

	// calculo de ajuste winkler
	int common_subs = 0;
	for(int i = 0; i < min(len_a, len_b); i++) {
		common_subs += (a[i] == b[i]);
	}

	return jaro_dist + (common_subs*(1-jaro_dist));
}

int main(int argc,char *argv[]){
	string a = argv[1];
	string b = argv[2];
	cout << "levenshtein: " << levenshtein(a, 0, a.length(), b, 0, b.length()) << endl;
	cout << "jaro-winkler: " << jaro_winkler(a, a.length(), b, b.length()) << endl;
	return 0;
}

