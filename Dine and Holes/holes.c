#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//create data structure
typedef struct process
{
    char name;
    int timer;
    int volume;
    int size;
    struct process *next;
    int spacer;

}space,proc;

//function prototyoes
void firstPrinter(int currentProcess,int taken,int totalMemory,int i, int currentHole);
void printEnd(int totalLoads, int totalProcess, int totalHoles, int totalMemory, int i);
proc *push(proc *process,int name, int size, int volume);
proc *getName(char name,int volume, int size);
space *merge(space *memory);
space *getSpace(char name, int spacer, int volume,int timer,  int size);
proc *pop(proc **process);
int countHoles(space * memory);
void fitFirstFunc(int looper, proc *process);
void nextFitFunc(int looper, proc *process);
void bestFitFunc(int looper,proc *process);
void worstFitFunc(int looper,proc *process);


int main(int argc, char const *argv[])
{
	char express [300];
  char* size;
  int lenOfSize;
  int looper;
  proc *process = NULL;
	FILE *fp;
	int i = 0;
	char* temp;
	int temp2;

  //opens file inserted through argument
	fp = fopen(argv[1], "r");


//loops through file
	while(!feof(fp))
	{
		while (fgets(express,300,fp) != NULL)
		{
      //gets len and make suere null character is parsed
			express[strlen(express)-1] = '\0';

      //parse through file
			temp = strtok(express, " ");
			temp2 = atoi(temp);
			size = strtok(NULL, temp);
			lenOfSize = atoi(size);

      //use simple push function
			process = push(process,temp2,lenOfSize,0);
			i++;
		}
	}

  //create if statments for arguments
	looper = i*3;
  if(strcmp(argv[2], "first") == 0){
	// printf("First fit\n");
	fitFirstFunc(looper,process);
  }
  else if(strcmp(argv[2], "next") == 0){
	// printf("Next fit\n");
	nextFitFunc(looper, process);
  }
  else if(strcmp(argv[2], "best") == 0){
    // printf("Best fit\n");
    bestFitFunc(looper, process);
  }
  else if(strcmp(argv[2], "worst") == 0){
	// printf("Worst fit\n");
	worstFitFunc(looper,process);
  }
  else{
    printf("invalid argument\n");
  }

	return 0;
}

//first fit function
//https://www.geeksforgeeks.org/program-first-fit-algorithm-memory-management/
void fitFirstFunc(int looper, proc *process)
{
    //declare variables
    int i;
    int currentProcess=0;
    int loadNum=0;
    int memoryNum=0;
    int older=0;
    int processNum=0;
    int taken=0;
    char dashString = '-';
    int numOfHoles=1;
    int currentHole=1;
    int worker = 1;

    proc *cloner;
    proc *temp;

    temp = process;
    cloner= NULL;

    while(temp!=NULL)
    {
        //use push function
        cloner=push(cloner,temp->name,temp->size,0);
        temp=temp->next;
    }
    space *memory;
    //space obj
    //range 1...128
    memory = getSpace(dashString,1,0,0, 128);

    //for loop going through the less then looper
    for(i=0;i < looper;++i)
    {
        //create objects
        space *temp;
        proc *adder;

        //set them equal
        temp = memory;
        //use pop function
        adder = pop(&cloner);

        //when the work ==1 loop
        while(worker == 1)
        {
            temp = memory;

            //sue merge function
            temp = merge(temp);

            //counter of holes
            currentHole = countHoles(temp);

            if(temp->next ==NULL||temp->spacer)
            {
                if(adder->size<temp->size+1)
                {
                    space *spaceNew;
                    space *memoryNew;

                    //call the get space function on these objects
                    spaceNew = getSpace(dashString,1,0,0, temp->size-adder->size);
                    memoryNew = getSpace(adder->name,0,adder->volume,i, adder->size);

                    if(adder->size != temp->size)
                    {
                        spaceNew->next = temp->next;
                        memoryNew->next = spaceNew;

                    }
                    else
                    {
                        memoryNew->next=temp->next;
                    }
                    //iterate
                    currentProcess++;
                    loadNum++;

                    memory = memoryNew;
                    taken = (memoryNew->size) + taken;
                    memoryNum = (taken*100) + memoryNum;
                    numOfHoles = currentHole + numOfHoles;
                    processNum = currentProcess + processNum;

                    //print stuff when cue isnt empty
                    firstPrinter(currentProcess,taken,memoryNum,i,currentHole);
                    break;
                }
            }


            while(temp->next != NULL)
            {

                if((adder->size+1 < temp->next->size) && (temp->next->spacer))
                {
                    currentProcess++;

                    numOfHoles = currentHole + numOfHoles;
                    processNum= currentProcess + processNum;

                    space *memoryNew;
                    space *spaceNew;

                    //use get function for these
                    memoryNew =getSpace(adder->name,0,adder->volume,i,adder->size);
                    spaceNew =getSpace(dashString,1,0,0,(temp->next->size)-(adder->size));

                    loadNum++;

                    if(adder->size == temp->next->size)
                    {
                        memoryNew->next=temp->next->next;
                    }
                    else
                    {

                        spaceNew->next = temp->next->next;
                        memoryNew->next = spaceNew;
                    }

                    temp->next = memoryNew;
                    taken = (memoryNew->size) + taken ;
                    memoryNum =(taken*100) +  memoryNum;
                    firstPrinter(currentProcess,taken,memoryNum,i, currentHole);
                    goto freeArea;
                }
                temp = temp->next;
            }

            space *leave;
            leave = memory;
            while(leave!=NULL)
            {
                if((!(leave->spacer) && leave->timer == older))
                {
                    taken -=leave->size;
                    //use push function
                    cloner = push(cloner,leave->name,leave->size,leave->volume+1);
                    //set to 1
                    leave->spacer=1;

                    //increment and decrement
                    currentProcess--;
                    older++;
                    //break
                    break;
                }
                leave=leave->next;
            }
        }
        freeArea: free(adder);
    }
    //print final line
    printEnd(loadNum, processNum, numOfHoles, memoryNum, i);
}

//https://www.geeksforgeeks.org/program-for-next-fit-algorithm-in-memory-management/
//used the cite for this algo
void nextFitFunc( int looper,proc *process)
{
    //declare varibales
    int i;
    int worker = 0;
    int loadNum=0;
    int memoryNum=0;
    int taken=0;
    int errorFlag = 0;
    int older=0;
    int processNum=0;
    int currentProcess=0;
    int numOfHoles=1;
    int currentHole=1;

    char saverd;
    char dashString = '-';
    char lastString = '-';

    //procc obj
    proc *cloner;
    proc *tempMem;
    cloner = NULL;
    tempMem = process;

    while(tempMem != NULL)
    {
        //use push function
        cloner = push(cloner,tempMem->name,tempMem->size,0);
        tempMem = tempMem->next;
    }
    space *memory;
    memory = getSpace(dashString,1,0,0,128);

    //loop through
    for(i=0;i < looper;++i)
    {
        space *temp;
        proc *adder;

        temp = memory;
        adder= pop(&cloner);

        //set the worker == 0
        worker = 0;

        while(worker != 1)
        {
            temp = memory;

            //use merge on the temp
            temp = merge(temp);

            //get the current by using the counter function
            currentHole = countHoles(temp);

            if(lastString == '-')
            {
                //iterate
                currentProcess++;

                processNum= currentProcess + processNum;
                numOfHoles = currentHole + numOfHoles ;

                //space obj
                space *spaceNew;
                space *memoryNew;

                spaceNew = getSpace(dashString,1,0,0,temp->size-adder->size);
                memoryNew = getSpace(adder->name,0,adder->volume,i,adder->size);

                memoryNew->next = spaceNew;
                memory = memoryNew;

                taken = memoryNew->size + taken;
                memoryNum =  (taken*100) + taken;

                //iterate
                loadNum++;
                lastString = adder->name;
                firstPrinter(currentProcess,taken,memoryNum,i,currentHole);
                //set the worker ==1
                worker = 1;

                //free string
                goto freeArea;
            }

            while((temp->name != lastString) && (temp->next!=NULL))
            {
                temp=temp->next;
            }

            //set error flag
            errorFlag = 0;
            //set saverd to the last string
            saverd = lastString;

            while(1)
            {
              if((temp->next!=NULL) && (errorFlag == 1) && (saverd == temp->next->name))
              {
                  break;
              }
              if(temp->next!=NULL && temp->next->spacer && temp->next->size >= adder->size)
                {

                    //space obj
                    space *memoryNew;
                    space *spaceNew;
                    //get space function
                    memoryNew = getSpace(adder->name,0,adder->volume,i,adder->size);
                    spaceNew = getSpace(dashString,1,0,0,temp->next->size-adder->size);

                    if(temp->next->size==adder->size)
                    {
                        //free space
                        free(spaceNew);
                        memoryNew->next = temp->next->next;
                    }
                    else
                    {
                      //set the next
                      memoryNew->next = spaceNew;
                      spaceNew->next = temp->next->next;

                    }

                    //iterate
                    loadNum++;
                    currentProcess++;
                    //get last string
                    lastString = adder->name;

                    temp->next = memoryNew;

                    processNum = currentProcess + processNum;
                    numOfHoles = currentHole + numOfHoles ;
                    taken = (memoryNew->size)+ taken;
                    memoryNum = (taken*100) + memoryNum;

                    firstPrinter(currentProcess,taken,memoryNum,i, currentHole);

                    //go to the free area to free the string
                    goto freeArea;

                }

                if(temp->next == NULL)
                {

                    lastString = adder->name;

                    if(memory->spacer && memory->size >= adder->size)
                    {
                        space *spaceNew;
                        space *memoryNew;
                        spaceNew = getSpace(dashString,1,0,0,memory->size-adder->size);
                        memoryNew = getSpace(adder->name,0,adder->volume,i,adder->size);

                        if(memory->size == adder->size)
                        {

                            free(spaceNew);
                            memoryNew->next = temp->next->next;
                        }
                        else
                        {
                          memoryNew->next=spaceNew;
                          spaceNew->next = memory->next;
                        }
                        //set last string to adder->name
                        lastString = adder->name;

                        //iterate
                        loadNum++;
                        currentProcess++;

                        //set memoryu
                        memory = memoryNew;

                        taken = (memoryNew->size) + taken;
                        memoryNum = (taken*100) + memoryNum;
                        processNum = currentProcess + processNum;
                        numOfHoles = currentHole + numOfHoles;

                        //printer
                        firstPrinter(currentProcess,taken,memoryNum,i, currentHole);
                        //go to free area
                        goto freeArea;
                    }
                    else if( adder->size > memory->size && memory->spacer)
                    {
                        temp = memory;
                        break;
                    }

                    temp = memory;
                    break;
                }
                //if not above
                else
                {
                    temp = temp->next;
                }
                //sett error flag to 1
                errorFlag = 1;
            }

            space *leave;
            leave = memory;
            while(leave != NULL)
            {
                if((leave->timer == older)&&!(leave->spacer))
                {
                    //decremment and increment
                    currentProcess--;
                    older++;
                    //set taken
                    taken = leave->size;

                    //use push
                    cloner = push(cloner,leave->name,leave->size,leave->volume+1);
                    //set leave spacer
                    leave->spacer = 1;
                    break;
                }
                leave=leave->next;
            }
        }
        //FREE AREA when ever there is a fre needed get code to go to there
        freeArea: free(adder);
    }
    //last printer
    printEnd(loadNum, processNum, numOfHoles, memoryNum, i);
}
//https://www.geeksforgeeks.org/program-best-fit-algorithm-memory-management/
//use this cite for the algo
void bestFitFunc(int looper, proc *process)
{
    //declare variables
    int i;
    int taken=0;
    int counter;
    int minimum;
    int saver;
    int loadNum=0;
    int memoryNum=0;
    int failFlag = 0;
    int older=0;
    int currentProcess=0;
    int processNum=0;
    int worker = 1;
    int numOfHoles=1;
    int currentHole=1;

    char dashString = '-';

    //process obj
    proc *cloner;
    proc *temp;

    cloner = NULL;
    temp = process;

    while(temp!=NULL)
    {
        //use push function
        cloner=push(cloner,temp->name,temp->size,0);
        temp=temp->next;
    }
    //space obj
    space *memory;
    memory = getSpace(dashString,1,0,0,128);

    for(i=0;i < looper;++i)
    {
        space *temp;
        proc *adder;

        temp = memory;
        adder = pop(&cloner);

        //whent the worker == 1
        while(worker == 1)
        {
            //set temp to memory then use the merge function
            temp = memory;
            temp = merge(temp);

            //use the countholes function to get the current Hole
            currentHole = countHoles(temp);

            if(temp->next ==NULL||temp->spacer)
            {
              //todo break
                if(adder->size <= temp->size )
                {
                    space *spaceNew;
                    space *memoryNew;
                    spaceNew = getSpace(dashString,1,0,0,temp->size-adder->size);
                    memoryNew = getSpace(adder->name,0,adder->volume,i,adder->size);

                    if(temp->size == adder->size)
                    {

                        memoryNew->next=temp->next;
                    }
                    else
                    {
                      memoryNew->next = spaceNew;
                      spaceNew->next = temp->next;
                    }

                    loadNum++;
                    currentProcess++;

                    //equations
                    memory = memoryNew;
                    taken = (memoryNew->size) + taken;
                    memoryNum = (taken*100) + memoryNum;
                    processNum = currentProcess + processNum;
                    numOfHoles =  currentHole + numOfHoles;

                    firstPrinter(currentProcess,taken,memoryNum,i, currentHole);
                    break;
                }
            }

            //set these variables min == 100
            minimum = 100;
            saver = 2;
            counter=0;

            temp = memory;

            while(temp->next != NULL)
            {
                if((adder->size+1 < temp->next->size) && (temp->next->spacer))
                {
                    if(((temp->next->size)-(adder->size)) < minimum)
                    {
                        //set min and saver
                        minimum = temp->next->size-adder->size;
                        saver = counter;
                    }
                    //iterate
                    counter++;
                }
                //set temp
                temp = temp->next;
            }
            //set temp to memory
            temp=memory;

            //once min doest == 100
            if(minimum != 100)
            {
                if(saver == 2)
                {
                    if(((temp->size < adder->size) || !temp->spacer))
                    {
                        break;
                    }

                    space *spaceNew;
                    space *memoryNew;
                    spaceNew = getSpace(dashString,1,0,0,temp->size-adder->size);
                    memoryNew = getSpace(adder->name,0,adder->volume,i,adder->size);

                    if(temp->size==adder->size)
                    {

                        free(spaceNew);
                        memoryNew->next=temp->next;
                    }
                    else
                    {
                      memoryNew->next=spaceNew;
                      spaceNew->next = temp->next;
                    }
                    //iterate
                    currentProcess++;
                    loadNum++;

                    //equations
                    memory = memoryNew;

                    taken = (memoryNew->size) + taken;
                    memoryNum = (taken*100)+memoryNum;
                    processNum = currentProcess + processNum;
                    numOfHoles = currentHole + numOfHoles;
                    //print info
                    firstPrinter(currentProcess,taken,memoryNum,i, currentHole);

                    //free stringh
                    goto freeArea;
                }
                else
                {
                    //set flag
                    failFlag = 0;
                    while(temp->next!=NULL)
                    {

                        if(temp->next->spacer)
                        {
                            failFlag++;
                        }

                        if(failFlag == saver+1)
                        {
                            break;
                        }
                        temp=temp->next;
                    }

                    //increment
                    //set worker to 1
                    worker = 1;
                    loadNum++;
                    currentProcess++;

                    //equations
                    numOfHoles = currentHole + numOfHoles;
                    processNum = currentProcess + processNum;

                    space *memoryNew;
                    space *spaceNew;

                    memoryNew=getSpace(adder->name,0,adder->volume,i,adder->size);
                    spaceNew = getSpace(dashString,1,0,0,temp->next->size-adder->size);

                    if(temp->next->size==adder->size)
                    {
                        free(spaceNew);
                        memoryNew->next=temp->next->next;
                    }
                    else
                    {
                      memoryNew->next=spaceNew;
                      spaceNew->next = temp->next->next;

                    }

                    //free
                    free(temp->next);
                    temp->next = memoryNew;

                    taken = memoryNew->size + taken;
                    memoryNum = (taken*100) + memoryNum;
                    firstPrinter(currentProcess,taken,memoryNum,i, currentHole);

                    goto freeArea;
                }
            }

            space *leave;
            leave = memory;
            while(leave!=NULL)
            {
                if(!(leave->spacer) && (leave->timer == older))
                {
                    taken-=leave->size;
                    //use push functiom
                    cloner = push(cloner,leave->name,leave->size,leave->volume+1);

                    //decrement and increment
                    currentProcess--;
                    older++;

                    //set too 1
                    leave->spacer = 1;
                    break;
                }
                leave=leave->next;
            }
        }
        //FREE AREA when need to free goes here
        freeArea: free(adder);
    }
    //printer
    printEnd(loadNum, processNum, numOfHoles, memoryNum, i);
}
//https://www.geeksforgeeks.org/worst-fit-allocation-in-operating-systems/
//worst fit algo 
void worstFitFunc(int looper,proc *process)
{

    int i;
    int counter;
    int max;
    int saver;
    int failFlag;
    int older=0;
    int processNum=0;
    int currentProcess=0;
    int loadNum=0;
    int memoryNum=0;
    int taken=0;
    int numOfHoles=1;
    int currentHole=1;
    int worker = 1;
    char dashString = '-';


    proc *cloner;
    proc *temp;
    cloner = NULL;
    temp = process;

    while(temp!=NULL)
    {
        //use push functiom
        cloner=push(cloner,temp->name,temp->size,0);
        temp=temp->next;
    }
    //1..128
    space *memory;
    memory = getSpace(dashString,1,0,0,128);


    //looper
    for(i=0;i < looper;++i)
    {
        //obj
        space *temp;
        proc *adder;

        temp= memory;
        adder = pop(&cloner);

        //when worker == 1
        while(worker == 1)
        {
            temp = memory;
            //use merger and counter
            temp = merge(temp);
            currentHole = countHoles(temp);

            if(temp->next ==NULL || temp->spacer)
            {
                if(adder->size <= temp->size )
                {
                    space *memoryNew;
                    space *spaceNew;
                    spaceNew = getSpace(dashString,1,0,0,temp->size-adder->size);
                    memoryNew = getSpace(adder->name,0,adder->volume,i,adder->size);

                    if(temp->size == adder->size)
                    {

                        memoryNew->next=temp->next;
                    }
                    else
                    {
                      memoryNew->next = spaceNew;
                      spaceNew->next = temp->next;
                    }

                    //iterate
                    currentProcess++;
                    loadNum++;

                    //set memory
                    memory = memoryNew;

                    processNum = currentProcess + processNum;
                    numOfHoles = currentHole + numOfHoles;
                    taken = (memoryNew->size) + taken;
                    memoryNum = (taken*100) + memoryNum;
                    //printer function called
                    firstPrinter(currentProcess,taken,memoryNum,i, currentHole);
                    break;
                }
            }
            //set these varibales
            saver = max = 2;
            counter = 0;
            temp = memory;

            while(temp->next!=NULL)
            {
                if(adder->size <= temp->next->size && temp->next->spacer)
                {
                    if(temp->next->size-adder->size > max)
                    {
                        max = temp->next->size-adder->size;
                        saver = counter;
                    }
                    counter++;
                }
                temp = temp->next;
            }
            //set temp to memeory
            temp=memory;

            if(max != 2)
            {
                if(saver == 2)
                {
                    if((temp->size < adder->size)||(!temp->spacer))
                    {
                        break;
                    }

                    space *memoryNew;
                    space *spaceNew;
                    memoryNew = getSpace(adder->name,0,adder->volume,i,adder->size);
                    spaceNew = getSpace(dashString,1,0,0,temp->size-adder->size);

                    if(temp->size == adder->size)
                    {

                        free(spaceNew);
                        memoryNew->next=temp->next;
                    }
                    else
                    {
                      memoryNew->next = spaceNew;
                      spaceNew->next = temp->next;
                    }
                    //iterate
                    currentProcess++;
                    loadNum++;

                    //set variable
                    memory = memoryNew;

                    processNum = currentProcess + processNum;
                    numOfHoles = currentHole + numOfHoles;
                    taken = (memoryNew->size) + taken;
                    memoryNum = (taken*100) + memoryNum;

                    //printer function
                    firstPrinter(currentProcess,taken,memoryNum,i,currentHole);

                    //free info
                    goto freeArea;
                }
                else
                {
                    //set fail flag
                    failFlag = 0;

                    while(temp->next != NULL)
                    {
                        if(temp->next->spacer)
                        {
                            failFlag++;
                        }

                        if(failFlag == saver+1)
                        {
                            break;
                        }
                        temp = temp->next;
                    }
                    loadNum++;
                    currentProcess++;

                    worker = 1;

                    processNum = currentProcess + processNum;
                    numOfHoles = currentHole +  numOfHoles;

                    space *memoryNew;
                    space *spaceNew;
                    spaceNew = getSpace(dashString,1,0,0,temp->next->size-adder->size);
                    memoryNew = getSpace(adder->name,0,adder->volume,i,adder->size);

                    if(temp->next->size==adder->size)
                    {

                        free(spaceNew);
                        memoryNew->next = temp->next->next;
                    }
                    else
                    {
                      memoryNew->next=spaceNew;
                      spaceNew->next = temp->next->next;
                    }

                    //free
                    free(temp->next);
                    temp->next = memoryNew;

                    taken = (memoryNew->size) + taken;
                    memoryNum =(taken*100) +  memoryNum;

                    //printer
                    firstPrinter(currentProcess,taken,memoryNum,i,currentHole);
                    goto freeArea;
                }
            }

            //space obj
            space *leave;
            leave = memory;

            while(leave != NULL)
            {
                if(!leave->spacer && leave->timer == older)
                {
                    //iterate
                    older++;
                    currentProcess--;

                    taken-=leave->size;
                    //pusg function
                    cloner = push(cloner,leave->name,leave->size,leave->volume+1);

                    //set leave sapcer
                    leave->spacer=1;
                    break;
                }
                leave = leave->next;
            }
        }
        //free Area when needed for free
        freeArea: free(adder);
    }
    //printer
    printEnd(loadNum, processNum, numOfHoles, memoryNum, i);
}
void firstPrinter(int currentProcess,int taken,int totalMemory,int i, int currentHole)
{
    int memUsage = (taken*100)/128;
    int cumal = totalMemory/((i+1)*128);

    printf("pid loaded,  #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n", currentProcess, currentHole, memUsage, cumal);

    return;
}

void printEnd(int totalLoads, int totalProcess, int totalHoles, int totalMemory, int i)
{
    int avgPro = totalProcess/i;
    int avgHol = totalHoles/i;
    int totMem = totalMemory/(i*128);
    printf("Total loads = %d, average #processes = %d, average #holes = %d, cumulative %%mem = %d\n", totalLoads, avgPro, avgHol, totMem);

    return;
}

int countHoles(space *memory)
{
    space *buf;
    buf = memory;
    int counter = 0;

    //simple counter function to count the amount of holes
    while(buf!=NULL)
    {
        if(buf->spacer)
        {
            counter++;
        }
        buf = buf->next;
    }
    return counter;
}

space *merge(space *memory)
{
    space * temp;
    space * temp2;

    temp = memory;
    temp2 = memory;

    //when temp->next == null
    if(temp->next == NULL)
    {
        return temp;
    }

    while(temp2->next->next != NULL)
    {
        if(temp2->next->next->spacer && temp2->next->spacer)
        {
            //merger algorithm
            space *temp;

            temp = getSpace('-',1,0,0,(temp2->next->size)+(temp2->next->next->size));
            temp->next = temp2->next->next->next;
            temp2->next = temp;
            continue;
        }
        temp2=temp2->next;
    }


    return temp;
}

//next two functions getter functions
space *getSpace(char name, int spacer, int volume,int timer, int size)
{
    space *temp = malloc(sizeof(space)+1);

    temp->name=name;
    temp->volume=volume;
    temp->timer=timer;
    temp->size=size;
    temp->spacer=spacer;
    temp->next=NULL;

    return temp;
}

proc *getName(char name,int volume, int size)
{
    proc *temp = malloc(sizeof(proc)+1);

    temp->name=name;
    temp->volume=volume;
    temp->size=size;
    temp->next=NULL;

    return temp;
}

//simple push and pop functions
//help from https://www.quora.com/How-do-I-write-push-and-pop-functions-in-C

proc *push(proc *process,int name, int size, int volume)
{
    proc *temp = process;
    if(temp == NULL)
    {
        return getName(name,volume,size);
    }

    while(temp->next!=NULL)
    {
        temp = temp->next;
    }
    temp->next = getName(name,volume,size);
    return process;
}

proc *pop(proc **process)
{
    proc *toReturn = (*process);
    if(!toReturn)
    {
        return NULL;
    }
    else
    {
        *process=(*process)->next;
    }
    return toReturn;
}
