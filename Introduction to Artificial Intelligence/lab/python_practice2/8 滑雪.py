r,c=map(int,input().split())
rec=[[0 for i in range(c)]for j in range(r)]
for i in range(r):
    L=input().split()
    for j in range(c):
        rec[i][j]=int(L[j])
# print(rec)#
dp=[[1 for i in range(c)]for j in range(r)]
# print(dp)#
nodes=[(rec[i][j],i,j)for i in range(r) for j in range(c)]
nodes.sort()
for node,x,y in nodes:
    for dx,dy in ((0,-1),(0,1),(1,0),(-1,0)):
        xx,yy=x+dx,y+dy
        if xx>=0 and xx<r and yy>=0 and yy<c and rec[xx][yy]<rec[x][y]:
                dp[x][y]=max(dp[x][y],dp[xx][yy]+1)
# print(dp)#
ans=max(max(r) for r in dp)
print(ans)

def longest_path(matrix):
    if not matrix:
        return 0
    rows, cols = len(matrix), len(matrix[0])
    # 初始化 dp 数组，dp[i][j] 表示从 (i,j) 出发的最长滑雪路径长度
    dp = [[1 for _ in range(cols)] for _ in range(rows)]
    # 将节点按高度升序排序，从而使在解决每个问题之前其子问题已经被解决
    nodes = [(matrix[i][j], i, j) for i in range(rows) for j in range(cols)]
    nodes.sort()
    # 依次处理每个节点
    for height, x, y in nodes:
        # 遍历当前节点的四个相邻节点
        for dx, dy in ((-1, 0), (1, 0), (0, -1), (0, 1)):
            new_x, new_y = x + dx, y + dy
            # 如果相邻节点的高度低于当前节点，则更新 dp 值
            if 0 <= new_x < rows and 0 <= new_y < cols and matrix[new_x][new_y] < matrix[x][y]:
                dp[x][y] = max(dp[x][y], dp[new_x][new_y] + 1)
    # 返回最大的 dp 值，即最长滑雪路径长度
    return max(max(row) for row in dp)

# print(longest_path(rec))

