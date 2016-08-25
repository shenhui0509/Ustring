#ifndef STATIC_FSM_H_
#define STATIC_FSM_H_

/* Note : this depends on the boost/mpl
 * Usage : For compile time finite state machine template, if some certain task is defined,
 * such as determine whether a string is a valid number form ("0.31231","+0.231e10","-0002.e-10"...)
 * then can implement a class from this static state machine for such task
 */

#include <boost/mpl/vector.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>

namespace mpl = boost::mpl;
using namespace boost::mpl::placeholders;

#endif
