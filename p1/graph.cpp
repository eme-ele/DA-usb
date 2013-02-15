#include "graph.h"

using namespace std;

graph::graph(int num_nodes){
	graph_container.resize(num_nodes);
	colors.resize(num_nodes);
}

// add edge to graph 
void graph::add_edge(int v1, int v2){
	// v1 -> v2
	set<int> adjacent; 
	adjacent = graph_container[v1];
	adjacent.insert(v2);
	graph_container[v1] = adjacent;
	// v2 -> v1
	adjacent = graph_container[v2];
	adjacent.insert(v1);
	graph_container[v2] = adjacent; 
}

// return all vertices adjacent to v
void graph::get_neighbours(int v, set<int>& neighbours) {
	neighbours = graph_container[v];
}

void graph::get_uncolored(set<int>& uncolored) {
	for(int i=1; i<graph_container.size(); i++){
		if (colors[i] == 0) {
			uncolored.insert(i);
		}
	}
}

// prints graph on console
void graph::print(){
	for(int i=1; i < graph_container.size(); i++) {
		cout << i << "|";
		for (set<int>:: iterator it=graph_container[i].begin(); it!=graph_container[i].end(); ++it){
			cout << *it << " ";
		}
		cout << endl; 
	}
}

void graph::print_colors(){
	for(int i=1; i< graph_container.size(); i++){
		cout << "\t" <<  i << "|" << colors[i] << " ";
		if (i % 10 == 0){
			cout << endl;
		}
	}
	cout << endl;
}

// sets color to vertex
void graph::set_color(int v, int k){
	colors[v] = k;
}

// returns coloring of a given vertex
int graph::get_color(int v){
	return colors[v];
}

int graph::degree(int v){
	return graph_container[v].size();
}

//returns uncolored vertex with max degree
int graph::max_degree(){
	int max = 0;
	int vertex;
	for(int i=1; i<graph_container.size(); i++){
		if (graph_container[i].size() > max && colors[i] == 0){
			max = graph_container[i].size();
			vertex = i;
		}
	}
	return vertex;
}

// returns number of nodes 
int graph::num_nodes(){
	return graph_container.size()-1;
}

// returns saturation of a given vertex
int graph::saturation(int v){
	set<int> diff_colors;
	for(set<int>::iterator it=graph_container[v].begin(); it!=graph_container[v].end(); ++it){
		if (colors[*it] != 0){
			diff_colors.insert(colors[*it]);
		}
	}
	return diff_colors.size();
}

//returns uncolored vertex with max saturation
int graph::max_saturation(){
	int new_sat;
	int max = 0;
	int vertex = 1;
	for(int i=1; i<graph_container.size(); i++){
		new_sat = saturation(i);
		if ( new_sat > max && colors[i] == 0){
			max = new_sat;
			vertex = i;
		}
	}
	return vertex;
}

// returns incidence of a given vertex
int graph::incidence(int v){
	int num_colored = 0;
	for(set<int>::iterator it=graph_container[v].begin(); it!=graph_container[v].end(); ++it){
		if (colors[*it] != 0){
			num_colored++;
		}
	}
//	cout << "incidence: " << num_colored << endl;
	return num_colored;
}

//return uncolored vertex with max incidence
int graph::max_incidence(){
	int new_inc;
	int max = 0;
	int vertex = 1;
	for(int i=1; i<graph_container.size(); i++){
		new_inc = incidence(i);
		if ( new_inc > max && colors[i] == 0){
			max = new_inc;
			vertex = i;
		}
	}
//	cout << "Max incidence: " << vertex << endl;
	return vertex;

}

int graph::diff_colors(){
	set<int> dif;
	for(int i =0; i<colors.size();i++){
		dif.insert(colors[i]);
	}
	return dif.size();
}


// load a graph from dimacs file
graph * load_graph(string file) {
	int v1;
	int v2;
	int num_nodes;
	ifstream data;
	graph * instance;
	//aux 
	int pos_space;
	string rest; 

	data.open(file.c_str());
	if (data.is_open()) {
		string line;
		while (data.good()) {
			getline(data, line);
			// line that specifies number of nodes 
			if (line.substr(0,1) == "p") {
				line.erase(0,7);
				pos_space = line.find(" ");
				istringstream s((line.substr(0, pos_space)));
				s >> num_nodes;
				instance = new graph(num_nodes+1); 
			}
			// comments
			if (line.substr(0, 1) == "c") { 
				continue;
			}
			// edge to be added 
			if (line.substr(0, 1) == "e"){
				line.erase(0,2);
				int pos_space = line.find(" ");
				istringstream s((line.substr(0, pos_space)));
				s >> v1;
				rest = line.substr(pos_space+1, line.length());
				istringstream t(rest);
				t >> v2;
				instance->add_edge(v1, v2);
			}	 
			// any other case
			else {
				continue;
			}
		}
	}
	else {
		cerr << "ERROR: No se pudo abrir el archivo" << endl;
		exit(1);
	}
	data.close();
	return instance; 
}



