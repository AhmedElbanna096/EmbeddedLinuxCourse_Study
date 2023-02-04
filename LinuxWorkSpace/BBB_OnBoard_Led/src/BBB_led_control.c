/*
 ============================================================================
 Name        : BBB_led_control.c
 Author      : Kiran N < niekiran@gmail.com >
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Controls the BBB user led 3 trigger and brightness values
 TODOs for Students : Make it generic for all the on board leds of the BBB
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
 #include <errno.h>

#define SYS_FS_LEDS_PATH "/sys/class/leds"


/*Write Trigger*/
int WriteTrigger(int LedNumber , char * CtrlValue)
{
	/*buffer to store the path of required file*/
	char buf[100];
	/*file descriptor referencing for the entry point of the required file in fs*/
	int fd=0;
	/*return container*/
	int ret;

	/*Concatenate the String to create full path with specified led number*/
	snprintf(buf,sizeof(buf),SYS_FS_LEDS_PATH "/beaglebone:green:usr%d/trigger",LedNumber);
	/*open file required in the path with Write Only Flag*/
	fd = open(buf, O_WRONLY );

	if(fd<=0)
	{
		/*Invalid File*/
		printf("Failed to open file\n");
		return -1;
	}

	/*Valid file*/
	ret = write(fd, (void*)CtrlValue, strlen(CtrlValue) );
	if(ret<0)
	{
		printf("Error in Writing\n");
		return -1;
	}
	return 1;
}



/*Trigger Function*/
void TriggerFunction(int LedNumber , char * CtrlVal)
{
	if( (LedNumber<4) &&
			(strcmp(CtrlVal, "timer") || strcmp(CtrlVal, "heartbeat") || strcmp(CtrlVal, "none")
					|| strcmp(CtrlVal, "oneshot") || strcmp(CtrlVal, "default-on")))
	{
		/*Write function */
		WriteTrigger(LedNumber,CtrlVal);
	}
	else
	{
		printf("Invalid Trigger function values\n");
	}

}

int WriteBrightness(int LedNumber , char  *CtrlVal)
{

	/*buffer to store the path of required file*/
	char buf[100];
	/*file descriptor referencing for the entry point of the required file in fs*/
	int fd=0;
	/*return container*/
	int ret;

	/*Concatenate the String to create full path with specified led number*/
	snprintf(buf,sizeof(buf),SYS_FS_LEDS_PATH "/beaglebone:green:usr%d/brightness",LedNumber);
	/*open file required in the path with Write Only Flag*/
	fd = open(buf, O_WRONLY );

	if(fd<=0)
	{
		/*Invalid File*/
		printf("Failed to open file\n");
		return -1;
	}

	/*Valid file*/
	ret = write(fd, (void*)CtrlVal, sizeof(CtrlVal) );
	//ret = write(fd, (void *)&value, sizeof(value) );

	if(ret<=0)
	{
		printf("brightness : Error in Writing\n");
		int errsv = errno;
		printf("The error number = %d",errsv);
		return -1;
	}
	return 1;



}

void BrightnessFunction(int LedNumber , char *CtrlVal)
{
	/*Check on the brightness ctrl option*/
	if( (LedNumber<4) &&
			(strcmp(CtrlVal,"0")==0 || strcmp(CtrlVal,"1")==0 ))
	{
		/*Valid*/
		WriteBrightness(LedNumber,CtrlVal);
	}
	else
	{
		/*Invalid*/
		printf("Invalid Trigger function values\n");
	}
}


int main( int argc, char *argv[] )
{

	int lednum=0;
	/*Check on the input arguments*/
	if(argc != 4)
	{
		/*Not Valid*/
		printf("Incorrect number of arguments\n");
		printf("arg0 > File name \n");
		printf("arg2 > Control Option : Trigger or Brightness\n");
		printf("arg3 > Control Value for Trigger[none,timer,heartbeat] or Brightness[0,1]\n");
		printf("arg4 > Led Number [0,1,2,3]\n");

	}
	else
	{
		/*Valid*/
		/*Check on the control option [Trigger , Brightness]*/
		if(strcmp(argv[1],"trigger")==0)
		{
			/*Valid Trigger*/
			/*Call trigger function with input LedNumber and CtrlValue*/
			lednum=atoi(argv[3]);
			TriggerFunction(lednum , argv[2]);
		}
		else if(strcmp(argv[1],"brightness")==0)
		{
			/*Valid brightness*/
			/*Call brightness function*/
			lednum=atoi(argv[3]);
			printf("Brightness\n");

			BrightnessFunction(lednum,argv[2]);
		}
		else
		{
			/*Invalid Values*/
			printf("Invalid Control Option\n");
			printf( "The valid control_options : brightness, trigger\n");

		}

	}







	return 0;

}
