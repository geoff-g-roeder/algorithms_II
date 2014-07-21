#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

using namespace std;

class Adj_List_Node {

private:
  int dest;
  int weight;
  Adj_List_Node* next;

public:

  Adj_List_Node(int dest, int weight, Adj_List_Node* next = nullptr)
  {
    this->dest = dest;
    this->weight = weight;
    this->next = next;
 }

  // GETTERS  
  int get_dest ( void ) {
    return dest;
  }

  int get_weight ( void ) {
    return weight;
  }

  Adj_List_Node * get_next ( void ) {
    return next;
  }

  // SETTERS
 
  void set_dest ( int dest ) {
    this->dest = dest;
  }

  void set_weight ( int weight ) {
    this->weight = weight;
  }

  void set_next (Adj_List_Node * next) {
    this->next = next;
  }

};

class Vertex {

protected:

  Adj_List_Node * head;


public:
  // Default no-argument constructor for derived classes
  Vertex() {}

  Adj_List_Node * get_head ( void ) {
    return head;
  }

  void set_head ( Adj_List_Node * head ) {
    this->head = head;
    return;
  }
};

class Graph {

private:
  Vertex* vertices;

protected:
  int V;
  int E;


public:
  Graph(int V_in){
    V = V_in;
    vertices = new Vertex[V];

    for (int i =0; i < V; i++) {
      vertices[i].set_head(nullptr);
  }
 }

  // Default no-argument constructor for derived classes
  Graph() {}

  virtual bool add_edge(int s, int d, int w) {
    if ( s < 0 || d < 0 || s > V - 1 || d > V - 1)
      return false;
   Adj_List_Node* source_head = this->vertices[s].get_head();
   Adj_List_Node* dest_head = this->vertices[d].get_head();
   Adj_List_Node* new_node = new Adj_List_Node(d, w, source_head);
   this->vertices[s].set_head(new_node);
   new_node = new Adj_List_Node(s, w, dest_head);
   this->vertices[d].set_head(new_node);
   return true;
  }
  
  int get_V ( void ) {
    return V;
  }

  int get_E( void ) {
    return E;
  }

  virtual Vertex * get_verts ( void ) {
    return vertices;
  }

};

#endif
