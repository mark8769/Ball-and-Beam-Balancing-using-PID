#include "serial.h"

void handleInput();
volatile double kP;
volatile double kI;
volatile int kD;


//Function that takes care of configurating the serial communication
/*Call this function in main setup to use serial monitor */
void setupUART(void) {
	UBRR1 = (F_CPU/9600/16) - 1;

	UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);		// 8 bit char size
	UCSR1B |= (1 << TXEN1);							// enable transmit

	UCSR1B |= (1 << RXEN1);     					// enable receive
	UCSR1B |= (1 << RXCIE1);     					// enable interrupt on data receipt
}
//ISR that is called whenever there is data to receive
ISR(USART1_RX_vect) {

	// BE CAREFUL with this. It is an ISR and will interfere with your
	// PCINT if it takes too long.

    uint8_t ch = UDR1;

    //Is user pressed 'Enter' it means that the user has finished typing the command
    if (ch == 13 || (recv_buffer_ptr >= RECEIVE_BUFFER_LENGTH)) {
        user_command_ready = 1;
    }

    //User deletes a character
    else if (ch == 8) {
        if (recv_buffer_ptr >= 1)
            --recv_buffer_ptr;
        recv_buffer[recv_buffer_ptr] = 0;
    }

    //Only store alphanumeric symbols, space, the dot, plus and minus sign
    else if
		( (ch == ' ') || (ch == '.') || (ch == '+') || (ch == '-') ||
		((ch >= '0') && (ch <= '9')) ||
		((ch >= 'A') && (ch <= 'Z')) ||
		((ch >= 'a') && (ch <= 'z')) ) {
        recv_buffer[recv_buffer_ptr] = ch;
        ++recv_buffer_ptr;
    }
}

void sendChar(char c) {
	while((UCSR1A & (1<<UDRE1)) == 0);	// wait while data register is NOT empty
	UDR1 = c;
}

void sendString(char *s) {
	while(*s != 0x00) {
		sendChar(*s);
		s++;
	}
	sendChar('\r');
}

//Function that parses the commands the user inputs into the serial console.
void handleInput() {

	// This function is called from main, but "released" in the ISR.
	// The ISR sets user_command_ready=1, main watches for the flag.
	char command;
	char second_command;
	int value;
	double value2;
	char fGood = 1;
	int targetGoal;

	// provides feedback to the terminal
	char outputBuffer[50] = "\r\n";

	// only accepting a command character with optional value
	sscanf((char *)recv_buffer,"%c %d",&command, &value);
	//sscanf((char *)recv_buffer,"%c %d %lf",&command, &value, &value2, &second_command);
	switch(command) {
	
	case 'q':
	case 'Q':
		if (flagDebug == 0){
			sendString("Starting sonic debug mode, press q to stop.\r\n");
			flagDebug = 1;
		}else{
			sendString("Stopping sonic debug mode, press q to start.\r\n");
			flagDebug = 0;
		}
		break;

	case 'P':
	case 'p':
		sprintf(output_buffer,"kP * 10: %d kI * 10000: %d kD: %d\r\n",(int)(kP * 10), (int)(kI * 10000), kD);
		sendString(output_buffer);
		break;	

	default:
		fGood = 0;
		printMenu();		
	}
	// reset the buffer
    recv_buffer_ptr = 0;
    memset((char *)recv_buffer, 0, RECEIVE_BUFFER_LENGTH);

	// feedback to terminal
	//if (fGood) sendString(outputBuffer);

	// set flag that ready for more
	user_command_ready = 0;
}
