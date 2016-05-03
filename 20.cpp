#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

// Minimum ratio between the duration of a call and the cube of the
// number of stars to be accepted. Obtained experimentally.
const int MIN_RATIO = 15;
// Probability of mutation in a genetic algorithm.
const int PROB_MUTATION = 3007;
// Probability of mutation in a diploid genetic algorithm.
const int PROB_MUTATION_DIPLOID = 507;
// Inverse of the proportion of invalid calls in the initial solutions
// of the genetic algorithm.
const int RATIO_INVALID = 7;
// Inverse of the proportion of invalid calls in the initial solutions
// of the diploid genetic algorithm.
const int RATIO_INVALID_DIPLOID = 2;
// Population for the genetic algorithm.
const int POPULATION = 30;

// Stores the coordinates of a Call or a POP.
struct Coords {
  Coords() {x_ = 0; y_ = 0;}
  Coords(int x, int y): x_(x), y_(y) {}
  
  // Computes the number of stars lost by the distance of a call.
  int stars(Coords c) {
    // Avoids floating point operations.
    int dx = x_ - c.x_;
    int dy = y_ - c.y_;
    int dist_square = dx * dx + dy * dy;
    if (dist_square < 100)
      return 0;
    if (dist_square < 400)
      return 1;
    if (dist_square < 900)
      return 2;
    if (dist_square < 1600)
      return 3;
    if (dist_square < 2500)
      return 4;
    return 5;
  }
  int x_;
  int y_;
};

// Stores the information of a call.
class Call {
 public:
  
  Call() {
    coords_ = Coords();
    start_ = duration_ = 0;
  }
  
  // Reads a new call from stdin.
  void Read() {
    int x, y;
    cin >> x >> y >> start_ >> duration_;
    coords_ = Coords(x, y);
  }
  
  int getStart() {
    return start_;
  }
  
  
  int getPop() {
    return pop_assigned_;
  }
  
  int getDuration() {
    return duration_;
  }
  
  void setPop(int pop) {
    pop_assigned_ = pop;
  }
  
  Coords getCoords() {
    return coords_;
  }
  
 private:
  Coords coords_;
  int start_;
  int duration_;
  int last_call_;
  int pop_assigned_;
};

// Stores the information of a POP.
class Pop {
 public:
  
  Pop() {
    coords_ = Coords();
    capacity_ = waiting_ = 0;
    
  }
  // Reads a new POP from stdin.
  void Read() {
    int x, y;
    cin >> x >> y >> capacity_;
    coords_ = Coords(x, y);
  }
  
  // Frees space for a new call.
  void EndCall() {
    if (waiting_ > 0)
      waiting_--;
    else
      finishing_times_.pop();
  }
  
  
  // Assigns a given call and returns the starting time.
  int Assign(Call& call) {
    int starts = Starts(call);
    UpdateFinishingTimes(starts + call.getDuration());
    return starts;
  }
  
  // Returns the number of stars that the call would receive
  // if it were assigned to the given POP.
  int getStars(Call& call) {
    int stars = 5;
    stars -= coords_.stars(call.getCoords());
    stars -= (9 + Starts(call) - call.getStart()) / 10;
    return stars;
  }
  
  Coords getCoords() {
    return coords_;
  }
  
 private:
  
  // Adds time to the list of finishing times.
  void UpdateFinishingTimes(int time) {
    finishing_times_.push(-time);
    if (finishing_times_.size() > capacity_) {
      finishing_times_.pop();
      waiting_++;
    }
  }
   
  // Returns when a call would start.
  int Starts(Call& call) {
    if (finishing_times_.size() < capacity_)
      return call.getStart();
    return -finishing_times_.top();
  }
  
  Coords coords_;
  int capacity_;
  int waiting_;
  // Finishing times for the last capacity_ calls
  priority_queue<int> finishing_times_;
};

int power(int a, int b) {
  if (b == 0) return 1;
  return a * power(a, b-1);
}

// Assigns calls to pops, provided that valid[call] == true. Stores the
// solution in vectors solp1, solp2 and solp3, and returns the number
// of stars of the solution. NOTE: this function is not used in my final
// solution, since it is replaced with AssignmentDiploid.
int Assignment(vector<Pop> pops,
               vector<Call> calls,
               vector<int>& solp1,
               vector<int>& solp2,
               vector<int>& solp3,
               vector<bool>& valids) {
  int num_pop = pops.size();
  int num_call = calls.size();
  // Instants where a new call will start
  priority_queue<pair<int, int> > starts;
  // Instants where a call will finish
  priority_queue<pair<int, int> > ends;
  for (int i = 0; i < num_call; i++) {
    if (valids[i])
      starts.push(pair<int, int>(-calls[i].getStart(), i));
  }
  int total_stars = 0;
  while (not starts.empty()) {
    if (not ends.empty() and (ends.top() >= starts.top())) {
      int call_id = ends.top().second;
      ends.pop();
      pops[calls[call_id].getPop()].EndCall();
    } else {    
      int call_id = starts.top().second;
      starts.pop();
      int best_stars = 0;
      int best_pop = 0;
      for (int pop_id = 0; pop_id < num_pop; pop_id++) {
        int stars = pops[pop_id].getStars(calls[call_id]);
        if (stars >= best_stars) {
          best_stars = stars;
          best_pop = pop_id;
        }
      }
      if (best_stars > 1) {
        if (calls[call_id].getDuration() / power(best_stars, 3) < MIN_RATIO) {
          int starting_time = pops[best_pop].Assign(calls[call_id]);
          solp1.push_back(call_id);
          solp2.push_back(best_pop);
          solp3.push_back(starting_time);
          ends.push(pair<int, int>(-starting_time-calls[call_id].getDuration(),
                                  call_id));
          calls[call_id].setPop(best_pop);
          total_stars += best_stars;
        } 
      }
    }
    
  }
  return total_stars;
}



// This function is equivalent to Assignment. It is used for the Diplod Genetic
// algorithm, and the difference is that valids is not a vector of bools,
// but a vector of two bools. A call is valid iff at least one of the two
// bools is true, so being valid is a dominant trait.
// Since the function is equivalent, the best option would have been to
// generate valids from its Diplod version, and call Assignment. However,
// this approach is faster, since we avoid generating a new vector.
int AssignmentDiploid(vector<Pop> pops,
                      vector<Call> calls,
                      vector<int>& solp1,
                      vector<int>& solp2,
                      vector<int>& solp3,
                      vector<vector<bool> >& valids) {
  int num_pop = pops.size();
  int num_call = calls.size();
  // Instants where a new call will start
  priority_queue<pair<int, int> > starts;
  // Instants where a call will finish
  priority_queue<pair<int, int> > ends;
  for (int i = 0; i < num_call; i++) {
    if (valids[i][0] || valids[i][1])
      starts.push(pair<int, int>(-calls[i].getStart(), i));
  }
  int total_stars = 0;
  while (not starts.empty()) {
    if (not ends.empty() and (ends.top() >= starts.top())) {
      int call_id = ends.top().second;
      ends.pop();
      pops[calls[call_id].getPop()].EndCall();
    } else {    
      int call_id = starts.top().second;
      starts.pop();
      int best_stars = 0;
      int best_pop = 0;
      for (int pop_id = 0; pop_id < num_pop; pop_id++) {
        int stars = pops[pop_id].getStars(calls[call_id]);
        if (stars >= best_stars) {
          best_stars = stars;
          best_pop = pop_id;
        }
      }
      if (best_stars > 1) {
        if (calls[call_id].getDuration() / power(best_stars, 3) < MIN_RATIO) {
          int starting_time = pops[best_pop].Assign(calls[call_id]);
          solp1.push_back(call_id);
          solp2.push_back(best_pop);
          solp3.push_back(starting_time);
          ends.push(pair<int, int>(-starting_time-calls[call_id].getDuration(),
                                  call_id));
          calls[call_id].setPop(best_pop);
          total_stars += best_stars;
        } 
      }
    }
    
  }
  return total_stars;
}


// Computes the best subset of calls to be assigned with the greedy Assign.
// algorithm, by using a Hill Climbing algorithm. NOTE: this function is not
// used in my final solution, since I got a better result by using a
// genetic approach.
int HillClimbing(vector<Pop> pops,
                 vector<Call> calls,
                 vector<int>& sol1,
                 vector<int>& sol2,
                 vector<int>& sol3) {
  int num_pop = pops.size();
  int num_call = calls.size();
  vector<bool> valids(num_call, true);
  bool improve = true;
  int best_value = Assignment(pops, calls, sol1, sol2, sol3, valids);
  int mod = 1000;
  while (improve) {
    int best_index = 0;
    improve = false;
    for (int i = 0; i < num_call; i++) {
      if (rand() % mod != 0) continue;
      vector<int> solp1, solp2, solp3;
      valids[i] = not valids[i];
      int new_value = Assignment(pops, calls, solp1, solp2, solp3, valids);
      if (new_value > best_value) {
        improve = true;
        best_value = new_value;
        best_index = i;
        sol1 = solp1;
        sol2 = solp2;
        sol3 = solp3;
      }
      valids[i] = not valids[i];
    }
    if (not improve and mod > 1) {
      improve = true;
      mod  /= 10;
    } else if (improve) {
      valids[best_index] = not valids[best_index];
      mod = 1000;
      ofstream fout("testparcial2.out");
      
      for (int i = 0; i < sol1.size(); i++) {
        fout << sol1[i] << " " << sol2[i] << " " << sol3[i] << endl;
      }
    }
    cerr << best_value << endl;
  }
  return best_value;
}

// Computes the descendant of two solutions, defined as follows.
// For each call, if it is valid in both parents, then it is valid
// in the descendant. Otherwise flip a coin. Add random mutations.
vector<bool> GetDescendant(vector<bool>& a, vector<bool>& b) {
  vector<bool> descendant(a.size());
  for (int i = 0; i < descendant.size(); i++) {
    if (a[i] == b[i])
      descendant[i] = a[i];
    else
      descendant[i] = rand() % 2;
    if (rand() % PROB_MUTATION == 0)
      descendant[i] = not descendant[i];
  }
  return descendant;
}

// Generates an initial solution for the genetic algorithm.
vector<bool> RandomVector(int size) {
  vector<bool> v(size);
  for (int i = 0; i < size; i++) {
    v[i] = rand() % RATIO_INVALID;
  }
  return v;
}

// Generates an initial solution for the genetic diplod algorithm.
vector<vector<bool> > RandomVectorDiploid(int size) {
  vector<vector<bool> > v(size, vector<bool>(2, false));
  for (int i = 0; i < size; i++) {
    v[i][0] = rand() % RATIO_INVALID_DIPLOID;
    v[i][1] = rand() % RATIO_INVALID_DIPLOID;
  }
  return v;
}

// Computes the descendant of two solutions, defined as follows.
// For each call, flip a coin. If it is head, the fist value is
// inherited from a, and the second one from b. Otherwise, the opposit.
// Adds random mutations.
vector<vector<bool> > GetDescendantDiploid(vector<vector<bool> >& a, vector<vector<bool> >& b) {
  vector<vector<bool> > descendant(a.size(), vector<bool> (2, false));
  for (int i = 0; i < descendant.size(); i++) {
    int kind = rand() % 2;
    if (kind == 0) {
      descendant[i][0] = a[i][0];
      descendant[i][1] = b[i][1];
    }
    else {
      descendant[i][0] = b[i][0];
      descendant[i][1] = a[i][1];
    }
    if (rand() % PROB_MUTATION_DIPLOID == 0)
      descendant[i][0] = not descendant[i][0];
    if (rand() % PROB_MUTATION_DIPLOID == 0)
      descendant[i][1] = not descendant[i][1];
  }
  return descendant;
}

// Computes the best subset of calls to be assigned with the greedy Assign.
// algorithm, by using a diploid genetic algorithm, where being valid
// is a dominant trait.
int GeneticDiploid(vector<Pop> pops,
                   vector<Call> calls,
                   vector<int>& sol1,
                   vector<int>& sol2,
                   vector<int>& sol3) {
  int num_pop = pops.size();
  int num_call = calls.size();
  vector<vector<vector<bool> > > offspring(POPULATION);
  for (int i = 0; i < POPULATION; i++) {
    offspring[i] = RandomVectorDiploid(num_call);
  }
  while (true) {
    vector<vector<vector<bool> > > new_offspring = offspring;
    for (int i = 0; i < POPULATION; i++)
      for (int j = 0; j <= i; j++) {
        new_offspring.push_back(GetDescendantDiploid(offspring[i],
                                                     offspring[j])); }
    vector<pair<int, int> > order(new_offspring.size());
    for (int i = 0; i < new_offspring.size(); i++) {
      vector<int> solp1, solp2, solp3;
      int new_value = AssignmentDiploid(pops,
                                        calls,
                                        solp1,
                                        solp2,
                                        solp3,
                                        new_offspring[i]);
      order[i] = pair<int, int>(-new_value, i);
    }
    sort(order.begin(), order.end());
    for (int i = 0; i < POPULATION; i++) {
      offspring[i] = new_offspring[order[i].second];
    }
    cerr << -order[0].first << endl;
    
    ofstream fout("testparcial2.out");
    
    vector<int> solp1, solp2, solp3;
    int new_value = AssignmentDiploid(pops,
                                      calls,
                                      solp1,
                                      solp2,
                                      solp3,
                                      offspring[0]);
    sol1 = solp1;
    sol2 = solp2;
    sol3 = solp3;
    for (int i = 0; i < sol1.size(); i++) {
      fout << sol1[i] << " " << sol2[i] << " " << sol3[i] << endl;
    }
  }
  vector<int> solp1, solp2, solp3;
  int new_value = AssignmentDiploid(pops,
                                    calls,
                                    solp1,
                                    solp2,
                                    solp3,
                                    offspring[0]);
  sol1 = solp1;
  sol2 = solp2;
  sol3 = solp3;
  return new_value;
}

// Computes the best subset of calls to be assigned with the greedy Assign.
// algorithm, by using a genetic algorithm. NOTE: This function is not used
// in my final solution, since it was replaced with a diploid version.
// Actually both results were mostly equivalent, so I kept the last I coded.
int Genetic(vector<Pop> pops,
            vector<Call> calls,
            vector<int>& sol1,
            vector<int>& sol2,
            vector<int>& sol3) {
  int num_pop = pops.size();
  int num_call = calls.size();
  vector<vector<bool> > offspring(POPULATION);
  for (int i = 0; i < POPULATION; i++) {
    offspring[i] = RandomVector(num_call);
  }
  while (true) {
    vector<vector<bool> > new_offspring = offspring;
    for (int i = 0; i < POPULATION; i++)
      for (int j = 0; j <= i; j++)
        new_offspring.push_back(GetDescendant(offspring[i], offspring[j]));
    vector<pair<int, int> > order(new_offspring.size());
    for (int i = 0; i < new_offspring.size(); i++) {
      vector<int> solp1, solp2, solp3;
      int new_value = Assignment(pops,
                                 calls,
                                 solp1,
                                 solp2,
                                 solp3,
                                 new_offspring[i]);
      order[i] = pair<int, int>(-new_value, i);
    }
    sort(order.begin(), order.end());
    for (int i = 0; i < POPULATION; i++) {
      offspring[i] = new_offspring[order[i].second];
    }
    cerr << -order[0].first << endl;
    
    ofstream fout("testparcial3.out");
    
    vector<int> solp1, solp2, solp3;
    int new_value = Assignment(pops,
                               calls,
                               solp1,
                               solp2,
                               solp3,
                               offspring[0]);
    sol1 = solp1;
    sol2 = solp2;
    sol3 = solp3;
    for (int i = 0; i < sol1.size(); i++) {
      fout << sol1[i] << " " << sol2[i] << " " << sol3[i] << endl;
    }
  }
  vector<int> solp1, solp2, solp3;
  int new_value = Assignment(pops, calls, solp1, solp2, solp3, offspring[0]);
  sol1 = solp1;
  sol2 = solp2;
  sol3 = solp3;
  return new_value;
}

int main() {
  srand (time(NULL));
  int num_pop, num_call;
  cin >> num_pop >> num_call;
  vector<Pop> pops(num_pop);
  for (int i = 0; i < num_pop; i++)
    pops[i].Read();
  vector<Call> calls(num_call);
  for (int i = 0; i < num_call; i++)
    calls[i].Read();
  
  int num_exp = 1;
  int best_sol = 0;
  vector<int> sol1, sol2, sol3;
  for (int exp = 0; exp < num_exp; exp++) {
    vector<int> solp1, solp2, solp3;
    // I tried many algorithms, but the ones with best results were the
    // genetic ones.
//     vector<bool> valids(num_call, true);
//     int total_stars = Assignment(pops, calls, solp1, solp2, solp3, valids);
//     int total_stars = HillClimbing(pops, calls, solp1, solp2, solp3);
//     int total_stars = Genetic(pops, calls, solp1, solp2, solp3);
    int total_stars = GeneticDiploid(pops, calls, solp1, solp2, solp3);
    
    if (total_stars > best_sol) {
      cerr << "Total stars: " << total_stars << endl;
      sol1 = solp1;
      sol2 = solp2;
      sol3 = solp3;
      best_sol = total_stars;
    }
  }
  for (int i = 0; i < sol1.size(); i++) {
    cout << sol1[i] << " " << sol2[i] << " " << sol3[i] << endl;
  }
}