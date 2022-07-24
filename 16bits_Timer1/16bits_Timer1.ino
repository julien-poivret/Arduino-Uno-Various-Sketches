/* 
      16 bits Timer1 on Arduino Uno 8 bits avr ATMEGA328PU study. 
	 *** blink the on board led at 250ms accurately. ***
*/

// Set Physical address register.
volatile uint8_t* _TCCR1A = (volatile uint8_t*) 0x80; //Ctrl Reg A.
volatile uint8_t* _TCCR1B = (volatile uint8_t*) 0x81; //Ctrl Reg B.

volatile uint8_t* _TCNT1H = (volatile uint8_t*) 0x85; //Counter Hight.
volatile uint8_t* _TCNT1L = (volatile uint8_t*) 0x84; //Counter low.

volatile uint8_t* _OCR1AH = (volatile uint8_t*) 0x89; //Output compare Hight.
volatile uint8_t* _OCR1AL = (volatile uint8_t*) 0x88; //Output compare Low.

volatile uint8_t* _ICR1H = (volatile uint8_t*) 0x87; //Input compare Hight.
volatile uint8_t* _ICR1L = (volatile uint8_t*) 0x86; //Input compare Low.

volatile uint8_t* _TIMSK1 = (volatile uint8_t*) 0x6F; //Output compare Low.

// Output portB physical address registers definitions.
volatile uint8_t* _PINB = (volatile uint8_t*) 0x23;// pin status and config
volatile uint8_t* _DDRB = (volatile uint8_t*) 0x24;// port direction
volatile uint8_t* _PORTB = (volatile uint8_t*) 0x25;// pin set or reset

volatile uint8_t* _SREG = (volatile uint8_t*) 0x5F;  // Global interupt

void setup() {

	Serial.begin(9600);

	*_DDRB |= 0x20; //Set PB5 in OUTPUT mode.

	*_TCCR1A &= ~0xFF; // Reset timer in Clear Timer On Compare Match  mode.
	*_TCCR1B &= ~0x1F; // & set the prescaler at 64 from (250ms*16^6Hz)/64=62500
	*_TCCR1B |= 0xB;  //
	//Serial.println(*_TCCR1A,HEX);  
	//Serial.println(*_TCCR1B,HEX);  

	*_TCNT1H = 0;  
	*_TCNT1L = 0;  

/* 
        The High 8bits register part must be written first for a "clean built-in chipset 
	atomic write" via an internal 8 Bits buffer, this simply let the ATMEGA328p
	a way to write the two registers high and low values in a same mcu clock cycle (since
	the high part is stored in the 8bits buffer at the time to write the lowest) 
	furthermore for reading the 16 bits register, the lowest part must be read before the high
	part register. 
*/

	*_OCR1AH = 0xF4; //  (250ms*16^6Hz)/64=62500
	*_OCR1AL = 0x24; //  16 Bits split in two 8 Bits.
	//Serial.println(*_OCR1AH,HEX);  
	//Serial.println(*_OCR1AL,HEX);  

	*_TIMSK1 |= 0x2; // Enable Interrupt on Output Compare Match 1
  
  *_SREG |= 0x80; // Enable globla interrupt always on by default (just for exemple).

}

void loop() {
// Free to do whatever you want in the main loop.
// except analogwrite() on pin 9 an 10. already used now the previous by timer1.
}

ISR(TIMER1_COMPA_vect){
 
  *_PORTB ^= 0x20;      // Toggle handled by Interrupt System Routine
	                // CTC mode clear the TCNT1 High & Low automaticly 	
			// for saving few extra cycles in the ISR routine.
// For normal mode you have to add:
//	                           *_TCNT1H = 0;  
//	                           *_TCNT1L = 0; 
}
