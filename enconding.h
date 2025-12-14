#pragma once

#include <string>
#include <unordered_map>
using namespace std;

class Encoding {
public:
    struct Node;
    string origin_url;
    unordered_map<char, string> grouped_mapping;

    Encoding (string url);

    void dfs (Node* root, string code);

    void build_huffman_tree ();

    string encode ();
};