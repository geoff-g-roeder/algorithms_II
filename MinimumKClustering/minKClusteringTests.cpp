#ifndef TESTS_CPP
#define TESTS_CPP

#include <gtest/gtest.h>
#include "minKClustering.h"
#include "g_adjacency_list.cpp"
#include <vector>
#include <queue>


TEST(cluster_node_test, CN_behavior) {

  // Constructor behaviour

  ClusterNode * cn = new ClusterNode();

  ASSERT_EQ(nullptr, cn->find_leader());


  // Union-Find ADT behavior

  ClusterNode * new_l = new ClusterNode();
  EXPECT_TRUE(cn->find_leader() == nullptr);

  cn->update_leader(new_l);

  EXPECT_TRUE(cn->find_leader() == new_l);

  EXPECT_FALSE(cn->find_leader() == nullptr);


}


TEST(Clustering_constructor, C_behavior) {
  int V = 65535;
  Clustering * minK = new Clustering(V,4);
  ClusterNode * verts = minK->get_verts();

  EXPECT_TRUE(minK->get_V() == V);
  EXPECT_TRUE(minK->get_num_leaders() == V)
    << "Number of Leaders is" << minK->get_num_leaders();
  EXPECT_EQ(minK->get_k(), 4);

  EXPECT_FALSE(verts == nullptr);

}

TEST(Clustering_constructor, follower) {
  int V = 65535;
  Clustering * minK = new Clustering(V,4);
  std::vector< std::vector<int>> * fol = minK->get_followers();
  ASSERT_TRUE(fol->at(0).at(0) == 0);
  ASSERT_TRUE(fol->at(65534).at(0) = 65534);
}

TEST(Clustering_add_edge, C_add_edge) {
  int V = 65535;
  Clustering * minK = new Clustering(V,4);
  ClusterNode * verts = minK->get_verts();

  ASSERT_TRUE(&(*(verts[0].find_leader())) == &verts[0]);
  ASSERT_TRUE(&(*(verts[1].find_leader())) == &verts[1]);
  ASSERT_FALSE(&(*(verts[1].find_leader())) == &verts[0]);

  ASSERT_TRUE(minK->add_edge(0,1,1000));
 
  ASSERT_EQ(verts[1].get_head()->get_next(), nullptr);
  ASSERT_EQ(verts[1].get_head()->get_dest(),0);
  ASSERT_EQ(verts[1].get_head()->get_weight(),1000);

  ASSERT_EQ(verts[0].get_head()->get_next(), nullptr);
  ASSERT_EQ(verts[0].get_head()->get_dest(),1);
  ASSERT_EQ(verts[0].get_head()->get_weight(),1000);

  ASSERT_FALSE(minK->add_edge(65534,65535,999));

  ASSERT_TRUE(minK->add_edge(65534,65533,999));

  ASSERT_EQ(verts[65534].get_head()->get_next(), nullptr);
  ASSERT_EQ(verts[65534].get_head()->get_dest(),65533);
  ASSERT_EQ(verts[65534].get_head()->get_weight(),999);

  ASSERT_EQ(verts[65533].get_head()->get_next(), nullptr);
  ASSERT_EQ(verts[65533].get_head()->get_dest(),65534);
  ASSERT_EQ(verts[65533].get_head()->get_weight(),999); 
}

TEST(Clustering_add_edge, unionfy) {
  int V = 2;
  Clustering * minK = new Clustering(V,1);
  ASSERT_TRUE( minK->add_edge(0,1,1));
  // Try to add same edge
  ASSERT_FALSE( minK->add_edge(0,1,2));
  ASSERT_TRUE(minK->forms_cycle(0,1));
  ASSERT_FALSE(minK->add_edge(1,0,2));

  ASSERT_EQ(minK->get_num_leaders(), 1);


  V = 3;
  minK = new Clustering(V,1);
  ClusterNode * verts = minK->get_verts();
  
  
  ASSERT_TRUE( minK->add_edge(0,1,1));
  ASSERT_EQ( minK->get_num_leaders(),2);
  ASSERT_TRUE( minK->add_edge(1,2,2));
  ASSERT_TRUE( verts[0].find_leader() == &verts[1]);

  ASSERT_FALSE( minK->add_edge(2,0,3));
  ASSERT_EQ( minK->get_num_leaders(),1);
  ASSERT_TRUE( verts[2].find_leader() == &verts[1]);

  V = 1000;
  minK = new Clustering(V,2);
  verts = minK->get_verts();
  std::vector< std::vector <int>> *fols = minK->get_followers();

  for (int i = 0; i < 1000; i++) {
    if (i % 2 == 0) {
      verts[i].update_leader(&verts[0]);
      verts[i].update_ldr_index(0);
    }
    else {
       verts[i].update_leader(&verts[1]);
       verts[i].update_ldr_index(1);
    }
  }
  ASSERT_TRUE(verts[995].get_ldr_index() == 1);

  ASSERT_TRUE(minK->add_edge(4,995,1));

  ASSERT_EQ(verts[0].get_ldr_index(), 1);
   
}

TEST(Clustering_add_edge, forms_cycle) {
  int V = 65535;
  Clustering * minK = new Clustering(V,4);
  ClusterNode * verts = minK->get_verts();
  for (int i = 1; i < 101; i ++) {
    verts[i].update_leader(&verts[0]);
  }

  ASSERT_TRUE(minK->forms_cycle(0,0));
  ASSERT_TRUE(minK->forms_cycle(1,2));
  ASSERT_FALSE(minK->forms_cycle(0,101));
}    

TEST(Clustering_add_edge, unionfy_partition) {
  int V = 100;
  int i,j;
  Clustering * cl = new Clustering(V,4);
  ClusterNode * verts = cl->get_verts();
  std::vector< std::vector <int>> *fols = cl->get_followers();

  // Create forest of connected components
  // Leaders should be second component in each
  for (i = 0; i < V; i++) {
    j = i + 1;
    if (i % 10 == 0|| j%10 == 0)
      continue;
    cl->add_edge(i,j,0);
  }
  ASSERT_EQ(cl->get_num_leaders(),20);

  ASSERT_FALSE(verts[0].get_ldr_index() == 2);
  ASSERT_TRUE(verts[1].get_ldr_index() == 2);
  ASSERT_TRUE(verts[9].get_ldr_index() == 2);
  ASSERT_FALSE(verts[10].get_ldr_index() == 2);

  ASSERT_FALSE(verts[10].get_ldr_index() == 12);
  ASSERT_TRUE(verts[16].get_ldr_index() == 12);
  ASSERT_FALSE(verts[20].get_ldr_index() == 12);

  ASSERT_FALSE(verts[90].get_ldr_index() == 92);
  ASSERT_TRUE(verts[99].get_ldr_index() == 92);

  ASSERT_TRUE(verts[9].get_ldr_index() == 2);
  ASSERT_TRUE(fols->at(2).size() == 9);
  ASSERT_EQ(fols->at(2).back(), 9);
  ASSERT_EQ(fols->at(2).front(), 2);

  ASSERT_TRUE(cl->add_edge(9,16,1));
  ASSERT_EQ(cl->get_num_leaders(),19);
  ASSERT_TRUE(verts[9].get_ldr_index() == 12);
  ASSERT_EQ(fols->at(2).size(),0);
  
  ASSERT_EQ(verts[3].get_ldr_index(), 12);
  ASSERT_EQ(verts[4].get_ldr_index(), 12);
  
  ASSERT_TRUE( cl->add_edge(99,1,0));
  ASSERT_EQ(cl->get_num_leaders(),18);
  ASSERT_EQ(verts[95].get_ldr_index(),12);
}


  
#endif
