#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <queue>
#include <algorithm>

using namespace std;
//structure that represents single process
struct proc {
    int id; //process id
    int b_time; //burst time Total CPU time required
    int rem_time; //remaining time Execution time left
    int arr_time; //arrival time When the process enters the ready queue
    int w_time; //waiting time Total time spent waiting in the ready queue
    int tot_time; //total time Total time from arrival to completion
};
//function which generates random parameters based on data provided by user
vector<proc> gen(int max_burst, int max_arrival) {
    int ile;
    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<> disBurst(1, max_burst);
    uniform_int_distribution<> disArrival(0, max_arrival);

    cout << "Type how many processes are you willing to generate? " << endl;
    if (!(cin >> ile) || ile <= 0) {
        cout << "Error, type integer larger than 0 " << endl;
        return {};
    }

    vector<proc> tab;
    for (int i = 0; i < ile; i++) {
        proc p;
        p.id = i + 1;
        p.b_time = disBurst(g);
        p.arr_time = disArrival(g);
        p.rem_time = p.b_time;
        tab.push_back(p);
    }
//shows control table which contains generated processes
    cout << "\n" << setw(5) << "PID" << setw(15) << "Arrival Time" << setw(15) << "Burst Time" << endl;
    cout << "------------------------------------------" << endl;
    for (const auto& p : tab) {
        cout << "P" << setw(4) << left << p.id
             << setw(15) << right << p.arr_time
             << setw(15) << p.b_time << endl;
    }
    return tab;
}
//Round Robin algorithm implementation
void RR(vector<proc> processes, int quantum) {
    int n = processes.size();
    if (n == 0) return;
//sort processes by arrival time
    sort(processes.begin(), processes.end(), [](const proc& a, const proc& b) {
        if(a.arr_time == b.arr_time) return a.id < b.id; // if processes have same arrival time decision is made based on ID number of process
        return a.arr_time < b.arr_time;
    });

    int current_time = 0; //system clock
    int completed = 0; //register of finished processes
    queue<int> q;//ready queue
    vector<bool> in_queue(n, false);//prevents duplication of processes

    int i = 0;
    cout << "\n--- Work RR (Quantum = " << quantum << ") ---\n";
//main loop executed until all processes are completed
    while (completed < n) {
        if (q.empty() && i < n) { //If the queue is empty and there are still processes that have not arrived simulator advance the clock until the next process arrives
            if (current_time < processes[i].arr_time)
                current_time = processes[i].arr_time;
        }

        while (i < n && processes[i].arr_time <= current_time) { //adds freshly arrived processes to queue
            if (!in_queue[i]) {
                q.push(i);
                in_queue[i] = true;
            }
            i++;
        }

        if (!q.empty()) {
            int idx = q.front(); //simulator takes first process from queue
            q.pop();

            int execute = min(processes[idx].rem_time, quantum); //process is executed for value of quantum or if shorter until is ended
            cout << "T=" << current_time << " | P" << processes[idx].id << " works for " << execute << endl;

            processes[idx].rem_time -= execute;
            current_time += execute;

            while (i < n && processes[i].arr_time <= current_time) { //After a quantum ends, simulator first check to see if new processes have arrived in the meantime. They take priority in the queue over the interrupted process.
                if (!in_queue[i]) {
                    q.push(i);
                    in_queue[i] = true;
                }
                i++;
            }

            if (processes[idx].rem_time > 0) { //If the process has not finished, it returns to the end of the queue.
                q.push(idx);
            } else { //If the process has finished, program calculates the statistics
                completed++;
                processes[idx].tot_time = current_time - processes[idx].arr_time;
                processes[idx].w_time = processes[idx].tot_time - processes[idx].b_time;
            }
        }
    }

    //Tab with results
    double total_wait = 0, total_tat = 0;
    cout << "\nEnd results:\n";
    cout << "ID\tArr\tBurst\tWait\tTAT\n";
    for (const auto& p : processes) {
        total_wait += p.w_time;
        total_tat += p.tot_time;
        cout << "P" << p.id << "\t" << p.arr_time << "\t" << p.b_time
             << "\t" << p.w_time << "\t" << p.tot_time << endl;
    }
    cout << fixed << setprecision(2);
    cout << "\nAverage waiting time: " << total_wait / n;
    cout << "\nAverage processing time(TAT): " << total_tat / n << endl;
}

int main() {
    int max_b, max_a, quantum;
    cout << "--- CPU Scheduling Algorithm Simulator ---\n" << endl;

    cout << "Type max Burst Time: ";
    cin >> max_b;

    cout << "Type max Arrival Time: ";
    cin >> max_a;
    // Getting the process vector from the generator
    vector<proc> procesy = gen(max_b, max_a);
// if user provided correct data
    if (!procesy.empty()) {
        cout << "\nType quantum value for Round Robin: ";
        cin >> quantum;
        RR(procesy, quantum); //simulation start
    }

    return 0;
}