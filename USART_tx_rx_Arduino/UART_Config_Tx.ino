/*                         Serial Communication Protocol study.
                   a light weight code for printing on Serial port.
                   ________________________________________________
                  /                                                \
                 |    _________________________________________     |
                 |   |                                         |    |
                 |   |  C:\> _                                 |    |
                 |   |                                         |    |
                 |   |                                         |    |
                 |   |                                         |    |
                 |   |                                         |    |
                 |   |                                         |    |
                 |   |                                         |    |
                 |   |                                         |    |
                 |   |                                         |    |
                 |   |                                         |    |
                 |   |                                         |    |
                 |   |                                         |    |
                 |   |_________________________________________|    |
                 |                                                  |
                 \_________________________________________________/
                        \___________________________________/
                     ___________________________________________
                  _-'    .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  --- `-_
               _-'.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--.  .-.-.`-_
            _-'.-.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-`__`. .-.-.-.`-_
         _-'.-.-.-.-. .-----.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-----. .-.-.-.-.`-_
      _-'.-.-.-.-.-. .---.-. .-------------------------. .-.---. .---.-.-.-.`-_
     :-------------------------------------------------------------------------:
     `---._.-------------------------------------------------------------._.---'



    */

    /////////////////////////     Define peripherals registers.

    volatile uint8_t* _SER = (volatile uint8_t *) 0x3f;     //      Global Interrupt 

    volatile uint8_t* _UDR0 = (volatile uint8_t *) 0xC6;    //      Data Register

    volatile uint8_t* _UCSR0A = (volatile uint8_t *) 0xC0;  //      Control and status register A
    volatile uint8_t* _UCSR0B = (volatile uint8_t *) 0xC1;  //      Control and status register B
    volatile uint8_t* _UCSR0C = (volatile uint8_t *) 0xC2;  //      Control and status register C

    volatile uint8_t* _UBRR0H = (volatile uint8_t *) 0xC5;  //     Baud Rate Hight register
    volatile uint8_t* _UBRR0L = (volatile uint8_t *) 0xC4;  //     Baud Rate low register


    //////////////////////////////////////////////////////////// UART0

    /*
       init UART0 with two possibles settings: 9600 or 115200.
     */
    void UART0_init(uint16_t BaudRate){
      *_SER &=~0x80;                                       // Disable global interrupt. 

      *_UCSR0C &= ~0xC;                                    // Set asynchronous mode.
      *_UCSR0A |= 0x2;                                     // Set the (FULL speed) for asynchronous mode.

      // Set the Baudrate. ----->  baud_setting = (F_CPU / 4 / baud - 1) / 2
      if(BaudRate==9600){ 
        *_UBRR0H &= ~ 0xF;
        *_UBRR0L = 0xCF;
      }else if(BaudRate==115200){
        *_UBRR0H &= ~ 0xF;
        *_UBRR0L = 0x10;
      }

      // Set the frame in 8 bits mode.
      *_UCSR0C |= 0x6;
      *_UCSR0C &= ~0x8; // (1 stop bit).

      // Enable Rx Tx mode + Interrupts + 8 bits mode part2.
      *_UCSR0B = 0xB8;

      *_SER |= 0x80; // Enable global interrupt. 
    }

/*
   Send data with exact predefined length (for speed).
*/
void UART_send_fast(char* asci_data,uint8_t length){
  *_UCSR0B |= 0x8; //Tx Enabled.
  for(uint8_t i=0;i<length;i++){
    while(!(*_UCSR0A&0x20)){ // wait for prev tx to complete
      asm("");
    }
    *_UDR0 = *(asci_data+i); // ready!, so write the next stacked bytes on buffer.
  }
}
/*
   Send data with automatic stack length (slower).
*/
void UART_send(char* asci_data){
	*_UCSR0B |= 0x8; //Tx Enabled.
	uint8_t ct = 0;
	while(1){
		while(!(*_UCSR0A&0x20)){ // wait for prev tx to complete
			asm("");
		}
		if(*(asci_data+ct)){        // if not a terminating bytes (0).
			*_UDR0 = *(asci_data+ct); //  write the next stacked bytes on buffer.
			ct++;
		}else{
			break;
		}
	}
}
void UART_receive(char* asci_data,uint8_t length, uint8_t* Error_flag){
	*_UCSR0B |= 0x10; // Rx Enabled.
	for(uint8_t i=0;i<length;i++){
		uint8_t ct = 0;
		while(!(*_UCSR0A&0x80)){
			if(ct==0xFF){    // if time out is reach => Error !
				*Error_flag = 1;
				break;
			}
			ct++;
		}
		*(asci_data+i) = *_UDR0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
char data_rx[24]={0};
int main(void){
	UART0_init(9600);      // Classic function call.
	UART_send_fast("test A\n",7);
	UART_send_fast("test B\n",7);
	UART_send_fast("Buffer Overflow !\n",18);
	UART_send_fast("Electro Magnetic wave !\n",24);
	uint8_t error_flg = 0;
	UART_receive((char *)data_rx,24,&error_flg);
	if(error_flg){
		UART_send("Error, Rx fail !\n");
	}else{
		UART_send_fast("Rx Completed\n",13);
	}
	while(1){
		asm("");
	}
}

