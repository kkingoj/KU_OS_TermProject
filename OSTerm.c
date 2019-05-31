// KOREA UNIVERSITY OPERATING SYSTEM TERM PROJECT
// Spring 2019
// Author: Youngjoon Yoon
// May 27th, 2019

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct process{
    int pid;
    int CPU_burstT;
//  int IO_burstT; // IO_burst time은 딱히 사용을 하지 않기 때문에 주석 처리하였음 
    int arrivalT;
    int priority;
    int waitingT;
    int TurnAroundT;
    int remainingT;
}processes;

// Create Process
int CreateProcess(processes P[]){
    int NumOfProcesses;
    int i;
    
    srand(time(NULL));
    
    printf("How many processes? Put 1 ~ 100: ");
    scanf("%d", &NumOfProcesses);
    
    for(i=0;i<NumOfProcesses;i++){
        P[i].pid = i+1; // Allocate process pid
        
        P[i].CPU_burstT = rand()%11;
        while(P[i].CPU_burstT == 0)
            P[i].CPU_burstT = rand()%16;  // Allocate CPU Burst Time Randomly
        
/*      P[i].IO_burstT = rand()%51;
        while(P[i].IO_burstT == 0)
            P[i].IO_burstT = rand()%51; // Allocate IO Burst Time Randomly */
        
        P[i].arrivalT = rand()%31; // Allocate Arrival Time Randomly
        
        P[i].remainingT = P[i].CPU_burstT;
        
        P[i].priority = rand()%21;
        while(P[i].priority == 0)
            P[i].priority = rand()%21; // Allocate Priority Randomly
    }
    P[0].arrivalT = 0;
    
    printf("\n PROC \tCPU\tIO\tA.T.\tPriority");
    for(i=0; i<NumOfProcesses; i++){
//      printf("\n %d\t%d\t%d\t%d\t%d", P[i].pid, P[i].CPU_burstT, P[i].IO_burstT, P[i].arrivalT, P[i].priority);
        printf("\n %d\t%d\t%d\t%d", P[i].pid, P[i].CPU_burstT, P[i].arrivalT, P[i].priority);
    }
    
    return NumOfProcesses;
}

// Processes go to Ready Queue sorted by arrival time
void ReadyQ_Sort(processes temp[], int NumOfProcesses){
    
    processes p;
    int i, j;
    for(i=1; i<NumOfProcesses; i++){
        for(j=0; j<NumOfProcesses-i;j++){
            if(temp[j].arrivalT > temp[j+1].arrivalT){
                p = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = p;
            }
        }
    }
}

// FCFS Algorithm
void FCFS(processes P[], int n){
    processes temp[100];
    int sumW = 0;  // Sum of waiting time
    int sumT = 0; // Sum of turnaround time
    int x;
    float avgW = 0.0; // Average waiting time
    float avgT = 0.0; // Average turnaround time
    
    int i, j;
    for(i=0;i<n;i++){
        temp[i] = P[i];
    }
    
    ReadyQ_Sort(temp, n);
    
    printf("\n\n PROC.\tBT\tAT");
    for(i=0;i<n;i++){
        printf("\n %d\t%d\t%d",temp[i].pid,temp[i].CPU_burstT,temp[i].arrivalT); // Process 정렬 
    }
    
    sumW = temp[0].waitingT = 0;
    sumT = temp[0].TurnAroundT = temp[0].CPU_burstT; // 첫 번째 Process의 Wainting Time, TurnAround Time 설정 및  Total Sum을 첫 번째 Process 것으로  초기화 
    
    for(i=1;i<n;i++){
        temp[i].waitingT = (temp[i-1].CPU_burstT + temp[i-1].waitingT + temp[i-1].arrivalT) - temp[i].arrivalT; // (i+1)번째 Process의 Waiting Time
        if(temp[i].waitingT<0){
            temp[i].waitingT = 0;
        } 
        temp[i].TurnAroundT = temp[i].waitingT + temp[i].CPU_burstT; // (i+1)번째 Process의 TurnAround Time 
        sumW += temp[i].waitingT; // Total Waiting Time 업데이트 
        sumT += temp[i].TurnAroundT; // Total Turnaround Time 업데이트 
    }
    
    avgW = (float) sumW/n; // Average Waiting Time
    avgT = (float) sumT/n; // Average Turnaround Time
    
    printf("\n\n PROC.\tBT\tAT\tWT\tTurnaroundT");
    for(i=0; i<n ; i++){
        printf("\n %d\t%d\t%d\t%d\t%d", temp[i].pid, temp[i].CPU_burstT, temp[i].arrivalT, temp[i].waitingT, temp[i].TurnAroundT);
    } // 각 Process의 정보 
    
    x = temp[0].arrivalT;
    // Gantt Chart 생성
    printf("\n\n Gantt Chart\n ");
    int a = 1;
    for(i=1;i<=n;i++){
        if((temp[i-1].CPU_burstT + temp[i-1].waitingT + temp[i-1].arrivalT) - temp[i].arrivalT < 0){
            a = 0;
            break;    
        }
    }
    if(a == 1){
    for(i=0;i<n;i++){
        printf("    %d   ", temp[i].pid);
    }
    
    printf("\n ");
    printf("%d\t",temp[0].arrivalT);
    
    for(i=1;i<=n;i++){
        x += temp[i-1].CPU_burstT;
        printf("%d      ",x);
        
    }
    }else{
    i = 0;
    while(i<n){
        if(i==0){
            printf("    %d   ", temp[0].pid);
            i++;
        }else{
        if((temp[i-1].CPU_burstT + temp[i-1].waitingT + temp[i-1].arrivalT) - temp[i-1].arrivalT >= 0)
            printf("    %d   ", temp[i].pid);
        else
            printf("       %d   ", temp[i].pid);
        i++;
    }
    }
    
    printf("\n ");

    i = 0;
    while(i<n){
        if(i==0){
            printf("%d\t%d", x, x+temp[0].CPU_burstT);
            x = x + temp[0].CPU_burstT;
            i++;
        }else{
        if((temp[i-1].CPU_burstT + temp[i-1].waitingT + temp[i-1].arrivalT) - temp[i].arrivalT >= 0){
            x += temp[i].CPU_burstT;
            printf("\t%d", x);
            i++;
        }else{
            x = temp[i].arrivalT;
            x += temp[i].CPU_burstT;
            printf(" %d\t%d",temp[i].arrivalT,x);
            i++;
            }
        }
    }
    }
    
    // Evaluation
    printf("\n\n Average Waiting Time = %0.2f\n Average Turnaround Time = %0.2f", avgW, avgT);
}

// Shortest Job First without Preemption
void SJFNP(processes P[], int n){
    
    processes temp[101];
    
    int sumW = 0;
    int sumT = 0;
    
    float avgW = 0.0;
    float avgT = 0.0;
    
    int smallest;
    int count = 0;
    int time;
    int i;
    int end;
    
    for(i=0; i<n; i++)
        temp[i] = P[i];
        
    ReadyQ_Sort(temp, n);
    
    temp[100].remainingT = 9999;
    
    // Algorithm with Gantt Chart
    printf("\n0");
    for(time = 0; count != n ; ){
        
        smallest = 100;
        
        for(i=0; i<n; i++){
            if(temp[i].arrivalT <= time && temp[i].remainingT < temp[smallest].remainingT && temp[i].remainingT > 0)
                smallest = i;
        }
        if(smallest != 100){
            time += temp[smallest].remainingT;
            temp[smallest].remainingT = 0;
            printf("--Process[%d]--%d", temp[smallest].pid, time);
        }
        else if(smallest == 100 && count != n && time < temp[count].arrivalT ){
            printf(" %d", temp[count].arrivalT);
            time = temp[count].arrivalT;
        }
        if(temp[smallest].remainingT == 0){
            count++;
            end = time;
            temp[smallest].waitingT = end - temp[smallest].arrivalT - temp[smallest].CPU_burstT;
            temp[smallest].TurnAroundT = end - temp[smallest].arrivalT;
            sumW = sumW + temp[smallest].waitingT;
            sumT = sumT + temp[smallest].TurnAroundT;
        }        
    }
    
    // Process Sorting
    printf("\n\n PROC.\tAT\tBT\tWT\tTurnaroundT");
    for(i=0; i<n; i++)
        printf("\n %d\t%d\t%d\t%d\t%d", temp[i].pid, temp[i].arrivalT, temp[i].CPU_burstT, temp[i].waitingT, temp[i].TurnAroundT);
        
    // Evaluation
    avgT = (float) sumT / n;
    avgW = (float) sumW / n;
    
    printf("\n\n Average Waiting Time = %0.2f\n Average Turnaround Time = %0.2f", avgW, avgT);
}

// Priority without Preemption
void PRINP(processes P[], int n){

    processes temp[101];
    
    int sumW = 0;
    int sumT = 0;
    
    float avgW = 0.0;
    float avgT = 0.0;
    
    int smallest;
    int count = 0;
    int time;
    int i;
    int end;
    
    for(i=0; i<n; i++)
        temp[i] = P[i];
        
    ReadyQ_Sort(temp, n);
    
    temp[100].priority = 9999;
    
    // Algorithm with Gantt Chart
    printf("\n0");
    for(time = 0; count != n ; ){
        
        smallest = 100;
        
        for(i=0; i<n; i++){
            if(temp[i].arrivalT <= time && temp[i].priority < temp[smallest].priority && temp[i].remainingT > 0)
                smallest = i;
        }
        if(smallest != 100){
            time += temp[smallest].remainingT;
            temp[smallest].remainingT = 0;
            printf("--Process[%d]--%d", temp[smallest].pid, time);
        }
        else if(smallest == 100 && count != n && time < temp[count].arrivalT ){
            printf(" %d", temp[count].arrivalT);
            time = temp[count].arrivalT;
        }
        if(temp[smallest].remainingT == 0){
            count++;
            end = time;
            temp[smallest].waitingT = end - temp[smallest].arrivalT - temp[smallest].CPU_burstT;
            temp[smallest].TurnAroundT = end - temp[smallest].arrivalT;
            sumW = sumW + temp[smallest].waitingT;
            sumT = sumT + temp[smallest].TurnAroundT;
        }        
    }
    
    // Process Sorting
    printf("\n\n PROC.\tAT\tBT\tWT\tPRI\tTurnaroundT");
    for(i=0; i<n; i++)
        printf("\n %d\t%d\t%d\t%d\t%d", temp[i].pid, temp[i].arrivalT, temp[i].CPU_burstT, temp[i].waitingT, temp[i].priority, temp[i].TurnAroundT);
        
    // Evaluation
    avgT = (float) sumT / n;
    avgW = (float) sumW / n;
    
    printf("\n\n Average Waiting Time = %0.2f\n Average Turnaround Time = %0.2f", avgW, avgT);    
}

// Round Robin
void RR (processes P[], int n){

    processes temp[100];
    
    int sumW = 0;
    int sumT = 0;
    float avgW = 0.0;
    float avgT = 0.0;
    
    int counter = 0;
    int total = 0;
    int x = n;
    
    int i;
    
    int TIME_QUANTUM;
    
    for(i=0; i<n; i++)
        temp[i] = P[i];
    
    // Arrival Time 순으로 정렬 
    ReadyQ_Sort(temp, n);
    
    // Time Quantum 설정 
    printf("\nEnter Time Quantum: ");
    scanf("%d", &TIME_QUANTUM);
    
    // RR Algorithm and Gantt Chart
    printf("\n0");
    for(total = 0, i = 0; x != 0 ; ){
        if(temp[i].remainingT <= TIME_QUANTUM && temp[i].remainingT > 0){
            total = total + temp[i].remainingT;
            temp[i].remainingT = 0;
            counter = 1;
            printf("--Process[%d]--%d", temp[i].pid, total);
        }
        else if(temp[i].remainingT > 0){
            temp[i].remainingT = temp[i].remainingT - TIME_QUANTUM;
            total = total + TIME_QUANTUM;
            printf("--Process[%d]--%d", temp[i].pid, total);
        }
        if(temp[i].remainingT == 0 && counter ==1){
            x--;
            temp[i].TurnAroundT = total - temp[i].arrivalT;
            temp[i].waitingT = total - temp[i].arrivalT - temp[i].CPU_burstT;
            sumW = sumW + total - temp[i].arrivalT - temp[i].CPU_burstT;
            sumT = sumT + total - temp[i].arrivalT;
            counter = 0;
        }
        if(i == n-1)
            i = 0;
        else if(temp[i+1].arrivalT <= total)
            i++;
        else if(counter == 0 && x == n-i-1 && temp[i+1].arrivalT > total){
            i++;
            total = temp[i].arrivalT;
            printf("--%d", total);
        }
        else
            i = 0;
    }
    
    // Process 정렬
    printf("\n\n PROC.\tBT\tAT\tWT\tTurnaroundT");
    for(i=0; i<n ; i++)
        printf("\n %d\t%d\t%d\t%d\t%d", temp[i].pid, temp[i].CPU_burstT, temp[i].arrivalT, temp[i].waitingT, temp[i].TurnAroundT);
    
    // Evaluation    
    avgW = (float) sumW / n;
    avgT = (float) sumT / n;
    
    printf("\n\n Average Waiting Time = %0.2f\n Average Turnaround Time = %0.2f", avgW, avgT);  
    
}

// Shortest Job First with Preemption
void SJFP(processes P[], int n){
    processes temp[101];
    
    int sumW = 0;
    int sumT = 0;
    
    float avgW = 0.0;
    float avgT = 0.0;
    
    int smallest;
    int count = 0;
    int time;
    int i;
    int end;
    
    for(i=0; i<n; i++)
        temp[i] = P[i];
        
    ReadyQ_Sort(temp, n);
    
    temp[100].remainingT = 9999;
    
    // Algorithm with Gantt Chart
    printf("\n0");
    for(time = 0; count != n ; time++){
        
        smallest = 100;
        
        for(i=0; i<n; i++){
            if(temp[i].arrivalT <= time && temp[i].remainingT < temp[smallest].remainingT && temp[i].remainingT > 0)
                smallest = i;
        }
        temp[smallest].remainingT--;
        if(smallest != 100)
            printf("--Process[%d]--%d", temp[smallest].pid, time + 1);
        else if(smallest == 100 && count != n && time == temp[count].arrivalT - 1)
            printf(" %d", time + 1);
        if(temp[smallest].remainingT == 0){
            count++;
            end = time + 1;
            temp[smallest].waitingT = end - temp[smallest].arrivalT - temp[smallest].CPU_burstT;
            temp[smallest].TurnAroundT = end - temp[smallest].arrivalT;
            sumW = sumW + temp[smallest].waitingT;
            sumT = sumT + temp[smallest].TurnAroundT;
        }        
    }
    
    // Process Sorting
    printf("\n\n PROC.\tAT\tBT\tWT\tTurnaroundT");
    for(i=0; i<n; i++)
        printf("\n %d\t%d\t%d\t%d\t%d", temp[i].pid, temp[i].arrivalT, temp[i].CPU_burstT, temp[i].waitingT, temp[i].TurnAroundT);
        
    // Evaluation
    avgT = (float) sumT / n;
    avgW = (float) sumW / n;
    
    printf("\n\n Average Waiting Time = %0.2f\n Average Turnaround Time = %0.2f", avgW, avgT);     
}

// Priority Scheduling with Preemption
void PRIP(processes P[], int n){
    processes temp[101];
    
    int sumW = 0;
    int sumT = 0;
    
    float avgW = 0.0;
    float avgT = 0.0;
    
    int smallest;
    int count = 0;
    int time;
    int i;
    int end;
    
    for(i=0; i<n; i++)
        temp[i] = P[i];
        
    ReadyQ_Sort(temp, n);
    
    temp[100].priority = 9999;
    
    // Algorithm with Gantt Chart
    printf("\n0");
    for(time = 0; count != n ; time++){
        
        smallest = 100;
        
        for(i=0; i<n; i++){
            if(temp[i].arrivalT <= time && temp[i].priority < temp[smallest].priority && temp[i].remainingT > 0)
                smallest = i;
        }
        temp[smallest].remainingT--;
        if(smallest != 100)
            printf("--Process[%d]--%d", temp[smallest].pid, time + 1);
        else if(smallest == 100 && count != n && time == temp[count].arrivalT - 1)
            printf(" %d", time + 1);
        if(temp[smallest].remainingT == 0){
            count++;
            end = time + 1;
            temp[smallest].waitingT = end - temp[smallest].arrivalT - temp[smallest].CPU_burstT;
            temp[smallest].TurnAroundT = end - temp[smallest].arrivalT;
            sumW = sumW + temp[smallest].waitingT;
            sumT = sumT + temp[smallest].TurnAroundT;
        }        
    }
    
    // Process Sorting
    printf("\n\n PROC.\tAT\tBT\tWT\tPRI\tTurnaroundT");
    for(i=0; i<n; i++)
        printf("\n %d\t%d\t%d\t%d\t%d\t%d", temp[i].pid, temp[i].arrivalT, temp[i].CPU_burstT, temp[i].waitingT,temp[i].priority, temp[i].TurnAroundT);
        
    // Evaluation
    avgT = (float) sumT / n;
    avgW = (float) sumW / n;
    
    printf("\n\n Average Waiting Time = %0.2f\n Average Turnaround Time = %0.2f", avgW, avgT);    
}
// Main Function
int main(){
    processes P[100];
    int ch, NumOfProcesses;
    
    do{
        printf("\n\n OPTIONS: ");
        printf("\n 0. Process Generation: Enter the number of processes.");
        printf("\n 1. FCFS");
        printf("\n 2. Shortest Job First with Preemption");
        printf("\n 3. Shortest Job First without Preemption");
        printf("\n 4. Priority Scheduling with Preemption");
        printf("\n 5. Priority Scheduling without Preemption");
        printf("\n 6. Round Robin");
        printf("\n 7. Exit");
        printf("\n Select: ");
        scanf("%d", &ch);
        switch(ch){
            case 0:
                NumOfProcesses = CreateProcess(P);
                break;
                
            case 1:
                FCFS(P, NumOfProcesses);
                break;
                
            case 2:
                SJFP(P, NumOfProcesses);
                break;
                
            case 3:
                SJFNP(P, NumOfProcesses);
                break;
                
            case 4:
                PRIP(P, NumOfProcesses);
                break;
            
            case 5:
                PRINP(P, NumOfProcesses);
                break;
                
            case 6:
                RR(P, NumOfProcesses);
                break;
                    
            case 7: exit(0);
        }
    }while(ch != 7);
    
    return 0;
}
    
