/*****************************************************************************
*
*   DESCRIPTION:
*     File containing the implementations of the class that handles loading
*     the TextEvent stimuli that drive the execution of the simulation.  
*
*     This file defines:
*
*     StimTextEventLoader - the class that manages the file system interface,
*             creates the events represented by each stimulus and handles
*             placing the new SimTextEvents into the simulation executive.
*
*     The base StimLoader class handles most of the heavy lifting.  This
*     class overrides a handful of methods specific to handling
*     SimTextEvents and their associated data fields.
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
#include <iostream>
#include <string>

#include "common_messages.hpp"
#include "sim_text_event.hpp"
#include "stim_text_event_loader.hpp"
#include "sim_exec.hpp"

constexpr SimTime::UserTime kReadPeriod = 1.0E3;

// The ctor attempts to access the stimulus file and examine the first
// stimulus record to establish a time baseline.
// "stimulus_path" specifies the pathname to the stimulus file.  If the
// specified file cannot be loaded, the program issues a fatal error and
// exists with a failure status.  This is necessary, because simulation
// cannot proceed without stimulus.
// IMPLEMENTATION NOTE:
// The base class cannot load the stimulus file directly.  The file format
// and contents is specific to each derived loader.  However, that much of
// the file open code and stimulus processing may be common among derived
// loaders.  Thus, the "OpenStimFile()" method is defined in the base class
// and not overriden by this derived class (although it could be).
// The StimTextEventLoader class adds all necessary customization for this
// example by overriding the "ReadStimRecord()" method.
//
// "stimulus_path" - pathname to the stimulus file.
StimTextEventLoader::StimTextEventLoader(const std::string &stimulus_path) :
                                                                StimLoader() {
  // Initialize member variables.
  ResetStimData();

  // Attempt to open the file.  As part of this process, the first 
  // stimulus record is examined both to ascertain whether this is a
  // useful stimulus file, and to establish a simulation time baseline 
  if (!OpenStimFile(stimulus_path)) {
    std::string error("Unable to open stimulus file \"");
    error.append(stimulus_path).append(".\"\nSimulation cannot proceed "
                                      "without stimulus.");
    UtilFatalErrorAndDie(error);
  }
}  // StimTextEventLoader


StimTextEventLoader::~StimTextEventLoader() {
  // Note:  The base class destructor should take care of closing the
  // stimulus file stream.
}  // ~StimTextEventLoader


// Resets the "cached" stimulus data to prepare for reading another record.
void StimTextEventLoader::ResetStimData() {
  StimLoader::ResetStimData();
  stim_payload_.clear();
}  // ResetStimData


// Read a single record from the stimulus file.
//
// Returns - "true" if all fields are read correctly, "false" otherwise.
bool StimTextEventLoader::ReadStimRecord() {
  // separator for the CSV stimulus file
  char separator;
  return stim_file_ >> stim_event_time_ >> separator >> stim_payload_;
}  // ReadStimRecord


// Creates a new SimTextEvent with data fields from the stimulus file and
// schedules the new event with the simulation executive
void StimTextEventLoader::PostEvent() {
  SimTextEvent *new_event = new SimTextEvent(stim_event_time_, stim_payload_);
  // The simulation executive will be responsible for the memory.
  SimExec::the_exec()->ScheduleEvent(new_event);
}  // PostEvent
