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

*/

using namespace std;

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
double similarity_score(char a, char b, double mu, double delta, bool from_monger){
    double result;
    if (a==b){
        result = mu;
    }
    else{
        if (from_monger && both_within_a_set(a,b)){
            result = 2;
        }else
            result = -delta;
    }
    return result;
}

// Retorna el valor maximo en el arreglo
double get_array_max(double array[],int len_t){
  double max = array[0];
  for(int i = 1; i<len_t; i++){
        if(array[i] > max){
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
    // Valores de Delta y Mu
    double mu = score, delta = penalty;

    // Se inicializa la matriz
    double H[len_a+1][len_b+1];
    for( int i=0; i<=len_a; i++ ){
        for( int j=0; j<=len_b; j++ ){
            H[i][j] = 0.0;
        }
    }
    
    double temp[4];

    for( int i=1; i<=len_a; i++ ){
        for( int j=1; j<=len_b; j++ ){
            temp[0] = H[i-1][j-1] + similarity_score( a[i-1],b[j-1],mu,delta,from_monger );
            temp[1] = H[i-1][j] - delta;
            temp[2] = H[i][j-1] - delta;
            temp[3] = 0.0;

            // Se busca el maximo valor y se guarda el indice 'ind'
            // donde se encontro tal valor
            H[i][j] = get_array_max( temp,4 );
        }
    }

    // Buscar score maximal de H
    double H_max = 0;
    for( int i=1; i<=len_a; i++ ){
        for( int j=1; j<=len_b; j++ ){
            if( H[i][j]>H_max ){
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

int main(int argc,char *argv[]){
	string a = argv[1];
	string b = argv[2];
	
    double score_SW = 5.0;
    double penalty_SW = 1.0;

    cout << "smith-waterman: " << smith_waterman(a, b, score_SW, penalty_SW,false) << endl;
    cout << "monger-elkan: " << monger_elkan(a, b, score_SW, penalty_SW) << endl;
    return 0;
}
