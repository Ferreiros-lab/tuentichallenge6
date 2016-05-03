#include <iostream>
#include <vector>

using namespace std;

// Gives the integer value of a character, defined as follows: If it is a dot,
// then its value is 0. If it is lowercase, then its value is negative,
// starting from a=-1. If it is upercase, then its value is positive, starting
// from A=1.
int intValue(char c) {
  if (c == '.') return 0;
  if (c >= 'A' and c <= 'Z') return c - 'A' + 1;
  return -c + 'a' - 1;
}

// Returns whether there exists a band with positive sum. A band is defined
// as either a full row or a full column.
bool positive_band(vector<string>& matrix) {
  int N = matrix.size();  // Rows
  int M = matrix[0].size();  // Columns
  for (int i = 0; i < N; i++) {  // Check rows
    int sum = 0;
    for (int j = 0; j < M; j++) {
      sum += intValue(matrix[i][j]);
    }
    if (sum > 0) return true;
  }
  for (int i = 0; i < M; i++) {  // Check columns
    int sum = 0;
    for (int j = 0; j < N; j++) {
      sum += intValue(matrix[j][i]);
    }
    if (sum > 0) return true;
  }
  return false;
}

// Computes the largest sum of consecutive subsequences of two consecutive
// copies of a given vector. If there is no positive sum, it returns 0.
int LargestSubsequenceTwo(vector<int>& values) {
  // Kadane's algorithm
  int M = values.size();
  int max_ending_here = 0;
  int max_so_far = 0;
  for (int iteration = 0; iteration < 2; iteration++) {  // Two copies
    for (int i = 0; i < M; i++) {
      max_ending_here = max(0, max_ending_here + values[i]);
      max_so_far = max(max_so_far, max_ending_here);
    }
  }
  return max_so_far;
}

// Returns the largest sum of a subrectangle of a matrix consisting of four
// copies of a given matrix. In particular, if the matrix is M, the function
// returns the largest subrectangle of the following matrix:
// MM
// MM
// If there is no positive sum, it returns 0.
int LargestSumFour(vector<string>& matrix) {
  int N = matrix.size();  // Rows
  int M = matrix[0].size();  // Columns
  
  // Since the new matrix may be huge, we will not compute it explicitly.
  // First, we conjecture the starting and ending rows. Then we compute
  // the sum for each subcolumn between the rows, so that the problem
  // becames 1-dimensional.
  int answer = 0;
  for (int i = 0; i < N; i++) {  // Initial row
    vector<int> sum (M, 0);  // Sum of elements between the rows
    for (int j = i; j < i + N; j++) {  // Final row
      int realj = j % N;  // Modulo is expensive: compute just once
      for (int k = 0; k < M; k++) {
        sum[k] += intValue(matrix[realj][k]);
      }
      answer = max(answer, LargestSubsequenceTwo(sum));
    }
  }
  return answer;
}

// Computes the largest sum of a rectangle in an infinite layout of matrices
// equal to matrix. If the answer is infinity, it returns -1 instead.
int LargestSum(vector<string>& matrix) {
  // The largest sum is infinity iff there exists either a row or a column
  // with positive sum.
  if (positive_band(matrix))
    return -1;
  // Otherwise, one of the rectangles with largest sum will not contain any
  // full row or column, so it will traverse the horizontal and vertical
  // borders at most once. The solution is then the largest subrectangle
  // of two times two copies of matrix.
  return LargestSumFour(matrix);
}

int main() {
  int test_cases;
  cin >> test_cases;
  for (int case_num = 1; case_num <= test_cases; ++case_num) {
    int N, M;
    cin >> N >> M;
    vector<string> matrix(N);
    for (int i = 0; i < N; i++)
      cin >> matrix[i];
    int max_sum = LargestSum(matrix);
    cout << "Case #" << case_num << ": ";
    if (max_sum >= 0)
      cout << max_sum << endl;
    else
      cout << "INFINITY" << endl;
  }
}