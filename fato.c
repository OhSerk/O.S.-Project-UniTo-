#include "header.h"

char buff[64];
char sq;
char * risposta(int val);
char * risponso(int action,int success);
const char * ASSEGNA_TEAM(int val);
void kill_handler(int sig);
int POINT[NTEAM];
FILE * myLog;
int main(int argc, char const *argv[]){
	if(argc < 2) return dprintf(1,"Impossibile eseguire il programma in questo modo!\n");
	srand(time(NULL));
	FILE * myData = fopen("config.txt","r");
	int arr[4];
	register int i = 0;
	for(i = 0; i < 4; i++){
		fscanf(myData,"%d", &arr[i]);
		if(i != 0)
			printf("%d %% possibilità di: %s\n", arr[i], azione[i-1]);
	}
	fclose(myData);
	myLog = fopen(LOG_FILE,"w");
	setvbuf(myLog, NULL, _IONBF, 0);
	fprintf(myLog, "Inizio nuova partita! Una regola partita di %d squadre composte da %d giocatori con %d palle in gioco!!\n\n",NTEAM,NGIOC,NBALL);
	int idmsg = msgget(KEY_MSG, 0666);
	struct mymsg MSG;
	int index, player=1;
	if(signal(SIGTERM, kill_handler)==SIG_ERR)perror("SIGTERM");
	while(TRUE){
		int old = player;
		if(msgrcv(idmsg, &MSG ,sizeof(MSG), 789, 0)<0){
			dprintf(1,"Errore nella ricezione del messaggio!(fato)\n");
			break;
		}
		sscanf(MSG.mtext,"%d %c %d",&index,&sq,&player);
		if(old != player) fprintf(myLog, "%d: ha preso il controllo del pallone\n", player); //Scrive nel Log il nuovo possessore del pallone
		sprintf(MSG.mtext,"%s",risposta(arr[index+1]));
		fprintf(myLog, "%d: della squadra %c ha: %s", player, sq, risponso(index,atoi(MSG.mtext))); //Scrive le azioni nel Log
		MSG.mtype = 25+player;
		if(msgsnd(idmsg,&MSG, sizeof(MSG.mtext)+1, 0)<0){
			dprintf(1,"Errore nell'invio del messaggio!(fato)\n");
			break;
		}
	}
	return dprintf(1,"Fine fato\n");
}

char * risposta(int val){
	if(rand()%100 >= val) {
		return "0";
	} else{
		return "1";
	}
}	

char * risponso(int action, int success){
	switch(action){
		case 0: //goal
		if(success==1){
			POINT[(int)sq-65]++;
			return "fatto GOAL!\n";
		}else{
			return "sbagliato il tiro!\n";
		}
		case 1: //dribbling
		if(success==1){
			return "driblato l'avversario!\n";
		}else{
			return "sbagliato il dribbling!\n";
		}
		case 2: //infortunio
		if(success==1){
			return "ricevuto una violenta botta!\n";
		}else{
			return "saltato la scivolata!\n";
		}
		default:
		exit(ERR);
	}
	return "";
}

void kill_handler(int sig){
	switch(sig){
		case SIGTERM:
		fprintf(myLog, "========== FINE PARTITA ==========\n");
		for(int i = 0; i < NTEAM; i++){
			fprintf(myLog,"Squadra %s: %d\n", ASSEGNA_TEAM(i), POINT[i]);
		}
		fflush(myLog);
		fclose(myLog);
		exit(EXIT_SUCCESS);
		break;
	}
}

const char * ASSEGNA_TEAM(int val){
	sprintf(buff,"%c",(char)(65+val));
	return buff;
}