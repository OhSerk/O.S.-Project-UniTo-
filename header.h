#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>

#define NTEAM 20
#define NBALL 12
#define LOG_FILE "log.txt"
#define FORK_ERROR case -1: perror("Fork"); break;
#define MSG_EXCEPT      020000  /* recv any msg except of specified type.*/
#define TRUE 1
#define FALSE 0
#define ERR -1
#define NGIOC 5
#define KEY_PALLA 1234
#define KEY_CAMBIO 1002
#define KEY_MSG 3452
#define KEY_SHM 6666
#define KEY_TALK 234
#define KEY_REPLY 6578

char * azione[] = {"Goal","Dribbling","Infortunio"};

struct mymsg {
	long mtype; /* Message type */
	char mtext[256]; /* Message body */
};

struct my_data{
	int PID;
};

union semun {
	int val;
	struct semid_ds* buf;
	unsigned short* array;
#if defined(__linux__)
	struct seminfo* __buf;
#endif
};

int initSemAvailable(int semId, int semNum) {
	union semun arg;
	arg.val = 1;
	return semctl(semId, semNum, SETVAL, arg);
}
int initSemInUse(int semId, int semNum) {
	union semun arg;
	arg.val = 0;
	return semctl(semId, semNum, SETVAL, arg);
}

int reserveSem(int semId, int semNum) {
	struct sembuf sops;
	sops.sem_num = semNum;
	sops.sem_op = -1;
	sops.sem_flg = 0;
	return semop(semId, &sops, 1);
}
int releaseSem(int semId, int semNum) {
	struct sembuf sops;
	sops.sem_num = semNum;
	sops.sem_op = +1;
	sops.sem_flg = 0;
	return semop(semId, &sops, 1);
}
int removeSem(int semId,int semNum){
	return semctl(semId,semNum,IPC_RMID);
}
int nuovoSemaforo(int nSem){
	return semget(IPC_PRIVATE, nSem, 0666 | IPC_CREAT);
}


const char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
    }
    return result;
  }