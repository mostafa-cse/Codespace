/**
 * Problem: For every index i in string S, find the length of the
 * longest palindrome ending at i.
 * Time Complexity: O(N) using Manacher's Algorithm
**/

#include <bits/stdc++.h>
using namespace std;
#ifndef ONLINE_JUDGE
#define dout(...) cerr << "Line:" << __LINE__ << " [" << #__VA_ARGS__ << "] = ["; _print(__VA_ARGS__)
#else
#define dout(...)
#endif
// Core Manacher's algorithm to calculate palindrome radii
vector<int> run_manacher(const vector<int>& s) {
    int n = s.size();
    vector<int> radius(n);
    int i = 0, j = 0;
    while (i < n) {
        while (i - j >= 0 && i + j < n && s[i - j] == s[i + j]) {
            j++;
        }
        radius[i] = j;
        int k = 1;
        while (i - k >= 0 && i + k < n && k + radius[i - k] < j) {
            radius[i + k] = radius[i - k];
            k++;
        }
        i += k;
        j -= k;
    }
    return radius;
}
int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    string s;
    cin >> s;

    int n = s.size();

    // Transform string to handle even/odd lengths uniformly
    // "aba" -> {a, -1, b, -1, a}
    vector<int> t;
    t.reserve(2 * n - 1);
    for (int i = 0; i < n - 1; ++i) {
        t.push_back(s[i]);
        t.push_back(-1); // Separator
    }
    t.push_back(s.back());
    dout(t);

    // Get palindrome radii for the transformed string
    vector<int> radii = run_manacher(t);

    // max_len[i] stores the length of the longest palindrome ending at index i
    vector<int> max_len(n, 1);

    // Map Manacher radii back to original string intervals
    for (int i = 0; i < t.size(); ++i) {
        int len_in_s; // Length of palindrome in original string
        int end_idx;  // End index in original string

        if (i % 2 == 0) {
            // Case: Center is a character (original index i/2)
            // radius in t includes separators.
            // Calculate span in original string:
            int w = (radii[i] - 1) / 2;
            int start = i / 2 - w;
            end_idx = i / 2 + w;
            len_in_s = end_idx - start + 1; // or 2*w + 1
        } else {
            // Case: Center is a separator
            int w = radii[i] / 2;
            if (w == 0) continue; // No palindrome around this separator
            int start = (i + 1) / 2 - w;
            end_idx = (i + 1) / 2 + w - 1;
            len_in_s = end_idx - start + 1; // or 2*w
        }

        // Update the maximum length ending at this specific end_idx
        if (end_idx < n) {
            max_len[end_idx] = max(max_len[end_idx], len_in_s);
        }
    }

    // Dynamic Programming propagation:
    // If a palindrome of length L ends at i, then a palindrome of length L-2 ends at i-1.
    // We iterate backwards to ensure all sub-palindromes are accounted for.
    for (int i = n - 2; i >= 0; i--) {
        max_len[i] = max(max_len[i], max_len[i + 1] - 2);
    }

    // Output results
    for (int i = 0; i < n; i++) {
        cout << max_len[i] << (i == n - 1 ? "" : " ");
    }
    cout << "\n";
    return 0;
}
