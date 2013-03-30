#include <stdlib.h>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstring>
#include <algorithm>

#define NOCHAR -1

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

int levenshtein(const string &s1, const string &s2);
bool both_within_a_set(char x,char y);
double similarity_score(char a, char b, double mu, double delta, bool from_monger);
double get_array_max(double array[],int len_t);
double smith_waterman(string a, string b, double score, double penalty, bool from_monger);
double monger_elkan(string a, string b, double score, double penalty);
double jaro_winkler(string a, string b); 
