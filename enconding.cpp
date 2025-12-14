#include "enconding.h"
#include <string>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

struct Encoding::Node {
    char data;
    int freq;
    Node* left;
    Node* right;

    bool operator<(const Node& other) const {
        return freq < other.freq;
    }
};

Encoding::Encoding (string url) {}

void Encoding::dfs (Node* root, string code) {
    if (!root) return;

    if (!root->left && !root->right) {
        Encoding::grouped_mapping[root->data] = code; 
    }

    dfs(root->left, code + "0");
    dfs(root->right, code + "1");
}

void Encoding::build_huffman_tree () {
    priority_queue<Node*> pq;
    unordered_map<char, int> freq;

    for (auto& ch : Encoding::origin_url) {
        freq[ch]++;
    }

    for (auto& [key, val] : freq) {
        Node* node;
        node->data = key;
        node->freq = val;
        pq.push(node);
    }

    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();

        Node* node;
        node->freq = left->freq + right->freq;
        node->left = left;
        node->right = right;

        pq.push(node);
    }

    dfs(pq.top(), "");
}

string Encoding::encode () {
    string encoded_url = "";

    for (auto& ch : Encoding::origin_url) {
        encoded_url += Encoding::grouped_mapping[ch];
    }

    return encoded_url;
}