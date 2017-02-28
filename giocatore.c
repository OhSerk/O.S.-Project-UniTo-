#include "header.h"

void giocata(char const *sq);
int CAMBIO,RAND,PALLA,MSGID,MYTEAM;
struct mymsg MSG;
int main(int argc, char const *argv[]){
	if(argc < 2) return dprintf(1,"Impossibile eseguire il programma in questo modo!\n");
	//printf("Sono vivo!\n");
	CAMBIO = semget(KEY_CAMBIO,NTEAM,0); //Semaforo per effettura un cambio del giocatore
	MYTEAM = (int)(argv[1][0])-65;
	dprintf(1,"Io sono il giocatore:%d della squadra:%s\n", getpid(), argv[1]);
	PALLA = semget(KEY_PALLA,1,0);
	MSGID = msgget(KEY_MSG, 0666);
	sleep(5+rand()%20);
	while(TRUE){
		if(reserveSem(PALLA,0)==ERR)exit(ERR);
		giocata(argv[1]);
	}

	return dprintf(1,"Fine giocatore %d\n",getpid());
}

void giocata(char const *sq){
	sleep(1);
	dprintf(1,"\n%d HA IL CONTROLLO DEL PALLONE\n\n",getpid());
	MSG.mtype = 789;
	RAND = rand()%3;//Creo la domanda del giocatore verso il fato
	sprintf(MSG.mtext,"%d %s %d",RAND,sq,getpid()); //effettuo la richiesta
	if(msgsnd(MSGID, &MSG, sizeof(MSG.mtext)+1, 0)==ERR){
		dprintf(1,"Errore nell'invio del messaggio!(giocatore:%d)\n",getpid());
		exit(1);
	}

	if(msgrcv(MSGID, &MSG ,sizeof(MSG.mtext)+1, 25+getpid(),0)==ERR){
		dprintf(1,"Errore nel ricevere il messaggio!(giocatore:%d)\n",getpid());
		exit(1);
	} else {
		switch(RAND){
			case 0:
			dprintf(1,"Tenta un tiro :%d\n", getpid());
			if(!strcmp(MSG.mtext,"1")){
				dprintf(1,"=============== GOAL! ===============\n");
				MSG.mtype = 123; //Manda un messaggio all'arbitro
				sprintf(MSG.mtext,"%s",sq); //effettuo la richiesta
				if(msgsnd(MSGID, &MSG, sizeof(MSG.mtext), 0)==ERR)
					perror("GOAL");
				releaseSem(PALLA,0);
			} else {
				dprintf(1,"%d ha tirato fuori!\n",getpid());
				releaseSem(PALLA,0);
				sleep(1);
			}
			break;
			case 1:
			dprintf(1,"Tenta un dribbling :%d\n", getpid());
			if(!strcmp(MSG.mtext,"1")){
				dprintf(1,"=============== DRIBBLING! ===============\n");
				giocata(sq);
				sleep(1);
			} else {
				dprintf(1,"%d ha perso la palla!\n",getpid());
				releaseSem(PALLA,0);
				sleep(1);
			}
			break;
			case 2:
			dprintf(1,"Arriva una scivolata verso :%d\n", getpid());
			if(!strcmp(MSG.mtext,"1")){
				dprintf(1,"=============== INFORTUNATO! ===============\n");
				releaseSem(CAMBIO,MYTEAM);
				sleep(5);
				releaseSem(PALLA,0);
				exit(1);
			} else {
				dprintf(1,"%d ha saltato il giocatore avversario!\n", getpid());
				giocata(sq);
				sleep(1);
			}
			break;
		}
	}
}