n=int(input())
sum=0
for i in range(n):
    age=int(input())
    sum+=age
print("%.2f"%(sum/n))