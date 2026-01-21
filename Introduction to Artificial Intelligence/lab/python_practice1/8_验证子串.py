def kmp(p,t):
    l1=len(p)
    l2=len(t)
    next=[0 for i in range(l1)]
    next[0]=-1
    j=0
    k=0
    while j<l1:
        while k>=0 and p[j]!=p[k]:
            k=next[k]
        j+=1
        k+=1
        if j==l1:
            break
        if p[j]==p[k]:
            next[j]=next[k]
        else:
            next[j]=k
        # print(j,next[j])
    i=0
    j=0
    # print(next)
    while j<l1 and i<l2:
        if j==-1 or p[j]==t[i]:
            i+=1
            j+=1
        else:
            j=next[j]
    if j>=l1:
        return True
    return False

s1=input()
s2=input()
l1=len(s1)
l2=len(s2)
flag=0
if l1>=l2:
    if kmp(s2,s1):
        print(s2,"is substring of",s1)
        flag=1
else:
    if kmp(s1,s2):
        print(s1,"is substring of",s2)
        flag=1
if flag==0:
    print("No substring")
