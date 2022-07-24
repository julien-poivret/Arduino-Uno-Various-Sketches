/* 

   A simple rotary encoder on PD6 for CLK rotary pin and PD7 for DT rotary pin 
   on arduino Arduino UNO 
   
*/

// Global variables
volatile uint8_t prev_clock = 0;
volatile uint8_t current_clock = 0;
volatile int ct = 0;

void setup() {
	Serial.begin(9600);
	DDRD &= ~0xC0;    // PD6 & PD7 as simple INPUT
	prev_clock = (PIND&0x40); // save Digital Read of PD6 (rotary CLK pin).
}

void loop() {
	Encoder_read(); // Function call
}

void Encoder_read(void){
	current_clock = ((PIND&0x40)>>6);//freeze the scope referance. 
	if(current_clock != prev_clock){// if the rotary have changed of state or not.
		if(((PIND&0x80)>>7) != current_clock){ // if the rotary pin DT lagg or not the rotary pin CLK
			ct--;  // Decrement
		}else{
			ct++;  // Increment
		}
		Serial.println(ct); // Serial feed back
	}
	prev_clock = current_clock; // update actual change for further detecting change.
}

