#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
using namespace std;

// Define the PCB structure (YOU MUST IMPLEMENT THIS)
struct PCB {
    // TODO: Add fields: pid, state, pc, total_work
    // TODO: Add constructor PCB
    int pid; 
    string state;
    int pc;
    int total_work;

    PCB(int p_id, int totalWork)
        : pid(p_id), state("Ready"), pc(0), total_work(totalWork) {}
};


// Print states of all processes, sorted by PID (PROVIDED - DO NOT MODIFY)
void printProcessStates(const std::vector<PCB>& pcbs, int timeSlice) {
    std::cout << "Interrupt " << timeSlice << ":" << std::endl;
    std::vector<PCB> sorted_pcbs = pcbs;
    std::sort(sorted_pcbs.begin(), sorted_pcbs.end(), 
              [](const PCB& a, const PCB& b) { return a.pid < b.pid; });
    for (const PCB& pcb : sorted_pcbs) {
        std::cout << "PID " << pcb.pid << ": " << pcb.state  << ", at pc " << pcb.pc << std::endl;
    }
    std::cout << std::flush;
}

// Kernel simulator (YOU MUST IMPLEMENT THIS)
void kernelSimulator(std::vector<PCB>& pcbs, int timeQuantum) {
    //Implement scheduling as decribed in the project 1 description
    //For each timed interrupt call printProcessStates
    //You may create helper functions as needed
    //Add comments to describe your implementation of this function and/or other helper functions
    
    queue<int> readyQueue;     
    /*ToDO place the proccesses in the ready queue
    PSEDO:
    Step 1: create empty ready queue
    


       */
    
    for (int i = 0; i < (int)pcbs.size(); i++) {  
        readyQueue.push(i);
    }
    int interruptcounter=1;

    while(true){
        bool terminated = true;
        for (size_t i = 0; i < pcbs.size(); ++i) {
            if (pcbs[i].state != "Terminated") {
                terminated = false;
                break;
            }
        }
        if (terminated) break;
        
        int next_process = readyQueue.front();
        readyQueue.pop();

        PCB& process = pcbs[next_process];
        if (process.state == "Terminated") {
            continue;
            
        } else {
            process.state = "Running";
            
            int cpuTime = min(timeQuantum, process.total_work - process.pc);
            process.pc += cpuTime;

            if (process.pc >= process.total_work) {
                process.state = "Terminated";
            } else {
                process.state = "Running";
            }
            printProcessStates(pcbs, interruptcounter++);
            if (process.state == "Running") {
                process.state = "Ready";
                readyQueue.push(next_process);
            }

        }
    }
}



int main() {
    // Read from standard input via redirection (PROVIDED)
    int numProcesses;
    if (!(std::cin >> numProcesses)) {
        std::cerr << "Error: Invalid input for number of processes" << std::endl;
        return 1;
    }
    if (numProcesses <= 0) {
        std::cerr << "Error: Invalid number of processes" << std::endl;
        return 1;
    }
    
    std::vector<PCB> pcbs;
    std::set<int> pids; // Track unique PIDs
    for (int i = 0; i < numProcesses; ++i) {
        int pid, work;
        if (!(std::cin >> pid >> work)) {
            std::cerr << "Error: Invalid input format for process data" << std::endl;
            return 1;
        }
        if (work <= 0) {
            std::cerr << "Error: Invalid work units for PID " << pid << std::endl;
            return 1;
        }
        if (pids.count(pid) > 0) {
            std::cerr << "Error: Duplicate PID " << pid << " detected" << std::endl;
            return 1;
        }
        // TODO: Add check for unique PIDs (e.g insert pid into the set pids)
        // TODO: Create PCB and add to pcbs (e.g., pcbs.emplace_back(pid, work))
        pids.insert(pid);
        pcbs.emplace_back(pid, work);
    }
    
    
    int timeQuantum = 2;
    kernelSimulator(pcbs, timeQuantum);
    
    std::cout << "All processes completed." << std::endl;
    return 0;
}
/*
clang++ -std=c++11 pcb_simulator.cpp -o pcb_simulator
./pcb_simulator < input.txt

clang++ -std=c++11 pcb_simulator.cpp -o pcb_simulator
./pcb_simulator < input5.txt
*/