// The code is made by Shone George Kutty Renjan and project seems easy
#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

struct BankerSystem{
    int Resource;
    int Process;
    vector<int> Available; // Available has one row so vector is enough Available[j]
    vector<vector<int> > Maximum;    // Maximum needs  rows and columns vector of vectors is needed (Maximum[i][j]
    vector<vector<int> > Allocation; // Allocation needs  rows and columns vector of vectors is needed (Allocation[i][j])
    vector<vector<int> > Need; // Need needs  rows and columns vector of vectors is needed (Need[i][j])
};

void ReadBankerSystem(BankerSystem &banker){ 

    //Read the Available
    banker.Available.resize(banker.Resource);
    for(int j=0;j<banker.Resource;j++){
        cin>> banker.Available[j];
    }

    //Read the Maximum
    banker.Maximum.resize(banker.Process,vector<int>(banker.Resource));
    for(int i=0;i<banker.Process;i++){
        for(int j=0;j<banker.Resource;j++){
           cin >> banker.Maximum[i][j];
        }

    }

    //Read the Allocation
    banker.Allocation.resize(banker.Process,vector<int>(banker.Resource));
    for(int i=0;i<banker.Process;i++){
        for(int j=0;j<banker.Resource;j++){
           cin >> banker.Allocation[i][j];
        }
    }

    // calculate the Need
    banker.Need.resize(banker.Process,vector<int>(banker.Resource));
    for(int i=0;i<banker.Process;i++){
        for(int j=0;j<banker.Resource;j++){
            banker.Need[i][j]=banker.Maximum[i][j]-banker.Allocation[i][j];
            cout << banker.Need[i][j] << " ";
        }

        // Ensure output is flushed so the matrix appears immediately in the terminal
        cout << flush;
    }
};



int main() {
    BankerSystem banker;
    cin >> banker.Resource >> banker.Process;   // e.g., 3 5
    ReadBankerSystem(banker);
    return 0;
}
// g++ -std=c++11 project3.cpp -o project3
// ./project3 < input.txt
