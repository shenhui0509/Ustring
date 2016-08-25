#ifndef USTR_NUMBER_CHECK_H_
#define USTR_NUMBER_CHECK_H_

/* dependencies : boost/mpl
 * Why this ? compile time dispatch rather than run time state machine
 */
 
#include "static_fsm.h"

/* using a state machine to check whether a string is a valid number
 * eg. "0.1332e10" is valid
 *     "+0.12e-100" is valid
 *     "00000012"  is valid
 *     "abc"      is invalid
 */

class number_check :
  public 
#endif
