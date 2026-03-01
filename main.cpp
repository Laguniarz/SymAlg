#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

// structure that represents single process
struct proc {
    int id;
    int b_time;
    int rem_time;
    int arr_time;
    int w_time = 0;
    int tot_time = 0;
};

// structure that represents algorityms statistics
struct Stats {
    double avg_wait;
    double avg_tat;
};

// process generator
vector<proc> gen(int max_burst, int max_arrival) {
    int ile;
    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<> disBurst(1, max_burst);
    uniform_int_distribution<> disArrival(0, max_arrival);

    cout << "Type how many processes are you willing to generate? ";
    if (!(cin >> ile) || ile <= 0) {
        cout << "Error, type integer larger than 0\n";
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

    cout << "\nGenerated processes:\n";
    cout << left << setw(5) << "PID"
         << setw(15) << "Arrival"
         << setw(15) << "Burst" << endl;
    cout << "-----------------------------------\n";

    for (const auto& p : tab) {
        cout << left << setw(5) << ("P" + to_string(p.id))
             << setw(15) << p.arr_time
             << setw(15) << p.b_time << endl;
    }

    return tab;
}

// ================= ROUND ROBIN =================
Stats RR(vector<proc> processes, int quantum) {

    int n = processes.size();
    sort(processes.begin(), processes.end(), [](const proc& a, const proc& b) {
        if (a.arr_time == b.arr_time)
            return a.id < b.id;
        return a.arr_time < b.arr_time;
    });

    int current_time = 0;
    int completed = 0;
    queue<int> q;
    vector<bool> in_queue(n, false);
    int i = 0;

    cout << "\n--- ROUND ROBIN (q=" << quantum << ") ---\n";

    while (completed < n) {

        if (q.empty() && i < n && current_time < processes[i].arr_time)
            current_time = processes[i].arr_time;

        while (i < n && processes[i].arr_time <= current_time) {
            if (!in_queue[i]) {
                q.push(i);
                in_queue[i] = true;
            }
            i++;
        }

        if (!q.empty()) {
            int idx = q.front();
            q.pop();

            int exec = min(processes[idx].rem_time, quantum);
            cout << "T=" << current_time << " | P"
                 << processes[idx].id
                 << " works for " << exec << endl;

            processes[idx].rem_time -= exec;
            current_time += exec;

            while (i < n && processes[i].arr_time <= current_time) {
                if (!in_queue[i]) {
                    q.push(i);
                    in_queue[i] = true;
                }
                i++;
            }

            if (processes[idx].rem_time > 0) {
                q.push(idx);
            } else {
                completed++;
                processes[idx].tot_time = current_time - processes[idx].arr_time;
                processes[idx].w_time = processes[idx].tot_time - processes[idx].b_time;
            }
        }
    }

    double total_wait = 0, total_tat = 0;

    for (const auto& p : processes) {
        total_wait += p.w_time;
        total_tat += p.tot_time;
    }

    Stats s;
    s.avg_wait = total_wait / n;
    s.avg_tat  = total_tat / n;
    return s;
}

//  FCFS
Stats FCFS(vector<proc> processes) {

}

// Compare Table 
void printComparison(const Stats& rr, const Stats& fcfs) {

    cout << "\n\n================== COMPARISON ==================\n";
    cout << left << setw(15) << "Algorithm"
         << setw(20) << "Avg Waiting Time"
         << setw(20) << "Avg Turnaround Time" << endl;
    cout << "------------------------------------------------\n";

    cout << left << setw(15) << "Round Robin"
         << setw(20) << fixed << setprecision(2) << rr.avg_wait
         << setw(20) << rr.avg_tat << endl;

    cout << left << setw(15) << "FCFS"
         << setw(20) << fcfs.avg_wait
         << setw(20) << fcfs.avg_tat << endl;

    cout << "================================================\n";
}

// MAIN
int main() {

    int max_b, max_a, quantum;

    cout << "--- CPU Scheduling Simulator ---\n";

    cout << "Type max Burst Time: ";
    cin >> max_b;

    cout << "Type max Arrival Time: ";
    cin >> max_a;

    vector<proc> processes = gen(max_b, max_a);

    if (!processes.empty()) {

        cout << "\nType quantum for Round Robin: ";
        cin >> quantum;

        Stats rr = RR(processes, quantum);
        Stats fcfs = FCFS(processes);

        printComparison(rr, fcfs);
    }

    return 0;
}
