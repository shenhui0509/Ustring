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

template<class Derived>
class state_machine;

template<
  class Transition,
  class Next
  >
struct event_dispatcher
{
  typedef typename Transition::fsm_t fsm_t;
  typedef typename Transition::event event;

  static int dispatch(fsm_t &fsm, int state, event const &e)
  {
    if(state == Transition::current_state) {
      Transition::execute(fsm, e);
      return Transition::next_state;
    } else {
      return Next::dispatch(fsm, state, e);
    }
  }
};

struct default_event_dispatcher {
  template<class FSM, class Event>
  static int dispatch(state_machine<FSM> &m, int state, Event const &e)
  {
    return m.call_no_transition(state, e);
  }
};

template<class Transition>
struct transition_event
{
  typedef typename Transition::event type;
};

template<class Table, class Event>
struct generate_dispatcher : 
  mpl::fold<
    mpl::filter_view<
      Table,
      boost::is_same<Event, transition_event<_1>>
    >,
    default_event_dispatcher,
    event_dispatcher<_2,_1>
  >
{};
  

template<class Derived>
class state_machine
{
public:
  template<class Event>
  int call_no_transition(int state, Event const &e)
  {
    return static_cast<Derived*>(this)->no_transition(state, e);
  }

  template<class Event>
    int process_event(Event const &e)
    {
      typedef typename generate_dispatcher<typename Derived::transition_table, Event>::type dispatcher;
      this->state = dispatcher::dispatch(
          *static_cast<Derived*>(this),
          this->state,
          e
          );
      return this->state;
    }

protected:
  template<
    int CurrentState,
    class Event,
    int NextState,
    void (Derived::*action)(Event const&)
    >
  struct row
  {
    static int const current_state = CurrentState;
    static int const next_state = NextState;
    typedef Event event;
    typedef Derived fsm_t;
    static void execute(Derived &fsm, Event const &e)
    {
      (fsm.*action)(e);
    }
  };

  state_machine():
    state(Derived::initial_state)
  {}

  template<class Event>
    int no_transition(int state, Event const &e)
    {
      return state;
    }
  int state;
};

#endif
