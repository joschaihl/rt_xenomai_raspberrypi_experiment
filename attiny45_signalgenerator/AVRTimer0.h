/*
 * AVRTimer0.h
 *
 *  Created on: 28.10.2012
 *      Author: ihl
 */

#ifndef AVRTIMER0_H_
#define AVRTIMER0_H_

#include "IActionListener.h"

namespace SystemSpecificLibrary {



class AVRTimer0 {
private:
	AVRTimer0() {}
public:
	enum Timer0Clock{
		/* Fuer Atmega8 */
		NO_CLOCK_SOURCE = 0,
		NO_PRESCALING = 1,
		PRESCALE_DIV_8 = 2,
		PRESCALE_DIV_64 = 3,
		PRESCALE_DIV_256 = 4,
		PRESCALE_DIV_1024 = 5,
		CLOCK_ON_FALLING_EDGE_ON_T0 = 6,
		CLOCK_ON_RISING_EDGE_ON_T0 = 7
	};
	static void setTimerClock(Timer0Clock &aTimer0Clock);
	static void setActionListener(RealTimeLibrary::IActionListener *timerActionListener);
	static void start();
	static void stop();
	static RealTimeLibrary::IActionListener *getActionListener()
	{
		//return mTimerActionListener;
		RealTimeLibrary::ActionListenerUndefined undefined;
		RealTimeLibrary::IActionListener *test;

		test = &undefined;
		return test;
	}
private:
	static Timer0Clock mTimer0Clock;

	static RealTimeLibrary::IActionListener *mTimerActionListener;

};
//RealTimeLibrary::ActionListenerUndefined actionListenerUndefined;
RealTimeLibrary::IActionListener *AVRTimer0::mTimerActionListener = 0;
//

} /* namespace SystemSpecificLibrary */
#endif /* AVRTIMER0_H_ */
