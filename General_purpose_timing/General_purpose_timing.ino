/*

       General purpose Timing Method

*/

uint32_t prev_milliseconds = 0;
uint32_t current_milliseconds = 0;
uint32_t _delay = 500; // In ms

void setup(){
	DDRB |= 0x20;  // PB5 OUTPUT.
}

void loop(){
	current_milliseconds = millis(); //Update Cursor
	if((current_milliseconds - prev_milliseconds) >= _delay){ // check updated cursordifference from target delay
		PORTB ^= 0x20; // toggle PB5.
		prev_milliseconds = current_milliseconds; // reset the cursor to start referance.
	}

	// Serial.println(prev_milliseconds);
}
