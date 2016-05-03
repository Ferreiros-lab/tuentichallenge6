#include <iostream>
#include <vector>
#include <queue>

using namespace std;



// Stores the information of a level
class Level {
 public:
  // Returns whether is possible to complete the level.
  // If so, updates variables that can be acceeded later.
  bool IsPossible() {
    ComputeDistances();  // Distances between distinguished points
    ComputeShortestPath();
    return best_frames_ < kInfinity;
  }
  
  int getFrames() {
    return best_frames_;
  }
  
  string getMovements() {
    return best_instr_;
  }
  
  // Prints distances and instructions between keys, for debug purpouses
  void PrintDistances() {
    for (int i = 0; i < positions_.size(); i++) {
      for (int j = 0; j < positions_.size(); j++) {
        cout << i << " " << j << " " << distance_[i][j] << " ";
        cout << instructions_[i][j] << endl;
      }
    }
  }
  
  // Reads a new level from stdin
  Level() {
    cin >> N_ >> M_;
    level_map_ = vector<string> (N_);
    for (int i = 0; i < N_; i++)
      cin >> level_map_[i];
    ParsePositions();
  }
  
 private:
  
  // A position in a map
  struct Position {
    int y;  // From 0 to N - 1, 0 is top
    int x;  // From 0 to M - 1, 0 is left
    Position(int xx, int yy): x(xx), y(yy) {}
    Position() {x=0;y=0;}
  };
  
  // Returns the number of ones in a number
  static int NumberOfOnes(int number) {
    if (number == 0)
      return 0;
    return number % 2 + NumberOfOnes(number / 2);
  }
  
  // Returns the shortest path from the start to key last_key, traversing
  // the keys contained in mask in the middle. Updates instructions
  // with the best path. Assumes that last_key is a key contained in mask.
  int PartialPath (int mask,
                   int last_key,
                   string& instructions,
                   vector<vector<int> >& dynamic_frames,
                   vector<vector<string> >& dynamic_instr) {
    
    if (NumberOfOnes(mask) == 1) {  // First key
      instructions = instructions_[instructions_.size()-2][last_key];
      return distance_[distance_.size()-2][last_key];
    }
    // Using dynamic programming to avoid computing the same twice
    if (dynamic_frames[mask][last_key] != -1) {
      instructions = dynamic_instr[mask][last_key];
      return dynamic_frames[mask][last_key];
    }
    
    int frames = kInfinity;
    instructions = "";
    // Conjectures the previous key
    for (int key = 0; key < keys_.size(); key++) {
      if ((((1 << key) & mask) != 0) && (key != last_key)) {
        string last_instructions;
        int last_frames = PartialPath(mask - (1 << last_key),
                                      key,
                                      last_instructions,
                                      dynamic_frames,
                                      dynamic_instr);
        if (last_frames + distance_[key][last_key] < frames) {
          frames = last_frames + distance_[key][last_key];
          instructions = last_instructions + instructions_[key][last_key];
        } else if (last_frames + distance_[key][last_key] == frames &&
                  Better(last_instructions + instructions_[key][last_key],
                          instructions)) {
          frames = last_frames + distance_[key][last_key];
          instructions = last_instructions + instructions_[key][last_key];
        }
      }
    }
    dynamic_instr[mask][last_key] = instructions;
    dynamic_frames[mask][last_key] = frames;
    return frames;
  }
  
  // Computes the shortest path between the start and the end and
  // stores the result in variables best_frames_ and best_ instr_
  void ComputeShortestPath() {
    
    if (keys_.size() == 0) {
      best_frames_ = distance_[0][1];
      best_instr_ = instructions_[0][1];
      return;
    }
    // Mask of visited keys
    int mask = (1 << keys_.size()) - 1;
    vector<vector<int> > dynamic_frames(1 << keys_.size(),
                                        vector<int> (keys_.size(), -1));
    vector<vector<string> > dynamic_instr(1 << keys_.size(),
                                          vector<string> (keys_.size()));
    
    int best_frames = kInfinity;
    string best_instr = "";
    // Last key before the end
    for (int i = 0; i < keys_.size(); i++) {
      string instr;
      int frames = PartialPath(mask, i, instr, dynamic_frames, dynamic_instr);
      if (frames + distance_[i][distance_.size()-1] < best_frames) {
        best_frames = frames + distance_[i][distance_.size()-1];
        best_instr = instr + instructions_[i][instructions_.size()-1]; 
      } else if (frames + distance_[i][distance_.size()-1] == best_frames &&
                 Better(instr + instructions_[i][instructions_.size()-1],
                        best_instr)) {
        best_frames = frames + distance_[i][distance_.size()-1];
        best_instr = instr + instructions_[i][instructions_.size()-1]; 
      }
    }
    
    best_frames_ = best_frames;
    best_instr_ = best_instr;
    
  }
  
  // Reads where keys, start and end positions are
  void ParsePositions() {
    
    for (int i = 0; i < N_; i++) {
      for (int j = 0; j < M_; j++) {
        if (level_map_[i][j] == 'S') {
          start_ = Position(j, i);
          level_map_[i][j] == '.';
        }
        if (level_map_[i][j] == 'E') {
          end_ = Position(j, i);
          level_map_[i][j] == '.';
        }
        if (level_map_[i][j] == 'K') {
          keys_.push_back(Position(j, i));
          level_map_[i][j] == '.';
        }
      }
    }
  }
  
  // Returns whether a set of instructions is better than another one.
  // Assumes that the instructions are different, but otherwise it will
  // return true
  static bool Better(string a, string b) {
    if (a.size() < b.size())
      return true;
    if (a.size() > b.size())
      return false;
    return a <= b;
  }
  
  // Returns a list of possible movements from a position
  vector<pair<Position, string> > Move(Position current) {
    // blocked
    if (level_map_[current.y][current.x] == '#')
      return vector<pair<Position, string> >(0);
    
    bool falling = true;
    if (level_map_[current.y][current.x] == 'H')
      falling = false;
    if (level_map_[current.y + 1][current.x] == 'H')
      falling = false;
    if (level_map_[current.y + 1][current.x] == '#')
      falling = false;
    
    if (falling) {
      Position new_position = current;
      new_position.y++;
      pair<Position, string> p(new_position, "");
      return vector<pair<Position, string> >(1, p);
    }
    
    vector<pair<Position, string> > movements;
    Position new_position = current;
    // UP 
    if (current.y > 0) {
      new_position.y--;
      pair<Position, string> p(new_position, "W");
      movements.push_back(p);
    }
    
    // DOWN
    new_position = current;
    new_position.y++;
    pair<Position, string> p(new_position, "S");
    movements.push_back(p);
    
    // LEFT
    new_position = current;
    new_position.x = (new_position.x + M_ - 1) % M_;
    p = pair<Position, string> (new_position, "A");
    movements.push_back(p);
    
    // RIGHT
    new_position = current;
    new_position.x = (new_position.x + 1) % M_;
    p = pair<Position, string> (new_position, "D");
    movements.push_back(p);
    
    return movements;
    
  }
  
  // Computes the distance between position pos_num and the rest
  // of positions.
  void ComputeDistancesFrom(int pos_num) {
    
    Position initial_position = positions_[pos_num];
    vector<vector<int> > distance(N_, vector<int> (M_, kInfinity));
    vector<vector<string> > instructions(N_, vector<string> (M_));
    distance[initial_position.y][initial_position.x] = 0;
    queue<Position> q;
    q.push(initial_position);
    while (not q.empty()) {  // Computes the distance to all the positions
      Position current = q.front();
      q.pop();
      int current_distance = distance[current.y][current.x];
      string current_instructions = instructions[current.y][current.x];
      vector<pair<Position, string> > movements = Move(current);
      for (int i = 0; i < movements.size(); i++) {
        Position new_position = movements[i].first;
        int new_distance = distance[new_position.y][new_position.x];
        string new_instructions = current_instructions + movements[i].second;
        if (current_distance + 1 < new_distance) {
          distance[new_position.y][new_position.x] = current_distance + 1;
          instructions[new_position.y][new_position.x] = new_instructions;
          q.push(new_position);
        } else if (current_distance + 1 == new_distance && 
                   Better(new_instructions, 
                          instructions[new_position.y][new_position.x])) {
          instructions[new_position.y][new_position.x] = new_instructions;
        }
      }
    }
    for (int i = 0; i < positions_.size(); i++) {
      Position p = positions_[i];
      distance_[pos_num][i] = distance[p.y][p.x];
      instructions_[pos_num][i] = instructions[p.y][p.x];
    }
  }
  
  // Computes the distance between each pair of distinguished positions
  // (start, end and keys), and the best string to move from one to the
  // other. A shorter string is better, and in case of tie a lexicographically
  // smaller string is better.
  void ComputeDistances() {
    positions_ = keys_;
    positions_.push_back(start_);
    positions_.push_back(end_);
    distance_ = vector<vector<int> > (positions_.size(),
                                      vector<int> (positions_.size()));
    instructions_ = vector<vector<string> > (positions_.size(),
                                      vector<string> (positions_.size()));
    
    for (int i = 0; i < positions_.size(); i++) {
      ComputeDistancesFrom(i);
    }
  }
  
  vector<string> level_map_;
  int N_;  // Number of rows
  int M_;  // Number of columns
  Position start_;
  Position end_;
  vector<Position> keys_;
  vector<Position> positions_;
  vector<vector<int> > distance_;
  vector<vector<string> > instructions_;
  static const int kInfinity = 100000000;
  int best_frames_;
  string best_instr_;
};

int main() {
  int test_cases;
  cin >> test_cases;
  for (int case_num = 1; case_num <= test_cases; ++case_num) {
    Level level = Level();
    cout << "Case #" << case_num << ": ";
    if (level.IsPossible()) {
      cout << level.getFrames() << " " << level.getMovements() << endl;
    } else {
      cout << "IMPOSSIBLE" << endl;
    }
  }
}