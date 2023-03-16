#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <queue>

using namespace std;

struct Node{
    string data;
    int freq;
    int time;
    Node* leftChild;
    Node* rightChild;
    Node* parent;

    Node(char newData, int newFreq){
        string newString = "";
        newString += newData;
        data = newString;
        freq = newFreq;
        time = 0;
        leftChild = NULL;
        rightChild = NULL;
        parent = NULL;
    }

    Node(string newData, int newFreq){
        data = newData;
        freq = newFreq;
        time = 0;
        leftChild = NULL;
        rightChild = NULL;
        parent = NULL;
    }
};

//Functor to compare to node objects for priority queue
class Compare
{
public:
    bool operator() (const Node* p1, const Node* p2){
        if(p1->freq<p2->freq) return false;
        else if(p1->freq==p2->freq) {
            if((p1->data).length()==1 && (p2->data).length()==1){
                if(p1->data<p2->data) return false;
            }
            else if((p1->data).length()>1 && (p2->data).length()==1) return true;
            else if((p1->data).length()==1 && (p2->data).length()>1) return false;
            else {
                if(p1->time<p2->time) return false;
                return true;
            }
        }
        return true;
    }
};

class HuffmanEncoding
{
private:

    priority_queue<Node*, vector<Node*>, Compare> encodingTree; // Priority queue holding the nodes of the tree
    map<char, string> encodings; // Map between a character and its encoding for quick retrieval
    vector<Node*> encodingNode; // Hold pointers to the leaf nodes(single characters) for easy access

public:

    // Class Constructor
    HuffmanEncoding(const map<char, int>& characterFreq){
        for(auto it = characterFreq.begin(); it!=characterFreq.end(); it++){
            Node* newNode = new Node(it->first, it->second);
            encodingTree.push(newNode);
            encodingNode.push_back(newNode);
        }
    }

    void GenerateEncoding(); // Generates the tree for huffman encoding
    string getEncoding(char C) {return encodings[C];} // Returns the encoding of a character C
};

void HuffmanEncoding::GenerateEncoding(){
    int clock = 1; // Clock to maintain the order in which nodes are pushed into the queue.

    // Keep building the tree until there is only one node in the priority queue
    while(encodingTree.size()>1){
        // Get the top two nodes with the minimum frequency
        Node* newLeft = encodingTree.top();
        encodingTree.pop();
        Node* newRight = encodingTree.top();
        encodingTree.pop();

        // Create a new node with the popped nodes as children
        Node* newNode = new Node(newLeft->data + newRight->data, newLeft->freq + newRight->freq);
        newNode->leftChild = newLeft;
        newNode->rightChild = newRight;
        newNode->time = clock;
        newLeft->parent = newNode;
        newRight->parent = newNode;

        // Push the new node into the stack
        encodingTree.push(newNode);

        // Increment the clock
        clock++;
    }

    // Find the encoding for all characters are store it in a map
    for(int i=0; i<encodingNode.size(); i++)
    {
        char C = (encodingNode[i]->data)[0];
        Node* charNode = encodingNode[i];
        string charEncoding = "";

        // Keep moving up the tree until we hit the root
        while(charNode->parent!=NULL){
            char nextChar;
            Node* charParent = charNode->parent;

            // If the node is on the left of the parent, then the encoding bit to be added is 0 else it is 1
            if(charParent->leftChild==charNode) nextChar = '0';
            else nextChar = '1';
            charEncoding = nextChar + charEncoding;
            charNode = charNode->parent;
        }
        encodings[C] = charEncoding;
    }
}

int main()
{
    int streamSize, windowSize;
    cin >> streamSize >> windowSize;

    map<char, int> characterFreq; // Stores frequencies of the characters

    int numWindows = streamSize/windowSize + 1; // Calculate the number of windows
    int numCharactersRead = 0; // Maintain the number if characters read
    for(int i=0; i<numWindows; i++){
        string curWindow = ""; // Stores the window to be read

        // Read characters until we reach window size or hit the end of the input string
        for(int j=0; j<windowSize && numCharactersRead<streamSize; j++){
            char nextChar;
            cin >> nextChar;
            numCharactersRead++;
            curWindow = curWindow + nextChar;
            characterFreq[nextChar]++;
        }

        // Generate the encoding based on the updated character frequencies
        HuffmanEncoding windowEncoding(characterFreq);
        windowEncoding.GenerateEncoding();

        //Print the encoding for the window previously read
        for(auto it = curWindow.begin(); it!=curWindow.end(); it++){
            cout << windowEncoding.getEncoding(*it);
        }
    }
    return 0;
}