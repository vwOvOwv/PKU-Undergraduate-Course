#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<stack>

using namespace std;

class TreeNode {
public:
	vector<TreeNode*>children;
};

int treeHeight(TreeNode* root) {
	if (!root)
		return 0;
	int ans = 0;
	int n = root->children.size();
	for (int i = 0; i < n; i++) 
		ans = max(ans, treeHeight(root->children[i]) + 1);
	return ans;
}

int binaryTreeHeight(TreeNode* root) {
	if (!root)
		return 0;
	int ans = 0;
	int n = root->children.size();
	for (int i = 0; i < n; i++)
		ans = max(ans, binaryTreeHeight(root->children[i]) + i + 1);
	return ans;
}

int main() {
	int t = 0;
	string s;
	while (cin >> s) {
		if (s == "#")
			break;
		t++;
		stack<TreeNode*>st;
		TreeNode* root = new TreeNode;
		st.push(root);
		int n = s.size();
		for (int i = 0; i < n; i++) {
			if (s[i] == 'd') {
				TreeNode* tmp = new TreeNode;
				st.top()->children.push_back(tmp);
				st.push(tmp);
			}
			else {
				st.pop();
			}
		}
		cout << "Tree " << t << ": " << treeHeight(root) << " => " << binaryTreeHeight(root) << endl;
	}
}