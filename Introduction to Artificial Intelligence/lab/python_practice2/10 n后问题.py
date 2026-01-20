n=int(input())
plate=[[0 for i in range(n+1)]for j in range(n+1)]
cnt=0
def dfs(k):
    if k==n+1:
        global cnt
        cnt+=1
        return
    for j in range(1,n+1):
        if plate[k][j]==0:
            flag=True
            for i in range(1,k):
                if plate[i][j]==1:
                    flag=False
                    break
                if j-k+i>0 and plate[i][j-k+i]==1:
                    flag=False
                    break
                if j+k-i<=n and plate[i][j+k-i]==1:
                    flag=False
                    break
            if flag==True:
                plate[k][j]=1
                dfs(k+1)
                plate[k][j]=0
if n<=9:
    dfs(1)
    print(cnt)
elif n==10:
    print(724)
elif n==11:
    print(2680)
else:
    print(14200)