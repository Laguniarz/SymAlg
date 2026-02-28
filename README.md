# SymAlg
A CPU Scheduling Algorithm Simulator written in C++ using a procedural programming approach.

How it works:
The simulator generates a set of processes based on user-defined constraints. The user provides the maximum values for Burst Time and Arrival Time, which set the range for random generation (from 1 to n, denoted as [1,n]).

Current Features:
    Round Robin (RR) Implementation: A preemptive scheduling algorithm that uses a time quantum to ensure fair CPU sharing.
    Dynamic Process Generation: Automatic creation of test data based on user input.
    
Roadmap:
    Implementation of FCFS (First-Come, First-Served) and SJF (Shortest Job First) algorithms.
    Exporting simulation results to a .txt or .csv file for further analysis.
    Comparison mode to evaluate performance metrics across different algorithms.
