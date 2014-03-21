/*****************************************************************************
*
*   DESCRIPTION:
*     Header file declaring the LogTextEvent example class
*     This class generates log entries for an example simulation using
*     SimTextEvents.
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

#ifndef SIM_EXAMPLES_TEXT_EVENT_LOG_TEXT_EVENT_HPP_
#define SIM_EXAMPLES_TEXT_EVENT_LOG_TEXT_EVENT_HPP_

#include <string>

#include "basic_defs.hpp"
#include "log_mgr.hpp"
#include "sim_time.hpp"


class LogTextEvent : public LogMgr {
 public:
  // The constructor handles creating and opening a stream to the log file
  // specified by "log_path".
  // NOTE:  A fatal error will be generated and the application will exit
  // with a failure if the "log_path" cannot be opened.  Callers should
  // ensure that the specified file exists, and is writable, before
  // constructing a "LogTextEvent" object
  //
  // "log_path" - pathname for the log file.
  LogTextEvent(std::string log_path);

  // For this example, we don't need the "std::ofstream*" version of the 
  // ctor.

  // Write the column headings for the output CSV
  // Failure signals something pretty serious so method generates a fatal
  // error message and terminates.
  void WriteHeaderOrDie();
  // Write a record that contains the staged data.
  // This method will attempt to verify that all data for the record is
  // properly staged before it is logged.
  // Failure signals something pretty serious so method generates a fatal
  // error message and terminates.
  void WriteARecordOrDie();

  // Reset the staged data fields to prepare for another pass
  void Reset();

  // Stage data for writing.  These methods prepare the raw data for output
  // to the appropriate data fields.
  // Of course, I recognize that caching is complete overkill for this
  // simple example, but it suggests a template for more sophisticated
  // output managers.

  // Stage the event's time data
  //
  // "event_time" - time value of the event to be logged
  void StageEventTime(const SimTime &event_time);
  
  // Stage the event's text data
  //
  // "event_text" - text value of the event to be logged
  void StageEventText(const std::string &event_text);

  // Sets the flag in the base class confirming that the data is ready.
  //
  // Returns - "true" if all of the data has been staged and is ready to
  //       write, "false" otherwise.
  bool VerifyStagedReady();

 private:
  // Enum gives index names to the elements of the "data_staged_" C-style
  // array.  
  // "kStagedCount" must always be the final enumeration constant, as it
  // is used to specify the size of the "data_staged_" array
  enum StagedReadyIndex {kEventTextStaged, kEventTimeStaged, kStagedCount};

  // Set the staged flag for the specified field to "true".
  //
  // "field" - specifies which component of the data is staged.
  void SetFieldStaged(StagedReadyIndex field) { data_staged_[field] = true; };
  // Set the staged flags to all "false", to prepare for another set of
  // staged data.  Also, sets the base class data_ready_ flag to "false"
  void ClearStagedFlags();

  // Text payload of the event to be written
  std::string event_text_;
  // Time of the event to be written
  SimTime event_time_;
  // Flags denoting staged status of each data element
  bool data_staged_[kStagedCount];
  // As per the coding standard
  DISALLOW_COPY_AND_ASSIGN(LogTextEvent);
};

#endif // SIM_EXAMPLES_TEXT_EVENT_LOG_TEXT_EVENT_HPP_
