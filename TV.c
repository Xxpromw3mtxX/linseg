/*
	Programma per telecomando TV scritto da Metta Francesco
	04/12/2017
	Linux Signals
*/

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

int volume_su=0; //Variabile per aumentare il volume
int volume_giu=0; //Variabile per diminuire il volume
int vol_status=0; //Variabile per lo stato del volume
int channel_su=0; //Variabile per aumentare il canale per il canale
int channel_giu=0; //Variabile per diminuire il canale
int channel_status=0; //Variabile per lo stato del canale
bool status_on; //Variabile per on
bool status_off; //Variabile per off
bool tv_status; //Variabile per lo stato on/off del tv

//Funzioni

//Volume
void vol_up(int signum){
	if(tv_status==1){
		volume_su=vol_status;
        	if(volume_su>=15){
               		printf("Volume massimo.\n");
			vol_status=15;
       		}else if(volume_su<=15){
			volume_su++;
			vol_status=volume_su;
		}
	}else{
		printf("La tv e' spenta!\n");
	}
}

void vol_down(int signum){
        if(tv_status==1){
		volume_giu=vol_status;
                if(volume_giu<=0){
                	printf("Il volume e' muto.\n");
			vol_status=0;
                }else if(volume_giu>0){
			volume_giu--;
			vol_status=volume_giu;
		}
        }else{
                printf("La tv e' spenta!\n");
        }
}

//Canale
void chan_up(int signum){
	if(tv_status==1){
		channel_su=channel_status;
		if(channel_su==99){
			printf("Canale massimo del DTV.\n");
			channel_status=99;
		}else if(channel_su<=99){
			channel_su++;
			channel_status=channel_su;
		}
	}else{
		printf("La TV e' spenta!\n");
	}
}

void chan_down(int signum){
        if(tv_status==1){
                channel_giu=channel_status;
                if(channel_giu<=0){
                        printf("Sei gia' al canale piu' basso.\n");
                        channel_status=0;
                }else if(channel_giu>0){
                        channel_giu--;
                        channel_status=channel_giu;
                }
        }else{
                printf("La tv e' spenta!\n");
        }
}

//Stato TV
void stat_on(int signum){
	status_on=1;
	printf("La TV e' accesa.\n");
	tv_status=status_on;
}

void stat_off(int signum){
	status_off=0;
	printf("La TV e' spenta.\n");
	tv_status=status_off;
}

/*----------------------------------------------------------------------------------------------------------*/

//Stampa ogni secondo lo stato del tv
int main(int argc, char **argv){
	/*Cattura dei segnali*/
	//Volume su
	signal(SIGRTMAX, vol_up);
	//Volume giù
	signal(SIGRTMIN, vol_down);
	//Canale su
	signal(SIGCHLD, chan_up);
	//Canale giù
	signal(SIGCONT, chan_down);
	//Status_on tv
	signal(SIGUSR1, stat_on);
	//Status_off tv
	signal(SIGTERM, stat_off);

	//Ciclo infinito con stampa dello stato
	while(1){
		printf("Lo stato del TV e':%d\n", tv_status);
		printf("Il volume della TV e': %d\n", vol_status);
		printf("Il canale della TV e': %d\n", channel_status);
		sleep(1);
	}
}

