#ifndef __PRINT_H__
#define __PRINT_H__

/* Xenomai Modules */
#include <rtdm/rtdm_driver.h>

#define DPRINT_PREFIX "DataRecorder: "
#define DPRINT(s) rtdm_printk(KERN_INFO DPRINT_PREFIX "%s\n", s) 

#endif
