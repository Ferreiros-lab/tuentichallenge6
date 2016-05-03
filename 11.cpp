#include <iostream>

using namespace std;

// Computes the number of ones in the binary representation of a given number.
unsigned int OnesInBinary(unsigned long long number) {
  if (number == 0)
    return 0;
  return number % 2 + OnesInBinary(number / 2);
}

// Computes the floor of the logarithm in base 2 of a number
int Logarithm(unsigned long long number) {
  if (number == 1)
    return 0;
  return 1 + Logarithm(number / 2);
}

// Computes the number of seconds needed to obtain a given number
// of slices starting with piles of a single slice. Assumes
// that it is possible.
int NumberOfCuts(unsigned long long piles,
                                unsigned long long total) {
  
  // Greedy algorithm: piles - 1 piles will stay with one slice,
  // and the other one will generate the needed number, obtained
  // as the logarithm of what we need plus the number of ones minus one.
  
  total -= piles - 1;
  return Logarithm(total) + OnesInBinary(total) - 1;
}


// Computes the number of seconds needed to obtain a given number
// of slices starting with some other number of slices in piles
// of equal length, or -1 if it is not possible.
int GetSeconds(unsigned long long piles,
               unsigned long long slices,
               unsigned long long total) {
  // Every cut adds a number of slices multiple of 'slices'
  if (total % slices != 0)
    return -1;
  
  total /= slices;  // Consider piles of one single slice
  
  // If we have more slices than we need at the beginning.
  if (piles > total)
    return -1;
  
  return NumberOfCuts(piles, total);
}

int main() {
  int C;
  cin >> C;
  for (int num_case = 1; num_case <= C; ++num_case) {
    unsigned long long N, M, K;
    cin >> N >> M >> K;
    int seconds = GetSeconds(N, M, K);
    
    cout << "Case #" << num_case << ": ";
    if (seconds >= 0)
      cout << seconds << endl;
    else
      cout << "IMPOSSIBLE" << endl;
  }
}