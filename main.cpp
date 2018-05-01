//*****************************************************************************************************************
// File: main.cpp
// Author: Michael Middleton
// Date: 06/09/2015
// Description: This file contains the main program loop for the console utility diagnostic application, as well
// as the initialization and support functions that manage initializing the comm port connections, and navigation
// of the main menu
//*****************************************************************************************************************

// header file includes
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <conio.h>
#include <windows.h>
#include "Diag.h"
#include "LevelUp.h"
#include "Comm.h"

// macro definitions
#define SUCCESS 0x01
#define FAIL 0x00
#define MAXCMDLEN 50

// namespaces and typedefs
using namespace std;
typedef unsigned char uchar; //	hex: 00
typedef unsigned int uint; 	 // hex: 0000 
typedef unsigned long ulong; // hex: 0000 0000

// non-volatile global variables
char* command;
HANDLE activeComm; // pointer to active comm port 
char guc_serialRXBuffer[64]; // holds incoming packets
char guc_serialTXBuffer[64]; // holds outgoing packets
int RXBufferPacketLocation;
bool endOfRXPacket;
uchar exitCondition;

//*****************************************************************************************************************
// Name: vExit:
// Description: This function sets the exit flag which will cause the program to close
//*****************************************************************************************************************
void vExit(){
	exitCondition = 0x01; // set flag
} // end exit

//*****************************************************************************************************************
// Name: ucInit
// Description: initializes diagnostic console utility
//*****************************************************************************************************************
uchar ucInit(void){
	// allocate command array
	//char* command = (char*)malloc(MAXCMDLEN);
	//memset(command, 0x00, MAXCMDLEN);
	RXBufferPacketLocation = 0;
	endOfRXPacket = FALSE;
	activeComm = NULL; // initialize active port
	exitCondition = 0x00; // clear exit flag
	
		// **** ToDo **** //
		// flesh out this function
	
	return SUCCESS; // verify successful init
	
} // end init

//*****************************************************************************************************************
// Name: vPrintRXBuffer
// Description: displays what's on the buffer
//*****************************************************************************************************************
void vPrintRXBuffer(){

	for(int i = 0; i < sizeof(guc_serialRXBuffer); i++){
		printf("%c", guc_serialRXBuffer[i]);	
	} // print every character on the buffer
	
} // end vPrintRXBuffer

//*****************************************************************************************************************
// Name: vPrintTXBuffer
// Description: displays what's on the buffer
//*****************************************************************************************************************
void vPrintTXBuffer(){
	
	for(int i = 0; i < sizeof(guc_serialTXBuffer); i++){
		printf("%c", guc_serialTXBuffer[i]);
	} // print every character on the buffer
	
} // end vPrintTXBuffer

//*****************************************************************************************************************
// Name: vDisplayCommands
// Description: displays commands available to the user
//*****************************************************************************************************************
void vDisplayCommands(){
	printf("Available commands:\r\n" );
	printf("Select Help to learn more about each option\r\n");
	printf("1: Help\r\n");
	//printf("2: Run Diagnostic Sequence\r\n");
	printf("2: Stream Sensor Data [Currently Inactive]\r\n");
	printf("3: Interactive Mode\r\n");
	printf("4: Exit\r\n\r\n");
	
} // end vDisplayCommands

//*****************************************************************************************************************
// Name: vHelp
// Description: displays descriptions of each option to the user
//*****************************************************************************************************************
void vHelp(){
	printf("\r\n----------------------------------------------------------------------------------------------------\r\n");
	printf("1: Help\r\n");
	printf("This option will print out these option descriptions\r\n\r\n");
	//printf("2: Run Diagnostic Sequence\r\n");
	//printf("This option will perform a full diagnostic sequence of the WiSARD device connected to this pc.\r\n");
	//printf("It will test each connected sensor, actuator, radio, LED, buzzer, SD card, and onboard temp sensor\r\n\r\n");
	printf("2: Stream Sensor Data [Currently Inactive]\r\n");
	printf("This option will prompt the user for an SP channel, corresponding to a specific transducer reading.\r\n");
	printf("Entering a channel will then begin a sequence which streams data from the sensor in real time\r\n\r\n");
	printf("3: Interactive Mode\r\n");
	printf("This option will allow a user to read activity and interact with the connected WiSARD like in PuTTY.\r\n");
	printf("The advantage of using this over PuTTy is that the parser runs in the background, leveling-up sensor\r\n");
	printf("Sensor data and diagnostic reports as they are recieved across the serial line.\r\n\r\n");
	printf("4: Exit\r\n");
	printf("This option will exit the application and return you to your desktop\r\n");
	printf("\r\n----------------------------------------------------------------------------------------------------\r\n\r\n");
} // end vHelp

//*****************************************************************************************************************
// Name: iGetCommand
// Description: fetches a command from the user
//*****************************************************************************************************************
int iGetCommand(/*char** commandArray*/){
	int commandVal = 0;
	scanf("%d", commandVal); // get option from the user
	return commandVal; // return value
	
} // end getCommand

//*****************************************************************************************************************
// Name: iParseCommand
// Description: parses the fetched command and dispatches the appropriate function
//*****************************************************************************************************************
int iParseCommand(int commandArray){
	// process user input
	switch(commandArray){
		//case (uchar)'1': // help
		case 1:
			//vDisplayCommands(); // displays available commands
			vHelp();
			break;
			
		//case (uchar)'2': // diagnostic sequence
		//case 2:
			//vRunDiagSequence();
			//break;
			
		//case (uchar)'3': // stream sensor data
		case 2:
			vStreamSensorData();
			break;
			
		//case(uchar)'4': // exit
		case 3:
			//exit(); // invokes exit function
			vInteractiveMode();
			break;
			
		//case(uchar)'5': // open port
		case 4:
			//ucOpenSpecifiedComPort(); // prompts user and opens port
			vExit();
			break;
			
		default:
			printf("ERROR: Invalid Command\r\n\r\n");
	}; // end switch(commandArray)
	return SUCCESS;
	
} // end parseCommand 

//*****************************************************************************************************************
// Name: main
// Description: Main program loop. This function, following application startup, will retrieve commands from the 
// user, process those commands, and facilitate execution of the appropriate functions
//*****************************************************************************************************************
int main(int argc, char** argv) {
	uchar ucRetCode = 0;
	
	ucRetCode = ucInit();
	if(ucRetCode == FAIL) return -1;
	
	// main loop code goes here
	
	printf("-----BEGIN-----\r\n\r\n"); // first thing the user sees
	
	vDisplayCommands(); // show user available commands
	
	while(1){
		//printf("I'm here\n");
		int commandVal;
		printf(">");
		scanf("%d", &commandVal);
		
		iParseCommand(commandVal);
		
		if(exitCondition == 0x01){
			printf("Halting execution\r\n");
			break;
		} // end if
		
	} // end while
	
	// wait for thread close and cleanup handle
//	WaitForSingleObject(hThread, INFINITE);
//	CloseHandle(hThread);
	
	return 0;
	
} // end main()
