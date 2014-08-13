/*****************************************************************************
*
*   DESCRIPTION:
*     Header file declaring the text event class for the Discrete Event 
*     Simulation system.
*     This class provides a very simple example of an actual event object.
*     beyond the base class member data, it includes a string that may
*     contain a textual payload.
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

#ifndef SIM_EXAMPLES_TEXT_EVENT_SIM_TEXT_EVENT_HPP_
#define SIM_EXAMPLES_TEXT_EVENT_SIM_TEXT_EVENT_HPP_

#include "sim_time.hpp"
#include "sim_base_event.hpp"

class SimTextEvent : public SimBaseEvent {
// Class that represents events containing a textual payload
 public:
  // "event_time" - time that the event will be "dispatched" by the simulation
  //       executive
  // "event_text" - the string that represents the object's data payload
  SimTextEvent(const SimTime &event_time, const std::string &event_text)
      : SimBaseEvent(event_time), event_text_(event_text) {};
  virtual ~SimTextEvent() {};

  // Called by dispatch loop as each event is executed from the event queue
  //   NOTE:  This method MUST be redefined for all derived classes.
  virtual void Dispatch() const;

  // Accessor for the event's text object
  //
  // Returns - const reference to the event's text payload
  const std::string &event_text() const { return event_text_; }

#ifdef TEST_HARNESS
  // Debug support for dumping the queue to std out.  Should never be present
  // in production code, but needed for test harness.
  virtual void DumpEvent() const;
#endif

 private:
  // As per the coding standard
  DISALLOW_COPY_AND_ASSIGN(SimTextEvent);

  // Time at which event is scheduled.
  std::string event_text_;
}; // class SimTextEvent

#endif  // SIM_EXAMPLES_TEXT_EVENT_SIM_TEXT_EVENT_HPP_
