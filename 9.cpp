#include <iostream>

using namespace std;

// Computes the next integer of the form 1* which is multiple of N.
// Assumes that N is not multiple of 2 or 5
long long Ones(long long N) {
  long long answer = 0;
  // We keep 1* % N to avoid working with big integers
  long long onesmod = 0;  // 1^answer % N
  do {
    ++answer;
    onesmod *= 10;
    ++onesmod;
    onesmod %= N;
  } while (onesmod != 0);
  return answer;
}

// Removes the 2 and 5 factors of N, and returns the maximum between the
// exponents of 2 and 5
long long Reduce(long long & N) {
  int twos = 0;
  int fives = 0;
  while (N % 2 == 0) {
    N /= 2;
    ++twos;
  }
  while (N % 5 == 0) {
    N /= 5;
    ++fives;
  }
  return max(twos, fives);
}

int main() {
  int C;
  cin >> C;
  for (int num_case = 1; num_case <= C; ++num_case) {
    long long N;
    cin >> N;
    long long zeros = Reduce(N);
    cout << "Case #" << num_case << ": " << Ones(N) << " " << zeros << endl;
  }
}