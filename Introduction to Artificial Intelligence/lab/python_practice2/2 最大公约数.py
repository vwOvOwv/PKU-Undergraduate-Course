def gcd(a,b):
    if a%b==0:
        return b
    else:
        return gcd(b,a%b)

n=int(input())
while n:
    a,b=map(int,input().split(' '))
    n=n-1
    print(gcd(a,b))