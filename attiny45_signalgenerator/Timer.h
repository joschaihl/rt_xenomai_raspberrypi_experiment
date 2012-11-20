/*
 * Timer.h
 *
 *  Created on: 28.10.2012
 *      Author: ihl
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "IActionListener.h"

namespace RealTimeLibrary {

class Timer {
public:
	Timer(const IActionListener &actionListener);
	void setFrequencyHz(const unsigned int frequencyHz);
	void start();
private:
	unsigned int mfrequencyHz;
	const IActionListener *m_actionListener;
};

} /* namespace RealTimeLibrary */
#endif /* TIMER_H_ */
