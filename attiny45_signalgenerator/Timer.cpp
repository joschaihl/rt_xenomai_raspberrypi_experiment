/*
 * Timer.cpp
 *
 *  Created on: 28.10.2012
 *      Author: ihl
 */

#include "Timer.h"

namespace RealTimeLibrary {

Timer::Timer(const IActionListener & actionListener) : m_actionListener(&actionListener)
{
	mfrequencyHz = 0;
}

void Timer::start()
{

}

void Timer::setFrequencyHz(unsigned int frequencyHz)
{

}


} /* namespace RealTimeLibrary */
