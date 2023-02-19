#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

struct Process{
    int pId;
    int burstDuration;
    int burstLeft;
    int arrivalTime;
    int endingTime;
    int waitingTime;
    int timeAround;
    int responseTime;
    bool seen = false;
};

bool operator<(const Process &process1, const Process &process2){
    return process1.arrivalTime < process2.arrivalTime;
}

struct RoundRobin{
    int timeQuantum;
    queue <Process *> readyQueue;
};

int main(){
    RoundRobin RR;
    cout << "Determine time quantum:";
    cin >> RR.timeQuantum;

    int n;
    cout << "Enter #of processes :";
    cin >> n;
    Process * Processes = new Process[n];
    for(int i = 0; i < n; i++){
        cout << "Enter process id:";
        cin >> Processes[i].pId;
        cout << "Enter arrival time:";
        cin >> Processes[i].arrivalTime;
        cout << "Enter Burst duration:";
        cin >> Processes[i].burstDuration;
        Processes[i].burstLeft = Processes[i].burstDuration;
    }

    sort(Processes, Processes + n);

    int Time = 0;
    int pnt = 0;
    Process* curProcess = NULL;
    int curProcessTimeLeft = 0;
    vector <int> log;
    while(pnt < n || curProcess != NULL || !RR.readyQueue.empty()){
        while(pnt < n && Processes[pnt].arrivalTime <= Time){
            RR.readyQueue.push(&Processes[pnt]);
            pnt ++;
        }

        if(curProcess == NULL && !RR.readyQueue.empty()){
            curProcess = RR.readyQueue.front();
            curProcessTimeLeft = RR.timeQuantum;
            RR.readyQueue.pop();
            if(!(*curProcess).seen){
                (*curProcess).seen = true;
                (*curProcess).responseTime = Time - (*curProcess).arrivalTime;
            }
        }

        if(curProcess != NULL){
            (*curProcess).burstLeft --;
            curProcessTimeLeft --;
            log.push_back((*curProcess).pId);
        }
        else{
            log.push_back(-1);
        }
        Time ++;

        if(curProcess != NULL && (*curProcess).burstLeft == 0){
            (*curProcess).endingTime = Time;
            curProcess = NULL;
            curProcessTimeLeft = 0;

        }
        else if(curProcessTimeLeft == 0 && curProcess != NULL){
            RR.readyQueue.push(curProcess);
            curProcess = NULL;
            curProcessTimeLeft = 0;
        }
    }

    for(int i = 0; i < n; i++){
        Processes[i].waitingTime = Processes[i].endingTime -
                                   Processes[i].burstDuration -
                                   Processes[i].arrivalTime;
        Processes[i].timeAround = Processes[i].endingTime -
                                  Processes[i].arrivalTime;
    }

    cout << "log is equal to (per second):" << endl;
    for(auto u : log) cout << u << " ";
    cout << endl;

    int sumwaitingtime = 0;
    int sumresponsetime = 0;
    for(int i = 0; i < n; i++){
        cout << "pID = " << Processes[i].pId << " ";
        cout << "endingTime = " << Processes[i].endingTime << " ";
        cout << "waitingTime = " << Processes[i].waitingTime << " ";
        cout << "timeAround = " << Processes[i].timeAround << " ";
        cout << "responseTime = " << Processes[i].responseTime << endl;
        sumwaitingtime += Processes[i].waitingTime;
        sumresponsetime += Processes[i].responseTime;
    }

    cout << "average waitingTime = " << (1.0 * sumwaitingtime / n) << endl;
    cout << "average responseTime = " << (1.0 * sumresponsetime / n) << endl;

    return 0;
}
