#include "header.h"

void creaGiocatore(int QNT, char const * sq){
	if(QNT == 0) return;
	else{
		switch(fork()){
			FORK_ERROR
			case 0:
			execl("giocatore","Player ",sq,NULL);
			exit(1);
			break;
			default:
			creaGiocatore(QNT-1,sq);
			break;
		}
	}
}
int main(int argc, char const *argv[]){
	if(argc < 2) return dprintf(1,"Impossibile eseguire il programma in questo modo!\n");
	dprintf(1,"Team:%s!\n",argv[1]);
	int PALLA = semget(KEY_PALLA, 1,  0600 ); //Creo il semaforo per il pallone
	int CAMBIO = semget(KEY_CAMBIO,NTEAM,0); //Semaforo per effettura un cambio del giocatore
	int myTeam = (int)(argv[1][0])-65;
	creaGiocatore(NGIOC,argv[1]); //Funzione ricorsiva per creare N giocatori
	initSemInUse(CAMBIO,myTeam); //Blocco il semaforo
	if(myTeam == NTEAM-1){
		dprintf(1,"Rilascio %d palle!\n",NBALL);
		for(int i = 0; i < NBALL; i++) 
			releaseSem(PALLA,0);
	}
	while(TRUE){
		if(reserveSem(CAMBIO,myTeam)==ERR)exit(ERR);
		dprintf(1,"Sostituire il giocatore!\n");
		creaGiocatore(1,argv[1]);
	}
	return dprintf(1,"Fine Squadra %s\n", argv[1]);
}