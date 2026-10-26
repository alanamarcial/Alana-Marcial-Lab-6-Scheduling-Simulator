// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"


// Function to find the waiting time for all  
// processes
// Admit every process that has arrived by time t, earliest arrival first
static void admitArrivals(ProcessType plist[], int n, int t, int admitted[],
                          int queue[], int head, int *count)
{
    for (;;) {
        int best = -1;
        for (int i = 0; i < n; i++)
            if (!admitted[i] && plist[i].art <= t &&
                (best == -1 || plist[i].art < plist[best].art))
                best = i;
        if (best == -1) return;
        queue[(head + *count) % n] = best;
        (*count)++;
        admitted[best] = 1;
    }
}

void findWaitingTimeRR(ProcessType plist[], int n, int quantum) 
{ 
    int *rem_bt   = malloc(n * sizeof(int));
    int *admitted = calloc(n, sizeof(int));
    int *queue    = malloc(n * sizeof(int));
    int head = 0, count = 0, completed = 0, t = 0;

    for (int i = 0; i < n; i++) {
        rem_bt[i] = plist[i].bt;
        plist[i].wt = 0;
    }

    while (completed < n) {
        admitArrivals(plist, n, t, admitted, queue, head, &count);

        if (count == 0) {
            int next = INT_MAX;
            for (int i = 0; i < n; i++)
                if (!admitted[i] && plist[i].art < next) next = plist[i].art;
            t = next;
            continue;
        }

        int cur = queue[head];
        head = (head + 1) % n;
        count--;

        int run = (rem_bt[cur] > quantum) ? quantum : rem_bt[cur];
        t += run;
        rem_bt[cur] -= run;

        admitArrivals(plist, n, t, admitted, queue, head, &count);

        if (rem_bt[cur] > 0) {
            queue[(head + count) % n] = cur;
            count++;
        } else {
            completed++;
            plist[cur].wt = t - plist[cur].art - plist[cur].bt;
        }
    }

    free(rem_bt);
    free(admitted);
    free(queue);
}

// Function to find the waiting time for all  
// processes 
void findWaitingTimeSJF(ProcessType plist[], int n)
{ 
      int *rem_bt = malloc(n *sizeof(int));
      int complete = 0;
      int t = 0;

      for (int i = 0; i < n; i++) {
        rem_bt[i] = plist[i].bt;
        plist[i].wt = 0;
      }

      while (complete < n) {


        int shortest = -1;
        int min_rem = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (plist[i].art <= t && rem_bt[i] > 0 && rem_bt[i] < min_rem) {
                min_rem = rem_bt[i];
                shortest = i;
            }
        }


      if (shortest == -1) {
        t++;
        continue;
      }
      rem_bt[shortest]--;
      t++;

      if (rem_bt[shortest] == 0) {
        complete++;

        plist[shortest].wt = t - plist[shortest].art - plist[shortest].bt;
      }
    }

    free(rem_bt);
} 


// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{
    int t = 0;
    for (int i = 0; i < n; i++) {
        if (t < plist[i].art)        // CPU idles until this process arrives
            t = plist[i].art;
        plist[i].wt = t - plist[i].art;
        t += plist[i].bt;
    }
}
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
int my_comparer(const void *this, const void *that)
{ 
    const ProcessType *a = (const ProcessType *)this;
    const ProcessType *b = (const ProcessType *)that;

    // higher pri value goes first
    if (a->pri < b->pri) return 1;
    if (a->pri > b->pri) return -1;

    // same priority: earlier arrival first, then lower pid
    if (a->art > b->art) return 1;
    if (a->art < b->art) return -1;
    if (a->pid > b->pid) return 1;
    if (a->pid < b->pid) return -1;
    return 0;
}

//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time
void findavgTimePriority( ProcessType plist[], int n)
{
    // 1- Sort the processes according to priority
    qsort(plist, n, sizeof(ProcessType), my_comparer);
 
    // 2- Now simply apply FCFS algorithm
    findWaitingTime(plist, n);
    findTurnAroundTime(plist, n);
 
    //Display processes along with all details
    printf("\n*********\nPriority\n");
}
 

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
        fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
        fflush(stdout);
        return 1;
    }
    
    // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeFCFS(proc_list, n);
    printMetrics(proc_list, n);
    free(proc_list);
  
    // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeSJF(proc_list, n); 
    printMetrics(proc_list, n);
    free(proc_list);
  
    // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    findavgTimePriority(proc_list, n); 
    printMetrics(proc_list, n);
    free(proc_list);
    
    // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeRR(proc_list, n, quantum); 
    printMetrics(proc_list, n);
    free(proc_list);
    
    return 0; 
}