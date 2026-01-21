#include <iostream>
#include <cstring>
#include <algorithm>
#include <string>
using namespace std;
int map[1002][1002] = { 0 };
int color[1002][1002] = { 0 };
const int dm[8] = { 0,0,-1,1,1,1,-1,-1 };
const int dn[8] = { 1,-1,0,0,1,-1,1,-1 };
bool CorrectColor[1000000] = { 0 };
int length[100000] = { 0 };
int M, N;
void paint(int m, int n, int painter)
{
	color[m][n] = painter;
	for (int i = 0; i < 8; i++)
	{
		if (map[m + dm[i]][n + dn[i]] == -1)
		{
			CorrectColor[painter] = false;
		}
		else if ((map[m + dm[i]][n + dn[i]] == 0) & (color[m + dm[i]][n + dn[i]] == -1))
		{
			paint(m + dm[i], n + dn[i], painter);
		}
		else if ((map[m + dm[i]][n + dn[i]] == 1) & (color[m + dm[i]][n + dn[i]] != painter))
		{
			length[painter] ++;
			color[m + dm[i]][n + dn[i]] = painter;
		}
	}
}
bool paint1(int m, int n, int painter)
{
	color[m][n] = painter+114514;
	int temp = 0;
	for (int i = 0; i < 4; i++)
	{
		if (map[m + dm[i]][n + dn[i]] == 1 && ((color[m + dm[i]][n + dn[i]] == painter) | (color[m + dm[i]][n + dn[i]] == painter + 114514)))
			temp += 1;
	}
	if (temp != 2) return false;
	bool tmp = true;
	for (int i = 0; i < 4; i++)
	{
		if (map[m + dm[i]][n + dn[i]] == 1 && (color[m + dm[i]][n + dn[i]] == painter))
			tmp &= paint1(m + dm[i], n + dn[i], painter);
	}
	return tmp;
}
bool exam(int painter)
{
	bool flag0 = true, flag1 = true;
	for (int m=1;m<=M;m++)
		if (flag1)
			for (int n=1;n<=N;n++)
				if (map[m][n] == 1 & color[m][n] == painter)
				{
					if (flag0 == true)
					{
						flag0 = false;
						if (!paint1(m, n, painter)) flag1 = false;
					}
					else flag1 = false;
				}
	return flag1;
}
int main()
{
	int N23g9 = 0; cin >> N23g9;
	while (N23g9-- > 0)
	{
		cin >> M >> N;
		memset(map, -1, sizeof(map));
		memset(color, -1, sizeof(color));
		memset(CorrectColor, true, sizeof(CorrectColor));
		memset(length, 0, sizeof(length));
		char temp;
		for (int i = 1; i <= M; i++)
		{
			cin.get();
			for (int j = 1; j <= N; j++)
			{
				cin.get(temp);
				map[i][j] = temp - '0';
			}
		}
		for (int i = 0; i <= M+1; i++)
		{
			map[i][0] = -1; map[i][N+1] = -1;
		}
		for (int i = 0; i <= N+1; i++)
		{
			map[0][i] = -1; map[M+1][i] = -1;
		}
		int painter = 0;
		int lenmax = 0;
		for (int m = 1; m <= M; m++)
			for (int n = 1; n <= N; n++)
				if (map[m][n] == 0 & color[m][n] == -1)
				{
					paint(m, n, painter);
					if ((length[painter] > lenmax) & CorrectColor[painter])
						if (exam(painter)) lenmax = length[painter];
					painter++;
				}
		if (lenmax > 0) cout << lenmax << endl;
		else
		{
			bool temp = false;
			for (int i = 1; i < M; i++)
				if (!temp)
					for (int j = 1; j < N; j++)
						if (map[i][j] + map[i + 1][j] + map[i + 1][j + 1] + map[i][j + 1] == 4)
							temp = true;
			if (temp == true) cout << 4 << endl;
			else cout << 0 << endl;
		}
	}
}
