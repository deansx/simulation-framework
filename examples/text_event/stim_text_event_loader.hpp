/*****************************************************************************
*
*   DESCRIPTION:
*     Header file describing the classes that implement loading the
*     SimTextEvent stimuli that drive the execution of a simulation.
*     This file declares:
*
*     StimTextEventLoader - the class that manages the file system interface,
*             creates the SimTextEvent event objects represented by each
*             stimulus record and handles placing the events in the
*             simulation executive.
*
*     Since this is a fairly thin derived class, most of the meaningful
*     interface documentation is available in the base class header file.
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

#ifndef SIM_EXAMPLES_TEXT_EVENT_STIM_TEXT_EVENT_LOADER_HPP_
#define SIM_EXAMPLES_TEXT_EVENT_STIM_TEXT_EVENT_LOADER_HPP_

#include <string>

#include "stim_loader.hpp"


class StimTextEventLoader : public StimLoader {

 public:
  // "stimulus_path" - pathname to the stimulus file.
  StimTextEventLoader(const std::string &stimulus_path);
  ~StimTextEventLoader();
 
 protected:
  // Loads a single record from the stimulus file.
  //
  // Returns - "true" if read succeeded for all record fields,
  //       otherwise "false".
  virtual bool ReadStimRecord();
  
  // Post a single event to the event queue.
  virtual void PostEvent();

  // Resets the stimulus data members back to initial states.  Potentially
  // useful for constructors and resets after a post.
  virtual void ResetStimData();

  // Stimulus record field(s):
  // String payload field
  std::string stim_payload_;

 private:
    // As per the coding standard
  DISALLOW_COPY_AND_ASSIGN(StimTextEventLoader);
}; // class StimTextEventLoader

#endif   // SIM_EXAMPLES_TEXT_EVENT_STIM_TEXT_EVENT_LOADER_HPP_
