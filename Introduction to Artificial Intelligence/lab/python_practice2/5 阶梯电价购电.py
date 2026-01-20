p1=240*0.4883
p2=160*0.5383
m=float(input())
ans=0
if m<=p1:
    ans=m/0.4883
elif m<=p1+p2:
    ans=240+(m-p1)/0.5383
else:
    ans=400+(m-p1-p2)/0.7883
print("%.2f" %ans)


