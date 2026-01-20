#include<iostream>
#include<string>
#include<stack>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<cmath>

using namespace std;

void bucketSort(vector<int>&rec, int d) {
	vector<int>count(10, 0);
	vector<vector<int> >output(10);
	int n = rec.size();
	for (int i = 0; i < n; i++) {
		int k = int(rec[i] / (pow(10, d))) % 10;
		count[k]++;
	}
	for (int i = 1; i < 10; i++)
		count[i] += count[i - 1];
	vector<int>tmp = rec;
	for (int i = n-1; i >= 0; i--) {
		int k = int(tmp[i] / (pow(10, d))) % 10;
		rec[--count[k]] = tmp[i];
		output[k].push_back(tmp[i]);
	}
	for (int i = 0; i < 10; i++) {
		cout << "Queue" << i << ':';
		int size = output[i].size();
		for (int j = size-1; j >= 0; j--) {
			cout << output[i][j];
			if (j > 0)
				cout << ' ';
		}
		cout << endl;
	}
}

void radixSort(vector<int>&rec) {
	int n = rec.size();
	int maxi = rec[0];
	for (int i = 0; i < n; i++) {
		maxi = max(maxi, rec[i]);
	}
	int cnt = 0;
	while (maxi) {
		cnt++;
		maxi /= 10;
	}
	for (int i = 0; i < cnt; i++) {
		cout << "Step" << i+1 << '.' << endl;
		bucketSort(rec, i);
	}
	for (int i = 0; i < n; i++) {
		cout << rec[i];
		if (i < n - 1)
			cout << ' ';
	}
	cout << endl;
}

int main() {
	int n;
	cin >> n;
	vector<int>rec(n);
	for (int i = 0; i < n; i++) {
		cin >> rec[i];
	}
	radixSort(rec);
	return 0;
}