#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

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

};

#endif
