#include "header.h"

const char * ASSEGNA_TEAM(int val);
void tempo(int sig); //Funzione per gestire i segnali
int PALLA,MSGID,CAMBIO; //Vari semafori
struct mymsg MSG; //Coda di messaggi 
int POINT[NTEAM]; //Segnao conto del punteggio delle due squadre
int TIME_P;
char buff[64];

int main(int argc, char const *argv[]){
	FILE *myData = fopen("config.txt","r");
	fscanf(myData,"%3d",&TIME_P);
	fclose(myData);
	PALLA = semget(KEY_PALLA, 1,  0600 | IPC_CREAT ); //Creo il semaforo per il pallone
	MSGID = msgget(KEY_MSG, 0600 | IPC_CREAT ); //Creo la coda di messaggi
	CAMBIO = semget(KEY_CAMBIO, NTEAM, 0600 | IPC_CREAT); //Creo il semaforo per la squadra A e B per effettuare il cambio
	initSemInUse(PALLA,0);
	for(int i = 0; i < NTEAM; i++){
		initSemInUse(CAMBIO,i);
		POINT[i] = 0;
	}
	struct sigaction si,si1,si_old,si1_old;
	si.sa_handler = &tempo;
	si.sa_flags = 0;
	if(sigaction(SIGALRM,&si,&si_old)==ERR)perror("SIGALRM");
	if(signal(SIGTERM, &tempo)==SIG_ERR)perror("SIGTERM");
	switch(fork()){
		FORK_ERROR //Macro definita in header.h
		case 0:
		execl("fato","FATO","",NULL); //Eseguo il fato
		exit(1); //Uccido il figlio nel caso la execl fallisce
		break;
		default:
		sleep(2);
		break;
	}

	for(int i = 0; i < NTEAM; i++){
		switch(fork()){
			FORK_ERROR
			case 0:
			execl("squadra","SQ",ASSEGNA_TEAM(i),NULL); //ASSEGNA_TEAM
			perror("execl");
			exit(1);
			break;
			default:
			sleep(2);
			break;
		}
	}
	dprintf(1,"Inizio partita!\nDurata della partita: %d secondi!",TIME_P);
	alarm(TIME_P);
	while(TRUE){
		if(msgrcv(MSGID, &MSG ,sizeof(MSG.mtext), 123,0)==ERR){
			perror("PUNTEGGIO");
			exit(ERR);
		} else {
			int i = (int)(MSG.mtext[0])-65;
			POINT[i]++;
			dprintf(1,"========== PUNTEGGIO ==========\n");
			dprintf(1,"Squadra %s: %d\n", ASSEGNA_TEAM(i), POINT[i]);
			
		}
	}
	return dprintf(1,"Fine main\n");
}

void tempo(int sig){
	int  i;
	switch(sig){
		case SIGALRM:
		dprintf(1,"========== FINE PARTITA ==========\n");
		for(int i = 0; i < NTEAM; i++){
			dprintf(1,"Squadra %s: %d\n", ASSEGNA_TEAM(i), POINT[i]);
		}
		kill(0,SIGTERM);
		msgctl(MSGID, IPC_RMID, NULL); //Rimuovo la coda di messaggi
		removeSem(PALLA,0); //Rimuovo i semafori creati
		for(int i = 0; i < NTEAM; i++)
			removeSem(CAMBIO,i);
		exit(EXIT_SUCCESS);
		break;
		case SIGTERM:
		wait(0);
		break;
		default:
		dprintf(1,"Si è verificato un errore, riprovare\n");
	}
}

const char * ASSEGNA_TEAM(int val){
	sprintf(buff,"%c",(char)(65+val));
	return buff;
}