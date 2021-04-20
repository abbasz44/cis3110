Assignment 3
Zachery Abbas
1019463
Due: March, 19

HOW TO RUN:
on the command line:
make

this will compile the two different programs holes.c and dine.c below are two examples how to execute
 ./dine 3 4
 ./holes fileName best

dine.c

In this program I have two functions one that joins, creates and mutexs, I have created two global variables and a mutex variable,
I use the global variabeles instad of getter functions just felt easier, I needed these variables because i use them in the loop through in my second function.

In the second function this is where it determines if it needs to eat, if it is thinking. whats it is locked its eating once it is not locked it is thinking.

in the main just get the proper arguments set to the functions and then print out.

holes.c

In this progam I did data structures:

typedef struct process
{
    char name;
    int timer;
    int volume;
    int size;
    struct process *next;
    int spacer;

}proc, space;

in this data structure I use it for the process and the memory i store names, volumes, timers, sizes and then the struct process next will whoich get the next term

I then create a number of helper functions.
  I create two getter functions then get the info for space and get info for memory and a merger function when getting the temp info I passs it and put in the merege function.
  in these functions i use them instead of global variabes cause i use them alot and it was alot easier to use these getter functions instead of actually trying to do this prosess everytime
  I needed inforation through the bestfit, nextfit, worstfit, firstfit

I then use a pop and push function, wiith these functions I got the insperation on line and put the website in the code. i use this info through the pushes and pops needed on the stack
and usinig these when need be

I use a count holes function when it comes to trying to find the current hole weather it is needed to loop through or get the final informstion of the amount of holes. this was a very simple algo
but very effective.

Lastly I used two printer functions one for the satus and one for the final feed.

with the worst fit best fit next fit and first fit, i used basically very similar frame work for all of them incoperating the helper functions above, I used geeksquad on the insperation
of these algos to completly understand I also looked at the different types of code that was in class or on courselink.
