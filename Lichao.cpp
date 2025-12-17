#include <bits/stdc++.h>

using namespace std;
using ll = long long;

class LiChao {
    int n;
    struct Line {
        ll m, b;
        Line(ll _m = 0, ll _b = -1e9) : m(_m), b(_b) {}
        ll Calc(ll x) const {
            return 1LL * m * x + b;
        }
    };
    vector<Line> st;

    void Add(int posleft, int posright, Line line, int node, int left, int right) {
        if (posleft <= left && right <= posright) {
            int m = (left + right) / 2;
            bool sl = line.Calc(left) > st[node].Calc(left);
            bool sm = line.Calc(m) > st[node].Calc(m);
            if (sm)
                swap(st[node], line);
            if (left == right)
                return;
            if (sl ^ sm)
                Add(posleft, posright, line, 2 * node, left, m);
            else
                Add(posleft, posright, line, 2 * node + 1, m + 1, right);
            return;
        }
        int m = (left + right) / 2;
        if (posleft <= m)
            Add(posleft, posright, line, 2 * node, left, m);
        if (posright > m)
            Add(posleft, posright, line, 2 * node + 1, m + 1, right);
    }

    ll Get(int point, int node, int left, int right) {
        if (left == right)
            return st[node].Calc(point);
        int m = (left + right) / 2;
        ll ans = st[node].Calc(point);
        if (point <= m)
            ans = max(ans, Get(point, 2 * node, left, m));
        else
            ans = max(ans, Get(point, 2 * node + 1, m + 1, right));
        return ans;
    }

public:
    LiChao(int _n) : n(_n) {
        int s = 1;
        while (s < 2 * n)
            s *= 2;
        st.resize(s);
    }

    void InsertRange(const Line& line, int l, int r) {
        if (l > r)
            return;
        Add(l, r, line, 1, 1, n);
    }

    ll Get(int x) {
        return Get(x, 1, 1, n);
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    string s = "$", t;
    cin >> t;
    int n = t.size();
    LiChao ds(n + 1);
    for (auto x : t)
        (s += x) += '$';
    n = s.length();
    vector<int> dp(n);
    vector<pair<int, int>> lines;
    for (int i = 0, l = 0, r = 0; i < n; ++i) {
        int j = i > r ? 1 : min(dp[l + r - i], r - i + 1);
        while (0 <= i - j && i + j < n && s[i - j] == s[i + j])
            ++j;
        dp[i] = j--;
        if (i + j > r)
            r = i + j, l = i - j;
        int start, length;
        if (s[i] == '$') {
            start = (i - 1) / 2 - (dp[i] / 2) + 1, length = 2 * (dp[i] / 2);
        }
        else {
            start = (i - 1) / 2 - (dp[i] / 2) + 1, length = 2 * (dp[i] / 2) - 1;
        }
        // ababbababaa
        //  0 1 2 3 4 5 6 7 8 9 10
        // $a$b$a$b$b$a$b$a$b$a$a$
        // 0 2 4 6 8

        // even $=8
        // start=4
        // len
        // odd a=5
        //
        // cerr << "CENTER: " << (i - 1) / 2 << '\n';
        // cout << i << ' ' << start << ' ' << length << ' ' << start + length - 1 << '\n';
        if (s[i] == '$') {
            int s = i / 2, f = i / 2 + length / 2 - 1;
            ++s, ++f;
            ds.InsertRange({ 2,2 - 2 * s }, s, f);
            //     for (int j = i / 2;j < i / 2 + length / 2;++j) {
            //         lines.emplace_back(i / 2, i / 2 + length / 2 - 1);
            //         // ans[j] = max(ans[j], 2 * (j - i / 2 + 1));
            //         cerr << j << ' ';
            //     }
            // cerr << '\n';
        }
        else {
            int s = i / 2, f = i / 2 + length / 2;
            ++s, ++f;
            ds.InsertRange({ 2,1 - 2 * s }, s, f);

            // for (int j = i / 2;j <= i / 2 + length / 2;++j) {
            //     lines.emplace_back(i / 2, i / 2 + length / 2);
            //     // ans[j] = max(ans[j], 2 * (j - i / 2) + 1);
            //     cerr << j << ' ';
            // }
            // cerr << '\n';
        }
    }
    // 2 4 6 8 10
    // y=2(x-s)+2=2x+2-2s [s,f]

    // 1 3 5 7 9
    // y=2(x-s)+1=2x+1-2s [s,f]
    for (int i = 1;i <= t.size();++i)
        cout << ds.Get(i) << ' ';
}
// https://cses.fi/problemset/hack/3138/entry/12969469/
