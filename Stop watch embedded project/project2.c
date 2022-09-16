/*
 * project2.c
 *
 *  Created on: Sep 20, 2021
 *      Author: MARIAM ANWAR
 */



#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

int counter_1=0; //first counter for seconds.
int counter_2=0; //second counter for seconds.
int counter_3=0; //first counter for minutes.
int counter_4=0; //second counter for minutes.
int counter_5=0; //first counter for hours.
int counter_6=0; //second counter for hours.


/*****************************************************************************************************/
/*Initialization and settings for Timer1*/
void Timer1_CTC_Init(void)
{
	TCNT1= 0;		// Set timer1 initial count to zero
	OCR1A = 1000;    // Set the Compare value to 1000
	TIMSK |= (1<<OCIE1A); // Enable Timer1 Compare A Interrupt
	TCCR1A = (1<<FOC1A); //non PWM-mode
	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS12); //set the timer on CTC mode with pre-scalar 1024.
}


/**************************************************************************************************/
/*Initialization for the first external interrupt  INT0*/
void INT0_init (void)
{
	DDRD  &= (~(1<<2));
	PORTD |= (1<<2);
	MCUCR &=(~(1<<ISC00));
	MCUCR |=  (1<<ISC01);
	GICR  |= (1<<INT0);
}

/**********************************************************************************************/
/*Initialization for the second external interrupt  INT1*/
void INT1_init (void)
{
	DDRD  &= (~(1<<3));
	MCUCR |=  (1<<ISC11) | (1<<ISC10);
	GICR  |= (1<<INT1);
}


/*********************************************************************************************/
/*Initialization for the third external interrupt  INT2*/
void INT2_init (void)
{
	DDRB  &= (~(1<<2));
	PORTB |= (1<<2);
	MCUCR &=(~(1<<ISC2));
	GICR  |= (1<<INT2);
}


/**********************************************************************************************/
/*ISR for reset*/
ISR (INT0_vect)
{
	counter_1=0;
	counter_2=0;
	counter_3=0;
	counter_4=0;
	counter_5=0;
	counter_6=0;
}

/**********************************************************************************************/
/*ISR for pausing*/
ISR(INT1_vect)
{
   TCCR1B = TCCR1B &~(1<<CS10);
   TCCR1B = TCCR1B &~(1<<CS12);
}

/*********************************************************************************************/
/*ISR for resuming*/
ISR(INT2_vect)
{
	TCCR1B |= (1<<CS10) | (1<<CS12);
}


/*********************************************************************************************/
ISR (TIMER1_COMPA_vect)
{
	counter_1++;
	if (counter_1>9)
	{
		counter_1=0;
		counter_2++;
	}
	if (counter_2>5)
	{
		counter_2=0;
		counter_3++;
	}
	if (counter_3>9)
	{
		counter_3=0;
		counter_4++;
	}
	if(counter_4>5)
	{
		counter_4=0;
		counter_5++;
	}
	if (counter_5>9)
	{
		counter_5=0;
		counter_6++;
	}
	if (counter_6>9)
	{
		counter_6=0;
	}
}

/******************************************************************************************************/
/*Function for displaying on the 6 7-segments*/
void print (void)
{
	PORTA=0b00000001;
	PORTC=counter_1;
	_delay_ms(5);

	PORTA=0b00000010;
	PORTC=counter_2;
	_delay_ms(5);

	PORTA=0b00000100;
	PORTC=counter_3;
	_delay_ms(5);

	PORTA=0b00001000;
	PORTC=counter_4;
	_delay_ms(5);

	PORTA=0b00010000;
	PORTC=counter_5;
	_delay_ms(5);

	PORTA=0b00100000;
	PORTC=counter_6;
	_delay_ms(5);
}


/*****************************************************************************************************/
int main (void)
{
	DDRC=0x0F;
	DDRA=0b00111111;
	SREG = SREG | (1<<7);
	Timer1_CTC_Init();
	INT0_init ();
	INT1_init ();
	INT2_init ();

	while(1)
	{
		print();
	}
}
