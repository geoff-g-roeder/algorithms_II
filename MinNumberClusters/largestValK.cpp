
#include <limits>
#include "g_adjacency_list.cpp"
#include "minKClustering.h"
#include <gtest/gtest.h>
#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <bitset>
#include <functional>


// DEBUG
int kcount = 0;
int kadded = 0;

int knum_nodes; // KLUDGE avoid use of this global variable
string kfile_name = "clustering_big.txt";
std::unordered_map<std::bitset<24>,int> NodesInGraph;
std::unordered_map<std::bitset<24>,int> EntriesSeen;

std::hash<std::bitset<24>> kbitset_hash;




void removeAllWhiteSpace(std::string& str) {
  int pos;
  pos = str.find(" ");
  // Find returns -1 if target string not found
  while (pos != -1){
    str.erase(str.find(" "),1);
    pos = str.find(" ");
  }
}



// Name of input file is hardcoded in function
// TODO: Rebuild this function to be robust against formatting
//       differences in input file
vector<std::bitset<24>> ParseInputFile ( void ) {
  std::hash<std::bitset<24>> kbitset_hash;

  std::ifstream readFile(kfile_name);
  std::string inStr;
  std::string firstLine;
  std::getline(readFile,firstLine);

  knum_nodes = atoi(firstLine.substr(0,firstLine.find(" ")).c_str());

  vector<std::bitset<24>> every_bitset;

  while (std::getline(readFile, inStr)) {
    removeAllWhiteSpace(inStr);
    std::bitset<24> these_bits (inStr);
    every_bitset.push_back(these_bits);
    NodesInGraph.emplace(these_bits, kbitset_hash(these_bits));
  }

  /* : KLUDGE : this should be a test; given more time,
     test this function using framework */
  /*
    for (int i = 0; i < every_bitset.size(); i++ ){
    std::cout << "is " << every_bitset.at(i).to_string() << " in the set? " << std::endl
    << "The answer is ";

    if ( NodesInGraph.find(every_bitset.at(i)) != NodesInGraph.end() ) {
    std::cout << "true" << std:: endl;
    } else{
    std:: cout << "false" << std::endl;
    break;
    }

    }

    std::cout << "the number of nodes in the vector is " << every_bitset.size() << std::endl;
  */
  std::cout << NodesInGraph.size() << std::endl;
  return every_bitset;
}
/*
StaticClustering * findClusteringLessThan3 ( vector<std::bitset<24>> nodes ) {



  StaticClustering * ret_Clustering = new StaticClustering(knum_nodes,4);
  UnprocessedEdge * edgeToProcess;

  // std::cout << edges->size() << std::endl;
  while (!ret_StaticClustering->clustering_complete()) {
    edgeToProcess = edges->top();
    edges->pop();
    ret_StaticClustering->add_edge(edgeToProcess->get_u(),edgeToProcess->get_v(),edgeToProcess->get_weight());
  }

  ret_StaticClustering->print_leaders();
  //   std::cout << edges->size() << std::endl;
  
  // std::cout << ret_StaticClustering->get_num_leaders() << std::endl;
  int minEdgeDist = std::numeric_limits<int>::max();
  while (!edges->empty()) {
    edgeToProcess = edges->top();
    edges->pop();

    if (ret_StaticClustering->forms_cycle(edgeToProcess->get_u(),edgeToProcess->get_v()))
      continue;
    else {   
      int tempDist = edgeToProcess->get_weight();
      if ( tempDist < minEdgeDist)
	minEdgeDist = tempDist;
  }

  }

  std::cout << "Max spacing: " << std::endl << minEdgeDist << std::endl;  
  return ret_StaticClustering;
  }
*/

// for a given bitset BSTR of length 24, generate all possible
// strings CSTR with hamming distance 1 from BSTR. If CSTR
// is in NodesInGraph, add to return vector.
void GenerateDist1 (vector<std::bitset<24>> &ret, std::bitset<24> src) {
  for (int i = 0; i < 24; i++) {
    std::bitset<24> new_bset (src.to_string());
    new_bset.flip(i);
    kcount++;
    if (NodesInGraph.find(new_bset) != NodesInGraph.end()) {
      ret.push_back(new_bset);
      kadded++;
    }
  }
  return;
}

// for a given bitset BSTR of length 24, generate all possible
// strings CSTR with hamming distance 2 from BSTR. If CSTR
// is in NodesInGraph, add to return vector.
void GenerateDist2 (vector<std::bitset<24>> &ret, std::bitset<24> src) {

  for (int i = 0; i < 24; i++) {
    for (int j = i + 1; j < 24; j++) {
      kcount ++;
      std::bitset<24> new_bset (src.to_string());
      new_bset.flip(i);
      new_bset.flip(j);
      if (NodesInGraph.find(new_bset) != NodesInGraph.end()) {
	ret.push_back(new_bset);
	kadded++;
      }
      
    }
    
  }

return;
}




vector < UnprocessedEdge* > GenerateValidEdges( vector<std::bitset<24>>* every_bitset) {

  vector < UnprocessedEdge* > ret_edges;

   UnprocessedEdge * this_edge;
   long int added_edges = 0;
  // mark as visited by keeping another map, this one to track
  // whether a node has been visited already 

  // add self to this other tracking map
  // through each iteration of i, add that i to this tracking map

  for (int i = 0; i < every_bitset->size(); i++) {
  vector<std::bitset<24>> candidates1;
  vector<std::bitset<24>> candidates2;
    kcount = 0;
    kadded = 0;
    // Since emplace returns a pair with second entry false when the key already 
    // is in the map, add the current node to the mape 
    EntriesSeen.emplace(every_bitset->at(i),kbitset_hash(every_bitset->at(i)));
   
    int src = (int) every_bitset->at(i).to_ulong();
    
    GenerateDist1(candidates1, every_bitset->at(i));
    GenerateDist2(candidates2, every_bitset->at(i));
  
    for (int j = 0; j < candidates1.size(); j++) {
      // So long as we haven't visited this node already, and hence already
      // have an edge from it to every_bitset->at(1) in ValidEdges, add it in
      if (EntriesSeen.find(candidates1.at(j)) == EntriesSeen.end()) {
	int dest = (int) candidates1.at(j).to_ulong();
	ret_edges.push_back( new UnprocessedEdge(src,dest,1));
	added_edges++;
      }
    }
 
    for (int j = 0; j < candidates2.size(); j++) {
       if (EntriesSeen.find(candidates2.at(j)) == EntriesSeen.end()) {
	int dest = (int) candidates2.at(j).to_ulong();
	ret_edges.push_back( new UnprocessedEdge(src,dest,2));
	added_edges++;
      }
    }
    // 354568 edges will be found...
    /*
      std::cout << "For node " << every_bitset->at(i).to_string() << ": "
		<< kcount << " possibilities inspected" << " and " <<
	kadded << " were valid - - I added " << added_edges << " edges so far." << std::endl;
	}   */   
  }
return ret_edges;

}

 

int main (int argc, char ** argv) {
  int number_unique_elts = 0;
  StaticClustering * target_cluster;
  vector<std::bitset<24>> every_bitset = ParseInputFile ();
  vector < UnprocessedEdge * >  every_valid_edge = GenerateValidEdges(&every_bitset);

  // TODO:: !!! 
  //  int num_nodes = findUniqueEntries(every_valid_edge*);



  std::cout << "Number of unique entries in EntriesSeen map: " << EntriesSeen.size() << std::endl;
  
  //target_cluster = findStaticClusteringLessThan3( edges );
  //::testing::InitGoogleTest(&argc,argv);
  
  return 0;
 
}



