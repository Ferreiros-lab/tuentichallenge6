#include <iostream>

using namespace std;

// Computes the number of tables needed requiered to seat all the diners.
// Assumes that 0 <= diners <= 2^16.
int NumberOfTables(int diners) {
  // The first table allows 4 seats. Any new table allows 2 more
  // seats. Therefore, if k>=1 then k tables allows 2k+2 seats.
  // If k=0 no seats are allowed. The inverse of this function
  // when k>=1 is ceiling((d-2)/2)=floor((d-1)/2).
  if (diners <= 2) 
    return (diners + 1) / 2;  // Exception when k=0.
  return (diners - 1) / 2;
}

int main() {
  int test_cases;
  cin >> test_cases;
  for (int case_num = 1; case_num <= test_cases; ++case_num) {
    int diners;
    cin >> diners;
    cout << "Case #" << case_num << ": " << NumberOfTables(diners) << endl;
  }
}