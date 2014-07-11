#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <map>
#include <queue> // std::priority_queue
#include <iomanip>

using namespace std;

struct Job {
  int weight;
  int length;
  double diff_rank;
  pair<int, pair<int,int> > d_tuple;
  pair<double, pair<int,int> > r_tuple;
  double rat_rank;
  
  Job(int in_w, int in_l): weight(in_w), length(in_l) {
    diff_rank = (double)(in_w-in_l);
    rat_rank = ((double)weight)/((double)length);
    r_tuple.first = (double)rat_rank;
    r_tuple.second.first = weight;
    r_tuple.second.second = length;

    d_tuple.first = diff_rank;
    d_tuple.second.first = weight;
    d_tuple.second.second = length;
  }
  };

// Comparison returns (pair<int, pair<int,int> >).first < (pair<int, pair<int,int> >).first unless equal,
// in which case the second member is compared with "<" to break ties
class diff_comp {
public:
  bool operator() (const pair<int, pair<int,int> >& lhs, const pair<int, pair<int,int> >& rhs) const {
    if (lhs.first == rhs.first)
      return lhs.second.first < rhs.second.first;
    else return lhs.first < rhs.first;
  }
};

class ratio_comp {
public:
  bool operator() (const pair<double, pair<int,int> >& lhs,
		   const pair<double, pair<int,int> >& rhs) const 
  {
    return lhs.first < rhs.first;
  }
};

typedef std::priority_queue
           < pair<int, pair<int,int> >, vector< pair<int,pair<int, int> > >,diff_comp>
             diff_comp_pq;

typedef std::priority_queue
             < pair<double, pair<int,int> >, vector< pair<double,pair<int, int> > >,ratio_comp>
               ratio_comp_pq;

void print_diff_rank_pq(diff_comp_pq* diff_rank) {
  int count = 0;

  while (!diff_rank->empty()) {
    cout << setprecision(40) <<  "[ " << (diff_rank->top()).first 
	 << " : " << "[ " << (diff_rank->top()).second.first
	 << " : " <<  (diff_rank->top()).second.second << "]"
	 << " ] " <<  endl;
    diff_rank->pop();
    count ++;
    }
}


void print_ratio_rank_pq(ratio_comp_pq* ratio_rank) {
  int count = 0;

  while (!ratio_rank->empty()) {
    cout << setprecision(40) <<  "[ " << (ratio_rank->top()).first 
	 << " : " << "[ " << (ratio_rank->top()).second.first
	 << " : " <<  (ratio_rank->top()).second.second << "]"
	 << " ] " <<  endl;
    ratio_rank->pop();
    count ++;
    }
}

double calc_weighted_sum_diff(diff_comp_pq* diff_rank) {
  double c_j = 0; // completion time
  double sum = 0;
  int count = 0;
  while (!diff_rank->empty()) {
    
    c_j += diff_rank->top().second.second;

    sum += diff_rank->top().second.first * c_j;
    count++;
    diff_rank->pop();
  }
  cout << count << endl;
  return sum;
}

double calc_weighted_sum_ratio(ratio_comp_pq* ratio_rank) {
  double c_j = 0; // completion time
  double sum = 0;
  int count = 0;
  while (!ratio_rank->empty()) {
    c_j += ratio_rank->top().second.second;
    sum += ratio_rank->top().second.first * c_j;
    ratio_rank->pop();
    count++;
  }
  cout << count << endl;
  return sum;
}


int main ( void ) {
  ifstream jobs_file ("jobs.txt");

  diff_comp_pq diff_rank;
  ratio_comp_pq ratio_rank;

  if (jobs_file.is_open())
    {
      string read_line;
      while (getline(jobs_file,read_line))
	{
	  //	  cout << read_line << endl;
	  int space = read_line.find(' ');
	  //	  cout << read_line.find(" ") << endl;
	  
	  int in_w = atoi(read_line.substr(0, read_line.find(" ")).c_str());
          int in_l = atoi(read_line.substr(space+1).c_str());
	  
	  Job* thisJob = new Job(in_w,in_l);
	  
	  diff_rank.push(thisJob->d_tuple);
	  ratio_rank.push(thisJob->r_tuple);

	}
      jobs_file.close();
    }

  // Print out diff tuples
  // print_diff_rank_pq(&diff_rank);
  // print_ratio_rank_pq(&ratio_rank);
  
    double diff_sum = calc_weighted_sum_diff(&diff_rank);
    double ratio_sum = calc_weighted_sum_ratio(&ratio_rank);

  cout << setprecision(20) <<  "Diff sum: " << diff_sum << endl
       << "Ratio sum: " << ratio_sum << endl;
  

}  

  



  
