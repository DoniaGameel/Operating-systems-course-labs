#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
struct msgbuff
{
    long mtype;
    char mtext[256];
};
 void handler(int signum);
 int Downq_id;
 int Upq_id;
int main()
{
    key_t key_id1;
    key_t key_id2;

    signal(SIGINT,handler);

    key_id1 = ftok("Up", 65);
    key_id2 = ftok("down", 65);
    Upq_id = msgget(key_id1, 0666 | IPC_CREAT);
    Downq_id = msgget(key_id2, 0666 | IPC_CREAT);

    if (key_id1 == -1|| key_id2 ==-1)
    {
        perror("Error in create");
        exit(-1);
    }
    struct msgbuff messageUp;

    while(1)
    {
        int rec_val = msgrcv(Upq_id, &messageUp, sizeof(messageUp.mtext),0, !IPC_NOWAIT);
        if (rec_val == -1)
            perror("Error in receive");
        else
        {
            char str[256];
            char reversed[256];
            strcpy(str,messageUp.mtext);
            //revering message
            for (int i=0; i<=strlen(str); ++i)
                reversed[i] = str[strlen(str)-i-1];
	     printf("\nWill Reverse: %s\n", str);
            strcpy(messageUp.mtext, reversed);
            int send_val = msgsnd(Downq_id, &messageUp, sizeof(messageUp.mtext), !IPC_NOWAIT);
            if (send_val == -1)
                perror("Errror in send");
        }


    }
    return 0;
}

void handler(int signum)
{
    msgctl(Downq_id, IPC_RMID, (struct msqid_ds *)0);
    msgctl(Upq_id, IPC_RMID, (struct msqid_ds *)0);
    exit(0);
}