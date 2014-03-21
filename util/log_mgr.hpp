/*****************************************************************************
*
*   DESCRIPTION:
*     Header file declaring the LogMgr (Log Manager) pure virtual class.
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

#ifndef SIM_UTIL_LOG_MGR_HPP_
#define SIM_UTIL_LOG_MGR_HPP_

#include <string>
#include <fstream>

#include "basic_defs.hpp"


class LogMgr {
 public:
  // This constructor handles creating and opening a stream to the log file
  // specified by "log_path".
  // NOTE:  A fatal error will be generated and the application will exit
  // with a failure if the "log_path" cannot be opened.
  //
  // "log_path" - pathname for the log file.
  LogMgr(std::string log_path);

  // This constructor implements the case where the "log_stream" was
  // created and opened earlier in the call stack.
  // NOTE:  This ctor will test to ensure that the stream is open and OK.
  // If not,a fatal error will be generated and the application will exit
  // with a failure.
  // This object will use the specified stream, but will not be responsible
  // for the storage and, more importantly, closing the stream.  The caller
  // MUST handle both ensuring that the "log_stream" is closed and that the
  // its storage is handled appropriately.  This could allow multiple
  // instances of "LogMgr" to share a single file, which is absolutely not
  // thread safe, but might be convenient.
  // Note that, while explicitly calling "fstream->close()" is a good
  // practice, an open file is automatically closed when the fstream is
  // destroyed.
  //
  // "log_stream" - pointer to an existing stream to be used for logging
  //       output.
  LogMgr(std::ofstream *log_stream);

  // If "delete_log_stream_" is "true", the dtor closes the stream and
  // destroys the stream object.  Other housekeeping details are
  // trivial.
  ~LogMgr();

  // Write the column headings for the log file CSV.
  // Failure signals something pretty serious so the override method
  // should generate a fatal error message and terminate.
  virtual void WriteHeaderOrDie() = 0;
  // Write the data collected for one output record
  // Failure signals something pretty serious so the override method
  // should generate a fatal error message and terminate.
  virtual void WriteARecordOrDie() = 0;

  // Reset the data fields to prepare for another pass
  virtual void Reset() = 0;

  // Accessor for the "log_stream_" data member
  //
  // Returns - pointer to the stream used for logging.
  std::ofstream *log_stream() { return log_stream_; };

  // Mutator/Accessor for the status of the data for the next record to
  // be written
  //
  // "ready_flag" - signifies that the data cached in the log manager
  //       is ready to be written to the log stream.  Since derived
  //       classes actually manage the data, they must be responsible
  //       for maintaining  this field.
  void set_data_ready(const bool ready_flag) { data_ready_ = ready_flag; };
  //
  // Returns - the current state of the "data_ready_" flag.
  bool data_ready() const { return data_ready_; };

 protected:
  // Stream for output
  std::ofstream *log_stream_;

 private:
  // The data in this record is ready to be written
  bool data_ready_;
  // Flag controlling whether the destructor should close and delete the
  // "log_stream_".
  // If this object was constructed with a "log_path", this object is
  // responsible for both the closure and the storage of the "log_stream_"
  // object.
  // If this object was constructed with "std::ofstream *", the orderly
  // termination of the "log_stream_" is the responsibility of the caller.
  bool delete_log_stream_;
  // As per the coding standard
  DISALLOW_COPY_AND_ASSIGN(LogMgr);
};

#endif // SIM_UTIL_LOG_MGR_HPP_
