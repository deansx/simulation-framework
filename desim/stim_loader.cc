/*****************************************************************************
*
*   DESCRIPTION:
*     File containing the base implementations of the classes that handle
*     loading the stimuli that drive the execution of the simulation.
*  
*     This file defines both:
*
*     StimLoader - the class that manages the file system interface, creates
*             the events represented by each stimulus and handles placing
*             the events in the simulation executive.
*     LoadStimTimerEvent - derived event class that represents timers for 
*             loading stimulus into the simulation executive.
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
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "common_strings.hpp"
#include "common_messages.hpp"
#include "stim_loader.hpp"
#include "sim_exec.hpp"
#include "sim_base_event.hpp"

// Duration of the "chunks" of stimuli to be read from the file on each
// pass.  Derived loaders may want to set a different value
constexpr SimTime::UserTime kReadPeriod = 1.0E3;

// Member initializer list takes care of all required initialization.
StimLoader::StimLoader() : read_until_(0.0), ready_(false),
                           stim_event_time_(0.0) {
}  // StimLoader


StimLoader::~StimLoader() {
  if (stim_file_.is_open()) {
    stim_file_.close();
  }
}  // ~StimLoader


// Resets the "cached" stimulus data to prepare for reading another record.
void StimLoader::ResetStimData() {
  stim_event_time_ = 0.0;
}


// Attempts to open the stimulus file, and validate that it is, indeed, a
// stimulus file by examining the first stimulus record.  The data in this
// record is used to establish the simulation time baseline.
// If successful, sets the both the "stim_event_time_" and the "ready_"
// flag.
//
// "stimulus_path" - pathname of the stimulus file
// Returns - "true" if the file opens OK and appears to be a valid
//       stimulus file, otherwise returns "false".
bool StimLoader::OpenStimFile(const std::string &stimulus_path) {
  ready_ = false;
  stim_file_.open(stimulus_path);
  if (stim_file_.is_open() && stim_file_.good()) {
    std::cout << kCommonStrNote << "Reading stimulus from file:  "
              << stimulus_path << std::endl;
    // The stimulus should be time ordered with earlest at the top
    // Reading a CSV file, this represents the comma separator
    char separator;
    // Dummy variables for other fields, as needed.  Note that the base
    // case does not have any fields beyond time.
    std::string field02;
    // Try to grab the first line.  Will use the event_time field as the
    // time baseline.  Other fields are read to verify type, but the
    // data isn't used in this method.
    bool read_success = ReadStimRecord();
    if (!read_success || stim_file_.fail()) {
      // Presence of a header line is a possible cause of failure.  Try
      // to get past it.
      const std::streamsize stream_size = 2048;
      char header_buffer[stream_size];
      // Clear the error flags
      stim_file_.clear();
      // Attempt to read the header line
      stim_file_.getline(header_buffer,stream_size);
      int file_position = stim_file_.tellg();
      // One more attempt to read good data
      read_success = ReadStimRecord();
      if (!read_success || stim_file_.fail()) {
        // Can't get good data after two tries.  Time to give up.
        std::string error("Could not read stimulus from:  \"");
        error.append(stimulus_path).append("\"\nEither bad pathname, "
                                           "or bad data.");
        UtilStdMsg(kCommonStrError, error);
      } else {
        // Second attempt succeeded.
        // Reset to reread the first data record.
        stim_file_.seekg (file_position, std::ios::beg);
        std::cout << kCommonStrNote
                  << "Stimulus file header line skipped.\n";
      }  // if second bad read
    } else {
      // Read succeeded, there does not appear to be a header line
      std::cout << "Base Time is:  " << stim_event_time_ << std::endl;
      // Reset the stream's get pointer
      stim_file_.seekg (0, std::ios::beg);
    }  // first read succeeded
    if (read_success && !stim_file_.fail()) {
      // The file seems OK, and the data seems OK.
      ready_ = true;
      read_until_ = stim_event_time_ + kReadPeriod;
    }
  } // file is open and good
  return ready_;
}  // OpenStimFilePath

// Attempt to load the first set of stimulus.  Once this call is complete
// the simulator should be ready to run
void StimLoader::StartLoadingOrDie() {
  if (!LoadQueue()) {
    std::cout << kCommonStrFatal << "Could not read stimulus data.  "
              << "Exiting.\n";
    exit(EXIT_FAILURE);
  }
}  // Init


// Loads stimulus records into the queue until a record is encountered 
// with a time that is equal to, or exceeds, the upper bound on time
// for this pass.
// Note: This method will die if OpenStimFileOrDie() has not been run
// successfully before calling LoadQueue().
//
// Returns - "true" as long as more stimulus is loaded onto queue, 
//       "false" otherwise
bool StimLoader::LoadQueue() {
  // LoadQueue() cannot be called until OpenStimFile() has succeeded.
  // This situation should never happen in normal programs, so it's a fatal
  // error if it does.
  if (!ready_) {
    std::cout << kCommonStrFatal << "OpenStimFile() MUST succeed before "
                                    "LoadQueue() is called.  Exiting.\n";
    exit(EXIT_FAILURE);
  }
  // IMPLEMENTATION NOTES:
  // This code in the base class provides a framework for loading stimulus
  // records.  While it may be useful for many derived classes, it can,
  // of course be overridden.  However, many derived classes may be able
  // to achieve specialization simply by overriding "ReadStimRecord()" with
  // a derived class specific implementation.
  // 
  // Stimulus field values are cached as data members named, by 
  // convention "stim_*" of this object to act as an ad hoc look ahead
  // buffer.  This enables LoadQueue() to read the record beyond the 
  // upper time limit, while retaining this information so that it can
  // be passed along to the simulator with the next batch of stimulus
  // entries.

  // Tells the method to use the data already "buffered" in this object
  // to generate the first event for this pass.
  static bool look_ahead = false;
  // The stimulus will be read until a record is encountered with a 
  //  time field that exceeds this value.
  static SimTime::UserTime end_read_time;
  // Status of the most recent read attempt
  bool success = false;
  // stim_event_time_ should be set either from OpenStimFile(), which
  // we know succeeded because of the ready_ flag, or the last pass
  // through LoadQueue()
  read_until_ = stim_event_time_ + kReadPeriod;

  if (look_ahead) {
    // There is data buffered from the previous read, post it now
    PostEvent();
    look_ahead = false;
  }
  if (stim_file_.is_open()) {
    // stim_event_time_ has an initial value either from OpenStimFile(),
    // (which we know succeeded because of the ready_ flag), or the last
    // pass through LoadQueue().
    // read_until_ was either set in OpenStimFile(), or on the previous
    // pass through LoadQueue().
    // ReadStimRecord will read the next stimulus record and store the
    // fields in the appropriate stim_* data members of this object.
    while ((stim_event_time_ < read_until_) && ReadStimRecord()) {
#if 0
      if (stimFileOK()) {
        cout << "\n<<<<<<<<<<<<<<<<<<<<<< FILE IS STILL GOOD \n";
      } else {
        cout << "\n<<<<<<<<<<<<<<<<<<<<<< AT EOF!!!!! \n";
      }
      cout << "Time is:     " << time << "\n";
      cout << "\n";
#endif
      success = true;
      // Either put the new information on the queue, or cache it for the
      // next pass through
      if (stim_event_time_ < read_until_) {
        // This event can go straight on to the queue
        PostEvent();
      } else {
        // Read progressed beyond the maximum time, need to buffer the
        // values for the next pass
        look_ahead = true;
      } // Has the reader reached the end of this period.
    }  // Either out of time, or end of file
    
  } // stimFile.is_open()
  if (success || StimFileOK()){
    // Either successfully read something in this pass, or there is still
    // more stimulus in the file. Post a timer event to make another pass.
    // If nothing was read in this pass, but there is still stimulus data
    // in the file, schedule another pass
    // If the current group of reads reached EOF, the loadQueue from the
    // timer created here will not be successful, but the simulation may
    // proceed without additional stimulus.
    if (success) {
      // Found at least one stimulus record, so set next read increment
      // using the last record read as a time baseline
      read_until_ = stim_event_time_ + kReadPeriod;
    } else {
      // Did not find any records in this period, use the current read period
      // as the time baseline to try to find something
      read_until_ += kReadPeriod;
    } // not success, but more to read
    // Create a time object with the current event time, so that the timer
    // fires at a time consistent with the most recently read object.

    // Schedule a timer event that will call for the next batch of events
    // to be loaded.  Use the current event time, so that the timer fires at a
    // time coincident with the most recently read object.  The compiler will
    //  generate code to convert the "time" to a SimTime object.
    SimExec::the_exec()->ScheduleEvent(new LoadStimTimerEvent(stim_event_time_, 
                                                              this));
  } // either success, or more to read
  return success;
}  // loadQueue


// Current status of the stimulus file.
//
// Returns - "true" if the file is not at EOF and the status is good
//       "false" otherwise
bool StimLoader::StimFileOK() {
  return (stim_file_.is_open() && !stim_file_.eof() && stim_file_.good());
}  // StimFileOK


// Called from the simulation executive when this event is "executed"
// Attempts to load more stimulus into the queue
void LoadStimTimerEvent::Dispatch() const {
#if 1
  std::cout << "#########Executing LoadStimTimerEvent Dispatch at:  " 
            << event_time_.GetUserTime() << std::endl;
#endif
  bool loaded = stim_loader_->LoadQueue();
}  // dispatch
