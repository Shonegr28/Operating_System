// The code is made by Shone George Kutty Renjan, and was much easier than last semester's project!
#include <iostream>
#include <vector>
#include <string>
using namespace std;

/* 
Read the frame and reference string from the input
store the number of frames 
Print the reference string



*/

struct LRU{
    int numFrames;        // number of frames
    vector<int> frames;  // current frames in memory
    vector<int> pages;    // reference string of pages
    vector<int> trackPage; // to track the last used time of pages
    int faults;           // number of page faults
};

// Read LRU data from standard input
void ReadFramesAndRefs(LRU &lru) {
    string label;  // will hold "Frames"
    if (!(cin >> label >> lru.numFrames)) {
        return;    // no input or bad format
    }
    int page;
    while (cin >> page) { // read all remaining ints as pages
        lru.pages.push_back(page); 
    }
}

void PrintLRU(const LRU &lru) {
    for (size_t i = 0; i < lru.pages.size(); ++i) {
        cout << lru.pages[i];
        if (i + 1 < lru.pages.size()) cout << ' ';
    }
    cout << "\n";
}

void ReplacePagesLRU(LRU &lru) {
        cout << "Page replacement using LRU\n";


    
}






int main() {
    LRU lru;
    ReadFramesAndRefs(lru);
    PrintLRU(lru);
    ReplacePagesLRU(lru);




    return 0;
}

/*
Compile:
    g++ -std=c++11 project3.cpp -o Project3

Run:
    ./Project3 < input1.txt
*/
