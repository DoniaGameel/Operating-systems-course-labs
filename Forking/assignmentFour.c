#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
//#include <sys/syscall.h>
#include <sys/wait.h>
int main(int argc, char **argv)
{
        if (argc<4)
    {
        printf("Too few arguments. Exiting!\n");
        exit(1);
    }
    char* FileName = argv[1];
    int TA_N = atoi(argv[2]);
    int p = atoi(argv[3]);
    int s=0;
    int *ptr = (int*)malloc((TA_N+1) * sizeof(int));
    int *Sol = (int*)malloc((TA_N+1) * sizeof(int));
    FILE *fptr;
    fptr=fopen(FileName,"r");
    if(fptr == NULL)
    {
      printf("Error opening file!");
      exit(1);
    }
    fscanf(fptr,"%d", &s);
    int *MidArr = (int*)malloc(s * sizeof(int));
    int *FinalArr = (int*)malloc(s * sizeof(int));
    for(int i=0 ; i<s ;i++)
    {
        fscanf(fptr,"%d", &MidArr[i]);
        fscanf(fptr,"%d", &FinalArr[i]);
    }
    int pid;
    int k=0;
    for(int i=1 ; i<=TA_N ;i++)
    {
        pid =fork();
        int num=0;
        if(pid==0)
    {
            if(i!=TA_N)
        {
            for(int j = 0;j<s/TA_N ; j++)
            {
                if ((MidArr[i*(s/TA_N)-j-1]+FinalArr[i*(s/TA_N)-j-1])>=p)
                {
                    num++;
                }
            }
            exit(num);
         }
             else
             {
                for(int j = 0;j< s-((s/TA_N)*(TA_N-1)) ; j++)
                {
                    //((i-1)*(s/TA_N))+j
                    //printf("pid= %d\n", pid);
                   // printf("%d %d\n",MidArr[((i-1)*(s/TA_N))+j]+FinalArr[((i-1)*(s/TA_N))+j],((i-1)*(s/TA_N))+j);     
                    if (MidArr[((i-1)*(s/TA_N))+j]+FinalArr[(((i-1)*(s/TA_N))+j)]>=p)
                    {
                        num++;
                    }
                }
                exit(num);
             }

    }
            else
            {
                ptr[i]=pid;
            }
    }
    for(int i = 1 ; i<=TA_N ; i++)
    {
        int stat_loc;
        int sid = wait(&stat_loc);
            if(!(stat_loc & 0x00FF))
          {
              for(int i=1 ; i<=TA_N ; i++)
                {
                    if(ptr[i]==sid)
                    Sol[i]=stat_loc>>8;
                    }

          }
    }
    for(int i = 1 ; i <= TA_N ; i++)
    {
        printf("%d ",Sol[i]);
    }
    printf("\n");
    fclose(fptr);
    return 0;
}