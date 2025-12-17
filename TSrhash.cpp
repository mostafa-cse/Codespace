#include <bits/stdc++.h>
using namespace std;
long long int bs = 31;
long long int mod = 1e9 + 7;
vector<long long int> pp;
vector<long long int> np;
long long int pow(long long int base, long long int exp)
{
    long long int ans = 1;
    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            ans *= base;
            ans %= mod;
        }
        exp /= 2;
        base *= base;
        base %= mod;
    }
    return ans % mod;
}
long long int modInv(long long int val)
{
    return pow(val, mod - 2) % mod;
}
long long int has(long long int l, long long int r, vector<long long int> &vec)
{
    long long int abhi = vec[r];
    abhi %= mod;
    if (l > 0)
    {
        abhi -= vec[l - 1];
        abhi += mod;
        abhi %= mod;
    }
    abhi *= np[l];
    abhi %= mod;
    return abhi;
}
int main()
{
    string s;
    cin >> s;
    cout << s.size() << endl;
    long long int n = s.size();
    pp.resize(n + 1, 1);
    np.resize(n + 1, 1);
    for (long long int i = 1; i <= n; i++)
    {
        pp[i] = pp[i - 1] * bs;
        pp[i] %= mod;
    }
    long long int inv = modInv(bs);
    // cout << inv << " yo " << endl;
    for (long long int i = 1; i <= n; i++)
    {
        np[i] = np[i - 1] * inv;
        np[i] %= mod;
    }
    string u = s;
    reverse(u.begin(), u.end());
    vector<long long int> shas(n, 0);
    vector<long long int> uhas(n, 0);
    shas[0] = (s[0] - 'a' + 1);
    for (long long int i = 1; i < n; i++)
    {
        shas[i] = (shas[i - 1]) + (s[i] - 'a' + 1) * pp[i];
        shas[i] %= mod;
    }
    uhas[0] = (u[0] - 'a' + 1);
    for (long long int i = 1; i < n; i++)
    {
        uhas[i] = (uhas[i - 1]) + (u[i] - 'a' + 1) * pp[i];
        uhas[i] %= mod;
    }
    vector<int> pakka(n, 1);
    for (int i = 0; i < n; i++)
    {
        // i is the centre
        // search odd pehle
        int lo = 0;
        //
        int hi = min(i, (int)(n - 1 - i));
        while (lo <= hi)
        {
            // len=2*mid+1
            int mid = (lo + hi) / 2;
            if (has(i - mid, i + mid, shas) == has(n - 1 - (i + mid), n - 1 - (i - mid), uhas))
            {
                lo = mid + 1;
                pakka[i + mid] = max(2 * mid + 1, pakka[i + mid]);
            }
            else
            {
                hi = mid - 1;
            }
        }
        // search even now
        lo = 1;
        //
        hi = min(i + 1, (int)(n - 1 - i));
        while (lo <= hi)
        {
            int mid = (lo + hi) / 2;
            if (has(i - mid + 1, i + mid, shas) == has(n - 1 - (i + mid), n - 1 - (i - mid + 1), uhas))
            {
                lo = mid + 1;
                pakka[i + mid] = max(2 * mid, pakka[i + mid]);
            }
            else
            {
                hi = mid - 1;
            }
        }
    }

    //
    // for (int i = (int)n - 1; i > 0; --i)
    // {
    //     pakka[i - 1] = max(pakka[i - 1], max(1, pakka[i] - 2));
    // }

    for (int i = 0; i < n; i++)
    {
        cout << pakka[i] << " ";
    }
    cout << endl;

    return 0;
}
// https://cses.fi/problemset/hack/3138/entry/12949636/
// https://cses.fi/problemset/hack/3138/entry/15191255/
// https://cses.fi/problemset/hack/3138/entry/14715856/
