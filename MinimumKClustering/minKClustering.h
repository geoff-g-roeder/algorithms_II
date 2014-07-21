#ifndef MINKCLUSTERING_H_
#define MINKCLUSTERING_H_
#include "g_adjacency_list.cpp"
#include <vector>
#include <iostream>
#include <queue>



class UnprocessedEdge {
private:
  int u;
  int v;
  int weight;
  int num_nodes;

public:
 UnprocessedEdge(int u_in, int v_in, int weight_in): 
    u(u_in), v(v_in), weight(weight_in) {};

   int get_u ( void ) const {
    return u;
  }
   int get_v ( void ) const {
    return v;
  }
   int get_weight ( void ) const {
    return weight;
  }
   int get_num_nodes( void ) const {
     return num_nodes;
   }
   void set_num_nodes (int num_nodes) {
     this->num_nodes = num_nodes;
   }
};

class EdgeComparator {
public:
  bool operator() (const UnprocessedEdge* lhs, const UnprocessedEdge* rhs) const {
    return lhs->get_weight() > rhs->get_weight();
  }
};

typedef std::priority_queue<UnprocessedEdge*, std::vector< UnprocessedEdge* >, EdgeComparator> EdgePQ;



class ClusterNode: public Vertex {

private:
  ClusterNode * leader; // For Union-Find ADT
  int leader_index;

public:

  ClusterNode * find_leader ( void ) {
    return leader;
  }
  
  void update_leader ( ClusterNode * leader ) {
    this->leader = leader;
  }
  
  void update_ldr_index ( int new_index ) {
    this->leader_index = new_index;
  }

  int get_ldr_index ( void ) {
    return leader_index;
  }

};

// TODO: write destructor for Clustering

class Clustering : public Graph {
private:  
  int num_leaders; // For Union-Find ADT in Kruskal
  int k; // k is the target number of clusters
  ClusterNode * vertices;
  std::vector < std::vector <int> > followers;

public:
  // Initially a grouping initializes each ClusterNode
  // in its own clustering
  
  Clustering(int V_in, int k_in) {
  V = V_in;
  k = k_in;
  num_leaders = 0;

  // NOTE:
  // Index into arrray is 0-based
  // Node number is 1-based
    vertices = new ClusterNode[V];
    for (int i = 0; i < V; i++) {

      ClusterNode * init = new ClusterNode();
      // Initialize head to nullptr
      init->set_head(nullptr);

      // Add to vertex to graph
      vertices[i] = *init;
      num_leaders++;
    }
    // Initialize leader to this, follower to self
    
    for (int i = 0; i < V; i ++) {
      vertices[i].update_leader(&vertices[i]);
      vertices[i].update_ldr_index(i);
      followers.push_back(std::vector<int> (1,i));
	}
  }

  // GETTER METHODS
   int get_k ( void ) {
    return k;
  }

  int get_num_leaders ( void ) {
    return num_leaders;
  }

  ClusterNode * get_verts ( void ) {
    return vertices;
  }
  
  std::vector< std::vector <int>> * get_followers ( void ) {
    return &followers;
  }

  bool clustering_complete ( void ) {
    return num_leaders == k;
  }

   // EFFECTS: if s,d < 0 || s,d > (V-1), edge is not added
   // and false returned. if forms_cycle(s,d) returns true, edge
   // is not added and false returned. otherwise, add edge to
   // graph and call unionfy(s,d) to update leader pointers
  // After call to unionfy, number of leaders is smaller by 
  // at least 1

bool add_edge(int s, int d, int w) {
  if ( s < 0 || d < 0 || s > V - 1 || d > V - 1)
    return false;
  else if (forms_cycle(s, d))
    return false;
  else if (clustering_complete()) {
    return false;
  }

   unionfy(s, d); 

  // Create adjacency list node entry for both directions
   Adj_List_Node* source_head = vertices[s].get_head();
   Adj_List_Node* dest_head = vertices[d].get_head();
   Adj_List_Node* new_node = new Adj_List_Node(d, w, source_head);
   vertices[s].set_head(new_node);
   new_node = new Adj_List_Node(s, w, dest_head);
   vertices[d].set_head(new_node);


   return true;
  }

  // !!!
  // Returns true iff leader pointer of node s
  // is same as leader pointer of node d
  bool forms_cycle(int s, int d) {
    return &*(vertices[s].find_leader()) == &*(vertices[d].find_leader());
  }


  // DEBUG: this is returning impossible behaviour.
  //        problem not yet clear. give small test case and debug from there
  void unionfy (int s, int d) {
    // Find leader indices

    int s_ldr = vertices[s].get_ldr_index();
    int d_ldr = vertices[d].get_ldr_index();
    vector<int> * s_followers = &followers.at(s_ldr);
     vector<int> * d_followers = &followers.at(d_ldr);
    
    if (s_followers->size() > d_followers->size()) {
      
      while (!d_followers->empty()) {
	int elt = d_followers->back();

	// update followers list
	d_followers->pop_back();
	s_followers->push_back(elt);

	// was a follower of d; change to follower of s
	vertices[elt].update_leader(&vertices[s_ldr]);
	vertices[elt].update_ldr_index(s_ldr);
      }     
      
    } else {
    
      while (!s_followers->empty()) {
	int elt = s_followers->back();

	// update followers list
	s_followers->pop_back();
	d_followers->push_back(elt);

	// was a follower of s; change to follower of d
	vertices[elt].update_leader(&vertices[d_ldr]);
	vertices[elt].update_ldr_index(d_ldr);
    }
    }
    // decrement the number of leaders
    num_leaders--;
  }

  void print_leaders ( void ) {
    for (int i = 0; i < V; i++) {
      if (followers.at(i).empty())
	continue;
      std::cout << "Node " << i + 1  << "'s followers: " << std::endl;
      for (int j = 0; j < followers.at(i).size(); j++)
	std::cout << followers.at(i).at(j) + 1 << ", ";
      std:: cout << std::endl;
    }
  }
};

#endif
