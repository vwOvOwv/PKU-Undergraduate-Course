str1,str2=input().split()
len1=len(str1)
len2=len(str2)
if len1>=len2:
    ans=str1+str2
else:
    ans=str2+str1
print(ans)