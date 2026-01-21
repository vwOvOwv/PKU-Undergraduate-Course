n=int(input())
rec=[]
for i in range(n):
    str=input()
    rec.append(str)
    ans=sorted(rec)
for i in range(n):
    print(ans[i])