// The code is made by Shone George Kutty Renjan, and was much easier than last semester's project!
#include <iostream>
#include <vector>
#include <string>
using namespace std;

/*

My Program Logic:
Read the frame and reference string from the input
store the number of frames 
Print the reference string

Replace pages using LRU page replacement algorithm
step 1: through each page in the reference string
step 2: get the first page from the reference string
step 3: check if the page is already in the current frames
step 4: if the page is not in the current frame push it to the frame
step 5: if reach the number of frames, find the least recently used page and replace it with the new page
step 6: keep track of the last used time of each page in the frame
step 7: print the page replacement process and the number of page faults
*/


struct LRU{
    int numFrames;        // number of frames
    vector<int> frames;  // current frames in memory
    vector<int> pages;    // reference string of pages
    vector<int> trackPage; // to track the last used time of pages
    int faults =0;           // number of page faults
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

void PrintLRU( LRU &lru) {
    for (size_t i = 0; i < lru.pages.size(); ++i) {
        cout << lru.pages[i];
        if (i + 1 < lru.pages.size()) cout << ' ';
    }
    cout << "\n";
}

void ReplacePages(LRU &lru) {
    // current pages in frames, -1 means empty
    vector<int> frames(lru.numFrames, -1);
    // last used time (index in reference string) for each frame
    vector<int> lastUsed(lru.numFrames, -1);

    lru.faults = 0;

    // step 1: go through each page in the reference string
    for (int time = 0; time < (int)lru.pages.size(); ++time) {
        int page = lru.pages[time];        // step 2: get the current page
        bool hit = false;
        int hitIndex = -1;

        // step 3: check if the page is already in the frames
        for (int i = 0; i < lru.numFrames; ++i) {
            if (frames[i] == page) {
                hit = true;
                hitIndex = i;
                break;
            }
        }

        if (hit) {
            // step 6: update last used time
            lastUsed[hitIndex] = time;
        } else {
            // page fault
            lru.faults++;

            // step 4: if there's an empty frame, use it
            int emptyIndex = -1;
            for (int i = 0; i < lru.numFrames; ++i) {
                if (frames[i] == -1) {
                    emptyIndex = i;
                    break;
                }
            }

            if (emptyIndex != -1) {
                // place page in empty frame
                frames[emptyIndex] = page;
                lastUsed[emptyIndex] = time;
            } else {
                // step 5: find least recently used page and replace it
                int lruIndex = 0;
                for (int i = 1; i < lru.numFrames; ++i) {
                    if (lastUsed[i] < lastUsed[lruIndex]) {
                        lruIndex = i;
                    }
                }
                frames[lruIndex] = page;
                lastUsed[lruIndex] = time;
            }
        }

        // step 7: print the page replacement process
        cout << "After accessing " << page << " : ";
        for (int i = 0; i < lru.numFrames; ++i) {
            if (frames[i] == -1) cout << "- ";
            else cout << frames[i] << " ";
        }
        
    }

    // step 7 (continued): print number of page faults
    cout << "Total page faults = " << lru.faults << endl;
}




int main() {
    LRU lru;
    ReadFramesAndRefs(lru);
    PrintLRU(lru);
    ReplacePages(lru);



    return 0;
}

/*
Compile:
    g++ -std=c++11 project3.cpp -o Project3

Run:
    ./Project3 < input1.txt
*/
