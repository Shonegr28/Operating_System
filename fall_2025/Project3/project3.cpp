// The code is made by Shone George Kutty Renjan, and was much easier than last semester's project!
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct BankerSystem{
    int Resource; // number of resources
    int Process; // number of processes
    vector<int> Available; // Available has one row so vector is enough Available[j]
    vector<vector<int> > Maximum;    // Maximum needs  rows and columns vector of vectors is needed (Maximum[i][j]
    vector<vector<int> > Allocation; // Allocation needs  rows and columns vector of vectors is needed (Allocation[i][j])
    vector<vector<int> > Need; // Need needs  rows and columns vector of vectors is needed (Need[i][j])

};

void ReadBankerSystem(BankerSystem &banker){ // Reads the input data for the BankerSystem
    string lineIgnore; // ignore the title lines
    cin >> lineIgnore; //ignore "Available"

    // Read the Available 
    banker.Available.resize(banker.Resource);  // available vector have size of Resource 
    for (int j = 0; j < banker.Resource; j++) { // loop through resources
        cin >> banker.Available[j]; 
    }

    cin >> lineIgnore; // "Max"

    // Read the Maximum
    banker.Maximum.resize(banker.Process, vector<int>(banker.Resource)); // maximum vector have size of Process x Resource
    for (int i = 0; i < banker.Process; i++) { // loop through processes
        for (int j = 0; j < banker.Resource; j++) { // loop through resources
           cin >> banker.Maximum[i][j]; 
        }
    }

    cin >> lineIgnore; // "Allocation"

    // Read the Allocation
    banker.Allocation.resize(banker.Process, vector<int>(banker.Resource)); // allocation vector have size of Process x Resource
    for (int i = 0; i < banker.Process; i++) { // loop through processes
        for (int j = 0; j < banker.Resource; j++) { // loop through resources
           cin >> banker.Allocation[i][j]; 
        }
    }

    // calculate the Need
    banker.Need.resize(banker.Process, vector<int>(banker.Resource)); // need vector have size of Process x Resource
    for (int i = 0; i < banker.Process; i++) { // loop through processes
        for (int j = 0; j < banker.Resource; j++) { // loop through resources
            banker.Need[i][j] = banker.Maximum[i][j] - banker.Allocation[i][j]; // need = maximum - allocation
        }
    }
};

bool checkSafe(BankerSystem &banker){ // Check if the system is safe or unsafe system
    
    vector<int> work = banker.Available;             // get the work which is available resource
    vector<int> determineProcess(banker.Process, 0); // set all processes to not determined

    int isSafe = 0; // the # of safe processes counted

    while (isSafe < banker.Process) { // loop until all processes are determined safe or no more can be found
        int processFound = 0; // check if a process is found this round

        for (int i = 0; i < banker.Process; i++) {
            if (determineProcess[i] == 0) { // this process not yet finished
                int process = 1;            // assume it can finish

                for (int j = 0; j < banker.Resource; j++) { // check the resources
                    if (banker.Need[i][j] > work[j]) { // need needs to more than work 
                        process = 0;        // this process cannot finish because not enough resources
                        break;
                    }
                }

                if (process == 1) { // if the process can finish
                
                    for (int j = 0; j < banker.Resource; j++) { // it loops through resources
                        work[j] = work[j] + banker.Allocation[i][j]; // release the resources
                    }
                    determineProcess[i] = 1; // mark this process as finished
                    processFound = 1; // found a process this round
                    isSafe++; // increment the safe process count
                }
            }
        }

        // if we did not find any process in this pass, break (unsafe) 
        if (processFound == 0) { 
            break;
        }
    }

    // if any process could not be finished, system is unsafe
    for (int i = 0; i < banker.Process; i++) { //
        if (determineProcess[i] == 0) {
            return 0;   
        }
    } 
    return 1; // system is safe
}



int main() {
    BankerSystem banker; // 
    char dummy;// reads Resources and Processes

    // 
    cin >> dummy >> banker.Resource; // gets the number of resources

    // 
    cin >> dummy >> banker.Process; // gets the number of processes

    ReadBankerSystem(banker); // reads the banker system data

    // Check initial state
    int currentState = checkSafe(banker); // check if the system is safe or unsafe
    if (currentState != 1) { // if unsafe
        cout << "The current system is in unsafe state." << endl; //prints this
        return 0;
    }

    string ProcessName;

    if (cin >> ProcessName) { 
        int processCount = 0; // count the number of processes
        if (ProcessName.size() > 1 && ProcessName[0] == 'P') { //  assign process number from input
            processCount = ProcessName[1] - '0';  // get the process number
        }

        vector<int> request(banker.Resource); //length of resources
        for (int j = 0; j < banker.Resource; j++) { // reads the request for each resource
            cin >> request[j];
        }
        
        // print this for before granting request
        cout << "Before granting the request of "  << ProcessName << ", the system is in safe state." << endl; 
        cout << "Simulating granting " << ProcessName << "'s request." << endl; 
        // Copy system and apply the request
        BankerSystem copySystem = banker; 
        for (int j = 0; j < copySystem.Resource; j++) { // loop through resources of the copied system
            copySystem.Available[j]                 -= request[j]; // give the resources to a process 
            copySystem.Allocation[processCount][j]  += request[j]; // process has more allocation
            copySystem.Need[processCount][j]        -= request[j]; // needs less resources
        }
        // prints new need matrix
        cout << "New Need" << endl; 
        for (int i = 0; i < copySystem.Process; i++) {
            for (int j = 0; j < copySystem.Resource; j++) {
                cout << copySystem.Need[i][j];
                if (j < copySystem.Resource - 1) cout << " ";
            }
            cout << endl;
        }

        int afterSafe = checkSafe(copySystem); // check if the new system is safe or unsafe after granting request 
        if (afterSafe == 1) { // if safe print this 
            cout << ProcessName << "'s request can be granted. The system will be in safe state." << endl;
        } else { // if not print this
            cout << ProcessName<< "'s request cannot be granted. The system will be in unsafe state." << endl;
        }
    } else {
        // no requests to process print this
        cout << "The current system is in safe state." << endl;
    }

    return 0;
}

/*
g++ -std=c++11 project3.cpp -o Project3
./Project3 < input.txt
*/
