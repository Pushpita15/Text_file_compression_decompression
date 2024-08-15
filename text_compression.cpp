#include<iostream>
#include<fstream> 
#include<bits/stdc++.h> 
using namespace std;




// function to initialize and create Min heap
struct Node {
    char character;
    int freq;
    Node *left, *right;

    Node(char c,int f):character(c),freq(f),left(NULL),right(NULL){}
};

//function to read contents from input file
vector<pair<char,int>> readFromFile(string filename, unordered_map<char,int> &freq)
{
    ifstream file(filename);
    char c;
    while(file.get(c))
    {
        freq[c]++;
    }
    file.close();

    vector<pair<char,int>> char_freq;
    for(auto i:freq)
    {
        char_freq.push_back({i.first,i.second});
    }
    return char_freq;
}
// function to compare two nodes
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// function to create huffmann tree
Node* createHuffmanTree(vector<pair<char,int>> char_freq)
{
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for(auto i:char_freq)
    {
        pq.push(new Node(i.first,i.second));
    }

    Node *root = new Node('$',0);
    while(pq.size() != 1)
    {
        Node* left = pq.top();
        pq.pop();
        // Node* left = left;
        Node* right = pq.top();
        pq.pop();
        // Node* right = right;

        Node* parent = new Node('$',left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    return pq.top();
}

// function to create huffmann code

void createHuffmanCode(Node* root, string code, unordered_map<char,string> &huffmanCode)
{
    if(root == NULL)
    {
        return;
    }
    if(root->character != '$')
    {
        huffmanCode[root->character] = code;
    }
    createHuffmanCode(root->left,code + "0",huffmanCode);
    createHuffmanCode(root->right,code + "1",huffmanCode);
}


// function to write huffman code to output file
void writeToFile(string filename, unordered_map<char,string> huffmanCode)
{
    ofstream file(filename);
    for(auto i:huffmanCode)
    {
        file<<i.first<<" "<<i.second<<endl;
    }
    file.close();
}
void writeBits(ofstream &out, const string &bitString) {
    static int bitBuffer = 0;
    static int bitCount = 0;

    for (char bit : bitString) {
        bitBuffer = (bitBuffer << 1) | (bit - '0');
        bitCount++;

        if (bitCount == 8) {
            out.put(static_cast<char>(bitBuffer));
            bitBuffer = 0;
            bitCount = 0;
        }
    }
}

// Flush remaining bits in the buffer
void flushBits(ofstream &out) {
    static int bitBuffer = 0;
    static int bitCount = 0;

    if (bitCount > 0) {
        bitBuffer <<= (8 - bitCount);
        out.put(static_cast<char>(bitBuffer));
        bitBuffer = 0;
        bitCount = 0;
    }
}
//function to substitute huffman code in place of characters
void substituteHuffmanCode(const string &filename, unordered_map<char, string> &huffmanCode) {
    ifstream file(filename, ios::in);
    ofstream temp("compressed.bin", ios::binary);

    char c;
    while (file.get(c)) {
        writeBits(temp, huffmanCode[c]);
    }

    flushBits(temp);

    file.close();
    temp.close();
}

//function to decompress the file
void decompressFile(const string &filename, Node *root) {
    ifstream file(filename, ios::binary);
    ofstream temp("decompressed.txt");

    Node *curr = root;
    char c;
    while (file.get(c)) {
        for (int i = 7; i >= 0; i--) {
            if (c & (1 << i)) {
                curr = curr->right;
            } else {
                curr = curr->left;
            }

            if (curr->left == nullptr && curr->right == nullptr) {
                temp << curr->character;
                curr = root;
            }
        }
    }

    file.close();
    temp.close();
}
int main()
{
    string filename = "input.txt";
    unordered_map<char,int> freq;
    vector<pair<char,int>> char_freq = readFromFile(filename,freq);
    Node* root = createHuffmanTree(char_freq);
    unordered_map<char,string> huffmanCode;
    createHuffmanCode(root,"",huffmanCode);
    writeToFile("output.txt",huffmanCode);
    substituteHuffmanCode(filename,huffmanCode);
    //print size of input file
    ifstream file(filename, ios::binary | ios::ate);
    cout<<"Size of input file: "<<file.tellg()<<" bytes"<<endl;
    //print size of output file
    ifstream file1("compressed.bin", ios::binary | ios::ate);
    cout<<"Size of output file: "<<file1.tellg()<<" bytes"<<endl;

    decompressFile("compressed.bin", root);
    return 0;
}




