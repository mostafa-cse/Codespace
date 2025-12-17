#include <bits/stdc++.h>
#define F first
#define S second
#define pb push_back
#define pob pop_back
#define all(x) begin(x), end(x)
#define FOR(i,a,b) for (int i = (a); i <= (b); i++)
#ifdef LOCAL
#define HEHE freopen("in.txt", "r", stdin);
#define debug(HEHE...) std::cerr << "\e[1;31m" << #HEHE << "\033[0m" << " = ", dout(HEHE)
void dout() { std::cerr << "\n"; }
template <typename T, typename... U>
void dout(T t, U... u) { std::cerr << t << ' ', dout(u...); }
#else
#define HEHE ios_base::sync_with_stdio(0), cin.tie(0);
#define debug(...) 7122
#endif
using namespace std;

template<typename T1, typename T2>
bool chmin(T1 &a, T2 b) { return a > b ? a = b, 1 : 0; }
template<typename T1, typename T2>
bool chmax(T1 &a, T2 b) { return a < b ? a = b, 1 : 0; }

#define int long long

mt19937_64 seed (time (NULL));
#define rnd(l,r) uniform_int_distribution<long long> ((l), (r)) (seed)

#define ssize(x) (int)(x.size())

struct SparseTable {
    vector<vector<int>> st;
    void build(vector<int> v){
        int h = __lg(v.size());
        st.resize(h + 1);
        st[0] = v;
        for (int i = 1; i <= h; i++) {
            int gap = 1 << (i - 1);
            for (int j = 0; j + gap < ssize(st[i-1]); j++){
                st[i].push_back(min(st[i - 1][j], st[i - 1][j + gap]));
            }
        }
    }
    // 回傳 [ll, rr) 的最小值
    int query(int ll, int rr){
        int h = __lg(rr - ll);
        return min(st[h][ll], st[h][rr - (1 << h)]);
    }
};
struct SuffixArray {
    string s;
    vector<int> sa, lcp;

    // 69ced9
    SuffixArray(string _s, int lim = 256) {
        s = _s;
        int n = s.size() + 1, k = 0, a, b;
        vector<int> x(s.begin(), s.end()), y(n), ws(max(n, lim)), rank(n);
        x.push_back(0);
        sa = lcp = y;
        iota(sa.begin(), sa.end(), 0);
        for (int j = 0, p = 0; p < n; j = max(1ll, j * 2), lim = p) {
            p = j;
            iota(y.begin(), y.end(), n-j);
            for (int i = 0; i < n; i++) if (sa[i] >= j) y[p++] = sa[i] - j;
            fill(ws.begin(), ws.end(), 0);
            for (int i = 0; i < n; i++) ws[x[i]]++;
            for (int i = 1; i < lim; i++) ws[i] += ws[i - 1];
            for (int i = n; i--;) sa[--ws[x[y[i]]]] = y[i];
            swap(x, y), p = 1, x[sa[0]] = 0;
            for (int i = 1; i < n; i++) {
                a = sa[i - 1];
                b = sa[i];
                x[b] = (y[a] == y[b] && y[a + j] == y[b + j]) ? p - 1 : p++;
            }
        }

        for (int i = 1; i < n; i++) rank[sa[i]] = i;
        for (int i = 0, j; i < n - 1; lcp[rank[i++]] = k)
            for (k && k--, j = sa[rank[i] - 1]; i + k < ssize(s) && j + k < ssize(s) && s[i + k] == s[j + k]; k++);
        sa.erase(sa.begin());
        lcp.erase(lcp.begin(), lcp.begin()+2);
    }

    // f49583
    vector<int> pos; // pos[i] = i 這個值在 pos 的哪個地方
    SparseTable st;
    void init_lcp(){
        pos.resize(sa.size());
        for (int i = 0; i < ssize(sa); i++) {
            pos[sa[i]] = i;
        }
        if (lcp.size()) st.build(lcp);
    }

    int get_lcp(int l1, int r1, int l2, int r2) {
        int pos_1 = pos[l1], len_1 = r1-l1+1;
        int pos_2 = pos[l2], len_2 = r2-l2+1;
        if (pos_1 > pos_2) {
            swap(pos_1, pos_2);
            swap(len_1, len_2);
        }
        if (l1 == l2) return min(len_1, len_2);
        return min({st.query(pos_1, pos_2), len_1, len_2});
    }

    int substring_cmp(int l1, int r1, int l2, int r2){
        int len_1 = r1 - l1 + 1;
        int len_2 = r2 - l2 + 1;
        int res = get_lcp(l1, r1, l2, r2);
        if (res < len_1 && res < len_2) {
            return s[l1 + res] - s[l2 + res];
        }
        else if (len_1 == res && len_2 == res) {
            // 如果不需要以 index 作為次要排序參數，這裡要回傳 0
            return l1 - l2;
        }
        else {
            return len_1 == res ? -1 : 1;
        }
    }

    pair<vector<int>, vector<int>> get_left_and_right_lcp(int p){
        vector<int> pre(p + 1);
        vector<int> suf(p + 1);

        // build pre
        int now = 0;
        for (int i = 0; i < ssize(s); i++) {
            if (sa[i] <= p) {
                pre[sa[i]] = now;
                if (i < ssize(lcp)) now = min(now, lcp[i]);
            }
            else {
                if (i < ssize(lcp)) now = lcp[i];
            }
        }

        // build suf
        int now = 0;
        for (int i = ssize(s) - 1; i >= 0; i--) {
            if (sa[i] <= p) {
                suf[sa[i]] = now;
                if (i - 1 >= 0) now = min(now, lcp[i - 1]);
            } else{
                if (i - 1 >= 0) now = lcp[i - 1];
            }
        }
        return {pre, suf};
    }
};

void solve() {
    string s; cin >> s;
    string t;
    t += s[0];
    FOR (i, 1, ssize(s) - 1) {
        t += ('z' + 1);
        t += s[i];
    }
    s = t;
    int n = s.size();

    s += string(rbegin(s), rend(s));
    SuffixArray SA(s);
    SA.init_lcp();

    vector<int> len(n), ans(n);
    FOR (i, 0, n - 1) {
        len[i] = SA.get_lcp(i, n - 1, ssize(s) - i - 1, ssize(s) - 1);
    }
    int add = 0;
    multiset<int> ss;
    vector<vector<int>> del(n);
    FOR (i, 0, n - 1)  {
        ss.insert(1 - add);
        ans[i] = *ss.rbegin() + add;

        del[i + len[i] - 1].pb(1 - add);
        add++;
        for (int x : del[i]) ss.extract(x);
    }
    FOR (i, 0, n - 1) {
        if (i % 2 == 0) cout << ans[i] << ' ';
    }
    cout << '\n';
}

signed main() {
    HEHE
    int t = 1; // cin >> t;
    while (t--) solve();
}
