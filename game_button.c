#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>


#define LED_R 3
#define LED_Y 2
#define LED_G 0
#define SW_R 6
#define SW_Y 5 
#define SW_G 4
#define SW_W 27


void init(void);
void off(void);
void blink(void);
void fail(void);



int main(void)
{
   
   int i;
   int A[5];
   int k=0;
   int B[5];
   int j = 0;
   int round =1; 
   int success = 1;
   int input_b = 1;
   
   init();
  
   srand(time(NULL));

   for (i = 0; i < 5; i++) {
      A[i] = (rand() % 3) + 1;
   }

   	blink();
	delay(150);
while(round<6 ){

  for (i = 0; i<round; i++){
      
      if (A[i] == 1) { //red
	  digitalWrite(LED_R,1);
	  delay(200);
	  digitalWrite(LED_R,0);
      }
      else if (A[i] ==2){ //yellow
	  digitalWrite(LED_Y,1);
	  delay(200);
	  digitalWrite(LED_Y,0);
      }
      else if (A[i]==3){//green
     	  digitalWrite(LED_G,1);
	  delay(200);
	  digitalWrite(LED_G,0);
	      
      }
   delay(100);   
  }
 
  input_b = 1;

j=0;
while(input_b == 1){

    if(digitalRead(SW_R)==0)
    {
//	printf("R\n");
	digitalWrite(LED_R , 1 );
	delay(100);
	if(j<5){
	    B[j++]=1;
	}
	digitalWrite(LED_R,0);

    }
    else if ( digitalRead(SW_Y)==0)
    {
//	printf("Y\n");
	digitalWrite(LED_Y,1);
	delay(100);
	if(j<5){
	    B[j++]=2;
	}
	digitalWrite(LED_Y,0);
    }
    else if (digitalRead(SW_G) == 0)
    {
//	printf("G\n");
	digitalWrite(LED_G,1);
	delay(100);
	if (j<5){
	     B[j++]=3;
	}
	digitalWrite(LED_G,0);
    }
    else if (digitalRead(SW_W)==0)
    {	input_b = 0;
//	printf("W %d\n", input_b);
   	break;	
    }
    off();

}

//printf("df");

if(round == j){
for (i=0;i<j;i++)
{
    if(A[i] != B[i])
    { 
	success = 0;
//	fail();
    	break;
    }

}}else{
success = 0;
}

if(success == 0 )
{
    fail();
    break;
}else {
round++;
}

if(round == 6 )
{
    blink();
    break;
}

}
return 0;

}

void init(void)
{
   if (wiringPiSetup() == -1)
   {
      exit(1);
   }

   pinMode(SW_R, INPUT);
   pinMode(SW_Y, INPUT);
   pinMode(SW_G, INPUT);
   pinMode(SW_W, INPUT);
  
   pinMode(LED_R, OUTPUT);
   pinMode(LED_Y, OUTPUT);
   pinMode(LED_G, OUTPUT);

   off();
}

void off(void)
{
   digitalWrite(LED_R, 0);
   digitalWrite(LED_Y, 0);
   digitalWrite(LED_G, 0);
}

void blink(void)
{
   int i = 0;

   while (i < 3)
   {
      digitalWrite(LED_R, 1);
      delay(250);
      digitalWrite(LED_R, 0);

      digitalWrite(LED_Y, 1);
      delay(250);
      digitalWrite(LED_Y, 0);

      digitalWrite(LED_G, 1);
      delay(250);
      digitalWrite(LED_G, 0);

      i++;
   }
   off();
}

void fail(void)
{
   int i = 0;
   while (i < 3)
   {
      digitalWrite(LED_R, 1);
      delay(100);
      digitalWrite(LED_R, 0);

      digitalWrite(LED_Y, 1);
      delay(100);
      digitalWrite(LED_Y, 0);

      digitalWrite(LED_G, 1);
      delay(100);
      digitalWrite(LED_G, 0);

    
      delay(250);

      i++;
   }

   off();
}
