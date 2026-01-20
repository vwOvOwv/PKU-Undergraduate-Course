n=int(input())
win=[0]*(100001)
lose=[0]*(100001)
sum=[0]*(100001)

for i in range(n):
    w,l=map(int,input().split())
    win[w]=win[w]+1
    lose[l]=lose[l]+1
    sum[w]=sum[w]+1
    sum[l]=sum[l]+1

ans=[]
for i in range(1,100001):
    if lose[i]==0 and sum[i]>0:
        ans.append(i)
length=len(ans)
if length==0:
    print("None")
else:
    for i in range(length):
        if i<length-1:
            print(ans[i],end=", ")
        else:
            print(ans[i])
