/*****************************************************************************
*
*   DESCRIPTION:
*     Header file describing the base classes that implement loading the
*     stimuli that drive the execution of the simulation.  These prototype
*     classes manage the base SimBaseEvent objects.  Users are expected to
*     override most of the StimLoader methods to properly deal with
*     non-trivial event objects.  The LoadStimTimerEvent object may be
*     usable as declared here.
*
*     This file declares both:
*
*     StimLoader - the base class that declares an interface for managing
*             the file system interaction and creating objects from entries
*             in the stimulus file.  Child classes will actually create
*             the events represented by each stimulus record and may handle
*             transferring the newly minted events to the simulation
*             executive.  For non-trivial simulation environments,
*             developers will probably want to derive a specific class from
*             this one, and define their own stimulus loading code.
*
*     LoadStimTimerEvent - derived event class that represents timers for 
*             loading stimulus into the simulation executive.  This class
*             will be appropriate for many loaders derived from the base
*
*     See ../examples/text_event/ for examples of working with the
*     derived classes.
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

#ifndef SIM_DESIM_STIM_LOADER_HPP_
#define SIM_DESIM_STIM_LOADER_HPP_

#include <fstream>
#include "sim_time.hpp"
#include "sim_base_event.hpp"


class StimLoader {

 public:
  // Specific Stimulus loaders derived from this base should attempt
  // to open
  // The simulator won't run without a valid source of stimulus, so the
  // ctor will try to open the stimulus file.  If it fails, a fatal error,
  // is issued and the program exits with a failure.
  // We can't append "OrDie" to the ctor, but attempting to initialize
  // without a valid stimulus file is a logic error.  The caller should
  // check for stimulus file existance, and readability before attempting
  // to construct this object, unless it's OK to just crash here.
  StimLoader();
  ~StimLoader();

  // Loads the initial set of stimulus.  After this call, the data is ready
  // for the simulator to run.  StartLoadingOrDie() either succeeds, or the
  // program exits with a fatal error, so no return value.
  virtual void StartLoadingOrDie();

  // Current status of the stimulus file.
  //
  // Returns - "true" if the file is not at EOF and the status is good
  //       "false" otherwise
  virtual bool StimFileOK();

  // The stimulus loader works on blocks of events bounded by an upper
  //       time limit.
  //
  // Returns - the current upper limit on time to load
  virtual SimTime::UserTime read_until() const { return read_until_; };

  // Load stimulus records into the queue until the upper bound on time
  // is reached.  Note: This method will die if OpenStimFileOrDie() has
  // not been run successfully before calling LoadQueue().  Since this
  // is not considered to be the normal course of events, "OrDie" was
  // not appended to the method name.
  //
  // Returns - "true" as long as more stimulus is loaded onto queue, 
  //       "false" otherwise
  virtual bool LoadQueue();
 
 protected:
  // Attempts to open the stimulus file at the specified path
  // OpenStimFile() MUST succeed before the caller can call either
  // StartLoadingOrDie() or LoadQueue().
  //
  // "stimulus_path" - pathname of the stimulus file
  // Returns - "true" if the file opens OK and appears to be a valid
  //       stimulus file, otherwise returns "false".
  virtual bool OpenStimFile(const std::string &stimulus_path);

  // Utility method to post a single event to the event queue.  Derived
  // classes must redefine this method with the proper arguments to
  // construct an appropriate event object.
  virtual void PostEvent() = 0;

  // Loads a single record from the stimulus file.  Must be redefined by
  // derived classes to implement each specific record type.
  //
  // Returns - "true" if read succeeded for all record fields,
  //       otherwise "false".
  virtual bool ReadStimRecord() = 0;

  // Resets the stimulus data members back to initial states.  Potentially
  // useful for constructors and resets after a post.  Should be
  // overridden for each derived class.
  virtual void ResetStimData();

  // Accessor/Mutator for the ready_ flag
  //
  // Returns - the value of the "ready_" flag
  bool ready() { return ready_; };
  // "ready_status" - value to be used to set the "ready_" flag
  void set_ready(bool ready_status) { ready_ = ready_status; };

  // NOTE: Most times used in this class are represented as UserTime, 
  // since these are being read directly from the stimulus file.

  // Rough upper limit on time to read for this pass
  SimTime::UserTime read_until_;

  // Stream to the stimulus file
  std::ifstream stim_file_;

  // Stimulus record field(s):
  // Event time, in UserTime, as read
  SimTime::UserTime stim_event_time_;

 private:
  // Flag signifying that the stimulus loader is ready for business.  Will 
  // be true if the stimulus file is open and apparently valid as well as
  // if the time baseline is set from the stimulus file.  See the acccessor/
  // mutator methods for access from derived classes.
  bool ready_;
  // As per the coding standard
  DISALLOW_COPY_AND_ASSIGN(StimLoader);
}; // class StimLoader


// Placed in the event queue to call for more events from the Stimulus loader.
class LoadStimTimerEvent : public SimBaseEvent {
 public: // methods
  LoadStimTimerEvent(const SimTime &event_time, StimLoader *stim_loader)
                     : SimBaseEvent(event_time), stim_loader_(stim_loader) {};

  ~LoadStimTimerEvent() {};

  // Method called when this event is "dispatched" from the simulation
  // executive.
  virtual void Dispatch() const;
 protected:
  // class that will be loading / feeding stimulus to the queue
  StimLoader *stim_loader_;

 private:
    // As per the coding standard
  DISALLOW_COPY_AND_ASSIGN(LoadStimTimerEvent);
}; // LoadStimTimerEv

#endif   // SIM_DESIM_STIM_LOADER_HPP_
