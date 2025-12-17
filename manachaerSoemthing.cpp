#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ld long double
using namespace std;
using namespace __gnu_pbds;
template <class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
const int N = 1e5 + 1,mod=1e9+7,p=31;
int inv_mod(int A, ll M)
{
    int m0 = M;
    int y = 0, r = 1;
    if (M == 1)
        return 0;
    while (A > 1)
    {
        int q = A / M;
        int t = M;
        M = A % M, A = t;
        t = y;
        y = r - q * y;
        r = t;
    }
    if (r < 0)
        r += m0;
    return r;
}

class hashed {
public:
    int n;
    vector<int> hashpref1, hashpref2, invmod1, invmod2;
    int mod1, mod2, p;

    hashed(string s, char first_letter = 'a', int mod11 = 1e9 + 7, int mod22 = 1e9 + 9, int pp = 31) {
        mod1 = mod11;
        mod2 = mod22;
        p = pp;
        n = s.size();
        hashpref1.resize(n);
        hashpref2.resize(n);
        invmod1.resize(n);
        invmod2.resize(n);

        int powerp1 = p, powerp2 = p;
        int inv1 = inv_mod(p, mod1), inv2 = inv_mod(p, mod2);
        hashpref1[0] = hashpref2[0] = s[0] - first_letter + 1;
        invmod1[0] = invmod2[0] = 1;

        for(int i = 1; i < n; i++) {
            hashpref1[i] = (hashpref1[i-1] + (int)((1LL * (s[i] - first_letter + 1) * powerp1) % mod1)) % mod1;
            hashpref2[i] = (hashpref2[i-1] + (int)((1LL * (s[i] - first_letter + 1) * powerp2) % mod2)) % mod2;
            powerp1 = (int)((1LL * powerp1 * p) % mod1);
            powerp2 = (int)((1LL * powerp2 * p) % mod2);
            invmod1[i] = (int)((1LL * invmod1[i-1] * inv1) % mod1);
            invmod2[i] = (int)((1LL * invmod2[i-1] * inv2) % mod2);
        }
    }

    pair<int, int> get_value(int l, int r) {
        if(l > r) return {0, 0};
        int val1 = (int)((1LL * ((hashpref1[r] - (l > 0 ? hashpref1[l-1] : 0) + mod1) % mod1) * invmod1[l]) % mod1);
        int val2 = (int)((1LL * ((hashpref2[r] - (l > 0 ? hashpref2[l-1] : 0) + mod2) % mod2) * invmod2[l]) % mod2);
        return {val1, val2};
    }
};

int longest_common_prefix(hashed &t1, int starting1, hashed &t2, int starting2, int max_size)
{
    int l = 0, r = max_size;
    while (l < r)
    {
        int m = (l + r + 1) / 2;
        if (t1.get_value(starting1, starting1 + m - 1) == t2.get_value(starting2, starting2 + m - 1))
            l = m;
        else
            r = m - 1;
    }
    return l;
}
class Segment_tree
{
public:
    vector<int> t;
    vector<int> lazy;
    int n, neutral;
    int merge(int x, int y)
    {
        return max(x,y);
    }
    void build(vector<int> &a, int v, int tl, int tr)
    {
        if (tl == tr)
            t[v] = a[tl];
        else
        {
            int tm = (tl + tr) / 2;
            build(a, 2 * v, tl, tm);
            build(a, 2 * v + 1, tm + 1, tr);
            t[v] = merge(t[2 * v], t[2 * v + 1]);
        }
    }
    void build(vector<int> &a)
    {
        build(a, 1, 0, n - 1);
    }
    Segment_tree(int sz)
    {
        n = sz;
        neutral = 0;
        t.resize(4 * n, 0);
        lazy.resize(4 * n,1e9);
        vector<int> a(n, 0);
        build(a);
    }
    void push(int v, int tl, int tr)
    {
        if (lazy[v]!=1e9)
        {
            if (tl != tr)
            {
                int tm=(tl+tr)/2;
                lazy[2 * v] = min(lazy[2*v], lazy[v]);
                lazy[2 * v + 1] = min(lazy[2*v+1], lazy[v]);
            }
            t[v] = max(t[v],tr-lazy[v]);
            lazy[v] = 1e9;
        }
    }
    int get_range(int l, int r, int v, int tl, int tr)
    {
        push(v, tl, tr);
        if (l > tr || r < tl)
            return neutral;
        if (l <= tl && tr <= r)
            return t[v];
        int tm = (tl + tr) / 2;
        return merge(get_range(l, r, 2 * v, tl, tm), get_range(l, r, 2 * v + 1, tm + 1, tr));
    }
    int get_range(int l, int r)
    {
        return get_range(l, r, 1, 0, n - 1);
    }
    void update(int pos, int new_val, int v, int tl, int tr)
    {
        push(v, tl, tr);
        if (tl == tr)
        {
            t[v] = new_val;
            return;
        }
        int tm = (tl + tr) / 2;
        if (pos <= tm)
            update(pos, new_val, 2 * v, tl, tm);
        else
            update(pos, new_val, 2 * v + 1, tm + 1, tr);
        t[v] = merge(t[2 * v], t[2 * v + 1]);
    }
    void update(int pos, int new_val)
    {
        update(pos, new_val, 1, 0, n - 1);
    }
    int get(int pos, int v, int tl, int tr)
    {
        push(v, tl, tr);
        if (tl == tr)
        {
            return t[v];
        }
        int tm = (tl + tr) / 2;
        ll val;
        if (pos <= tm)
            val=get(pos, 2 * v, tl, tm);
        else
            val=get(pos, 2 * v + 1, tm + 1, tr);
        t[v] = merge(t[2 * v], t[2 * v + 1]);
        return val;
    }
    int get(int pos)
    {
        return get(pos, 1, 0, n - 1);
    }
    void update_range(int l, int r, int val, int v, int tl, int tr)
    {
        push(v, tl, tr);
        if (l > tr || r < tl)
            return;
        if (l <= tl && tr <= r)
        {
            lazy[v] = val;
            push(v,tl,tr);
            return;
        }
        int tm = (tl + tr) / 2;
        update_range(l, r, val, 2 * v, tl, tm);
        update_range(l, r, val, 2 * v + 1, tm + 1, tr);
        t[v] = merge(t[2 * v], t[2 * v + 1]);
    }
    void update_range(int l, int r, int val)
    {
        update_range(l, r, val, 1, 0, n - 1);
    }
};
void solve()
{
    string s;
    cin>>s;
    int n=s.size();
    hashed hash(s);
    reverse(s.begin(),s.end());
    hashed hash_rev(s);
    reverse(s.begin(),s.end());
    Segment_tree todd(n),teven(n);
    // odd length palindromes
    for(int i=0;i<n;i++){
        int l=0,r=min(i,n-i-1);
        while(l<r){
            int mid=(l+r+1)/2;
            if(hash.get_value(i-mid,i+mid)==hash_rev.get_value(n-i-1-mid,n-1-i+mid)) l=mid;
            else r=mid-1;
        }
        todd.update_range(i,i+l,i);
    }
    // even length palindromes
    for(int i=0;i<n-1;i++){
        if(s[i]!=s[i+1]) continue;
        int l=0,r=min(i,n-i-2);
        while(l<r){
            int mid=(l+r+1)/2;
            if(hash.get_value(i-mid,i+mid+1)==hash_rev.get_value(n-i-2-mid,n-1-i+mid)) l=mid;
            else r=mid-1;
        }
        teven.update_range(i+1,i+1+r,i);
    }
    for(int i=0;i<n;i++){
        cout<<max(2*teven.get(i),2*todd.get(i)+1)<<" ";
    }
}
int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int t = 1;
    //cin >> t;
    while (t--)
        solve();
    return 0;
}

// https://cses.fi/problemset/hack/3138/entry/13355161/
