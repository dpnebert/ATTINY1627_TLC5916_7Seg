#include <atmel_start.h>
#include <util/delay.h>

#define REFRESH_RATE_DELAY	1500

void send(char p);
void latch();
void enableOutput();
void disableOutput();
void load(char p);

char characters[] = { 0b11111100,
	0b01100000,
	0b11011010,
	0b11110010,
	0b01100110,
	0b10110110,
	0b10111110,
	0b11100000,
	0b11111110,
0b11100110 };

uint32_t temp;

int ones, tens, hundreds, thousands;
char c_ones, c_tens, c_hundreds, c_thousands;
adc_result_t reading;

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	temp = 1173;
	
	ADC_0_enable();
	
	ones = 0;
	tens = 0;
	hundreds = 0;
	thousands = 0;
	
	c_ones = characters[ones];
	c_tens = characters[tens];
	c_hundreds = characters[hundreds];
	c_thousands = characters[thousands];
	
	L1_set_level(true);

	while (1) {
		
		reading = (ADC_0_get_conversion(2) + reading) / 2;
		uint32_t temp = (0.9508 * reading) - 41.2131;
		
		ones = temp % 10;
		temp = temp - ones;
		
		tens = temp % 100;
		temp = temp - tens;
		tens /= 10;
		
		hundreds = temp % 1000;
		temp = temp - hundreds;
		hundreds /= 100;
		
		thousands = temp / 1000;		
		
		
		load(characters[ones]);
		L4_set_level(true);
		_delay_us(REFRESH_RATE_DELAY);
		L4_set_level(false);
		
		load(characters[tens]);
		L3_set_level(true);
		_delay_us(REFRESH_RATE_DELAY);
		L3_set_level(false);
		
		load(characters[hundreds]);
		L2_set_level(true);
		_delay_us(REFRESH_RATE_DELAY);
		L2_set_level(false);
		
		load(characters[thousands]);
		L1_set_level(true);
		_delay_us(REFRESH_RATE_DELAY);
		L1_set_level(false);
		
		_delay_us(1000);		
	}
}

void send(char p) {
	char bitMask = 1;
	for(char i = 0; i < 8; i++) {
		SDI_set_level((p & bitMask) > 0);
		bitMask = bitMask << 1;
		CLK_set_level(true);
		CLK_set_level(false);
	}
	SDI_set_level(false);
}
void latch() {
	LE_set_level(true);
	LE_set_level(false);
}
void enableOutput() {
	OE_set_level(false);
}
void disableOutput() {
	OE_set_level(true);
}
void load(char p) {
	disableOutput();
	send(p);
	latch();
	enableOutput();
}
