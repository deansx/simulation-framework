/*****************************************************************************
*
*   DESCRIPTION:
*     File implementing ctors and dtor of the LogMgr (Log Manager) pure
*     virtual class.
*     This class provides a template for implementation specific log file
*     managers.  Since the class is pure virtual, most of the methods must
*     be overridden in derived classes to create objects that are actually
*     useful.
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

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "common_strings.hpp"
#include "common_messages.hpp"
#include "log_mgr.hpp"

// This constructor handles creating and opening a stream to the log file
// specified by "log_path".
// Since this is the ctor, "data_ready_" is initialized to "false"
// Since this method will be creating / managing the stream, the
// "delete_log_stream_" flag is set to true.
// NOTE:  A fatal error will be generated and the application will exit
// with a failure if the "log_path" cannot be opened.
//
// "log_path" - pathname for the log file.
LogMgr::LogMgr(std::string log_path) : data_ready_(false),
                               delete_log_stream_(true) {
  log_stream_ = new std::ofstream(log_path);
  if ((log_stream_->is_open()) && (log_stream_->good())) {
    std::cout << kCommonStrNote << "Opened log output file:  \""
         << log_path << "\" successfully.\n";
  } else {
    // Not necessary, since we're exiting, but just to be safe
    log_stream_ = nullptr;
    // Form the string for the error message
    std::stringstream line1;
    line1 << "Could not open log output file:  \"" 
          << log_path << "\". (LogMgr)";
    UtilFatalErrorAndDie(line1.str());
  }
}


// This constructor implements the case where the "log_stream" was
// created and opened earlier in the call stack.
// Since this is the ctor, "data_ready_" is initialized to "false"
// Since this method will not be creating / managing the stream, the
// "delete_log_stream_" flag is set to false.
// NOTE:  This ctor will test to ensure that the stream is open and OK.
// If not,a fatal error will be generated and the application will exit
// with a failure.
// This object will use the stream, but will not be responsible for
// the storage and, more importantly, closing the stream.
//
// "log_stream" - pointer to an existing stream to be used for logging
//       output.
LogMgr::LogMgr(std::ofstream *log_stream) : log_stream_(log_stream),
                                    data_ready_(false),
                                    delete_log_stream_(false) {
  // See if the specified stream is usable
  if (!((log_stream_->is_open()) && (log_stream_->good()))) {
    // Probably a logic error in the caller's code...
    UtilFatalErrorAndDie("Unable to use the specified stream.\n"
                         "The stream is either not open, or returned a bad"
                         "status. (LogMgr)");
  }
}


// If "delete_log_stream_" is "true", the dtor closes the stream and
// destroys the stream object.  Just to be safe, the dtor also sets
// "data_ready_" to "false".
LogMgr::~LogMgr() {
  // Since we're destroying the object, this probably isn't needed, but
  // just to be safe
  data_ready_ = false;
  // If this object is managing the "log_stream_"
  if (delete_log_stream_) {
    // delete will force the stream to close, but this seems like good
    // hygiene
    log_stream_->close();
    delete log_stream_;
  }
}
