#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define LED_R	3
#define LED_Y	2
#define LED_G	0

#define SW_R	6
#define SW_Y	5
#define SW_G	4
#define SW_W	27

void init(void);
void off(void);
void blink_success(void);
void blink_fail(void);

void R(void);
void Y(void);
void G(void);
void W(void);

void gamestart(void);

int A[5];

int B_input=0;
int B[6];

sem_t RYGW;

sem_t R_B;
sem_t Y_B;
sem_t G_B;
sem_t W_B;


int click_stop;
int end;

int main(void){

	int i;
	init();

	srand(time(NULL));
	for( i = 0 ; i < 5 ; i++){
		A[i] = rand()%3 + 1;
	}

	sem_init(&R_B,0,0);
	sem_init(&Y_B,0,0);	
	sem_init(&G_B,0,0);
	sem_init(&W_B,0,0);
	sem_init(&RYGW,0,1);

	blink_success();
	delay(250);

	gamestart();

	off();
	sem_destroy(&R_B);
	sem_destroy(&Y_B);
	sem_destroy(&G_B);
	sem_destroy(&W_B);
	sem_destroy(&RYGW);

	return 0;
}
void gamestart(void){

	int i;
	int status;
	end=1;
	int currentRound = 1;
	pthread_t sw[4];
	click_stop = 1;

	pthread_create(&sw[0],NULL,(void*)&R,NULL);
	pthread_create(&sw[1],NULL,(void*)&Y,NULL);
	pthread_create(&sw[2],NULL,(void*)&G,NULL);
	pthread_create(&sw[3],NULL,(void*)&W,NULL);

	//	th_end = 1;

	while( currentRound < 6 && end == 1){

		//click_stop=1;
		B_input=0;

		//if(currentRound != 1 ){
		//sem_wait(RYGW);
		//}

		delay(250);


		for( i = 0 ; i < currentRound ; i++){
			if( A[i] == 1){
				digitalWrite(LED_R,1);
				delay(150);
				digitalWrite(LED_R,0);
			}else if(A[i] == 2){
				digitalWrite(LED_Y,1);
				delay(150);
				digitalWrite(LED_Y,0);
			}else if(A[i] == 3){
				digitalWrite(LED_G,1);
				delay(150);
				digitalWrite(LED_G,0);
			}
			delay(100);
		}

		//pthread_create(&sw[0],NULL,(void*)&R,NULL);
		//pthread_create(&sw[1],NULL,(void*)&Y,NULL);
		//pthread_create(&sw[2],NULL,(void*)&G,NULL);
		//pthread_create(&sw[3],NULL,(void*)&W,NULL);

		//sem_post(RYGW);
		click_stop=0;

		sem_post(&R_B);
		sem_post(&Y_B);
		sem_post(&G_B);

		sem_wait(&W_B);

		//th_end =0;
		//click_stop=1;

		if( B_input == currentRound ){
			for(i=0 ; i< currentRound ; i++ ){
				if( B[i] != A[i] ){
					delay(50);
					printf("click X\n");
					blink_fail();
					end = 0;
					break;
				}
			}
		}else{
			delay(50);
		//	printf("a lot b %d c %d\n",B_input,currentRound);
			blink_fail();
			end = 0;
		}

		currentRound++;

	}

	if(currentRound==6 && end != 0){
		end = 0;
		
		delay(100);
		blink_success();
	}
		click_stop=0;

		sem_post(&R_B);
		sem_post(&Y_B);
		sem_post(&G_B);
		

	for( i=0 ; i<4 ; i++ ) {
		pthread_join(sw[i],(void*)&status);
	}

}


void R(void){ 

	while(end==1){

		if(click_stop==1){
			sem_wait(&R_B);
		}
		else{
			if(digitalRead(SW_R) == 0){

				digitalWrite(LED_R,1);
				//delay(50);
				while(digitalRead(SW_R) == 0){
					sleep(0.5);
				}

				digitalWrite(LED_R,0);

				sem_wait(&RYGW);

				if( B_input < 6 ){
					B[ B_input ] = 1;
					B_input = B_input+1;
		//			printf("R %d\n",B_input);
				}
				sem_post(&RYGW);
			}
		}
	}	
}

void Y(void){
	while(end==1){
		if(click_stop==1){
		sem_wait(&Y_B);
		}else{
			if(digitalRead(SW_Y) == 0){
				digitalWrite(LED_Y,1);
				//delay(50);

				while(digitalRead(SW_Y) == 0){
					sleep(0.5);
				}

				digitalWrite(LED_Y,0);

				sem_wait(&RYGW);

				if( B_input < 6 ){
					B[ B_input] = 2;
					B_input = B_input+1;
		//			printf("Y %d\n",B_input);
				}

				sem_post(&RYGW);
			}
		}
	}
}

void G(void){
	while(end==1){
		if(click_stop==1){
		sem_wait(&G_B);
		}else{
			if(digitalRead(SW_G)==0){
				digitalWrite(LED_G,1);
				//delay(50);

				while(digitalRead(SW_G) == 0){
					sleep(0.5);
				}

				digitalWrite(LED_G,0);

				sem_wait(&RYGW);
				if(B_input < 6){
					B[B_input] = 3;
					B_input = B_input+1;
		//			printf("G %d\n",B_input);
				}
				sem_post(&RYGW);
			}
		}
	}
}

void W(void){

	while(end==1){
		if(click_stop ==0){
			if(digitalRead(SW_W)==0){

				while(digitalRead(SW_W) == 0){
					sleep(0.5);
				}

				click_stop = 1;
				sem_wait(&RYGW);

				sem_post(&W_B);
				sem_post(&RYGW);
			}
		}
	}
}

void init(void)
{
	if(wiringPiSetup() == -1)
	{
		exit(1);
	}

	pinMode(SW_R,INPUT);
	pinMode(SW_Y,INPUT);
	pinMode(SW_G,INPUT);
	pinMode(SW_W,INPUT);

	pinMode(LED_R,OUTPUT);
	pinMode(LED_Y,OUTPUT);
	pinMode(LED_G,OUTPUT);

	off();
}

void off(void)
{
	digitalWrite(LED_R,0);
	digitalWrite(LED_Y,0);
	digitalWrite(LED_G,0);
}


void blink_success(void)
{
	int i=0;

	while(i<3)
	{
		digitalWrite(LED_R, 1);
		delay(250);
		digitalWrite(LED_R, 0);

		digitalWrite(LED_Y,1);
		delay(250);
		digitalWrite(LED_Y,0);

		digitalWrite(LED_G,1);
		delay(250);
		digitalWrite(LED_G,0);

		delay(250);

		i++;
	}
}

void blink_fail(void)
{
	int i=0;
	while(i<3)
	{
		digitalWrite(LED_R,1);
		delay(50);
		digitalWrite(LED_R,0);

		digitalWrite(LED_Y,1);
		delay(50);
		digitalWrite(LED_Y,0);

		digitalWrite(LED_G,1);
		delay(50);
		digitalWrite(LED_G,0);


		delay(300);

		i++;
	}
}
