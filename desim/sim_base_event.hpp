/*****************************************************************************
*
*   DESCRIPTION:
*     Header file containing the base event class for the Discrete Event 
*     Simulation system.
*     The root base class Sim_Base_Event provides a prototype for all 
*     system events, and serves as a common base type to store in the event
*     queue.  All event classes in the system must be derived from this base.
*     
*   STATUS:  Prototype
*   VERSION:  1.00
*   CODER:  Dean Stevens
*
*   LICENSE:  The MIT License (MIT)
*             See LICENSE.txt in the root (sim) directory of this project.
*   Copyright (c) 2014 Spinnaker Advisory Group, Inc.
*
*****************************************************************************/

#ifndef SIM_DESIM_SIM_BASE_EVENT_HPP_
#define SIM_DESIM_SIM_BASE_EVENT_HPP_

#include "sim_time.hpp"

class SimBaseEvent {
// Generic base class for all events
 public:
  // "event_time" - time value specifying when the event is to be
  //       dispatched
  SimBaseEvent(const SimTime &event_time) : event_time_(event_time) {};
  virtual ~SimBaseEvent() {};

  // Called by dispatch loop as each event is executed from the event queue
  // NOTE:  This method MUST be redefined for all derived classes.
  virtual void Dispatch() const = 0;

  // Accessor for the event's time object
  //
  // Returns - this event's time value.  Note, this method returns a 
  //       const& avoid making a copy of the time object.  The caller
  //       must make a copy of the returned value before modifying it.
  const SimTime &event_time() const { return event_time_; }

  // Comparison / Sequencing for event objects and time objects.  The
  // following methods compare "this" event's time either to another
  // event's time, or directly to time objects.  The convention for the
  // following is that "this" event should be considered to be on the
  // left side of the comparison operator (the coding standard
  // strongly discourages operator overloading).
  //
  // For the following comparison methods:
  // "test" - event, or time, object to sequence relative to "this"
  //       event.
  // Returns - "true" if "this" event's time meets the criteria relative
  //       to the specified "test" event's, or time object's, time.
  //       "false" otherwise
  
  // "This" event's time is less than ("<") the "test" event's time
  bool EarlierThan(const SimBaseEvent &test) const 
                  { return event_time_.EarlierThan(test.event_time_); };
  // "This" event's time  is less than ("<") the "test" time
  bool EarlierThan(const SimTime &test) const 
                  { return event_time_.EarlierThan(test); };

  // "This" event's time is less than, or equal to ("<=") the "test"
  // event's time
  bool AsEarlyAs(const SimBaseEvent &test) const 
                  { return event_time_.AsEarlyAs(test.event_time_); };
  // "This" event's time is less than, or equal to ("<=") the "test" time
  bool AsEarlyAs(const SimTime &test) const 
                  { return event_time_.AsEarlyAs(test); };

  // "This" event's time is equal to ("==") the "test" event's time
  bool SameTimeAs(const SimBaseEvent &test) const 
                  { return event_time_.SameAs(test.event_time_); };
  // "This" event's time is equal to ("==") the "test" time
  bool SameTimeAs(const SimTime &test) const 
                  { return event_time_.SameAs(test); };

#ifdef TEST_HARNESS
  // Debug support for dumping the queue to std out.  Should never be present
  // in production code, but needed for test harness.
  virtual void DumpEvent() const;
#endif
 protected:
  // Time at which event is scheduled.
  SimTime event_time_;

 private:
  // As per the coding standard
  DISALLOW_COPY_AND_ASSIGN(SimBaseEvent);

}; // class SimBaseEvent

#endif  // SIM_DESIM_SIM_BASE_EVENT_HPP_
