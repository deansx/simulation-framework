/*****************************************************************************
*
*   DESCRIPTION:
*     Header file declaring the execution executive for the Discrete Event
*     Simulation system.
*     This executive actually excutes the simulation, and provides the
*     primary public interface to the simulation engine.
*     
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

#ifndef SIM_DESIM_SIM_EXEC_HPP_
#define SIM_DESIM_SIM_EXEC_HPP_

#include <fstream>
#include <list>
#include <string>

#include "basic_defs.hpp"
#include "sim_time.hpp"
#include "sim_base_event.hpp"
#include "log_mgr.hpp"
#include "config_mgr.hpp"


class StimLoader;
class SimExec;

// This class provides a discrete event simulator executive for the
// simulation environment.  In practice, this is expected to be a
// singleton for the whole environment.
// In addition to some administrative functions, this class manages
// the simulation event data structure, which is ordered by time.
// Simulation progresses by accessing the earliest event, executing it,
// then moving to the next event... As the simulation executes, it's
// reasonable to expect that new events will be added. These execute and
// may add new events.  Processing continues until either the simulation
// runs out of events (the event queue becomes empty), or the specified
// "run until" time is reached.
class SimExec {
 public:
  // Users may want to attempt to provide a suggestion regarding which
  // direction the SimExec scans when inserting events.  This is not
  // guaranteed to significantly speed up insertion, but it could be
  // beneficial.
  // "kInsertFromNow" - tells the algorithm to start scanning for the
  //       proper insertion point from the current time.
  // "kInsertFarFuture" - suggests that the scan start from the latest
  //       event currently managed by the SimExec
  enum EventInsert { kInsertFromNow, kInsertFarFuture };

  // The simulation environment expects a singleton object to manage
  // the event queue for the environment.  Accessing the executive 
  // exclusively through this method ensures that we don't, inadvertently
  // have multiple executives managing events.  This method creates
  // a new executive, if needed, and returns a pointer to the singleton
  // executive object.
  //
  // Returns - a pointer to the singleton simulation executive object.
  static SimExec *the_exec() {
    if (the_exec_ == nullptr)
      the_exec_ = new SimExec;
    return the_exec_;
  } // instance()
  
  // Access the current simulation time, as kept by the simulator.  Simulator
  // current time is defined as the time of the event most recently executed
  // from the event queue.
  //
  // Returns - a time object representing the simulator's current time.
  SimTime curr_time() const { return curr_time_; };

  // Initialize the SimExec.  Note that the stimulus loader is specific to
  // each simulation application, so the executive takes a pointer to the
  // base stimulus loader class.  Creation of the "config_manager", the
  // "log_manager" and the "stim_loader" is the responsibility of the
  // caller, but the executive will be responsible for the memory.
  // The executive will also call the new "stim_loader"'s method to load the
  // first set of stimulus.
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
  void Init(const SimTime &run_until_tm,
            ConfigMgr *const config_manager,
            LogMgr *const log_manager,
            StimLoader  *const stim_loader);

  // Methods to add events to the simulation executive.
  //
  // Events are inserted in time order.  If times are identical, the
  // event is inserted after all other events already scheduled at the
  // same time.
  // The caller should allocate the event instance.  The
  // object will be deallocated by the simulation executive when the event
  // is processed.  The caller DOES  NOT need to delete the object.
  //
  // This method tries to determine the best way approach for the
  // insertion algorithmically.  Considerations may include the underlying
  // data structure as well as the expected final insertion point.
  //
  // "new_event" - the event to place on the simulation event queue
  void ScheduleEvent(SimBaseEvent *new_event);

  // This variant accepts a suggestion regarding insertion direction 
  // from the caller
  //
  // "new_event" - the event to place on the simulation event queue
  // "insert_from" - specifies whether to insert from the head, or the tail
  void ScheduleEvent(SimBaseEvent *new_event, const EventInsert insert_from );

  // Call this method to launch simulation.  Everything should be initialized
  // before calling run().  Initialization should have scheduled the
  // initial events.  After that, normal processing can schedule
  // events. For instance, the stimulus reader should schedule the initial
  // 1 second's worth of events, then schedule a timer to be called 0.5
  // seconds in for the next 1 second, etc.
  //
  // Returns - the time when the simulation ended.
  SimTime Run();
  
  // Accessor for the log manager.  Use this object to send log output
  // from the execution of the simulator to an external log file.
  //
  // Returns - a pointer to the log manager
  LogMgr *log_manager() { return log_manager_; };

  // Since this class has a private dtor, the TearDown method takes care of
  // cleaning up the object and its allocated data.
  void TearDown();

#ifdef TEST_HARNESS
  // Debug support for dumping the queue to std out.  Should never be present
  // in production code, but needed for test harness.
  void DumpQueue();
#endif

 private:
  // Ctor & Dtor private since only internal simulator code should be
  // creating / destroying the singleton simulation executive.
  SimExec() : curr_time_(0.0), run_until_time_(0.0), stim_loader_(nullptr),
              log_manager_(nullptr), config_manager_(nullptr) { };
  ~SimExec();

  // Methods that actually insert events into the simulation data structure:

  // Inserts an event by starting at the earliest event in the queue 
  // searching forward for the proper insertion point. 
  //
  // "new_event" - the event to place on the simulation event queue
  void EnqueueEventFromEarliest(SimBaseEvent *new_event);

  // Inserts an event by starting at the latest event in the queue and
  // searching backward for the proper insertion point. For event
  // management implementations where backwards traversal is relatively
  // efficient, this method allows the option of a potential optimization
  // from searching backward from the latest event.
  //
  // "new_event" - the event to place on the simulation event queue
  void EnqueueEventFromLatest(SimBaseEvent *new_event);

  // Current simulation time object.  This reflects the time of the event most
  // recently processed from the event queue, and is initialized to 0.
  SimTime curr_time_;

  // Time to stop the run.  Note that the run will also terminate when the
  // event queue is empty, which might occur before the run_until time is
  // reached.
  SimTime run_until_time_;

  // The data structure containing events staged for execution.  Currently
  // using a doubly linked list as the queue.
  std::list<SimBaseEvent *> event_queue_;

  // object that will be loading / feeding stimulus to the queue
  StimLoader *stim_loader_;
  // Object that manages the log file.  Derived execs could have multiple
  // log managers
  LogMgr *log_manager_;
  // Object that manages configuration data
  ConfigMgr *config_manager_;
  

  // Pointer to the singleton instance of this class
  static SimExec *the_exec_;

 private:
    // As per the coding standard
  DISALLOW_COPY_AND_ASSIGN(SimExec);
}; // class SimExec

#endif   // SIM_DESIM_SIM_EXEC_HPP_
