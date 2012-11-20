/*
 * AVRTimer0.cpp
 *
 *  Created on: 28.10.2012
 *      Author: ihl
 */

#include "AVRTimer0.h"
#include <avr/io.h>
#include <avr/interrupt.h>

namespace SystemSpecificLibrary {

AVRTimer0::Timer0Clock AVRTimer0::mTimer0Clock = NO_PRESCALING;
//RealTimeLibrary::IActionListener AVRTimer0::mTimerActionListener = ActionListenerUndefined;

ISR (TIMER0_OVF_vect)
{
	AVRTimer0::getActionListener()->actionPerformed();
}

// TODO Auto-generated constructor stub
void AVRTimer0::setActionListener(RealTimeLibrary::IActionListener *timerActionListener)
{
	//RealTimeLibrary::ActionListenerUndefined test;
//AVRTimer0::mTimerActionListener = &test;
}
/*
RealTimeLibrary::IActionListener AVRTimer0::getActionListener()
{
	return mTimerActionListener;
}*/

void AVRTimer0::start()
{
	TCCR0B |= mTimer0Clock;
	TIMSK |= (1 << TOIE0);
	sei();
}
void AVRTimer0::stop()
{

}

void AVRTimer0::setTimerClock(enum Timer0Clock &aTimer0Clock)
{
	mTimer0Clock = aTimer0Clock;
}

} /* namespace SystemSpecificLibrary */
