#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;


// Stores the information of a level
class Level {
 public:
  
  // Reads a new level from stdin
  Level() {
    cin >> num_steps_;
    step_forces_ = vector<int> (num_steps_);
    for (int i = 0; i < num_steps_; i++)
      cin >> step_forces_[i];
    graph_ = vector<vector<BridgeInfo> >(0);
    num_ledges_ = 0;
    max_toughness_ = 0;
    int num_bridges;
    cin >> num_bridges;
    for (int i = 0; i < num_bridges; i++) {
      int a, b, toughness;
      cin >> a >> b >> toughness;
      AddBridge(a, b, toughness);
    }
  }
  
  // Returns whether it is possible to finish the level by sinking
  // all the bridges. If it is possible it updates information about
  // the initial and final ledge, to be accessed later.
  bool IsPossible() {
    if (not Connected())
      return false;
    
    // For each possible bridge toughness compute whether it can be traversed
    // in an odd and an even number of traversals.
    UpdateBridgeParity();
    
    
    // There is a solution if there is a subset of bridges with both possible
    // parity such that, together with bridges with only odd parity,
    // they form a graph where all the vertices, except possibly two of them
    // (or none) have even parity. In order to test that, remove the bridges
    // with an assigned parity, and there must be at most two connected
    // components with an odd number of vertices incident to bridges with only
    // an odd parity. The proof is that if there is an even number of vertices
    // incident with an odd number of bridges with only odd parity, then
    // we can match them, an consider any path between each pair. Now, for each
    // edge that appears twice, we remove it. The result is a subset of edges
    // that satisfy the constraints.
    
    // True if the sum of the parities of the bridges incident to a ledge
    // where only one possible parity is possible is odd.
    vector<bool> odd_parity(num_ledges_, false);
    
    // Graph only considering bridges that can be traversed in both an odd 
    // and an even number of traversals.
    vector<vector<int> > new_graph(num_ledges_);
    
    for (int i = 0; i < num_ledges_; i++) {
      for (int j = 0; j < graph_[i].size(); j++) {
        int d = graph_[i][j].destination;
        int t = graph_[i][j].toughness;
        // Edges appear twice, so we only consider one of them.
        if (i < d) {
          if (traversals_[0][t]) {
            if (traversals_[1][t]) {
              new_graph[i].push_back(d);
              new_graph[d].push_back(i);
            }
          } else {
            if (traversals_[1][t]) {
              odd_parity[i] = not odd_parity[i];
              odd_parity[d] = not odd_parity[d];
            } else {
              // If we cannot traverse a bridge we cannot solve the level.
              return false;
            }
          }
        }
      }
    }
    
    
    // For each connected component of the new graph, if there is an odd number
    // of vertices with odd parity, then the vertex with lowest label appears
    // in odd_components.
    set<int> odd_components;
    vector<bool> visited(num_ledges_, false);
    for (int i = 0; i < num_ledges_; i++) {
      if (not visited[i]) {
        int lowest_label = id2label_[i];
        if (Dfs2(i, new_graph, visited, odd_parity, lowest_label) % 2 == 1) {
          odd_components.insert(lowest_label);
        }
      }
    }
    if (odd_components.size() == 0) {
      // There exists an Eulerian Cycle, so we start and finish in the ledge
      // with lowest label.
      start_ = end_ = label2id_.begin()->first;
      return true;
    }
    if (odd_components.size() == 2) {
      // There is an Eulerian path. We start in the ledge with lowest label
      // from an odd component and finish in the ledge with lowest label 
      // from the other odd component.
      start_ = *(odd_components.begin());
      end_ = *(++odd_components.begin());
      return true;
    }
    return false;
  }
  
  int getStart() {
    return start_;
  }
  
  int getEnd() {
    return end_;
  }
  
 private:
  
  // Information of a bridge to be stored in a vector that depends
  // on the origin, so we do not store the origin.
  struct BridgeInfo {
    int destination;
    int toughness;
    BridgeInfo(int d, int t): destination(d), toughness(t) {}
    BridgeInfo() {destination = 0; toughness = 0;}
  };
  
  // Adds a bridge between a and b with the given toughness
  void AddBridge(int a, int b, int toughness) {
    int id_a = GetId(a);
    int id_b = GetId(b);
    graph_[id_a].push_back(BridgeInfo(id_b, toughness));
    graph_[id_b].push_back(BridgeInfo(id_a, toughness));
    max_toughness_ = max(max_toughness_, toughness);
  }
  
  // Traverses graph_ starting from ledge
  void Dfs(int ledge, vector<bool>& visited) {
    if (visited[ledge])
      return;
    visited[ledge] = true;
    for (int i = 0; i < graph_[ledge].size(); i++) {
      Dfs(graph_[ledge][i].destination, visited);
    }
  }
  
  // Traverses the graph starting from ledge. Returns the number of
  // traversed vertices with odd_parity = true, and updates the lowest_label.
  int Dfs2(int ledge,
            vector<vector<int> >& graph,
            vector<bool>& visited,
            vector<bool>& odd_parity,
            int& lowest_label) {
    if (visited[ledge]) {
      return 0;
    }
    visited[ledge] = true;
    lowest_label = min(lowest_label, id2label_[ledge]);
    int odd_ledges = 0;
    if (odd_parity[ledge])
      odd_ledges = 1;
    for (int i = 0; i < graph[ledge].size(); i++) {
      odd_ledges += Dfs2(graph[ledge][i],
                         graph,
                         visited,
                         odd_parity,
                         lowest_label);
    }
    return odd_ledges;
  }
  
  // Returns whether the graph is connected
  bool Connected() {
    vector<bool> visited(num_ledges_, false);
    Dfs(0, visited);
    for (int i = 0; i < visited.size(); i++) {
      if (not visited[i]){
        return false;
      }
    }
    return true;
  }
  
  // Returns the id of a label. A label that appears before in the input
  // has a lower id, which are numerated from 0 to num_ledges_ - 1.
  int GetId(int a) {
    if (label2id_.find(a) == label2id_.end()) {
      label2id_[a] = num_ledges_;
      id2label_.push_back(a);
      num_ledges_++;
      graph_.push_back(vector<BridgeInfo>(0));
    }
    return label2id_[a];
  }
  
  // For each toughness compute whether a bridge of this toughness
  // can be traversed in an odd and an even number of traversals
  // using steps from step_forces_
  void UpdateBridgeParity() {
    traversals_ = vector<vector<bool> > (2, vector<bool> (max_toughness_ + 1,
                                                         false));
    // There are no bridges of toughness 0, but it simplifies the algorithm.
    traversals_[0][0] = true;
    // Dynamic programming.
    for (int i = 0; i < max_toughness_; i++) {
      for (int parity = 0; parity < 2; parity++) {
        if (traversals_[parity][i]) {
          for (int step = 0; step < num_steps_; step++) {
            int step_force = step_forces_[step];
            if (i + step_force <= max_toughness_) {
              traversals_[(parity + 1) % 2][i + step_force] = true;
            }
          }
        }
      }
    }
  }
  
  // Labels: number of ledge according to the input.
  // Id: order in which a label appears in the input.
  map<int, int> label2id_;
  vector<int> id2label_;
  int num_ledges_;
  int num_steps_;
  vector<vector<BridgeInfo> > graph_;
  vector<int> step_forces_;
  int max_toughness_;
  // True iff a brigde with a given toughness can be traversed in an
  // odd/even number of traversals using steps from step_forces_.
  // traversals_[0] is for even parities, and traversals_[1] for 
  // odd parities.
  vector<vector<bool> > traversals_;
  int start_;
  int end_;
};

int main() {
  int test_cases;
  cin >> test_cases;
  for (int case_num = 1; case_num <= test_cases; ++case_num) {
    Level level = Level();
    cout << "Case #" << case_num << ": ";
    if (level.IsPossible()) {
      cout << "I TOLD YOU SO from " << level.getStart();
      cout << " to " << level.getEnd() << endl;
    } else {
      cout << "GOODBYE CRUEL WORLD" << endl;
    }
  }
}