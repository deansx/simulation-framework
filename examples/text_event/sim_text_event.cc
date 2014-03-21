/*****************************************************************************
*
*   DESCRIPTION:
*     File containing the implementation of the SimTextEvent class.  This is
*     a relatively trivial class intended to illustrate how to define event
*     objects that are derived from the parent SimBaseEvent.
*   
*   STATUS:  Prototype
*   VERSION:  0.00
*   CODER:  Dean Stevens
*
*   LICENSE:  The MIT License (MIT)
*             See LICENSE.txt in the root (sim) directory of this project.
*   Copyright (c) 2014 Spinnaker Advisory Group, Inc.
*
*****************************************************************************/

#ifdef TEST_HARNESS
#include <iostream>
#include <iomanip>
#endif

#include "common_strings.hpp"
#include "sim_text_event.hpp"
#include "sim_exec.hpp"
#include "log_text_event.hpp"

//
void SimTextEvent::Dispatch() const {
#ifdef TEST_HARNESS
  std::cout << kCommonStrNote << "Dispatched - " << event_text_ << " at: "
            << this->event_time().GetUserTime() << std::endl;
#endif
  // Get a pointer to the log manager.  "static_cast" would be more 
  // efficient, but dynamic might be a tad bit safer.
  LogTextEvent *log_mgr = dynamic_cast<LogTextEvent*>
                                       (SimExec::the_exec()->log_manager());
  // Stage the data from this event for logging
  log_mgr->StageEventTime(event_time_);
  log_mgr->StageEventText(event_text_);
  // Write the staged data to the log file
  log_mgr->WriteARecordOrDie();
}

#ifdef TEST_HARNESS
  // Debug support for dumping the queue to std out.  Should never be present
  // in production code, but needed for test harness.
void SimTextEvent::DumpEvent() const {
  std::cout << std::setw(3) << ' ' << "SimTextEvent Time "
            << event_time_.GetUserTime() << "; Text:  "
            << event_text_ << std::endl;
  
}
#endif
