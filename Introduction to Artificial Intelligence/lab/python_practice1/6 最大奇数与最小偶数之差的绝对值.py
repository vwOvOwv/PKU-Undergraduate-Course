maxOdd=0
minEven=120
num=input().split()
for i in range(6):
    tmp=int(num[i])
    if tmp%2==1:
        maxOdd=max(maxOdd,tmp)
    else:
        minEven=min(minEven,tmp)
print(abs(maxOdd-minEven))