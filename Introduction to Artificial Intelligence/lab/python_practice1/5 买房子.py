n,k=map(int,input().split())
curPrice=200
curIncome=0
cnt=0
ans=0
while cnt<=20:
    if cnt>0:
        curPrice*=(100+k)/100
    curIncome+=n
    cnt+=1
    if curIncome>=curPrice:
        ans=cnt
        break
if ans==0:
    print("Impossible")
else:
    print(ans)

