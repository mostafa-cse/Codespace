#include <bits/stdc++.h>
using namespace std;
#define int long long
int st[800005];
int lazy[800005];
int ans[200005];
int bs[2]={37,41};
int mods[2]={(int)1e9+7,(int)1e9+9};
void push_down(int l,int r,int ni)
{
    if(lazy[ni]==0)return;
    if(l == r)
    {
        ans[l]=max(ans[l],lazy[ni]);
        lazy[ni]=0;
        return;
    }
    lazy[ni*2]=max(lazy[ni*2],lazy[ni]);
    int m=(l+r)/2;
    lazy[ni*2+1]=max(lazy[ni*2+1],lazy[ni] + (m-l+1)*2);
    lazy[ni]=0;
}
void upd(int l,int r,int ql,int qr,int v,int ni)
{
    push_down(l,r,ni);
    if(l>qr || r<ql)return;
    if(l>=ql && r<=qr)
    {
        lazy[ni]=v+(l-ql)*2;
        push_down(l,r,ni);
        return;
    }
    int m=(l+r)/2;
    upd(l,m,ql,qr,v,ni*2);
    upd(m+1,r,ql,qr,v,ni*2+1);
}
int pwr(int b,int p,int md)
{
    if(p==0)return 1;
    int ret=pwr(b,p/2,md);
    ret*=ret;
    ret%=md;
    if(p&1)
    {
        ret*=b;
        ret%=md;
    }
    return ret;
}
int inv(int x,int md)
{
    return pwr(x,md-2,md);
}
    string s;
    int n;
array<int,2>prf[200005],suf[200005];
void calchshprf()
{
    array<int,2>cb = {1,1};
    array<int,2>hshs = {0,0};
    for(int j=0;j<2;j++)
    {
        for(int i=1;i<=n;i++)
        {
            hshs[j]= (s[i]-'a'+1)*cb[j]%mods[j];
            hshs[j]%=mods[j];

            prf[i][j]=prf[i-1][j]+hshs[j];
            prf[i][j]%=mods[j];

            cb[j]*=bs[j];
            cb[j]%=mods[j];
        }
    }

}
void calchshsuf()
{
    array<int,2>cb = {1,1};
    array<int,2>hshs = {0,0};
    for(int j=0;j<2;j++)
    {
        for(int i=n;i>=1;i--)
        {
            hshs[j]= (s[i]-'a'+1)*cb[j]%mods[j];
            hshs[j]%=mods[j];

            suf[i][j]=suf[i+1][j]+hshs[j];
            suf[i][j]%=mods[j];

            cb[j]*=bs[j];
            cb[j]%=mods[j];
        }
    }
}
int invv[200005][2];
array<int,2>getlrp(int l,int r)
{
    return {(prf[r][0]-prf[l-1][0]+mods[0])%mods[0] * invv[l-1][0]%mods[0],
            (prf[r][1]-prf[l-1][1]+mods[1])%mods[1] * invv[l-1][1]%mods[1]};

}
array<int,2>getlrs(int l,int r)
{
    swap(l,r);
    return {(suf[r][0]-suf[l+1][0]+mods[0])%mods[0] * invv[n-l][0]%mods[0],
            (suf[r][1]-suf[l+1][1]+mods[1])%mods[1] * invv[n-l][1]%mods[1]};
}
int32_t main()
{
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);

    cin>>s;
    n=s.size();
    invv[n][0]=inv(pwr(bs[0],n,mods[0]),mods[0]);
    invv[n][1]=inv(pwr(bs[1],n,mods[1]),mods[1]);
    for(int i=n-1;i>=0;i--)
    {
        for(int j=0;j<2;j++)
            invv[i][j]=invv[i+1][j]*bs[j]%mods[j];
    }
    s="#"+s;
    calchshprf();
    calchshsuf();
    for(int i=1;i<=n;i++)
    {
        ans[i]=max(ans[i],1ll);
        {
            int l=1,r=n,m;
            int ans=0;
            while(l<=r)
            {
                m=(l+r)/2;
                int odd_pl = i-m+1;
                int odd_pr = i+m-1;
                if(odd_pl<1 || odd_pr > n || getlrp(odd_pl,odd_pr)!=getlrs(odd_pl,odd_pr))r=m-1;
                else
                {
                    ans=max(ans,m);
                    l=m+1;
                }
            }
            if(ans)
                upd(1,n,i,i+ans-1,1,1);
        }
        {
            int l=1,r=n,m;
            int ans=0;
            //1 2 3 4
            while(l<=r)
            {
                m=(l+r)/2;
                int even_pl = i-m;
                int even_pr = i+m-1;
                if(even_pl<1 || even_pr > n || getlrp(even_pl,even_pr)!=getlrs(even_pl,even_pr))r=m-1;
                else
                {
                    ans=max(ans,m);
                    l=m+1;
                }
            }
            if(ans)
                upd(1,n,i,i+ans-1,2,1);
        }
    }
    for(int i=1;i<=n;i++)
    {
        cout<<ans[i]<<" ";
    }
    cout<<endl;

    return 0;
}
// https://cses.fi/problemset/hack/3138/entry/13342162/
// https://cses.fi/problemset/hack/3138/entry/14396378/
// https://cses.fi/problemset/hack/3138/entry/14634056/
// https://cses.fi/problemset/hack/3138/entry/15098735/
// https://cses.fi/problemset/hack/3138/entry/13203079/

// https://cses.fi/problemset/hack/3138/entry/14383452/
// good appraoch
