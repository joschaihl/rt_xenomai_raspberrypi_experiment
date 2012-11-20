/*
 * GPIOPins.h
 *
 *  Created on: 28.10.2012
 *      Author: ihl
 */

#ifndef GPIOPINS_H_
#define GPIOPINS_H_

namespace RealTimeLibrary {

class GPIOPins {
public:
	GPIOPins();
	static void setOutputPin(unsigned int pinNr);
	static void setOutputPinLevelToHIGH(unsigned int pinNr);
	static void setOutputPinLevelToLOW(unsigned int pinNr);
	static void setInputPin(unsigned int pinNr);
	static bool isInputLevelHigh(unsigned int pinNr);
};

} /* namespace RealTimeLibrary */
#endif /* GPIOPINS_H_ */
