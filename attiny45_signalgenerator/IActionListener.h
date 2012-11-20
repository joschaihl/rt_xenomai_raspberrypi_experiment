/*
 * IActionListener.h
 *
 *  Created on: 28.10.2012
 *      Author: ihl
 */

#ifndef IACTIONLISTENER_H_
#define IACTIONLISTENER_H_

namespace RealTimeLibrary {

class IActionListener {
public:
	//IActionListener();
	//IActionListener(const IActionListener &rhs);
	virtual void actionPerformed() = 0;
	//IActionListener& operator = (const IActionListener& src);  // Zuweisungsoperator
};

class ActionListenerUndefined : public IActionListener {
public:
	void actionPerformed()
	{
		// Empty action when no action listener is defined
	}
};

//ctionListenerUndefined actionListenerUndefined;
} /* namespace RealTimeLibrary */
#endif /* IACTIONLISTENER_H_ */
