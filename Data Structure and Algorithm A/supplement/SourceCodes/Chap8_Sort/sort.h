#pragma once
#include <time.h>  // Used by timing functions
#include <stdlib.h>
#include <iostream>
using namespace std;
int	 THRESHOLD = 28;

// Utility functions and macros


// Random number generator functions

inline void Randomize() // Seed the generator
  { srand(1); }

// Return a random value in range 0 to n-1
inline int Random(int n)
  { return rand() % (n); }

// Swap two elements in a generic array
template<class Record>
inline void swap(Record A[], int i, int j) {
  Record temp = A[i];
  A[i] = A[j];
  A[j] = temp;
}

// Swap two objects passed by reference
template<class Record>
inline void swap(Record &e1, Record &e2) {
  Record temp = e1;
  e1 = e2;
  e2 = temp;
}

template<class Record>
void Print(Record* array, int listsize) {
  for(int i=0; i<listsize; i++)
    cout << array[i] << " ";
  cout << "\n";
} 


// Timing variables and functions
clock_t tstart = 0;  // Time at beginning of timed section

// Initialize the program timer
void Settime()
  { tstart = clock(); }

// Return the elapsed time since the last call to Settime
double Gettime()
  { return (double)((double)clock() - (double)tstart)/
           (double)CLOCKS_PER_SEC; }

// Your basic int type as an object.
class Int {
private:
  int val;
public:
  Int(int input=0) { val = input; }
  // The following is for those times when we actually
  //   need to get a value, rather than compare objects.
  int key() const { return val; }
  // Overload = to support Int foo = 5 syntax
  Int operator= (int input) { val = input; }
};

// Let us print out Ints easily
ostream& operator<<(ostream& s, const Int& i)
  { return s << i.key(); }
ostream& operator<<(ostream& s, const Int* i)
  { return s << i->key(); }
