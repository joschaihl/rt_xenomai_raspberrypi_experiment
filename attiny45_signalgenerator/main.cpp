#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "IActionListener.h"
#include "Timer.h"
#include "AVRTimer0.h"

using namespace RealTimeLibrary;
using namespace SystemSpecificLibrary;

class TimerListener : public IActionListener {
public:
	void actionPerformed(void)
	{

	}
};

#define PROG_CHANGE_PORT PB2
#define LED_PIN 		DDB4
#define LED_PORT		PB4

volatile int program_state = 0;
volatile int counter = 0;
volatile int step = 31250;

void entprellung( volatile uint8_t *port, uint8_t maske ) {
  uint8_t   port_puffer;
  uint8_t   entprellungs_puffer;

  for( entprellungs_puffer=0 ; entprellungs_puffer!=0xff ; ) {
    entprellungs_puffer<<=1;
    port_puffer = *port;
    _delay_us(150);
    if( (*port & maske) == (port_puffer & maske) )
      entprellungs_puffer |= 0x01;
  }
}

ISR(INT0_vect)
{
	if(step > 0)
	{
		step -= 100;
	}
	else
	{
		step = 4000;
	}
	/*cli();

	// Wenn
	program_state++;
	if(program_state > 3)
	{
		program_state = 0;
	}
	_delay_ms(1000);
	//entprellung(&PORTB, (1 << PROG_CHANGE_PORT));


	sei();*/
//	cli();
//	PORTB |= (1 << LED_PORT);
//	entprellung( &PINB, (1<<PINB2) );
//	if( PINB & (1<<PINB2) ) pcint_flag=1;
//	speed = speed / 2;
//	sei();
	//reti();
}
/*
ISR(TIMER0_OVF_vect)
{
	if(counter > step)
	{
		counter = 0;
		PORTB ^= (1 << LED_PORT);
	}
	//reti();

	counter++;
}
*/
int main(void)
{

	// Internen Pullup Widerstand an Taster einschalten, so dass
	// Widerstand gegen VCC am Port geschalten wird
	// und Taster Low-Aktiv ist, d.h. auf Masse zieht
//	PORTB |= (1 << PROG_CHANGE_PORT);
	DDRB |= (1 << LED_PIN); // LED als Output, Rest als Input



	while(1)
	{

	}
}
