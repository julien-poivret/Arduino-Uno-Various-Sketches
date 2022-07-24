/*

 A Digital rotary switch,that allow an accurate & 
   infinite digital incrementation since mechaniclly 
   speaking the rotary switch turn in the void.
   
   Neat for driving thresholds values or stepped setting. 
 
 Rotary encoder:
                on pin 7 for DT
                on Pin 6 for CLK 
 
 Board: Arduino Uno.
 
 */

volatile unsigned long prev_milliseconds = 0;
volatile unsigned long current_milliseconds = 0;
volatile uint32_t _delay = 500;                                 // in ms

// Global variables
volatile uint8_t prev_clock = 0;
volatile uint8_t current_clock = 0;
volatile int32_t ct = 500;
volatile int32_t interval = 500;

void setup(){
	Serial.begin(9600);
	DDRB |= 0x20;                                           // PB5 OUTPUT.
	DDRD &= ~0xC0;                                          // PD6 & PD7 as simple INPUT
	prev_clock = (PIND&0x40);                               // save Digital Read of PD6 (rotary CLK pin).
}

void loop(){
	current_milliseconds = millis();                        // freeze the time frame referance. 
	if((current_milliseconds - prev_milliseconds) >= ct){   // track time frame evolution from target delay
		PORTB ^= 0x20;                                  // toggle PB5 (Blink on board Led).
		prev_milliseconds = current_milliseconds;       // reset the cursor to zero.
		Serial.println(ct);
	}
	Encoder_read(); // Function call
}

void Encoder_read(void){
	current_clock = ((PIND&0x40)>>6);                 // hold the current rotary state for compare
	if(current_clock != prev_clock){                  // if the rotary has changed of state or not.
		if(((PIND&0x80)>>7) != current_clock){    // if the rotary pin DT lagg or not the rotary pin CLK
			if(ct-5>=5){                      // check minimum value.
				ct-=5;                    // Decrement
			}
		}else{ 
			if(ct<0xFFFFFFFFFFFFFFFF){        // forbid 32 bit overflow (likely not).
			ct+=5;                            // Increment
			}
		}
	}
	prev_clock = current_clock;                       // update actual change for further detecting change.
}
