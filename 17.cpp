#include <iostream>
#include <vector>

using namespace std;

long long abs_value(long long a) {
  if (a >= 0) return a;
  return -a;
}
// Checks whether is possible to divide a vector in K consecutive
// subsequences with sum less or equal than limit.
bool VerticalPossible(vector<long long>& differences, int K, int limit) {
  int N = differences.size();  // N-1 according to the statement
  // Greedy algorithm: walk every day as much as possible
  long long accumulated = 0;
  int days = 0;
  for (int i = 0; i < N; i++) {
    if (differences[i] > limit) {
      return false;
    }
    if (accumulated + differences[i] > limit) {
      days++;
      accumulated = differences[i];
    } else {
      accumulated += differences[i];
    }
  }
  return days + 1 <= K;
}

// Computes the minimal value such that differences can be divided
// in K consecutive subsequences summing less or equal than the value.
// Assumes that K is smalles or erqual than the size of differences.
long long MinVertical(vector<long long>& differences, int K) {
  int N = differences.size();  // N-1 according to the statement
  // Binary search
  long long minimum = 0;
  long long maximum = 0;
  for (int i = 0; i < N; i++) // The sum is always a valid solution.
    maximum += differences[i];
  while (minimum < maximum) {
    long long middle = (minimum + maximum) / 2;
    if (VerticalPossible(differences, K, middle)) {
      maximum = middle;
    } else {
      minimum = middle + 1;
    }
  }
  return minimum;
}

// Checks whether is possible to divide a vector in K consecutive
// subsequences with length less or equal than limit_horizontal
// and sum less or equal than limit_vertical.
bool HorizontalPossible(vector<long long>& differences,
                      int K,
                      int limit_horizontal,
                      long long limit_vertical) {
  // Greedy algorithm: walk every day as much as possible
  if (limit_horizontal == 0) {
    return false;
  }
  int N = differences.size();  // N-1 according to the statement
  // Greedy algorithm: walk every day as much as possible
  long long accumulated = 0;
  int length = 0;
  int days = 0;
  for (int i = 0; i < N; i++) {
    if (accumulated + differences[i] > limit_vertical ||
        length + 1 > limit_horizontal) {
      days++;
      accumulated = differences[i];
    length = 1;
    } else {
      accumulated += differences[i];
      length++;
    }
  }
  return days + 1 <= K;
}

// Computes the minimal value such that differences can be divided
// in K consecutive subsequences of length the value summing less
// or equal than limit_vertical. Assumes that K is smalles or equal
// than the size of differences, and that there is a valid division
// of differences in K subsequences summing les than limit_vertical
int MinHorizontal(vector<long long>& differences,
                  int K,
                  long long limit_vertical) {
  int N = differences.size();  // N-1 according to the statement
  // Binary search
  int minimum = 0;
  int maximum = N;  // N is always a valid solution.
  while (minimum < maximum) {
    int middle = (minimum + maximum) / 2;
    if (HorizontalPossible(differences, K, middle, limit_vertical)) {
      maximum = middle;
    } else {
      minimum = middle + 1;
    }
  }
  return minimum;
}


// For each position i computes the minimal number of days needed
// to finish the route starting at position i, and not walking
// more than limit_horizontal km each day, nor climbing more than
// limi_vertical. Assumes that it is always possible.
vector<int> MinimalDays(vector<long long>& differences,
                        int limit_horizontal,
                        long long limit_vertical) {
  
  // Greedy algorithm: start from the end and walk backwards
  // as much as possible.
  int N = differences.size();  // N-1 according to the statement
  // Greedy algorithm: walk every day as much as possible
  long long accumulated = 0;
  int length = 0;
  int days = 1;
  vector<int> min_days(N);
  for (int i = N-1; i >= 0; i--) {
    if (accumulated + differences[i] > limit_vertical ||
        length + 1 > limit_horizontal) {
      days++;
      accumulated = differences[i];
    length = 1;
    } else {
      accumulated += differences[i];
      length++;
    }
    min_days[i] = days;
  }
  return min_days;
}

// Computes the distance that must be walked every day to minimize
// the lexicographical order, and satisfying that the maximum
// distance walked every day is at most limit_horizontal,
// and the height is less than limi_vertical.
vector<int> ConstrainedDistances(vector<long long>& differences,
                                 int K,
                                 int limit_horizontal,
                                 long long limit_vertical) {
  
  int N = differences.size();  // N-1 according to the statement
  vector<int> min_days = MinimalDays(differences,
                                     limit_horizontal,
                                     limit_vertical);

  vector<int> distances(K);
  int days = 0;
  int last = 0;
  for (int i = 1; i < N; i++) {
    if (min_days[i] + days < K) {
      distances[days] = i - last;
      last = i;
      days++;
    }
  }
  distances[K-1] = N - last;
  return distances;
}

// Computes the distance that must be walked every day to minimize:
//  -The sum of differences of heights
//  -If there is a tie, the maximum distance walked in one day
//  -Finally, if there is a tie, the lexicographical order.
vector<int> Distances(vector<long long>& heights, int K) {
  int N = heights.size();
  // We only need the list of differences between heights.
  vector<long long> differences(N-1);
  for (int i = 0; i < N-1; i++)
    differences[i] = abs_value(heights[i] - heights[i+1]);
  
  // Minimal sum of meters up and down
  long long min_vertical = MinVertical(differences, K);
  int min_horizontal = MinHorizontal(differences, K, min_vertical);
  return ConstrainedDistances(differences, K, min_horizontal, min_vertical);
}

int main() {
  int test_cases;
  cin >> test_cases;
  for (int case_num = 1; case_num <= test_cases; ++case_num) {
    int N, K;
    cin >> N >> K;
    vector<long long> heights(N);
    for (int i = 0; i < N; i++)
      cin >> heights[i];
    vector<int> distances = Distances(heights, K);
    cout << "Case #" << case_num << ":";
    for (int i = 0; i < K; i++) {
      cout << " " << distances[i];
    }
    cout << endl;
  }
}