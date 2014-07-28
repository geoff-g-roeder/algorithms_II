#ifndef PA3_KNAPSACK_H
#define PA3_KNAPSACK_H
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <gtest/gtest.h>


class PackableItem {

 public:
  // Returns new PackableItem
 PackableItem(int init_value, int init_weight): value_(init_value), weight_(init_weight) {}
  
  // Returns the weight of this item
  const int get_weight() {
    return weight_;
  }

  // Returns the value of this item
  const int get_value() {
    return value_;
  }

 private:

  int value_;
  int weight_;
};

class PackableItemCollection {

 public:
  PackableItemCollection() {
    packable_items_ = new std::vector<PackableItem*>;
  }

  // Creates new PackableItem and adds to this
  void add_item (PackableItem* new_item_) {
    packable_items_->push_back(new_item_);
  }

  // Returns all packable_items_ vector (for testing)
  std::vector<PackableItem*> * get_items () {
    return packable_items_;
  }
  
  // Return -1 if no such item or PackableItem * at i
  PackableItem * get_item_at ( int i ) {
    if (i < 0 || i >= packable_items_->size())
      return 0;
    else
      return packable_items_->at(i);
  }

  // Return number of items in collection
  int number_items () {
    return packable_items_->size();
  }

 private:

  std::vector<PackableItem*> *packable_items_;

};
    
class Knapsack {

 public:

 Knapsack(int init_weight_cap):
  weight_capacity_(init_weight_cap) {
    knapsack_contents_ = new std::vector<PackableItem*>;
    solution_so_far_ = new std::vector< std::vector<int>* >;

    // Create a vector for zeroth row (weights are row)
    std::vector<int> *zeroth_vector = new std::vector<int>;
    std::vector<int> *first_vector = new std::vector<int>;
    solution_so_far_->push_back(zeroth_vector);
    solution_so_far_->push_back(first_vector);

    // Initialize optimum solutions for 0 items to zero max value
    for (int x = 0; x <= weight_capacity_; x++) {
      solution_so_far_->at(0)->push_back(0);
    }

  }

  // Returns total value of the optimum packing solution
  int pack (PackableItemCollection * things_to_pack_) {
    int number_items  = things_to_pack_->number_items();
    
    // Over all items i
    for (int i = 1; i <= number_items; i++) {
      
      // Over all possible weights x, determine optimum solution to A[i,x]
      for (int x = 0; x <= weight_capacity_; x++) {
	// when the weight of the item is greater than the capacity x, leave it

	// ::GOTCHA:: things_to_pack[0] is 1st item to pack; adjust index accordingly
	if (things_to_pack_->get_item_at(i-1)->get_weight() > x) {
	  solution_so_far_->at(1)->push_back(solution_so_far_->at(0)->at(x));
	} else {

	  // else, decide to take or leave it
	  int leave_item_i = solution_so_far_->at(0)->at(x);
	  int take_item_i = solution_so_far_->at(0)->at(x - things_to_pack_->get_item_at(i-1)->get_weight()) +
	    things_to_pack_->get_item_at(i-1)->get_value();
	  if (leave_item_i > take_item_i)
	    // A[i,x]=A[i-1,x]
	    solution_so_far_->at(1)->push_back(leave_item_i);
	  else
	    // A[i,x]=A[i-1,x-w_i]+v_i
	    solution_so_far_->at(1)->push_back(take_item_i);				 
	}
      }

      // Memory optimization: remember only the last 2 vector<int> of weights
      // after solution_so_far_->at(1) has been filled
      // e.g., forget the previous solution and replace with this solution.
      //::TODO:: design recursive solution that optimizes speed
      solution_so_far_->at(0)->clear();
      solution_so_far_->at(0)->swap(*(solution_so_far_->at(1)));
    }
    
    return solution_so_far_->at(0)->at(weight_capacity_);
  }
	
 private:

  int weight_capacity_;
  std::vector<PackableItem*> *knapsack_contents_;
  std::vector< std::vector<int>* > *solution_so_far_;
	     
};


class PackableItemsParser {

 public:

 PackableItemsParser(std::string input_name):file_name_(input_name) {}
   
  
  PackableItemCollection * make_collection ( void ) {

    PackableItemCollection * collection = 
      new PackableItemCollection();

    std::ifstream read_(file_name_.c_str());
    std::string in_string_;
           
    getline(read_,in_string_);
    std::stringstream stream(in_string_);

    stream >> knapsack_capacity_;
    stream >> number_items_;
    
    int temp_val;
    int temp_weight;
    while (getline(read_,in_string_)) {
      std::stringstream temp(in_string_);
      temp >> temp_val;
      temp >> temp_weight;
      collection->add_item( new PackableItem(temp_val,temp_weight));
    }
    return collection;
  }

  int get_knapsack_capacity_(){
    return knapsack_capacity_;
  }

  void get_knapsack_capacity_( int set_val){
    knapsack_capacity_=set_val;
  }

  int get_number_items_(){
    return number_items_;
  }

  void get_number_items_( int set_val){
    number_items_=set_val;
  }

 private:

  int number_items_;
  int knapsack_capacity_;
  std::string file_name_;
};

TEST(PackableItemsParserTests,all_tests) {
  PackableItemsParser * test_parser = 
    new PackableItemsParser("knapsack_small.txt");

  PackableItemCollection* test_collection = 
    test_parser->make_collection();

  ASSERT_EQ(3, test_parser->get_number_items_());
  ASSERT_EQ(10000, test_parser->get_knapsack_capacity_());


  std::vector<PackableItem*> *items = test_collection->get_items();
  ASSERT_EQ(items->size(),3);

  PackableItem * item = test_collection->get_item_at(0);
  ASSERT_EQ(item->get_value(),16808);
  ASSERT_EQ(item->get_weight(),250);
 
  Knapsack * test_pack = new Knapsack(test_parser->get_knapsack_capacity_());
  ASSERT_EQ(test_pack->pack(test_collection),75813);
}

#endif
