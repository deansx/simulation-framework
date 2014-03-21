/*****************************************************************************
*
*   DESCRIPTION:
*     File containing the implementation of the LogTextEvent class.
*     This class generates log entries for events SimTextEvents.  Although
*     the data involved here is trivial, a more complex class might involve
*     quite a bit of data manipulation to prepare for logging a record.
*     the class is defined here to provide an example of how it might work.
*   
*   LICENSE:  TBD
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
#include <fstream>

#include "common_messages.hpp"
#include "log_text_event.hpp"


// Constructor initializes staged data fields
//
// "log_path" - pathname for the log file.
LogTextEvent::LogTextEvent(std::string log_path) : LogMgr(log_path) {
  // Call reset to set initial states for members
  Reset();
}


// Write the column headings for the output CSV.  Execution terminates if
// the method encounters problems with the write.
void LogTextEvent::WriteHeaderOrDie() {
  // Dump the header line into the file
  if ((log_stream_->is_open()) && (log_stream_->good())) {
    *log_stream_ << "time,text\n";
    if (!(*log_stream_)) {
      // Probably a logic error in the caller's code...
      UtilFatalErrorAndDie("Failed to write log header.\n"
                           "Output stream returned bad status"
                           "(LogTextEvent)");
    }  // write succeeded
  } else {
    // Probably a logic error in the caller's code...
    UtilFatalErrorAndDie("Unable to write log header.\n"
                         "Output stream either not open, or returned bad "
                         "status. (LogTextEvent)");
  }  // writable file
}  // writeHeadings


// Write the data collected for one record.  Execution terminates if
// the method encounters problems with the write.
void LogTextEvent::WriteARecordOrDie() {
  // Make sure that the data is ready to go
  if (VerifyStagedReady()) {
    if ((log_stream_->is_open()) && (log_stream_->good())) {
      *log_stream_ << event_time_.GetUserTime() << "," << event_text_ 
                      << std::endl;
      if (*log_stream_) {
        // The steam's status seems to be OK, so it's likely that the write
        // was successful.  We can reset the data fields
        Reset();
      } else {
        // Probably a logic error in the caller's code...
        UtilFatalErrorAndDie("Unable to write log record.\n"
                             "Output stream returned bad status. "
                             "(LogTextEvent)");
      }  // write succeeded
    } else {
      // Probably a logic error in the caller's code...
      UtilFatalErrorAndDie("Unable to write log record.\n"
                           "Output stream either not open, or returned bad "
                           "status.\n(LogTextEvent)");
    }  // writable file
  } else {
    // Probably a logic error in the caller's code...
    UtilFatalErrorAndDie("LogTextEvent: Unable to write log record.\n"
                         "Staged data not ready. (LogTextEvent)");
  }  // data ready
}  // WriteARecord


// Set the staged flags to all "false", to prepare for another set of
// staged data.  Also, sets the base class data_ready_ flag to "false"
void LogTextEvent::ClearStagedFlags() {
  // Walk through the array of flags and negate them all
  for (auto &staged : data_staged_) {
    staged = false;
  }
  // set the data_ready field in the base class
  set_data_ready(false);
}


// Reset the data fields and staged flags to prepare for another pass
void LogTextEvent::Reset() {
  // Clear the staged flags first, no writes will succeed once this is
  // complete
  ClearStagedFlags();
  // Initialize the data fields
  event_time_.SetTime(0.0);
  event_text_.clear();
}  // Reset


// Stages the event's time data for logging
//
// "event_time" - time value of the event to be logged
void LogTextEvent::StageEventTime(const SimTime &event_time) {
  event_time_.SetTime(event_time);
  SetFieldStaged(kEventTimeStaged);
}


// Stages the event's text data for logging
//
// "event_text" - text value of the event to be logged
void LogTextEvent::StageEventText(const std::string &event_text) {
  event_text_ = event_text;
  SetFieldStaged(kEventTextStaged);
}


// Checks all data fields to make sure that they are staged and sets
// the base class data_ready flag appropriately.
//
// Returns - "true" if all of the data has been staged and is ready to
//       write, which also means that the base class has been informed
//       that the data is ready.  Otherwise, "false", which also means
//       that the base class has been informed that the data is not ready.
bool LogTextEvent::VerifyStagedReady() {
  // Walk through the array of flags and see if any are not true
  bool all_staged = true;
  for (auto staged : data_staged_) {
    if (!staged) {
      // We encountered a flag still set to false.
      all_staged = false;
      // No point in continuing, we just need the one
      break;
    }
  }
  // Set the flag in the base class appropriately.
  set_data_ready(all_staged);
  return all_staged;
}
