n=int(input())
for i in range(n):
    num1,num2=input().split()
    # print(num1,num2)#
    len1=len(num1)
    len2=len(num2)
    # print(len1,len2)#
    num1=num1[::-1]
    num2=num2[::-1]
    # print(num1,num2)#
    ans=""
    if(len1>=len2):
        for k in range(len2):
            tmp=ord(num1[k])+ord(num2[k])-ord('0')
            ans+=chr(tmp)
        # print(num1[len2:])
        ans=ans+num1[len2:]
    else:
        for k in range(len1):
            tmp=ord(num1[k])+ord(num2[k])-ord('0')
            ans+=chr(tmp)
        ans=ans+num2[len1:]
    l=len(ans)
    # print(l)#
    for k in range(l-1):
        if ord(ans[k])-ord('0')>=10:
            ans=ans[:k]+chr(ord(ans[k])-10)+ans[k+1:]
            ans=ans[:k+1]+chr(ord(ans[k+1])+1)+ans[k+2:]#注意python不能直接修改字符串！！！
        # print(k,ans)#
    if ord(ans[l-1])-ord('0')>=10:
        ans=ans[:l-1]+chr(ord(ans[l-1])-10)+"1"
    ans=ans[::-1]
    print(ans)
