#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cstring>

using namespace std;

// Maximum total characters in all patterns combined
const int MAXN = 100005; 
const int ALPHABET = 26;

struct AhoCorasick {
    // trie[u][c] stores the next node index. 
    // If edge exists, it points to child. If not, it points to failure state (DFA optimization).
    int trie[MAXN][ALPHABET];
    
    // fail[u] is the suffix link
    int fail[MAXN];
    
    // count[u] stores how many patterns end exactly at node u
    int end_count[MAXN];
    
    int sz; // Node count

    void init() {
        memset(trie, 0, sizeof(trie));
        memset(fail, 0, sizeof(fail));
        memset(end_count, 0, sizeof(end_count));
        sz = 1; // Start with root at index 0 (or 1 depending on preference, here 0 is root)
    }

    // 1. Insert patterns into the Trie
    void insert(const string& s) {
        int u = 0;
        for (char c : s) {
            int next_char = c - 'a';
            if (!trie[u][next_char]) {
                trie[u][next_char] = sz++;
            }
            u = trie[u][next_char];
        }
        end_count[u]++;
    }

    // 2. Build Failure Links (BFS) and Optimize Transitions
    void build() {
        queue<int> q;
        
        // Push children of root (depth 1) to queue
        // Their failure link is 0 (root)
        for (int i = 0; i < ALPHABET; i++) {
            if (trie[0][i]) {
                q.push(trie[0][i]);
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int i = 0; i < ALPHABET; i++) {
                if (trie[u][i]) {
                    // Case 1: Real edge exists
                    // The fail link of the child is: 
                    // jump to u's fail node and take character 'i'
                    fail[trie[u][i]] = trie[fail[u]][i];
                    
                    // Merge info: If the fail node is an end of a word, 
                    // the current node effectively contains that word as a suffix.
                    // (Optional: depends on problem. Here we merge counts).
                    end_count[trie[u][i]] += end_count[fail[trie[u][i]]];
                    
                    q.push(trie[u][i]);
                } else {
                    // Case 2: No real edge (Optimization)
                    // Point this missing edge to the destination of the fail node
                    // This creates the "Automaton" structure (O(1) transition)
                    trie[u][i] = trie[fail[u]][i];
                }
            }
        }
    }

    // 3. Query Text
    int query(const string& text) {
        int u = 0;
        int total_matches = 0;
        
        for (char c : text) {
            int next_char = c - 'a';
            // Because of our optimization in build(), we just move!
            // No while loops to traverse failure links needed here.
            u = trie[u][next_char];
            
            total_matches += end_count[u];
        }
        return total_matches;
    }
};

int main() {
    AhoCorasick ac;
    ac.init();

    // Patterns
    ac.insert("he");
    ac.insert("she");
    ac.insert("his");
    ac.insert("hers");

    // Build the automaton
    ac.build();

    string text = "ahishers";
    cout << "Text: " << text << endl;
    
    // Search
    // Matches: "his" (at index 1..3), "he" (at index 4..5), "hers" (at index 4..7)
    // Note: "he" is a suffix of "she" (not in text) but also a prefix of "hers".
    // "hers" contains "he".
    
    cout << "Total occurrences found: " << ac.query(text) << endl;

    return 0;
}
