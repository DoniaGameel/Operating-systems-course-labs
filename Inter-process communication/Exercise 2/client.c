#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
int shmid;
int sem1;
int sem2;
union Semun
{
    int val;               /* value for SETVAL */
    struct semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
    ushort *array;         /* array for GETALL & SETALL */
    struct seminfo *__buf; /* buffer for IPC_INFO */
    void *__pad;
};
void up(int sem)
{
    struct sembuf v_op;

    v_op.sem_num = 0;
    v_op.sem_op = 1;
    v_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &v_op, 1) == -1)
    {
        perror("Error in up()");
        exit(-1);
    }
}
void down(int sem)
{

    struct sembuf p_op;

    p_op.sem_num = 0; /*Semapore index in the array*/
    p_op.sem_op = -1; /*Semapore operation */
    p_op.sem_flg = !IPC_NOWAIT; /*Operation flag*/

    if (semop(sem,/*pinter to arrayn of ops*/ &p_op, /*# of OPs in this array*/1) == -1)
    {//Returns 0 on success
        perror("Error in down()");
        exit(-1);
    }
}
int main()
{
    key_t key_id;
    key_id = ftok("Up", 65);
    key_t key_id2;
    key_id2 = ftok("down", 65);
    shmid = shmget(key_id, 4096, IPC_CREAT | 0644);
    //1 is the # of sem sets
    sem1 = semget(key_id, 1, 0666 | IPC_CREAT);
    sem2 = semget(key_id2, 1, 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    if (sem1 == -1 || sem2 == -1)
    {
        perror("Error in create sem");
        exit(-1);
    }
    //argument for sem control systrm call
    union Semun semun;
    semun.val = 0; /* initial value of the semaphore, Binary semaphore */
    if (semctl(sem1, 0, SETVAL, semun) == -1) // set initial value of sem 0 in the sem1 set = semun.val
    {
        perror("Error in semctl");
        exit(-1);
    }
    if (semctl(sem2, 0, SETVAL, semun) == -1)
    {
        perror("Error in semctl");
        exit(-1);
    }

    void *shmaddr = shmat(shmid, (void *)0, 0);
    if ((long)shmaddr == -1)
    {
        perror("Error in attach in writer");
        exit(-1);
    }
    char str[256] ;
    while(1)
    {
        printf("\nclient: Enter message of max 256 character:\n");
        fgets(str,256,stdin);
        strcpy((char *)shmaddr, str);
        up(sem1);
        down(sem2);
        printf("\nclient: Reversed Message %s\n",(char *)shmaddr);
    }

    return 0;
}