rec=input().split()
size=len(rec)
rec.reverse()
for i in range(size):
    print(rec[i],end="")
    if i<size-1:
        print(" ",end="")
print("\n")