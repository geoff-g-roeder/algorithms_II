#include "knapsack.h"
#include <gtest/gtest.h>
#include <iostream>

int main (int argc, char * argv[]) {

  // Replace "knapsack1.txt" with name of target file to calculate optimum packing solution

  // ::KLUDGE:: requires file to be formatted identically to "knapsack1.txt". A more robust parser class is needed.

  PackableItemsParser * q1 = new PackableItemsParser("knapsack1.txt");

  PackableItemCollection * q1_collection = q1->make_collection();

  Knapsack * q1_pack = new Knapsack(q1->get_knapsack_capacity_());

  std::cout << "Best possible packed value is: " << q1_pack->pack(q1_collection) << std::endl;

  ::testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
