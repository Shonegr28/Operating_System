// The code is made by Shone George Kutty Renjan and project seems easy
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

// Structure to represent a process control block
struct PCB {
    string id;
    int priority;
    int burst_time;
    int arrival_time;
    int remaining_time;
    // feel free to add more variables here
    int last_run_time;  // Time left in current quantum
    int completion_time; // Time when process completes
    int remaining_quantum; // Time when process first starts running
};

int main() {
    string line;
    char dummy;
    int time_quantum;
    vector<PCB> processes;

    // Read time quantum from standard input
    cin >> dummy >> time_quantum;

    // Read processes from standard input
    while (cin >> line) {
        PCB p;
        p.id = line;
        cin >> p.priority >> p.burst_time >> p.arrival_time;
        p.remaining_time = p.burst_time;
        p.last_run_time = time_quantum;
        p.last_run_time = -1;
        p.remaining_quantum = time_quantum;
        processes.push_back(p);
    }

    // TODO: Create CPU scheduler simulator
    
    // You can create any data structures, classes, functions helpers as you wish
    // Do not forget to include comments describing how your simulator works.
    map<int, queue<int>, greater<int>> readyQueues; // ready queues for each priority level


    // variables we need for the simulator
    int current_time = 0;   // [e]
    int total_cpu_time = 0;
    int finished_count = 0;
    int n = processes.size();
    int running_process_idx = -1; // -1 means nothing is running
    
    // these variables help us keep track of what to print
    int interval_start = 0;
    string interval_id = "";
    int interval_priority = -1;

    // keep looping untill all process are done
    while (finished_count < n) {
        
        // check every single process to see if it arrives now
        // i know this isnt the fastest way but it works fine
        for(int i=0; i<n; i++) {
            if (processes[i].arrival_time == current_time) {
                // add the process to its priority queue
                readyQueues[processes[i].priority].push(i);
            }
        }

        // check if we need to interupt the current process
        // this happens when a higher priority process is waiting
        if (running_process_idx != -1) {
            // look at all the ready queues to find if any have higher priority
            bool need_to_switch = false;
            if (!readyQueues.empty()) {
                // the map gives us the biggest priority first
                int top_waiting_priority = readyQueues.begin()->first;
                // compare with whats running now
                if (top_waiting_priority > processes[running_process_idx].priority) {
                    need_to_switch = true;
                }
            }
            
            // do the switch if we found a higher priority
            if (need_to_switch) {
                // save what we were running before
                int old_process = running_process_idx;
                int old_priority = processes[old_process].priority;
                
                // print out what just happened
                if (interval_id != "") {
                    cout << "Time " << interval_start << "-" << current_time << ": " << interval_id << " (Priority " << interval_priority << ")" << endl;
                }
                
                // put the old process back so it can run later
                readyQueues[old_priority].push(old_process);
                
                // nothing is running anymore
                running_process_idx = -1;
                interval_start = current_time;
                interval_id = "";
            }
        }

        // pick a process to run if nothing is currently executing
        if (running_process_idx == -1) {
            // see if any processes are waiting
            bool found_process = false;
            int selected_process = -1;
            int selected_priority = -1;
            
            if (!readyQueues.empty()) {
                // grab the priority level with most importance
                selected_priority = readyQueues.begin()->first;
                queue<int> &waiting = readyQueues[selected_priority];
                selected_process = waiting.front();
                waiting.pop();
                found_process = true;
                
                // cleanup the map if that queue is empty now
                if (waiting.empty()) {
                    readyQueues.erase(selected_priority);
                }
            }
            
            // now handle what we found
            if (found_process) {
                // assign it to the cpu
                running_process_idx = selected_process;
                processes[selected_process].last_run_time = time_quantum;
                
                // check if we need to print anything
                string switching_to = processes[selected_process].id;
                int switching_priority = processes[selected_process].priority;
                
                // is this different from what we were tracking?
                bool is_different = (interval_id != switching_to);
                
                if (is_different && interval_id != "") {
                    // print the previous thing we were doing
                    if (interval_id == "Idle") {
                        cout << "Time " << interval_start << "-" << current_time << ": Idle" << endl;
                    } else {
                        cout << "Time " << interval_start << "-" << current_time << ": " << interval_id << " (Priority " << interval_priority << ")" << endl;
                    }
                    // now track the new one
                    interval_start = current_time;
                    interval_id = switching_to;
                    interval_priority = switching_priority;
                } else if (interval_id == "") {
                    // this is the beginning
                    interval_start = current_time;
                    interval_id = switching_to;
                    interval_priority = switching_priority;
                }
                // if its the same process continuing, dont print
            } else {
                // no process to run, we are idling
                bool was_idle_already = (interval_id == "Idle");
                bool was_nothing = (interval_id == "");
                
                if (!was_idle_already && !was_nothing) {
                    // we just became idle from running something
                    cout << "Time " << interval_start << "-" << current_time << ": " << interval_id << " (Priority " << interval_priority << ")" << endl;
                    interval_start = current_time;
                    interval_id = "Idle";
                    interval_priority = 0;
                } else if (was_nothing) {
                    // starting idle
                    interval_start = current_time;
                    interval_id = "Idle";
                    interval_priority = 0;
                }
            }
        }

        // execute if something is assigned to cpu
        if (running_process_idx != -1) {
            // get reference to the running process
            int current_idx = running_process_idx;
            PCB& proc = processes[current_idx];
            
            // do one unit of work
            proc.remaining_time = proc.remaining_time - 1;
            proc.last_run_time = proc.last_run_time - 1;
            total_cpu_time = total_cpu_time + 1;
            
            // figure out if we need to do anything special
            bool process_done = (proc.remaining_time == 0);
            bool quantum_done = (proc.last_run_time == 0);
            
            if (process_done) {
                // this process finished everything
                proc.completion_time = current_time + 1;
                finished_count = finished_count + 1;
                
                // output the final segment for this process
                cout << "Time " << interval_start << "-" << (current_time + 1) << ": " << interval_id << " (Priority " << interval_priority << ")" << endl;
                
                // free up the cpu
                running_process_idx = -1;
                interval_start = current_time + 1;
                interval_id = "";
            } else if (quantum_done) {
                // ran out of time slice but not finished
                // send it back to wait again
                int proc_priority = proc.priority;
                readyQueues[proc_priority].push(current_idx);
                
                // cpu is available now
                running_process_idx = -1;
            }
            // if neither condition then just keep running
        }

        // move time forward by one
        current_time++;
    }

    // if we ended while idle we need to print that
    if (interval_id == "Idle" && interval_start < current_time) {
        cout << "Time " << interval_start << "-" << current_time << ": Idle" << endl;
    }

    // calculate and print turnaround time
    // turnaround time is how long from when it arrived to when it finished
    cout << "\nTurnaround Time" << endl;
    for(int i=0; i<n; i++) {
        int turnaround = processes[i].completion_time - processes[i].arrival_time;
        cout << processes[i].id << " = " << turnaround << endl;
    }

    // calculate and print waiting time
    // waiting time is turnaround minus the actual work time
    cout << "\nWaiting Time" << endl;
    for(int i=0; i<n; i++) {
        int turnaround = processes[i].completion_time - processes[i].arrival_time;
        int waiting = turnaround - processes[i].burst_time;
        cout << processes[i].id << " = " << waiting << endl;
    }

    // print how much the cpu was being used
    cout << "\nCPU Utilization Time" << endl;
    cout << total_cpu_time << "/" << current_time << endl;

    return 0;
}
