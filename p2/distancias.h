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

double jaro_winkler(string a, string b); 
int levenshtein(const string &s1, const string &s2);
