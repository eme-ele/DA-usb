#include "distancias.h"
/*
	Ricardo Lunar 08-10655	
	Maria Leonor Pacheco 07-41302

*/

// funcion que calcula la similitud jaro-winkler de dos strings 
// retorna un valor entre 0 y 1
double jaro_winkler(string a, string b) {
	int len_a = a.length();
	int len_b = b.length();

	if (len_a == 0 || len_b == 0) {
		return 0;
	}

	int match_window = floor(max(len_a, len_b)/2) - 1;
	int a_pos = 0;
	int b_pos = 0;
	vector<int> a_seenin_b (len_b, 0);
	vector<int> b_seenin_a (len_a, 0);
	int transpositions = 0;
	int matches = 0;
	char c, a_char, b_char; 

	while (a_pos < len_a || b_pos < len_b) {

		// encontrar el siguiente match de a en b
		a_char = NOCHAR;
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
			if (a_char != NOCHAR) {
				break;
			}				
		}

		// encontrar el siguiente match de b en a
		b_char = NOCHAR;
		while (b_pos < len_b){
			c = b[b_pos];
			for (int i = max(0, b_pos-match_window); i < min(len_a, b_pos+match_window); i++ ) {
				if (b_seenin_a[i]) { continue; }
				if (c == a[i]) {
					b_char = c;
					b_seenin_a[i] = 1;
					break;
				}
			}
			b_pos++;
			if (b_char != NOCHAR) {
				break;
			}		
		}

		if (a_char == NOCHAR && b_char == NOCHAR) { break; }
		if (a_char != NOCHAR && b_char == NOCHAR) { return 0;}
		if (a_char == NOCHAR && b_char != NOCHAR) { return 0;}

		matches++;
		if (a_char != b_char) { transpositions++; }
	}

	if (matches == 0) { return 0;}
	transpositions = transpositions/2;

	// calculo de la distancia jaro
	double jaro_dist = (matches/double(len_a) + matches/double(len_b) + (matches-transpositions)/double(matches))/3.0;

	// calculo de ajuste winkler
	int common_subs = 0;
	for(int i = 0; i < min(len_a, len_b); i++) {
		if (i >= len_a || i >= len_b)
			break;
		if (a[i] != b[i])
			break;
		common_subs = i;
	}

	return jaro_dist;
	//return jaro_dist + (common_subs*0.1*(1-jaro_dist));
}

int levenshtein(const string &a, const string &b) {
	const int len_a(int(a.size()));
	const int len_b(int(b.size()));
 
	if( len_a==0 ) return len_b;
	if( len_b==0 ) return len_a;
 
	int *costs = new int[len_b + 1];
 
	for( int k=0; k<=len_b; k++ ) 
		costs[k] = k;
 
	int i = 0;
	for ( string::const_iterator it1 = a.begin(); it1 != a.end(); ++it1) {
		costs[0] = i+1;
		int corner = i;
		++i;

 
		int j = 0;
		for ( string::const_iterator it2 = b.begin(); it2 != b.end(); ++it2) {
			int upper = costs[j+1];
			if( *it1 == *it2 ) {
				costs[j+1] = corner;
			}
			else {
				int t;
			
				if (upper<corner) 
					t = upper;
				else 
					t = corner;

				if (costs[j] < t ) 
					costs[j+1] = costs[j]+1;
				else 
					costs[j+1] = t+1;

	 		}
 
      		corner = upper;
			++j;
    	}
  	}
 
	int result = costs[len_b];
	delete [] costs; 
	return result;
}
