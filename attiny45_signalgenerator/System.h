/*
 * System.h
 *
 *  Created on: 28.10.2012
 *      Author: ihl
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

/*!
 * \brief Pure-virtual workaround.
 *
 * The avr-libc does not support a default implementation for handling
 * possible pure-virtual calls. This is a short and empty workaround for this.
 */
extern "C" {
  void __cxa_pure_virtual()
  {
    // put error handling here
  }
}

namespace RealTimeLibrary {

class System {

};

} /* namespace RealTimeLibrary */
#endif /* SYSTEM_H_ */
