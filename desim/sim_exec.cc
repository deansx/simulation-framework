/*****************************************************************************
*
*   DESCRIPTION:
*     Defines the class responsible for actually providing the Discrete
 *    Event Simulation environment
*     This executive actually excutes the simulation, and provides the
*     primary public interface to the simulation engine.
*   
*   STATUS:  Prototype
*   VERSION:  0.00
*   CODER:  Dean Stevens
*
*   LICENSE:  The MIT License (MIT)
*             See LICENSE.txt in the root (sim) directory of this project.
*   Copyright (c) 2014 Spinnaker Advisory Group, Inc.
*
 ****************************************************************************/

#include <stdlib.h>
#include <iostream>
#include <sstream>

#include "sim_exec.hpp"
#include "common_strings.hpp"
#include "common_messages.hpp"
#include "stim_loader.hpp"

// Initialize that static singleton pointer
SimExec* SimExec::the_exec_ = nullptr;

// Initialize the SimExec.  This method calls the new "stim_loader"'s
// method to load the first set of stimulus events.
// Note: The executive is responsible for the memory allocated for the
// objects passed in as pointers.
//
// "run_until_tm" - specifies the ending time for the run.  The
//       simulation may end earlier, if the event queue is exhausted.
//       However, once "run_until_tm" is reached simulation halts and
//       events still on the queue are not executed.
// "config_manager" - pointer to the object that manages configuration
//       data.
// "log_manager" - pointer to the object that manages log data and
//       coordinates logging for simulation events
// "stim_loader" - pointer to the object that manages the stimulus file
//       and handles loading stimulus events
void SimExec::Init(const SimTime &run_until_tm,
                   ConfigMgr *const config_manager,
                   LogMgr *const log_manager,
                   StimLoader  *const stim_loader) {
  // valid range for ruTime assumed
  run_until_time_.SetTime(run_until_tm);

  // Set up the config and log managers.  Both may be nullptr.
  config_manager_ = config_manager;
  log_manager_ = log_manager;
  
  // Initialize the stimulus loader
  if (stim_loader!= nullptr) {
    stim_loader_ = stim_loader;
    // Load the first set of stimulus
    stim_loader_->StartLoadingOrDie();
  } else {
    UtilFatalErrorAndDie("Encountered unexpected issue with stimulus.\n"
                         "Stimulus must be provided to run the simulation.");
  }  // stimulus loader initialization
  
}  // init

// Class Destructor
SimExec::~SimExec() {
  // Clean up the stimulus load class
  if (stim_loader_ != nullptr) {
    delete stim_loader_;
  }
  // Clean up the config manager
  if (config_manager_ != nullptr) {
    delete config_manager_;
  }

  // Clean up the log manager
  if (log_manager_ != nullptr) {
    delete log_manager_;
  }

  // Clean up any remaining events on the event queue.  They're useless at
  // this point, and SimExec is responsible for returning their memory
  while (!event_queue_.empty()) {
    SimBaseEvent *head_event = event_queue_.front();
    event_queue_.pop_front();
    delete head_event;
  }
  // null the singleton.
  the_exec_ = nullptr;
}  // ~SimExec

// Call this method to launch simulation.  Everything should be initialized
// before calling Run().  Initialization should have scheduled the initial
// events.  After that, normal processing can schedule events.  For
// instance, the stimulus reader might schedule the initial 1 second's worth
// of events, then schedule a timer to be called 0.5 seconds in for the next
// 1 second, etc.
//
// Returns - the time when the simulation ended.  This could either be the
//           time of the final event, or the run_until_time, whichever is
//           earlier.
SimTime SimExec::Run() {
  // To avoid continuously setting the current time while processing events
  // with the same time, two loops are employed.  The inner loop processes
  // events with the same time, while the outer loop advances the time.
  // Note that any events added by the Dispatch() method will be added
  // no earlier than the last of the events at the current time, since
  // events earlier than the current simulator time are discarded.  Thus,
  // these loops will process all valid events added to the queue as a 
  // result of Dispatch actions.
  // Set up the initial conditions for the event loop

  bool have_events = false;
  SimBaseEvent *head_event = nullptr;
  // Determine whether there is at least one event to process, and, if so,
  // get the first event.
  if (!event_queue_.empty()) {
    have_events = true;
    // Note that the result of attempting to get the front event from an 
    // empty list is undefined, so always check for emptiness first.
    head_event = event_queue_.front();
    // The first event to be processed sets the initial current time
    curr_time_.SetTime(head_event->event_time());
  }
  // Loop as long as there are events and the simulation hasn't exceeded the
  // specified time limit.
  while (have_events && curr_time_.AsEarlyAs(run_until_time_)) {
    // Loop through the events at the current time.
    // head_event will not be null as long as there is at least one event to
    // process.
    while (have_events && 
           head_event->SameTimeAs(curr_time_)) {
      head_event->Dispatch();
      // Remove the just processed event from the queue & delete it
      event_queue_.pop_front();
      delete head_event;
      if (!event_queue_.empty()) {
        head_event = event_queue_.front();
      } else {
        have_events = false;
      }
    }
    // We've run out the string of events at curr_time_...
    if (have_events) {
      // ... but we still have more events in the queue.  head_event was
      // advanced in the while() above, so we use it to set the new
      // time for current_time
      curr_time_.SetTime(head_event->event_time());
    }
  }

  // Since "curr_time_" might have been advanced beyond "run_until_time_"
  // in the loop above, we'll report the earlier of the two values as the
  // simulation stop time.  If we ran out of events before "run_until_time_"
  // then "curr_time_" will be earlier than "run_until_time_", and that's
  // what should be reported.  If "curr_time_" is later than
  // "run_until_time_" then we hit the run time limit between events, but the
  // "curr_time_" value was advanced to the next value in the event loop
  // which caused the test in the event loop to fail.  In this case, we want
  // to report the "run_until_time_" as the end of the simulation.
  SimTime return_time((curr_time_.AsEarlyAs(run_until_time_)) ?
                      curr_time_.GetUserTime() :
                      run_until_time_.GetUserTime());

  std::cout << std::endl << std::endl;
  std::stringstream message;
  message << "Simulation finished at time " << return_time.GetUserTime();
  UtilStdMsg(kCommonStrNote, message.str());

  return return_time;
} // run


// Public method to force the cleanup of the SimExec singleton.  This allows
// the class to have a private ctor, but still be a good citizen w.r.t. memory
// NOTE:  Once the singleton has been torn down, all pointer references to it
// will be invalid and all state will be lost.  Be sure that this is what you
// want.
void SimExec::TearDown() {
  delete this;
  // Should be handled in the dtor, but this is extra safety...
  the_exec_ = nullptr;

} // TearDown()


// Events are inserted in time order.  If times are identical, the
// event is inserted after all other events already scheduled at the
// same time.
// This method tries to determine the best way approach for the
// insertion algorithmically.  Considerations may include the underlying
// data structure as well as the expected final insertion point.
//
// "new_event" - the event to place on the simulation event queue
void SimExec::ScheduleEvent(SimBaseEvent *new_event) {
  EnqueueEventFromEarliest(new_event);
}

// Events are inserted in time order.  If times are identical, the
// event is inserted after all other events already scheduled at the
// same time.
// This method accepts a suggestion regarding insertion direction 
// from the caller
//
// "new_event" - the event to place on the simulation event queue
// "insert_from" - specifies whether to insert from the head, or the tail
void SimExec::ScheduleEvent(SimBaseEvent *new_event, 
                            const EventInsert insert_from ) {
  if (insert_from == kInsertFromNow) {
    EnqueueEventFromEarliest(new_event);
  } else {
    EnqueueEventFromLatest(new_event);
  }
}

#ifdef TEST_HARNESS
  // Test harness support
void SimExec::DumpQueue() {
  std::cout << "*** Contents of the Event Queue:\n";
  std::list<SimBaseEvent *>::iterator iter = event_queue_.begin();
  while (iter != event_queue_.end()) {
    (*iter)->DumpEvent();
    iter++;
  }
}
#endif


// Inserts an event by starting at the earliest event in the queue 
// searching forward for the proper insertion point. 
//
// "new_event" - the event to place on the simulation event queue
void SimExec::EnqueueEventFromEarliest(SimBaseEvent *new_event) {
  // Sanity check on incoming event.  If it's in the past, print and throw it
  // away.
  if (new_event->EarlierThan(curr_time_)) {
    std::cerr << kCommonStrError << "Attempted to schedule event in the past. "
                 " Event Time: " << new_event->event_time().GetUserTime() 
              << "Current Simulation Time: " << curr_time_.GetUserTime() 
              << std::endl;
    delete new_event;
    return;
  }
  // Iterate through the event queue as long as the time of the events is
  // earlier than, or equal to, the new event's time.
  std::list<SimBaseEvent *>::iterator iter = event_queue_.begin();
  while ((iter != event_queue_.end()) &&
         (*iter)->AsEarlyAs(*new_event)) {
    iter++;
  }
  // By this point, the iteration has reached one of three states.  The most
  // likely case is that the iterater is at the first event with a time, 
  // later than the new event, so this is the insertion point.  Alternatively,
  // the queue might be empty, or the the new event is later than any events
  // already in the queue.  In either of these two cases, the new event should
  // be appended to the queue.
  if (iter != event_queue_.end()) {
    // Insert before the event with a time later than the new event.
    event_queue_.insert(iter, new_event);
  } else {
    // Special case, when the new item is to be inserted at the end of the
    // list, "iter" won't hold a valid insertion point.  Also handles the case
    // where the list was empty.
    event_queue_.push_back(new_event);
  }
} // EnqueueEventFromEarliest()


// Inserts an event by starting at the latest event in the queue and
// searching backward for the proper insertion point. For event
// management implementations where backwards traversal is relatively
// efficient, this method allows the option of a potential optimization
// from searching backward from the latest event.
//
// "new_event" - the event to place on the simulation event queue
void SimExec::EnqueueEventFromLatest(SimBaseEvent *new_event) {
  // TBD, not yet implemented.
  EnqueueEventFromEarliest(new_event);
}
