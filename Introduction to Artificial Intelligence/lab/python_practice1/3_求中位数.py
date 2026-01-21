n=int(input())
rec=map(int,input().split())
rec=sorted(rec)
if n%2==1:
    print(rec[(n-1)//2])
else:
    ans=(rec[(n-1)//2]+rec[(n+1)//2])/2
    tmp=ans
    while tmp>=1:
        tmp-=1
    # print(tmp)
    if abs(tmp)<1e-5:
        print(int(ans))
    else:
        print("%.1f"%ans)