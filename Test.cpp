#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

// --- Node Structure for Syntax Tree ---
struct Node {
    char value;             // Operator or Operand (a, b, ., |, *)
    int position;           // Leaf position (1, 2, 3...), 0 for internal nodes
    Node *left, *right;
    
    bool nullable;
    set<int> firstpos;
    set<int> lastpos;

    Node(char v) : value(v), position(0), left(nullptr), right(nullptr), nullable(false) {}
};

// Global map to store followpos for each position
map<int, set<int>> followpos_table;
// Map to link position number to actual character (e.g., 1 -> 'a')
map<int, char> pos_to_char;
int leaf_counter = 0;

// --- Helper: Check if character is an operand ---
bool isOperand(char c) {
    return isalnum(c) || c == '#';
}

// --- Step 1: Preprocessing (Add '.' and Convert to Postfix) ---

int getPrecedence(char c) {
    if (c == '*') return 3;
    if (c == '.') return 2;
    if (c == '|') return 1;
    return 0;
}

// Inserts explicit '.' concatenation operator
string addConcat(string regex) {
    string newRegex = "";
    for (size_t i = 0; i < regex.length(); i++) {
        char c1 = regex[i];
        newRegex += c1;
        if (i + 1 < regex.length()) {
            char c2 = regex[i + 1];
            // Cases where '.' should be inserted:
            // 1. Operand . Operand (a.b)
            // 2. Operand . (       (a.()
            // 3. * . Operand       (*.a)
            // 4. * . (             (*.()
            // 5. ) . Operand       ().a)
            // 6. ) . (             ().()
            bool c1_valid = isOperand(c1) || c1 == '*' || c1 == ')';
            bool c2_valid = isOperand(c2) || c2 == '(';
            if (c1_valid && c2_valid) {
                newRegex += '.';
            }
        }
    }
    return newRegex;
}

string infixToPostfix(string regex) {
    string postfix = "";
    stack<char> st;
    for (char c : regex) {
        if (isOperand(c)) {
            postfix += c;
        } else if (c == '(') {
            st.push(c);
        } else if (c == ')') {
            while (!st.empty() && st.top() != '(') {
                postfix += st.top();
                st.pop();
            }
            st.pop();
        } else {
            while (!st.empty() && getPrecedence(st.top()) >= getPrecedence(c)) {
                postfix += st.top();
                st.pop();
            }
            st.push(c);
        }
    }
    while (!st.empty()) {
        postfix += st.top();
        st.pop();
    }
    return postfix;
}

// --- Step 2 & 3: Build Tree and Calculate Attributes ---

void calculateFunctions(Node* node) {
    // 1. Leaf Node
    if (isOperand(node->value)) {
        node->nullable = false;
        node->firstpos = {node->position};
        node->lastpos = {node->position};
    }
    // 2. Or Node (|)
    else if (node->value == '|') {
        node->nullable = node->left->nullable || node->right->nullable;
        
        node->firstpos = node->left->firstpos;
        node->firstpos.insert(node->right->firstpos.begin(), node->right->firstpos.end());
        
        node->lastpos = node->left->lastpos;
        node->lastpos.insert(node->right->lastpos.begin(), node->right->lastpos.end());
    }
    // 3. Cat Node (.)
    else if (node->value == '.') {
        node->nullable = node->left->nullable && node->right->nullable;
        
        // Firstpos logic
        if (node->left->nullable) {
            node->firstpos = node->left->firstpos;
            node->firstpos.insert(node->right->firstpos.begin(), node->right->firstpos.end());
        } else {
            node->firstpos = node->left->firstpos;
        }
        
        // Lastpos logic
        if (node->right->nullable) {
            node->lastpos = node->left->lastpos;
            node->lastpos.insert(node->right->lastpos.begin(), node->right->lastpos.end());
        } else {
            node->lastpos = node->right->lastpos;
        }

        // --- Calculate Followpos for CAT Node ---
        // For every i in lastpos(c1), all positions in firstpos(c2) are in followpos(i)
        for (int i : node->left->lastpos) {
            followpos_table[i].insert(node->right->firstpos.begin(), node->right->firstpos.end());
        }
    }
    // 4. Star Node (*)
    else if (node->value == '*') {
        node->nullable = true;
        node->firstpos = node->left->firstpos;
        node->lastpos = node->left->lastpos;

        // --- Calculate Followpos for STAR Node ---
        // For every i in lastpos(n), all positions in firstpos(n) are in followpos(i)
        for (int i : node->lastpos) {
            followpos_table[i].insert(node->firstpos.begin(), node->firstpos.end());
        }
    }
}

Node* buildSyntaxTree(string postfix) {
    stack<Node*> st;
    leaf_counter = 0;
    
    for (char c : postfix) {
        Node* node = new Node(c);
        
        if (isOperand(c)) {
            leaf_counter++;
            node->position = leaf_counter;
            pos_to_char[leaf_counter] = c;
            calculateFunctions(node); // Calculate immediate attributes
            st.push(node);
        }
        else if (c == '*') {
            node->left = st.top(); st.pop();
            calculateFunctions(node);
            st.push(node);
        }
        else if (c == '.' || c == '|') {
            node->right = st.top(); st.pop();
            node->left = st.top(); st.pop();
            calculateFunctions(node);
            st.push(node);
        }
    }
    return st.top();
}

// --- Helper to print Sets ---
void printSet(set<int> s) {
    cout << "{ ";
    for (int i : s) cout << i << " ";
    cout << "}";
}

// --- Step 4: Generate DFA Table ---

void generateDFA(Node* root, set<char> inputs) {
    // State is represented by a set of positions
    vector<set<int>> d_states;
    map<set<int>, int> state_mapping; // Maps set -> State ID (0, 1, 2...)
    vector<bool> marked;
    
    // Transition Table: state_id -> (input_char -> next_state_id)
    map<int, map<char, int>> transitions;

    // Start State = firstpos of root
    set<int> start_state = root->firstpos;
    d_states.push_back(start_state);
    state_mapping[start_state] = 0;
    marked.push_back(false);
    
    int ptr = 0;
    while(ptr < d_states.size()) {
        set<int> S = d_states[ptr];
        marked[ptr] = true;
        int current_id = ptr;
        ptr++;

        // For each input symbol
        for(char a : inputs) {
            set<int> U; // New state
            
            // For each position p in S
            for(int p : S) {
                if(pos_to_char[p] == a) {
                    U.insert(followpos_table[p].begin(), followpos_table[p].end());
                }
            }
            
            if(!U.empty()) {
                // If U is not in d_states, add it
                if(state_mapping.find(U) == state_mapping.end()) {
                    state_mapping[U] = d_states.size();
                    d_states.push_back(U);
                    marked.push_back(false);
                }
                transitions[current_id][a] = state_mapping[U];
            }
        }
    }

    // --- Print DFA Table ---
    cout << "\n\n--- DFA Transition Table ---\n";
    cout << "State\t\t";
    for (char c : inputs) cout << c << "\t\t";
    cout << "\n------------------------------------\n";
    
    for (int i = 0; i < d_states.size(); i++) {
        // Check if accepting state (contains position of #)
        bool isFinal = false;
        for(int pos : d_states[i]) {
            if(pos_to_char[pos] == '#') isFinal = true;
        }
        
        cout << (isFinal ? "*" : " ") << "q" << i << "\t\t";
        
        for (char c : inputs) {
            if (transitions[i].find(c) != transitions[i].end()) {
                cout << "q" << transitions[i][c] << "\t\t";
            } else {
                cout << "-\t\t";
            }
        }
        cout << "\n";
    }
    
    // Print State Definitions
    cout << "\n--- State Definitions (Sets of Positions) ---\n";
    for (int i = 0; i < d_states.size(); i++) {
        cout << "q" << i << ": ";
        printSet(d_states[i]);
        cout << "\n";
    }
}

// --- Main Driver ---
int main() {
    string regex;
    cout << "Enter Regular Expression (e.g., (a|b)*abb): ";
    cin >> regex;

    // 1. Augment with #
    regex = "(" + regex + ")#";
    
    // 2. Add implicit concatenation
    string expandedRegex = addConcat(regex);
    cout << "\nExpanded Regex: " << expandedRegex;

    // 3. Convert to Postfix
    string postfix = infixToPostfix(expandedRegex);
    cout << "\nPostfix Regex:  " << postfix << "\n";

    // 4. Build Tree & Calculate Attributes
    Node* root = buildSyntaxTree(postfix);

    // 5. Print Followpos Table
    cout << "\n--- Followpos Table ---\n";
    cout << "Pos\tSymbol\tFollowpos\n";
    for (int i = 1; i <= leaf_counter; i++) {
        cout << i << "\t" << pos_to_char[i] << "\t";
        printSet(followpos_table[i]);
        cout << "\n";
    }

    // 6. Gather Input Symbols
    set<char> inputs;
    for(auto const& [pos, ch] : pos_to_char) {
        if(ch != '#') inputs.insert(ch);
    }

    // 7. Generate DFA
    generateDFA(root, inputs);

    return 0;
}