#include <bits/stdc++.h>
using namespace std;

const int MAXN = 300005;

struct Node {
    int start, end;      // Start and end positions in original string
    int length;          // Length of the palindrome
    int insertEdg[26];   // Insertion edges for characters a-z
    int suffixEdg;       // Suffix link (fail pointer)
};

Node root1, root2;
Node tree[MAXN];
int currNode;
string s;
int ptr;

void insert(int idx) {
    // STEP 1: Find node X such that s[idx] X s[idx] is maximum palindrome
    int tmp = currNode;
    while (true) {
        int curLength = tree[tmp].length;
        if (idx - curLength >= 1 && s[idx] == s[idx - curLength - 1])
            break;
        tmp = tree[tmp].suffixEdg;
    }

    // STEP 2: Check if s[idx] X s[idx] already exists
    if (tree[tmp].insertEdg[s[idx] - 'a'] != 0) {
        currNode = tree[tmp].insertEdg[s[idx] - 'a'];
        return;
    }

    // STEP 3: Create new node
    ptr++;
    tree[tmp].insertEdg[s[idx] - 'a'] = ptr;
    tree[ptr].length = tree[tmp].length + 2;
    tree[ptr].end = idx;
    tree[ptr].start = idx - tree[ptr].length + 1;

    // STEP 4: Set suffix link
    tmp = tree[tmp].suffixEdg;
    currNode = ptr;

    if (tree[currNode].length == 1) {
        tree[currNode].suffixEdg = 2;  // Points to even root
        return;
    }

    // Find node Y for suffix link
    while (true) {
        int curLength = tree[tmp].length;
        if (idx - curLength >= 1 && s[idx] == s[idx - curLength - 1])
            break;
        tmp = tree[tmp].suffixEdg;
    }

    tree[currNode].suffixEdg = tree[tmp].insertEdg[s[idx] - 'a'];
}

int main() {
    // Initialize tree
    root1.length = -1;
    root1.suffixEdg = 1;
    root2.length = 0;
    root2.suffixEdg = 1;

    tree[1] = root1;
    tree[2] = root2;
    ptr = 2;
    currNode = 1;

    // Read string
    cin >> s;
    int l = s.length();

    // Insert each character
    for (int i = 0; i < l; i++)
        insert(i);

    // Print all distinct palindromic substrings
    cout << "All distinct palindromic substrings for " << s << ":\n";
    for (int i = 3; i <= ptr; i++) {
        cout << (i - 2) << ") ";
        for (int j = tree[i].start; j <= tree[i].end; j++)
            cout << s[j];
        cout << "\n";
    }

    return 0;
}
