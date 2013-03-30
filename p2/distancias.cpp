#include "distancias.h"
/*
	Ricardo Lunar 08-10655	
	Maria Leonor Pacheco 07-41302

*/

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

// Verifica si ambos caracteres se encuentran el
// algun conjunto. De ser asi, en el algoritmo
// de Monger-Elkan se considera un score distinto.
bool both_within_a_set(char x,char y){
    string set1 = "dt";
    string set2 = "gj";
    string set3 = "lr";
    string set4 = "mn";
    string set5 = "bpv";
    string set6 = "aeiou";
    string set7 = ",.";
    
    char a = tolower(x);
    char b = tolower(y);

    if ( (set1.find(a,0)!=string::npos && set1.find(b,0)!=string::npos)
        || (set2.find(a,0)!=string::npos && set2.find(b,0)!=string::npos)
        || (set3.find(a,0)!=string::npos && set3.find(b,0)!=string::npos)
        || (set4.find(a,0)!=string::npos && set4.find(b,0)!=string::npos)
        || (set5.find(a,0)!=string::npos && set5.find(b,0)!=string::npos)
        || (set6.find(a,0)!=string::npos && set6.find(b,0)!=string::npos)
        || (set7.find(a,0)!=string::npos && set7.find(b,0))!=string::npos){
        return true;
    }
    return false;
}

// Retorna el valor de similaridad entre dos caracteres dados
// considera 'match aproximado' planteado por monger en el paper
// dado en la bibliografia ( http://pdf.aminer.org/000/473/089/the_field_matching_problem_algorithms_and_applications.pdf )
double similarity_score(char a, char b, double score, double penalty, bool from_monger){
    double result;
    if (a==b){
        result = score;
    }
    else{
        if (from_monger && both_within_a_set(a,b)){
            result = 2;
        }else
            result = -penalty;
    }
    return result;
}

// Retorna el valor maximo en el arreglo
double get_array_max(double array[],int len_t){
  double max = array[0];
  for (int i = 1; i<len_t; i++){
        if (array[i] > max){
            max = array[i];
        }
  }
  return max;
}

// Calcula la distancia Smith-Waterman dados los strings, sus tamaños,
// el puntaje y la penalización a considerar
double smith_waterman(string a, string b, double score, double penalty, bool from_monger){
    int len_a = a.length();
    int len_b = b.length();

    // Se inicializa la matriz
    double H[len_a+1][len_b+1];
    for ( int i=0; i<=len_a; i++ ){
        for ( int j=0; j<=len_b; j++ ){
            H[i][j] = 0.0;
        }
    }
    
    double temp[4];

    for ( int i=1; i<=len_a; i++ ){
        for ( int j=1; j<=len_b; j++ ){
            temp[0] = H[i-1][j-1] + similarity_score( a[i-1],b[j-1],score,penalty,from_monger );
            temp[1] = H[i-1][j] - penalty;
            temp[2] = H[i][j-1] - penalty;
            temp[3] = 0.0;

            // Se busca el maximo valor y se guarda el indice 'ind'
            // donde se encontro tal valor
            H[i][j] = get_array_max( temp,4 );
        }
    }

    // Buscar score maximal de H
    double H_max = 0;
    for ( int i=1; i<=len_a; i++ ){
        for ( int j=1; j<=len_b; j++ ){
            if ( H[i][j]>H_max ){
                H_max = H[i][j];
            }
        }
    }
    
    return H_max;
}

// Calcula la distancia Monger-Elkan basado en el algoritmo
// de Smith-Waterman
double monger_elkan(string a, string b, double score, double penalty){
    
    return smith_waterman(a,b,score,penalty,true);

}

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
