#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/sleep.h>

#define irLed		1
#define Button		4

void send(uint8_t *signal, uint8_t length)
{
	DDRB = 0;
	for (uint8_t i = 0; i < length; i++)
	{
		TCNT1 = 0;
		DDRB ^= 1 << irLed;
		while (TCNT1 < signal[i]);
	}
	DDRB = 0;
}

int main(void)
{
	uint8_t channel_up1[41] = {167, 55, 28, 55, 28, 28, 28, 28, 28, 55, 55, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 55, 55, 28, 28, 28, 28, 28, 28, 28, 28, 28};

	// MCUCR = 1 << SE | 1 << SM1; //enable sleep mode and power down
	// PRR = 1 << PRADC | 1 << PRUSI; //disable all clocks except of Timer/Counter0-1

	DDRB = 0;
	PORTB = (1 << Button);
	_delay_ms(1);

	//set timer0 to output 36khz on pin 1
	OCR0A = 221;
	OCR0B = 110;
	TCCR0A = 1 << WGM00 | 1 << WGM01 | 1 << COM0B1;
	TCCR0B = 1 << WGM02 | 1 << CS00;

	//set timer1 to increment every 16us
	PLLCSR = 0;
	TCCR1 = 1 << CS13;
	GTCCR = 0;

	//sleep_cpu();
	while (1)
	{
		if (!(PINB & (1 << Button)))
		{
			send(channel_up1, sizeof(channel_up1));
			_delay_ms(500);
		}
	}
}
