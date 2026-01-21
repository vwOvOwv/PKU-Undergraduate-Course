num=input()
tmp=ord(num[0])
if tmp>=ord('A') and tmp<=ord('Z'):
    l=len(num)
    num+='.'
    i=0
    ans=0
    while i<l:
        if num[i]=='I':
            if num[i+1]=='V':
                ans+=4
                i+=1
            elif num[i+1]=='X':
                ans+=9
                i+=1
            else:
                ans+=1
        elif num[i]=='V':
            ans+=5
        elif num[i]=='X':
            if num[i+1]=='L':
                ans+=40
                i+=1
            elif num[i+1]=='C':
                ans+=90
                i+=1
            else:
                ans+=10
        elif num[i]=='L':
            ans+=50
        elif num[i]=='C':
            if num[i+1]=='D':
                ans+=400
                i+=1
            elif num[i+1]=='M':
                ans+=900
                i+=1
            else:
                ans+=100
        elif num[i]=='D':
            ans+=500
        elif num[i]=='M':
            ans+=1000
        i+=1
    print(ans)
else:
    num=int(num)
    ans=''
    while num>=1000:
        ans+='M'
        num-=1000
    while num>=900:
        ans+="CM"
        num-=900
    while num>=500:
        ans+='D'
        num-=500
    while num>=400:
        ans+="CD"
        num-=400
    while num>=100:
        ans+='C'
        num-=100
    while num>=90:
        ans+="XC"
        num-=90
    while num>=50:
        ans+='L'
        num-=50
    while num>=40:
        ans+="XL"
        num-=40
    while num>=10:
        ans+='X'
        num-=10
    while num>=9:
        ans+="IX"
        num-=9
    while num>=5:
        ans+='V'
        num-=5
    while num>=4:
        ans+="IV"
        num-=4
    while num>=1:
        ans+='I'
        num-=1
    print(ans)
