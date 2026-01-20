#include <iostream>
using namespace std;

int bitManipulation2(int n, int i) {
	//
	return n^(1<<i);
	//
}

int main() {
	int t, n, i;
	cin >> t;
	while (t--) {
		cin >> n >> i;
		cout << bitManipulation2(n, i) << endl;
	}
	return 0;
}
