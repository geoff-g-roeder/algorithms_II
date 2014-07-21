
#include <limits>
#include "g_adjacency_list.cpp"
#include "minKClustering.h"
#include <gtest/gtest.h>
#include <iostream>
#include "minKClusteringTests.cpp"
#include <queue>
#include <vector>
#include <fstream>




int knum_nodes; // KLUDGE avoid use of this global variable
string kfile_name = "clustering1.txt";


// Name of input file is hardcoded in function
// TODO: Rebuild this function to be robust against formatting
//       differences in input file
EdgePQ * ParseInputFile ( void ) {

  EdgePQ* retPtr = new EdgePQ ();
  UnprocessedEdge * thisEdge; 

  std::ifstream readFile(kfile_name);
  std::string inStr;
  std::string firstLine;
  std::getline(readFile,firstLine);

  knum_nodes = atoi(firstLine.substr(0,firstLine.find(" ")).c_str());

  while (std::getline(readFile, inStr)){
    // Process string into substrings of numbers
    std::string val1 = inStr.substr(0,inStr.find(" "));
    std::string val2Temp = inStr.substr(val1.size() + 1);
    std::string val2 = val2Temp.substr(0,val2Temp.find(" "));
    std::string val3Temp = inStr.substr(val2.size() + val1.size() + 2);
    std::string val3 = val3Temp.substr(0, val3Temp.find(" "));
    
    // :KLUDGE: Correct the 1-indexed nodes to 0-indexed for adjacency list graph
    //          Without assuming all graphs are labelled using 1-indexing, not
    //          sure how to fix this better.
    thisEdge = new UnprocessedEdge(std::stoi(val1)-1,std::stoi(val2)-1,std::stoi(val3));
    retPtr->push(thisEdge); 
  }
  
  return retPtr;
}

Clustering * findMinKClustering ( EdgePQ* edges ) {
  Clustering * ret_Clustering = new Clustering(knum_nodes,4);
  UnprocessedEdge * edgeToProcess;

  // std::cout << edges->size() << std::endl;
  while (!ret_Clustering->clustering_complete()) {
    edgeToProcess = edges->top();
    edges->pop();
    ret_Clustering->add_edge(edgeToProcess->get_u(),edgeToProcess->get_v(),edgeToProcess->get_weight());
  }

  ret_Clustering->print_leaders();
  //   std::cout << edges->size() << std::endl;
  
   // std::cout << ret_Clustering->get_num_leaders() << std::endl;
  int minEdgeDist = std::numeric_limits<int>::max();
  while (!edges->empty()) {
    edgeToProcess = edges->top();
    edges->pop();

    if (ret_Clustering->forms_cycle(edgeToProcess->get_u(),edgeToProcess->get_v()))
      continue;
    else {   
      int tempDist = edgeToProcess->get_weight();
      if ( tempDist < minEdgeDist)
	minEdgeDist = tempDist;
    }

  }

  std::cout << "Max spacing: " << std::endl << minEdgeDist << std::endl;  
   return ret_Clustering;
}

TEST(findMin, clusters) {
   Clustering * answer;
   EdgePQ *edges = ParseInputFile();
   answer = findMinKClustering( edges );
}
  
  

int main (int argc, char ** argv) {
  Clustering * answer;
  EdgePQ * edges = ParseInputFile ();

  answer = findMinKClustering( edges );
  ::testing::InitGoogleTest(&argc,argv);
  
  return RUN_ALL_TESTS();
 
}



