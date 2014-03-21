/*****************************************************************************
*
*   DESCRIPTION:
*     Definitions for the base event class for the Discrete Event 
*     Simulation system.
*     The root base class Sim_Base_Event provides a prototype for all 
*     system events, and serves as a common base type to store in the event
*     queue.  All event classes in the system must be derived from this base.
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

#include <iostream>
#include <iomanip>

#include "sim_base_event.hpp"

#ifdef TEST_HARNESS
  // Debug support for dumping the queue to std out.  Should never be present
  // in production code, but needed for test harness.
void SimBaseEvent::DumpEvent() const {
  std::cout << std::setw(3) << ' ' << "SimBaseEvent Time "
            << event_time_.GetUserTime() << std::endl;
}
#endif
