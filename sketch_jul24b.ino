volatile uint8_t prev_clock = 0;
volatile uint8_t current_clock = 0;
volatile int ct = 0;

void setup() {
	Serial.begin(9600);
	DDRD &= ~0xC0;
	prev_clock = (PIND&0x40);
}

void loop() {
	Encoder_read();
}

void Encoder_read(void){
	current_clock = ((PIND&0x40)>>6);
	if(current_clock != prev_clock){// if the rotary have changed.
		if(((PIND&0x80)>>7) != current_clock){
			ct--;
		}else{
			ct++;
		}
		Serial.println(ct);
	}
	prev_clock = current_clock;
}

