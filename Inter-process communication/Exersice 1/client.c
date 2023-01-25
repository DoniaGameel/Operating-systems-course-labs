#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
struct msgbuff
{
    long mtype;
    char mtext[256];
};
int main()
{
    key_t key_id1;
    key_t key_id2;


    key_id1 = ftok("Up", 65);
    key_id2 = ftok("down", 65);
    int Upq_id = msgget(key_id1, 0666 | IPC_CREAT);
    int Downq_id = msgget(key_id2, 0666 | IPC_CREAT);

    if (key_id1 == -1|| key_id2 ==-1)
    {
        perror("Error in create");
        exit(-1);
    }
    char str[256] ;
    struct msgbuff messageUp;
    messageUp.mtype = getpid()%10000;

while(1){
    printf("\nclient: Enter message of max 256 character\n");
    fgets(messageUp.mtext,256,stdin);
    //strcpy(messageUp.mtext, str);
    int send_val = msgsnd(Upq_id, &messageUp, sizeof(messageUp.mtext), !IPC_NOWAIT);
    if (send_val == -1)
        perror("Errror in send");
    else
    {
        printf("\nMessage Sent: %s\n", messageUp.mtext);

        int rec_val = msgrcv(Downq_id, &messageUp, sizeof(messageUp.mtext), getpid()%10000, !IPC_NOWAIT);
        if (rec_val == -1)
            perror("Error in receive");
        else
            printf("\nreversed Message received: %s\n", messageUp.mtext);
    }


}
    return 0;
}