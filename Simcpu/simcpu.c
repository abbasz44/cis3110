/*Zachery Abbas
  1019463
  Assignment 2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void robin(int verbose, int quantum, int detail);
void FCFS(int verbose, int detail);

int main(int argc, char const *argv[])
{
	int detail = 0;
	int verbose = 0;
	int rounder = 0;
	int quantumum = 0;

	//loop through user input
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "-d") == 0)
		{

			detail = 1;
		}
		if (strcmp(argv[i], "-v") == 0)
		{

			verbose = 1;
		}
		if (strcmp(argv[i], "-r") == 0)
		{
			//set the rounder too 1
			rounder = 1;
			if (argv[i + 1] != NULL)
			{
				//quantem
				quantumum = atoi(argv[i + 1]);
			}
			else
			{
				printf("\n****didn't put quantem value, Restart program****\n");
				return 0;
			}
		}
	}

	//when the rounder == 1 call round function
	if (rounder == 1)
	{
		robin(verbose, quantumum, detail);
		// printf("\n%d,   %d,    %d\n", detail, verbose, rounder);
	}
	//if rounder is not 1 call FCFS
	else
	{
		// printf("\n%d,   %d,    %d\n", detail, verbose, rounder);
		FCFS(detail, verbose);
	}
}

void robin(int verbose, int quant, int detail)
{

	//declare varibales
	int buffFinal = 0;
	int threadTimer = 0;
	int counterThread = 0;
	int wastedNum = 0;
	double averageT = 0.0;
	int vCount = 0;
	int iBuf = 0;
	int ibuffoTime = 0;
	int buff = 0;
	int threadNum = 0;
	int timerA = 0;
	int cpuCount = 0;
	int processCount = 0;
	int switcherThread = 0;
	int switcherProcess = 0;
	int processNum = 0;
	int threadCount = 0;
	int cpuTimer = 0;
	int ioTimer = 0;
	int totioTimer = 0;
	int buffcpuTimer = 0;
	int totcpuTimer = 0;
	int finalTime = 0;
	int j;

	//give these inters 200 space
	int cpuTimerQ[200];
	int ioTimerQ[200];
	int totcpuTimerA[200];
	int totioTimerA[200];

	//scan info
	scanf("%d %d %d", &processCount, &switcherThread, &switcherProcess);

	//final time and wasted num
	finalTime = finalTime + (switcherProcess * processCount);
	wastedNum = switcherProcess * processCount;

	//loop through till i<processCount
	for (int i = 0; i < processCount; i++)
	{
		scanf("%d %d", &processNum, &threadCount);

		counterThread = counterThread + threadCount;

		//final time and wasted num
		finalTime = finalTime + (threadCount * switcherThread);
		wastedNum = wastedNum + (threadCount * switcherThread);

		//loop through till k<threadCount
		for (int k = 0; k < threadCount; k++)
		{
			//scan info
			scanf("%d %d %d", &threadNum, &timerA, &cpuCount);

			for (j = 0; j < cpuCount - 1; j++)
			{
				scanf("%d %d %d", &buff, &cpuTimer, &ioTimer);

				//RR changes here
				for (int l = 0; l < cpuCount - 1; l++)
				{
					cpuTimerQ[l] = cpuTimer;
					ioTimerQ[l] = ioTimer;

					int differnce = ioTimerQ[l] - quant;

					//statment when difference >= 0
					if (differnce >= 0)
					{
						cpuTimerQ[l] = cpuTimerQ[l] - quant;
						ioTimerQ[l] = ioTimerQ[l] - quant;
					}
					//if passes the if statment
					totcpuTimerA[j] = cpuTimerQ[l];
					totioTimerA[j] = ioTimerQ[l];
				}
			}
			//scan info
			scanf("%d %d", &buff, &cpuTimer);
			totcpuTimerA[j] = cpuTimer;

			// the buf time needs to reset
			//set ibuffoTime to 0

			ibuffoTime = 0;
			for (j = 0; j < cpuCount - 1; j++)
			{

				ibuffoTime = ibuffoTime + totioTimerA[j];
				//total io timer
				totioTimer = totioTimer + totioTimerA[j];
			}

			// the bufcputimer time needs to reset
			//set ibuffoTime to 0
			buffcpuTimer = 0;
			for (j = 0; j < cpuCount; j++)
			{

				buffcpuTimer = buffcpuTimer + totcpuTimerA[j];
				//total cpu Timer
				totcpuTimer = totcpuTimer + totcpuTimerA[j];
			}

			//finish up calculation
			buffFinal = ibuffoTime + buffcpuTimer;
			threadTimer = threadTimer + switcherThread + buffFinal;

			//when the thread num == 1 make sure thread timer gets hit
			if (threadNum == 1)
			{
				threadTimer = switcherProcess + threadTimer;
			}

			//calculation of average
			averageT = averageT + (threadTimer - timerA);

			if (verbose == 1)
			{
				if (processNum == 1 && threadNum == 1)
				{
					printf("****Error please restart****\n");
				}
			}
			if (detail == 1)
			{
				int serviceTimeFinal = buffFinal - ibuffoTime;
				int turnaroundTimerFinal = threadTimer - timerA;

				//printers
				printf("\nThread %d of Process %d: \n\n", threadNum, processNum);
				printf("\tarrival time: %d \n", timerA);
				printf("\tService time: %d units,I/O time: %d units,turnaround time: %d units,finish time: %d units \n", serviceTimeFinal, ibuffoTime, turnaroundTimerFinal, threadTimer);
			}
		}
	}

	finalTime = finalTime + totioTimer + totcpuTimer;

	//get percentage
	double utili = (finalTime - wastedNum);
	utili = (utili / finalTime) * 100;

	//get the turnover time
	double turnerAvg = averageT / counterThread;

	//printers
	printf("Round Robin Scheduling(quantum = %d time units)", quant);
	printf("\nTotal Time Required is %d units\n", finalTime);
	printf("Average Turnaround Time is %.2lf time units \n", turnerAvg);
	printf("CPU Utilization is %.2lf%% \n", utili);
}

void FCFS(int detail, int verbose)
{
	//declare varibales
	int counterProcess = 0;
	int switcherThread = 0;
	int switcherProcess = 0;
	int num = 0;
	int cpuTime = 0;
	int ioTime = 0;
	int procNum = 0;
	int numThread = 0;
	int totThread = 0;
	int threadNum = 0;
	int timeOfArrive = 0;
	int numCpu = 0;
	int turnaroundTimer = 0;
	int finisher = 0;
	int totturnaroundTimer = 0;
	int totiotime = 0;
	int totcputime = 0;
	int cpuTimerTurnAround = 0;
	int l = 0;
	int buff = 0;
	int SwitcherCounterThread  = 0;
	int SwitcherCounterProcess  = 0;
	int curThread = 0;
	int curProcess = 0;
	float averageTurnaround = 0;
	float cpuUtility = 0;
	//this is where the information will be put in create space for it
	int info[200][10];

	//scan info
	scanf("%d %d %d",&counterProcess, &switcherThread, &switcherProcess );
	//loop through till i <= proccessCounter
	for (int i = 1; i <= counterProcess; ++i)
	{
		//next loop scan info
		scanf("%d %d", &procNum, &numThread);
		//get the total thread
		totThread =  numThread+totThread;
		//loop through till j<= threadCount
		for (int j = 1; j <= numThread; ++j)
		{
			//scan through it
			scanf("%d %d %d", &threadNum, &timeOfArrive, &numCpu);

			int z = 1;
			for (z; z < numCpu; ++z)
			{
				//scan info
				scanf("%d %d %d", &num, &cpuTime, &ioTime);

				totcputime = totcputime+cpuTime;
				totiotime = totiotime+ ioTime;
			}
			scanf("%d %d", &num, &cpuTime);
			totcputime = totcputime+cpuTime;

			//now get the process, thread, total io, total cpu, arrival

			info[l][4] = timeOfArrive;
			info[l][3] = totcputime;
			info[l][2] = totiotime;
			info[l][0] = i;
			info[l][1] = j;

			l++;

			//set the timers to zero and then create a loop to store inforamtion
			totiotime = 0;
			totcputime = 0;

		}
	}

/*
https://c-for-dummies.com/blog/?p=1794
I used this site to use brute force to help me store the information
*/
	for (int x = 0; x < l; ++x)
	{
		for (int y = 0; y < l; ++y)
		{
			if (info[x][4] < info[y][4])
			{

				buff = info[y][4];
				info[y][4] = info[x][4];
				info[x][4] = buff;

				buff = info[y][3];
				info[y][3] = info[x][3];
				info[x][3] = buff;

				buff = info[y][2];
				info[y][2] = info[x][2];
				info[x][2] = buff;

				buff = info[y][1];
				info[y][1] = info[x][1];
				info[x][1] = buff;

				buff = info[y][0];
				info[y][0] = info[x][0];
				info[x][0] = buff;


			}
		}
	}

	if (verbose == 1)
	{
		for (int n = 0; n < l; ++n)
		{
			printf("At time: %d Thread %d of process %d moves from new to ready\n", info[n][4], info[n][1], info[n][0]);
		}
	}


	//https://www.geeksforgeeks.org/program-for-fcfs-cpu-scheduling-set-1/
	//fcfs algo
	for (int m = 0; m < l; ++m)
	{
		if (info[m][0] != curProcess)
		{
			SwitcherCounterProcess ++;
		}
		if (info[m][1] != curThread)
		{
			SwitcherCounterThread ++;
		}

		turnaroundTimer = info[m][3] + info[m][2] - info[m][4];
		finisher  = turnaroundTimer + info[m][4] + finisher ;
		//-v flag
		if (verbose == 1)
		{
			int atTimer = finisher - info[m][3] - info[m][2];
			printf("At time %d: Thread %d of Process %d moves from new to ready\n", atTimer, info[m][1], info[m][0]);
		}
		//-d flag
		if (detail == 1)
		{
			printf("\nThread %d of Process %d\n\n", info[m][1], info[m][0]);
			printf("arrival time: %d,service time: %d,i/0 time: %d,turnaround time: %d, finish time: %d\n", info[m][4], info[m][3], info[m][2], turnaroundTimer, finisher);
		}
		totturnaroundTimer = totturnaroundTimer + turnaroundTimer;
		curProcess = info[m][0];
		curThread  = info[m][1];

		//-v flag
		if (verbose == 1)
		{
			printf("At time: %d Thread %d of process %d moving from ready to running\n", finisher, info[m][1], info[m][0]);

		}

		cpuTimerTurnAround = cpuTimerTurnAround+info[m][3];
	}
	//final calculation
	cpuUtility = (cpuTimerTurnAround * 200)/(finisher );
	averageTurnaround = (totturnaroundTimer)/(totThread);

	printf("\nTotal time required is %d units\n", finisher );
	printf("Average turnaround time is %.1f time units\n", averageTurnaround);
	printf("CPU Utilization is %.0f%%\n",cpuUtility);

	return;
}
