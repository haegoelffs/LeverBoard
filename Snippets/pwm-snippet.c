
/*
 * Lcontrol.c
 *
 * Created: 29.03.17 14:42:23
 * Author : Samuel
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

static int pwmode_new = 0;

void timer1_init()
{

	//set up timer in PWM, Phase and frequency Correct mode

	TCCR1A |= (1<<WGM10);

	// set up timer with prescaler = 64 and PWM, Phase and frequency correct mode
	TCCR1B |= (1 << WGM13)|(1 << CS11)|(1 << CS10);

	// initialize counter
	TCNT1 = 0;

	// initialize compare value
	OCR1A = 10000;

	//TIMSK1 |= (1 << OCIE1A)|(1<<TOIE1);
	TIMSK1 |= (1 << OCIE1A);
	// enable global interrupts
	sei();
}


// this ISR is fired whenever a match occurs
ISR (TIMER1_COMPA_vect)
{
	static int pwmode_old = 0;

		if (pwmode_new != pwmode_old)
		{
				PORTB = 0x0;	//PWM1

				PORTH = 0x0;
				pwmode_old = pwmode_new;
		}

		switch(pwmode_new)
		{

			case 0:
				PORTB ^= (1 << 7);	//PWM1
				PORTB ^= (1 << 4);	//PWM4
				break;

			case 1:
				PORTB ^= (1 << 7);	//PWM1
				PORTH ^= (1 << 5);	//PWM6
				break;

			case 2:
				PORTH ^= (1 << 5);	//PWM6
				PORTB ^= (1 << 5);	//PWM3
				break;

			case 3:
				PORTB ^= (1 << 5);	//PWM3
				PORTB ^= (1 << 6);  //PWM2
				break;

			case 4:
				PORTB ^= (1 << 6);  //PWM2
				PORTH ^= (1 << 6);	//PWM5
				break;

			case 5:
				PORTH ^= (1 << 6);	//PWM5
				PORTB ^= (1 << 4);	//PWM4
				break;
		}
}


int main(void)
{

	// connect Output PWM to pin PB7,PB6, PB5, PB4, PH5, PH6
	DDRB =0b11110000;
	DDRH =0b01100000;

	// initialize timer
	timer1_init();


	while(1)
	{
		_delay_ms(100);
		if (pwmode_new < 5)
		{
			++pwmode_new;
		}
		else
		{
			pwmode_new = 0;
		}
	}
}


