/*
Author: Geoff G. Roeder
Date: 11 July 2014
Purpose: Implement Prim's MST algorithm on a small graph (500 nodes)
*/

#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <vector>
#include <climits>
#include <cstdlib>
#include <set>


struct Adj_List_Node {
  int key;
  int dest;
  int weight;

 Adj_List_Node* next;

 Adj_List_Node( int k, int d, int w,Adj_List_Node* n)
    :key(k),dest(d),weight(w),next(n) {};    
};

struct Vertex {
  
  Adj_List_Node * head;
 };

struct Graph {

  int V;
  int E;
  Vertex* vertices;

  Graph(int V_in):V(V_in){
    vertices = new Vertex[V];

    for (int i =0; i < V; i++) {
      vertices[i].head = NULL;
  }
 }

  void add_edge(int s, int d, int w) {
   Adj_List_Node* new_node = new Adj_List_Node(INT_MAX, d, w, NULL);
    new_node->next = vertices[s-1].head; // shift 1-based to 0-based index
    vertices[s-1].head = new_node;
    new_node = new Adj_List_Node(INT_MAX, s, w, NULL);
    new_node->next = vertices[d-1].head;
    vertices[d-1].head = new_node;
  }

  void print( void ) {
    for (int v = 0; v < V; v++) {
     Adj_List_Node* cursor = vertices[v].head;
      std::cout << "\n Adjacency list of vertex " << v+1 << std::endl;
      while (cursor) {
	std::cout << "-->" << cursor->dest << "(" << cursor->weight << ")";
	cursor = cursor->next;
      }
      std::cout << std::endl;
    }
  }
};

bool X_equals_V(int V_size, std::set<int> &X) {
  // Correctness of this function consequence of loop invariants in Prim_MST
  return X.size() == V_size;

}

       
Graph * Prim_MST(Graph * G) {

  // Let G=(V,E) be a simple graph
  // Let X be the labels of the vertices in the MST-so-far
  // Let V-X be the labels of the vertices in G not in the MST-so-far
  // v in V-X iff G->vertices[v - 1] == false
  // v in X iff G->vertices[v - 1] == true;

  // Let T be the MST-so-far
  Graph * T = new Graph(G->V);
  std::set<int> X;
  std::set<int>::iterator iter;
  Adj_List_Node * list_walker;

  long int sum_of_edges = 0; // Answer to HW #1 pt. 3
  
  // Arbitrary choice of Vertex to start Prim's Algorithm
  int s = 1; 
  
  // initialize X = [s] where s is 1st Vertex in G <-> G->vertices[0].head
  // NOTE: array of Vertex is 0-based, but 1<= Vertex.vertex_label <= 500
  X.insert(s);
  
  while (!X_equals_V(G->V,X)) {
    // Loop guard evaluates to false once all vertices v in V are now in X
    
    int v = -1;
    int u = -1;    
    int candidate_min_weight = INT_MAX;

    // Req(u,v) := (u,v) is cheapest edge of G with u in X, v not in X seen so far
    // Invariant: either (u,v) = (u,-1) or Req(u,v) is true

    for (iter = X.begin(); iter != X.end(); iter++) {
      // Goal: find the cheapest v that satisfies requirements

      // Consider the current u, stored at iter
      int u_current = *iter;
      //     std::cout << u_current << std::endl; // DEBUG

      list_walker = G->vertices[u_current-1].head;
      
      while (list_walker!= NULL) {

	// v is not in X iff X.find(v) returns X.end() iterator
	// since X.end() iterator is returned when function can't find
	// the element in the set X

	if (X.find(list_walker->dest) == X.end()) {

	  // I found a (u,v) where u in X, v not in X
	  // if this v is the new minimum, update minimum
	  if (list_walker->weight < candidate_min_weight) {
	    v = list_walker->dest;
	    u = u_current; // Take a reference to u so we can add to T
	    candidate_min_weight = list_walker->weight; // This is now the weight to beat
	  }
	}

	  // Otherwise, found a (u,v) where u,v in X
	  // Req(u,v) is false, so ignore candidate

	  // Check the next candidate v for this u
	  list_walker = list_walker->next;

      }
    }
    
     
    //  found (u,v) s.t. Req(u,v) true
    // add to T since (u,v) satisfies greedy requirement

    T->add_edge(u,v,candidate_min_weight);
    
    sum_of_edges += (long)candidate_min_weight;

    // add v to X
    X.insert(v);
  }
  
  delete list_walker;    
  std::cout << "The sum of edges in the Minimum Spanning Tree is " << sum_of_edges << std::endl;
  return T;
}


int main ( void ) {

  // Algorithms II HW #1.3: Prim's algorithm
  // REQUIRES: formatted text file in project dir as described in comments

  std::ifstream read_file("edges.txt");
  std::string in_str;
  std::string first_line;
  std::getline(read_file,first_line);

  // number of nodes is first number in first line of file
  // ""        edges "" second ""  
  int num_nodes = atoi(first_line.substr(0,first_line.find(" ")).c_str());
  std::string num_edges = first_line.substr(first_line.find(" ")+1);
  
  Graph* G =  new Graph(num_nodes);
  G->E = atoi(num_edges.c_str());

  // Format of every line in "edges.txt" after and not including first line:
  // source dest weight
  // where 1 <= source, dest <= 500, weight nonzero integer

  while (std::getline(read_file, in_str)) {
    std::string s_val1 = in_str.substr(0,in_str.find(" "));
    std::string s_val2_temp = in_str.substr(s_val1.size() + 1);
    std::string s_val2 = s_val2_temp.substr(0,s_val2_temp.find(" "));
    std::string s_val3_temp = in_str.substr(s_val2.size() + s_val1.size() + 2);
    std::string s_val3 = s_val3_temp.substr(0, s_val3_temp.find(" "));
    G->add_edge(std::atoi(s_val1.c_str()),std::atoi(s_val2.c_str()),std::atoi(s_val3.c_str()));    
 }

  // Prim: starting at zero,
  Graph * mst = Prim_MST(G);
  return 0;
}      


//========================================================
// APPENDICES: other attempted solutions

    /* This is an implementation the produces a forest of minimum edges rather than a tree. */

    // Walk through array of Vertex
    // After execution, min_weight* variables hold minimum edge information
    // std::cout << "looking for next smallest edge..." << std::endl;     // DEBUG
    /*   

    int min_weight = INT_MAX;
    int min_weight_src = 0;
    int min_weight_dest = 0;

	 for (int i = 0; i < V; i++) {

      adj_list_cursor = G->vertices[i].head;
      // Walk through list of Adj_List_Node (representing edges incident on Vertex)

      // Loop invariant: min_weight is the smallest weight of edges
      //                 seen so far

      while (adj_list_cursor != NULL) {

	// if a vertex mark is true, destination Vertex is in mst already
	// adding another edge will produce a cycle, so ignore it
	if (G->vertices[adj_list_cursor->dest].mark == true) {
	  adj_list_cursor = adj_list_cursor->next;
	  continue;
	} else if (adj_list_cursor->weight < min_weight) {

	  // Update current minimum weight edge 
	  min_weight = adj_list_cursor->weight;
	  min_weight_src = i + 1; // Correct 0-based to 1-based index
	  min_weight_dest = adj_list_cursor->dest;
	  }

	adj_list_cursor = adj_list_cursor->next;
      }
    }
    
    // Add source and dest vertices to MST-so-far
    G->vertices[min_weight_dest].mark = true;

    // Add edge to MST
    if (!(MST->edge_in_graph(min_weight_src,min_weight_dest,min_weight))){
	MST->add_edge(min_weight_src,min_weight_dest, min_weight);
	    items_so_far++; // DEBUG
      }
    
    
    } */
	  

/*
bool X_equals_V(Graph * G) {
  // Let G be a simple graph. 
  // X_equals_V returns true iff V-X is empty iff X=V
  
  int V = G->V;
  bool result = true;
  for (int i = 0; i < V; i++) {
    result = result && (G->vertices[i].mark);
    if (result == false) break; // Short-circuit
  }
  
  return result;
}*/

/*

  bool edge_in_graph(int s, int d, int w) {
    for (int i = 0; i < V; i++) {
      Adj_List_Node * cursor = vertices[i].head;
      while (cursor != NULL) {
	if ((i+1) == s && cursor->dest == d && cursor->weight == w)
	  return true;
	      cursor = cursor->next;
      }

    }
    return false;
  }
*/

/*

Abortive attempt to write a fast implementation using PQ implemented
over vector<HeapNode*>

struct HeapNode {

  int key;
  int vertexNumber;

  HeapNode(int k, int vN):key(k),vertexNumber(vN) {}

  const int getKey( void ) {
    return key;
  }
  void setKey ( int newKey ) {
    key = newKey;
  }
  const int getVertexNumber( void ) {
    return vertexNumber;
  }
  void setVertexNumber( int newVN ) {
    vertexNumber = newVN;
  }
};

class MinHeapNodeComparator {
public:
  bool operator() (const HeapNode* lhs, const HeapNode* rhs) const {
    return lhs->key > rhs->key;
  }
};

typedef std::priority_queue<HeapNode*, std::vector<HeapNode*>, MinHeapNodeComparator> HeapNodePQ;

*/
