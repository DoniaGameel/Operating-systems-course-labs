
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
 void handler(int signum);
 int *Pnum;
 int val;
int main(int argc, char **argv)
{
    Pnum = (int*)malloc (2*sizeof(int));
    val = atoi(argv[1]);
    int n = argc-2;
    int *Arr = (int*)malloc(n * sizeof(int));

    for(int i=0 ; i<n ;i++)
    {
        Arr[i]=atoi(argv[i+2]);
    }
     int pid=getpid();
     printf("I am the parent, PID = %d \n", pid);

     int n1 = fork();
     int n2 = fork();

    if(n1>0 && n2>0)
    {
         sleep(5);
         for(int i = 0 ; i<2 ; i++)
        {
        int stat_loc;
        wait(&stat_loc);
        }
       printf("Value Not Found");
       exit(0);
    }
     if(n1==0 && n2>0)
     {
         //First Child
        Pnum[0]=pid;
        printf("I am the First Child, PID = %d \n", pid);
        for(int i =0 ; i<n/2 ; i++)
        {
            if(Arr[i]==val)
            {
                 signal (SIGUSR1, handler);
                 exit(i);
            }
        }
        sleep(3);
        printf("Child 1 Terminate ");
        exit(0);
     }
     if(n2==0 && n1>0)
     {
         // Second Child
         Pnum[1]=pid;
         printf("I am the Second Child, PID = %d \n", pid);

        for(int i =n/2 ; i<n ; i++)
        {
            if(Arr[i]==val)
            {
                signal (SIGUSR1, handler);
                exit(i);
            }
        }
        sleep(3);
        printf("Child 2 Terminate ");
        exit(0);
     }



    return 0;
}
void handler(int signum)
    {
     for(int i = 0 ; i<2 ; i++)
    {
        int stat_loc;
        int sid = wait(&stat_loc);
        if(!(stat_loc & 0x00FF))
        {
         for(int i=0 ; i<2 ; i++)
            {
                if(Pnum[i]==sid)
                {
                    printf("Child %d : Value %d found at position %d \n", i+1, val,stat_loc>>8);
                    killpg(getpgrp(),SIGKILL);
                }
            }
        }
    }
    printf("Value Not Found");
    exit(0);
  }