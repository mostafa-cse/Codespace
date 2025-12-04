#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 1. Trie Node Structure
struct TrieNode {
    TrieNode* children[26]; // Array for 'a' through 'z'
    bool isEndOfWord;
    int value; // The data associated with the string

    // Constructor to Initialize Node
    TrieNode() {
        isEndOfWord = false;
        value = -1; // Default "empty" value
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};

class Trie {
private:
    TrieNode* root;

    // Helper for Deletion (Recursive)
    // Returns true if the current node can be deleted (has no children and not a word)
    bool _delete(TrieNode* current, string word, int depth) {
        if (!current) return false;

        // Base Case: If we have reached the end of the word
        if (depth == word.length()) {
            if (!current->isEndOfWord) return false; // Word doesn't exist

            // Unmark this node as a word end
            current->isEndOfWord = false;
            current->value = -1; // Reset value

            // If node has no other children, it is safe to delete it
            return isEmpty(current);
        }

        // Recursive Step: Go deeper
        int index = word[depth] - 'a';
        if (_delete(current->children[index], word, depth + 1)) {
            // If the child returned true, delete the child pointer
            delete current->children[index];
            current->children[index] = nullptr;

            // Check if current node is essentially empty (no children, not a word)
            // If so, return true to let the parent delete this node
            return !current->isEndOfWord && isEmpty(current);
        }

        return false;
    }

    // Helper to check if a node has any children
    bool isEmpty(TrieNode* node) {
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) return false;
        }
        return true;
    }

public:
    // E. Initiate (Constructor)
    Trie() {
        root = new TrieNode();
    }

    // C. Insert
    void insert(string word, int val) {
        TrieNode* node = root;
        for (char c : word) {
            int index = c - 'a';
            if (!node->children[index]) {
                node->children[index] = new TrieNode();
            }
            node = node->children[index];
        }
        node->isEndOfWord = true;
        node->value = val;
        cout << "Inserted: " << word << " [" << val << "]" << endl;
    }

    // A. Update
    void update(string word, int newVal) {
        TrieNode* node = root;
        for (char c : word) {
            int index = c - 'a';
            if (!node->children[index]) {
                cout << "Error: Word '" << word << "' not found. Cannot update." << endl;
                return;
            }
            node = node->children[index];
        }
        if (node->isEndOfWord) {
            node->value = newVal;
            cout << "Updated: " << word << " to [" << newVal << "]" << endl;
        } else {
            cout << "Error: '" << word << "' is a prefix, not a complete word." << endl;
        }
    }

    // D. Get
    int get(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int index = c - 'a';
            if (!node->children[index]) return -1; // Not found
            node = node->children[index];
        }
        if (node->isEndOfWord) {
            return node->value;
        }
        return -1; // Word exists as prefix but not a marked word
    }

    // B. Deletion
    void remove(string word) {
        if (_delete(root, word, 0)) {
            cout << "Deleted: " << word << endl;
        } else {
            // Note: _delete might succeed internally but return false if the root itself wasn't deleted
            // so we just print a confirmation for clarity here.
            cout << "Delete operation called on: " << word << endl;
        }
    }
};

// Main Driver
int main() {
    // E. Initiate
    Trie myTrie;

    // C. Insert
    myTrie.insert("apple", 100);
    myTrie.insert("app", 50);

    // D. Get
    cout << "Get 'apple': " << myTrie.get("apple") << endl;

    // A. Update
    myTrie.update("apple", 200);
    cout << "Get 'apple' after update: " << myTrie.get("apple") << endl;

    // B. Deletion
    myTrie.remove("apple");
    cout << "Get 'apple' after delete: " << myTrie.get("apple") << endl; // Should be -1
    cout << "Get 'app' after deleting apple: " << myTrie.get("app") << endl; // Should still be 50

    return 0;
}
